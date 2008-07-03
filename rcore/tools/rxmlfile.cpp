/*

	R Project Library

	RXMLFile.cpp

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
#include <rxmlfile.h>
#include <rcursor.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RXMLFile::Namespace
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RXMLFile::Namespace
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
// class RXMLFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RURI& uri,RXMLStruct* xmlstruct,const RString& encoding)
 : RTextFile(uri,encoding), XMLStruct(xmlstruct), NewStruct(false), CurTag(0), Namespaces(20),
   DefaultNamespace(5), AvoidSpaces(false), Attrs(10)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");

}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RURI& uri,RXMLStruct& xmlstruct,const RString& encoding)
 : RTextFile(uri,encoding), XMLStruct(&xmlstruct), NewStruct(false),CurTag(0), Namespaces(20),
   DefaultNamespace(5), AvoidSpaces(false), Attrs(10)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");

}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RString& encoding)
 : RTextFile(file,encoding), XMLStruct(xmlstruct), NewStruct(false),CurTag(0), Namespaces(20),
   DefaultNamespace(5), AvoidSpaces(false), Attrs(10)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");

}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(RIOFile& file,RXMLStruct& xmlstruct,const RString& encoding)
 : RTextFile(file,encoding), XMLStruct(&xmlstruct), CurTag(0), Namespaces(20),
   DefaultNamespace(5), AvoidSpaces(false), Attrs(10)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");

}


//------------------------------------------------------------------------------
void RXMLFile::SetEncoding(const RString& name)
{
	XMLStruct->SetEncoding(name);
	RTextFile::SetEncoding(name);
}


//------------------------------------------------------------------------------
size_t RXMLFile::GetCurrentDepth(void) const
{
	if(Mode!=RIO::Read)
		throw RIOException("File not in read mode");
	return(CurDepth);
}


//------------------------------------------------------------------------------
size_t RXMLFile::GetLastTokenPos(void) const
{
	if(Mode!=RIO::Read)
		throw RIOException("File not in read mode");
	return(LastTokenPos);
}


//------------------------------------------------------------------------------
void RXMLFile::SetDocType(const RString& docType)
{
	DocType=docType;
}


//------------------------------------------------------------------------------
void RXMLFile::Open(RIO::ModeType mode)
{
	RTextFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			CurDepth=0;
			CurTag=0;
			CurAttr=0;
			if(!XMLStruct)
			{
				XMLStruct=new RXMLStruct();
				NewStruct=true;
			}
			else
				XMLStruct->Clear(); // Make sure the xml structure is empty
			ProcessBody=false;
			LoadHeader();
			ProcessBody=true;
			LoadNextTag();
			CurDepth=0;
			break;

		case RIO::Append:
		case RIO::Create:
		{
			if(!XMLStruct)
				throw RIOException("A XML structure must be assign");
			RXMLTag* top(XMLStruct->GetTop());
			RString Header("<?xml");
			if(!XMLStruct->GetVersion().IsEmpty())
				Header+=" version=\""+XMLStruct->GetVersion()+"\"";
			if(!XMLStruct->GetEncoding().IsEmpty())
				Header+=" encoding=\""+XMLStruct->GetEncoding()+"\"";
			Header+="?>";
			(*this)<<Header;
			if(!AvoidSpaces)
				(*this)<<endl;
			Header="<!DOCTYPE "+top->GetName();
			if(XMLStruct)
			{
				RString DTD=XMLStruct->GetDTD();
				if(DTD.GetLen())
					Header+=" \""+DTD+"\"";
			}
			RCursor<RXMLAttr> Cur(XMLStruct->GetXMLEntitiesCursor());
			if(Cur.GetNb())
			{
				Header+="[";
				if(!AvoidSpaces)
					Header+="\n";
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					if(!AvoidSpaces)
						Header+="\t";
					else
						Header+=" ";
					Header+="<!ENTITY "+Cur()->GetName()+" \""+Cur()->GetValue()+"\">";
					if(!AvoidSpaces)
						Header+="\n";
				}
				Header+=" ]>";
			}
			else
				Header+=">";
			(*this)<<Header;
			if(!AvoidSpaces)
				(*this)<<endl;
			CurTag=top;
			SaveNextTag(0);
			break;
		}

		default:
			throw RIOException("No Valid Mode");
	};
}


//------------------------------------------------------------------------------
void RXMLFile::Close(void)
{
	if(NewStruct)
	{
		delete XMLStruct;
		XMLStruct=0;
		NewStruct=false;
	}
}


//------------------------------------------------------------------------------
RChar RXMLFile::CodeToChar(RString& str)
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
RString RXMLFile::CharToCode(RChar car,bool strict)
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
RString RXMLFile::XMLToString(const RString& str)
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
RString RXMLFile::StringToXML(const RString& str,bool strict)
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
void RXMLFile::LoadHeader(void)
{
	// Search after "<? xml"
	if(CurString("<?"))
	{
		SkipSpaces();
		if(!CurString("xml",false))
			throw RIOException(this,"Wrong XML header");
		SkipSpaces();

		// Search for parameters until ?> is found
		bool PopDefault;
		RContainer<Namespace,false,false> PopURI(5);
		Attrs.Clear();
		LoadAttributes(PopDefault,PopURI,'?','>');
		if((CurAttr=Attrs.GetPtr("version")))
			XMLStruct->SetVersion(CurAttr->GetValue());
		if((CurAttr=Attrs.GetPtr("encoding")))
			SetEncoding(CurAttr->GetValue());

		// Skip '>' and the spaces after that
		Next();
		SkipSpaces();
	}

	// Search after "<?xml-stylesheet"
	if(CurString("<?xml-stylesheet"))
	{
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

					// Skip " and >
					Next();
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
				RString DTD=GetToken(What);
				if(XMLStruct)
					XMLStruct->SetDTD(DTD);
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
void RXMLFile::LoadNextTag(void)
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
void RXMLFile::LoadAttributes(bool& popdefault,RContainer<Namespace,false,false>& popuri,RChar EndTag1,RChar EndTag2)
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
void RXMLFile::SaveNextTag(int depth)
{
	int i;
	RString text;
	RCursor<RXMLAttr> Cur;
	RCursor<RXMLTag> Tags;
	RString line;

	if(!AvoidSpaces)
		for(int i=0;i<depth;i++) line+="\t";
	Cur=CurTag->GetAttrs();
	Tags=CurTag->GetNodes();
	if(Cur.GetNb())
	{
		line+="<"+CurTag->GetName();
		for(Cur.Start(),i=Cur.GetNb();--i;Cur.Next())
		{
			line+=" "+Cur()->GetName()+"=\""+StringToXML(Cur()->GetValue())+"\"";
		}
		if(Tags.GetNb()||CurTag->GetContent().GetLen())
		{
			line+=" "+Cur()->GetName()+"=\""+StringToXML(Cur()->GetValue())+"\">";
			(*this)<<line<<endl;
			line.Clear();
		}
		else
		{
			line+=" "+Cur()->GetName()+"=\""+StringToXML(Cur()->GetValue())+"\"/>";
			(*this)<<line<<endl;
			line.Clear();
		}
	}
	else
	{
		if(Tags.GetNb()||CurTag->GetContent().GetLen())
			line+="<"+CurTag->GetName()+">";
		else
			line+="<"+CurTag->GetName()+"/>";
		(*this)<<line<<endl;
		line.Clear();
	}
	if(CurTag->GetContent().GetLen())
	{
		if(!AvoidSpaces)
			for(int i=0;i<depth+1;i++) line+="\t";
		line+=StringToXML(CurTag->GetContent(),false);
		(*this)<<line<<endl;
		line.Clear();
	}
	for(Tags.Start();!Tags.End();Tags.Next())
	{
		CurTag=Tags();
		SaveNextTag(depth+1);
		CurTag=CurTag->GetParent();
	}
	if(Tags.GetNb()||CurTag->GetContent().GetLen())
	{
		if(!AvoidSpaces)
			for(int i=0;i<depth;i++) line+="\t";
		line+="</"+CurTag->GetName()+">";
		(*this)<<line<<endl;
	}
}


//------------------------------------------------------------------------------
void RXMLFile::BeginTag(const RString& namespaceURI, const RString&, const RString& name)
{
	RXMLTag* tag;
//	RCursor<RXMLAttr> Cur(Attrs);

	// Create the tag and add it to the XML structure
	tag=XMLStruct->NewTag(name,namespaceURI);
	XMLStruct->AddTag(CurTag,tag);

/*	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RString* ptr=Cur()->GetNamespace();
		if(ptr)
			tag->InsertAttr(Cur()->GetName(),Cur()->GetValue(),*ptr);
		else
			tag->InsertAttr(Cur()->GetName(),Cur()->GetValue(),RString::Null);
	}*/

	// If no top tag -> insert it
	if(!XMLStruct->GetTop())
	{
		// Test if is a XML document (DocType=true) or a HTML document (DocType=false)
		if(!DocType.IsEmpty())
		{
			// Name of the Tag must be name of the DocType
			if(tag->GetName()!=DocType)
				throw RIOException(this,"Not a valid XML file");
		}
		else
		{
			RString TopName=tag->GetName().ToLower();

			// Is it a HTML file?
			// -> If Not, consider the first tag is the DOCTYPE
			if(TopName!="html")
				DocType=TopName;
				//throw RIOException(this,"Not a valid HTML file");
		}
	}

	// Make the tag the current one
	CurTag=tag;
}


//------------------------------------------------------------------------------
void RXMLFile::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
{
	if(CurTag)
		CurTag->InsertAttr(CurAttr=XMLStruct->NewAttr(lName,namespaceURI));
	else
		Attrs.InsertPtr(CurAttr=XMLStruct->NewAttr(lName,namespaceURI));
}


//------------------------------------------------------------------------------
void RXMLFile::Value(const RString& value)
{
	CurAttr->AddValue(value);
}


//------------------------------------------------------------------------------
void RXMLFile::EndTag(const RString&, const RString&, const RString& name)
{
	if(CurTag->GetName()!=name)
		throw RIOException(this,"Found closing tag '"+name+"' while closing tag '"+CurTag->GetName()+"' was expected.");
	CurTag=CurTag->GetParent();
}


//------------------------------------------------------------------------------
void RXMLFile::Text(const RString& text)
{
	CurTag->AddContent(text);
}


//------------------------------------------------------------------------------
bool RXMLFile::OnlyQuote(void)
{
	return(true);
}


//------------------------------------------------------------------------------
bool RXMLFile::InvalidXMLCodeAccept(void)
{
	return(false);
}


//------------------------------------------------------------------------------
RXMLFile::~RXMLFile(void)
{
}
