/*

	R Project Library

	RXMLParser.cpp

	XML file - Implementation.

	Copyright 2000-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlparser.h>
#include <rcursor.h>
#include <rxmlattr.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RXMLParser::Namespace
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RXMLParser::Namespace
{
public:
	RString Prefix;
	RStack<RString,true,true,true> URI;

	Namespace(const RString& prefix) : Prefix(prefix), URI(5) {}
	int Compare(const Namespace& sp) const {return(Prefix.Compare(sp.Prefix));}
	int Compare(const Namespace* sp) const {return(Prefix.Compare(sp->Prefix));}
	int Compare(const RString& sp) const {return(Prefix.Compare(sp));}
};



//------------------------------------------------------------------------------
//
// class RXMLParser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLParser::RXMLParser(const RURI& uri,const RString& encoding)
 : RTextFile(uri,encoding), Namespaces(20),
   DefaultNamespace(5), AvoidSpaces(false), Attrs(10)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
}


//------------------------------------------------------------------------------
RXMLParser::RXMLParser(RIOFile& file,const RString& encoding)
 : RTextFile(file,encoding), Namespaces(20),
   DefaultNamespace(5), AvoidSpaces(false), Attrs(10)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
}


//------------------------------------------------------------------------------
size_t RXMLParser::GetCurrentDepth(void) const
{
	if(Mode!=RIO::Read)
		throw RIOException("File not in read mode");
	return(CurDepth);
}


//------------------------------------------------------------------------------
size_t RXMLParser::GetLastTokenPos(void) const
{
	if(Mode!=RIO::Read)
		throw RIOException("File not in read mode");
	return(LastTokenPos);
}


//------------------------------------------------------------------------------
void RXMLParser::Open(RIO::ModeType mode)
{
	RTextFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			CurDepth=0;
			LoadHeader();
			Section=Body;
			LoadNextTag();
			CurDepth=0;
			break;

		case RIO::Append:
		case RIO::Create:
			throw RIOException("A XML Parser is read-only");
			break;

		default:
			throw RIOException("No Valid Mode");
	};
}


//------------------------------------------------------------------------------
RChar RXMLParser::CodeToChar(RString& str)
{
	if(!str.Compare("quot"))
		return('"');
	if(!str.Compare("amp"))
		return('&');
	if(!str.Compare("apos"))
		return('\'');
	if(!str.Compare("lt"))
		return('<');
	if(!str.Compare("gt"))
		return('>');
	return(0);
}


//------------------------------------------------------------------------------
RString RXMLParser::CharToCode(RChar car,bool strict)
{
	RString res;

	switch(car.Unicode())
	{
		case '"':
			if(strict)
				res="&quo;";
			else
				res="\"";
			break;
		case '&':
			res="&amp;";
			break;
		case '\'':
			if(strict)
				res="&apos;";
			else
				res="'";
			break;
		case '<':
			res="&lt;";
			break;
		case '>':
			if(strict)
				res="&gt;";
			else
				res=">";
			break;
		default:
			res+=car;
	}

	return(res);
}


//------------------------------------------------------------------------------
RString RXMLParser::XMLToString(const RString& str)
{
	RString res;
	RCharCursor Cur(str);
	RString Code;
	RChar What;

	while(!Cur.End())
	{
		// Verify if it is not a code
		if(Cur()==RChar('&'))
		{
			// Skip &
			Cur.Next();

			// Read what could be a code
			Code.Clear();
			while((!Cur.End())&&(Cur()!=RChar(';'))&&(!Cur().IsSpace()))
			{
				Code+=Cur();
				Cur.Next();
			}

			// Is it a code (len<10 and last character is ";'))
			if((!Cur.End())&&(Cur()==RChar(';'))&&(Code.GetLen()<10))
			{
				What=CodeToChar(Code);
				if(What.IsNull())
				{
					if(InvalidXMLCodeAccept())
						return(Code);
					throw RIOException(this,"Invalid XML/HTML code \""+Code+"\"");
				}
				else
					res+=What;

				// Skip ;
				if(!Cur.End())
					Cur.Next();
			}
			else
			{
				if(InvalidXMLCodeAccept())
					return(Code);
				throw RIOException(this,"Invalid XML/HTML code \""+Code+"\"");
			}
		}
		else
		{
			res+=Cur();
			Cur.Next();
		}
	}

	return(res);
}


//------------------------------------------------------------------------------
RString RXMLParser::StringToXML(const RString& str,bool strict)
{
	RCharCursor Cur(str);
	RString res;

	if(str.FindStr("<![CDATA[")!=-1)
		return(str);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		res+=CharToCode(Cur(),strict);
	}
	return(res);
}


//------------------------------------------------------------------------------
void RXMLParser::LoadHeader(void)
{
	// Search after "<? xml"
	if(CurString("<?"))
	{
		Section=Header;
		SkipSpaces();
		if(!CurString("xml",false))
			throw RIOException(this,"Wrong XML header");
		SkipSpaces();

		// Search for parameters until ?> is found
		bool PopDefault;
		RContainer<Namespace,false,false> PopURI(5);
		Attrs.Clear();
		LoadAttributes(PopDefault,PopURI,'?','>');

		// Skip '>' and the spaces after that
		Next();
		SkipSpaces();
	}

	// Search after "<?xml-stylesheet"
	if(CurString("<?xml-stylesheet"))
	{
		Section=StyleSheet;
		SkipSpaces();

		// Search for parameters until ?> is found
		bool PopDefault;
		RContainer<Namespace,false,false> PopURI(5);
		Attrs.Clear();
		LoadAttributes(PopDefault,PopURI,'?','>');

		// Skip '>' and the spaces after that
		Next(); SkipSpaces();
	}


	// Search after "!DOCTYPE"
	if(CurString("<!"))
	{
		Section=DOCTYPE;
		if(!CurString("DOCTYPE",false))
			throw RIOException(this,"Wrong DOCTYPE command");

		RString Content(GetToken('>','['));
		SetDocType(XMLToString(Content));
		SkipSpaces();

		for(bool Cont=true;Cont;Cont=false)
		{
			if(GetNextChar()=='[')
			{
				GetChar(); // Skip [
				SkipSpaces();

				// Read Entities
				while((!End())&&(GetNextChar()!=']'))
				{
					// Get next name
					RString cmd=GetWord();
					if(cmd!="<!ENTITY")
						throw RIOException(this,"Wrong entities formating");

					RString ns(GetWord());
					Cur=GetChar();
					if(Cur!='"')
						throw RIOException(this,"Wrong entities formating");
					RString uri(GetToken(RChar('"')));
					Cur=GetChar();
					if(Cur!='"')
						throw RIOException(this,"Wrong entities formating");

					// Verify if the attribute is a namespace
					int i=ns.Find(':');
					if(i!=-1)
					{
						Namespace* ptr=Namespaces.GetInsertPtr(ns.Mid(i+1,ns.GetLen()-i));
						ptr->URI.Push(new RString(uri));
					}
					else
					{
						// If not : -> default namespace
						DefaultNamespace.Push(new RString(uri));
					}

					// Skip >
					SkipSpaces();
					Next();
					SkipSpaces();
				}

				if(End())
					throw RIOException(this,"Wrong entities formating");

				// Skip ] and spaces
				Next();
				SkipSpaces();
				Cont=true;
			}

			if(CurString("SYSTEM"))
			{
				SkipSpaces();
				RChar What=GetChar();
				bool Quotes=((What==RChar('\''))||(What==RChar('"')));
				if(!Quotes)
					throw RIOException(this,"Bad XML file");
				SetDTD(GetToken(What));
				SkipSpaces();
				if(GetChar()!=What)
					throw RIOException(this,"Bad XML file");
				SkipSpaces();
				Cont=true;
			}
		}
		Next(); // Skip >
		SkipSpaces();
	}
}


//------------------------------------------------------------------------------
void RXMLParser::LoadNextTag(void)
{
	RString attrn,attrv;
	RString TagName;
	RString Contains;
	RChar What;
	RString Code;
	bool PopDefault;
	RContainer<Namespace,false,false> PopURI(5);
	bool CDATA;

	// If not a tag -> Error
	if(GetChar()!='<')
		throw RIOException(this,"Not a tag");

	// Read name of the tag
	LastTokenPos=GetPos();
	TagName=GetToken('>','/');

	// Treat the tag
	// Search if it has a namespace
	RString uri,lname;
	int i=TagName.Find(':');
	if(i!=-1)
	{
		RString prefix=TagName.Mid(0,i);
		Namespace* ptr=Namespaces.GetPtr(prefix);
		if(!ptr)
			throw RIOException(this,"Namespace with prefix '"+prefix+"' no defined");
		uri=(*ptr->URI());
		lname=TagName.Mid(i+1,TagName.GetLen()-i);
	}
	else
	{
		if(DefaultNamespace.GetNb())
			uri=(*DefaultNamespace()); // Default namespace.
		lname=TagName;
	}

	BeginTag(uri,lname,TagName);

	// Read Attributes
	Attrs.Clear();
	LoadAttributes(PopDefault,PopURI);

	// It is a closing tag? -> Skip / and > -> else Skip >
	CurTagClosing=(Cur==RChar('/'));
	if(CurTagClosing)
		Next();
	SkipSpaces();



	// Verify if it has sub-tags
	if(CurTagClosing)
	{
		EndTag("","",TagName);
		return;
	}

	// Treat sub-tags
	while((!End())&&(!CurString("</",false)))
	{
		LastTokenPos=GetPos();
		// If only '<' -> new tag
		if(GetNextChar()==RChar('<'))
		{
			// It is a tag -> read it
			CDATA=CurString("<![CDATA[",true);
			if(CDATA)
			{
				Text("<![CDATA[");
				SkipSpaces();
				SetParseSpace(RTextFile::LeaveSpaces);

				while(!CurString("]]>"))
				{
					LastTokenPos=GetPos();
					RString tmp(GetToken("]]>"));
					if(tmp.GetLen())
						Text(tmp);

					// Add all spaces
					while(GetNextChar().IsSpace())
						Text(GetChar());
				}
				Text("]]>");
				SetParseSpace(RTextFile::SkipAllSpaces);
				SkipSpaces();
			}
			else
			{
				CurDepth++;   	// Increase de depth
				LoadNextTag();
				CurDepth--;   	// Decrease de depth
			}
		}
		else
		{
			// It is content -> read it as long as there is no open tag.
			Contains.Clear();
			CDATA=true; // Suppose that first '<' found is a "<![CDATA["
			SetParseSpace(RTextFile::LeaveSpaces);
			while(CDATA)
			{
				// Get the next word
				LastTokenPos=GetPos();
				RString tmp(GetToken(RChar('<')));
				if(tmp.GetLen())
					Text(XMLToString(tmp));

				// Add all spaces
				while(GetNextChar().IsSpace())
					Text(GetChar());

				// Look if the next '<' is the beginning of "<![CDATA["
				CDATA=CurString("<![CDATA[",true);
				if(CDATA)
				{
					cout<<"<![CDATA[ found"<<endl;
					Text("<![CDATA[");
					SkipSpaces();
					SetParseSpace(RTextFile::LeaveSpaces);

					while(!CurString("]]>"))
					{
						LastTokenPos=GetPos();
						RString tmp(GetToken("]]>"));
						if(tmp.GetLen())
							Text(tmp);

						// Add all spaces
						while(GetNextChar().IsSpace())
							Text(GetChar());
					}
					Text("]]>");
					SetParseSpace(RTextFile::SkipAllSpaces);
					SkipSpaces();
				}
			}
		}
		SetParseSpace(RTextFile::SkipAllSpaces);
		SkipSpaces();
	}

	// Read the close tag
	CurTagClosing=true;
	SkipSpaces();
	LastTokenPos=GetPos();
	TagName=GetToken(RChar('>'));
	SkipSpaces();
	Next(); // Skip >
	EndTag(uri,lname,TagName);
	if(PopDefault)
		DefaultNamespace.Pop();
	while(PopURI.GetNb())
	{
		RCursor<Namespace> Cur(PopURI);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->URI.Pop();
		PopURI.Clear();
	}
	SkipSpaces();
}


//------------------------------------------------------------------------------
void RXMLParser::LoadAttributes(bool& popdefault,RContainer<Namespace,false,false>& popuri,RChar EndTag1,RChar EndTag2)
{
	RChar What;
	bool Quotes;

	// No namespace
	popdefault=false;

	while((!End())&&(GetNextChar()!=EndTag1)&&(GetNextChar()!=EndTag2))
	{
		// Clear name, value and uri
		RString ns;
		RString uri;
		RString lname;
		bool GetNs(false);

		// Read the name of the attribute
		LastTokenPos=GetPos();
		RString attrn=GetToken('=','>');

		// Verify if the attribute name has a namespace
		int i=attrn.Find(':');
		if(i!=-1)
		{
			// Namespace defined
			RString prefix=attrn.Mid(0,i);
			lname=attrn.Mid(i+1,attrn.GetLen()-i);
			if(prefix=="xmlns")
			{
				uri="xmlns";  // New namespace declared
				GetNs=true;
			}
			else
			{
				Namespace* ptr=Namespaces.GetPtr(prefix);
				if(!ptr)
					throw RIOException(this,"Namespace with prefix '"+prefix+"' no defined");
				uri=(*ptr->URI());
			}
		}
		else
		{
			lname=attrn;
			if(attrn=="xmlns")
			{
				uri="xmlns";  // New namespace declared
				GetNs=true;
			}
			else
			{
				if(DefaultNamespace.GetNb())
					uri=(*DefaultNamespace()); // Default namespace.
			}
		}

		// Add the attribute
		AddAttribute(uri,lname,attrn);

		// Determine if a value is assign
		Next();
		if(Cur==RChar('='))
		{
			SkipSpaces();

			// Determine if the parameter is delimited by quotes
			Next();
			What=Cur;
			Quotes=((What==RChar('\''))||(What==RChar('"')));

			// Read the value
			if(Quotes)
			{
				SetParseSpace(RTextFile::LeaveSpaces);
				while((!End())&&(GetNextChar()!=What))
				{
					// Get the next word
					LastTokenPos=GetPos();
					RString tmp(GetToken(What));
					if(tmp.GetLen())
						Value(tmp);
					if(GetNs)
						ns+=tmp;

					// Add all spaces
					while(GetNextChar().IsSpace())
						Value(GetChar());
				}
				SetParseSpace(RTextFile::SkipAllSpaces);
				Next();
			}
			else
			{
				// If Quote must be used -> generate an exeception
				if(OnlyQuote())
					throw RIOException(this,"Quote must be used to delimit the parameter value in a tag.");
				SetParseSpace(RTextFile::LeaveSpaces);
				while((!End())&&(GetNextChar()!=What))
				{
					// Get the next word
					LastTokenPos=GetPos();
					RString tmp(GetToken(EndTag1,EndTag2));
					if(tmp.GetLen())
						Value(tmp);

					// Add all spaces
					while(GetNextChar().IsSpace())
						Value(GetChar());
				}
				SetParseSpace(RTextFile::SkipAllSpaces);
				Next();
			}

			// Verify if the attribute is a namespace
			if(GetNs)
			{
				uri=XMLToString(ns);
				int i=attrn.Find(':');
				if(i!=-1)
				{
					Namespace* ptr=Namespaces.GetInsertPtr(attrn.Mid(i+1,attrn.GetLen()-i));
					ptr->URI.Push(new RString(uri));
					popuri.InsertPtr(ptr);
				}
				else
				{
					// If not : -> default namespace
					DefaultNamespace.Push(new RString(uri));
					popdefault=true;
				}
			}
			SkipSpaces();
		}
	}

	Next(); // Put the internal pointer to the first ending character found
}


//------------------------------------------------------------------------------
void RXMLParser::SetDocType(const RString& docType)
{
	DocType=docType;
}


//------------------------------------------------------------------------------
void RXMLParser::BeginTag(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::AddAttribute(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::Value(const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::EndTag(const RString&, const RString&, const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::Text(const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::SetDTD(const RString&)
{
}


//------------------------------------------------------------------------------
bool RXMLParser::OnlyQuote(void)
{
	return(true);
}


//------------------------------------------------------------------------------
bool RXMLParser::InvalidXMLCodeAccept(void)
{
	return(false);
}



//------------------------------------------------------------------------------
RXMLParser::~RXMLParser(void)
{
}
