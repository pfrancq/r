/*

	R Project Library

	RXMLParser.cpp

	XML parser - Implementation.

	Copyright 2000-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
// class RXMLParser::Attribute
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RXMLParser::Attribute
{
public:
	RString Name;
	RContainer<RString,true,false> Values;
	size_t NbValues;

	Attribute(void) : Values(20) {}
	int Compare(const Attribute&) const {return(-1);}
};


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
// class RXMLParser::Tag
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
RXMLParser::HTMLTag::HTMLTag(const RString& name,int level,bool single,bool self)
		: Name(name), Level(level), Single(single), SelfContained(self)
{
}


//------------------------------------------------------------------------------
int RXMLParser::HTMLTag::Compare(const RXMLParser::HTMLTag& t) const
{
	return(Name.Compare(t.Name));
}


//------------------------------------------------------------------------------
int RXMLParser::HTMLTag::Compare(const RString& t) const
{
	return(Name.Compare(t));
}



//------------------------------------------------------------------------------
// Global variables
RContainer<RXMLParser::HTMLTag,true,true> RXMLParser::Tags(200,10);



//------------------------------------------------------------------------------
//
// class RXMLParser
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLParser::RXMLParser(void)
	: RTextFile(), Namespaces(20), DefaultNamespace(5), Attributes(20),
	  AvoidSpaces(false), InvalidXMLCodes(false), HTMLMode(false)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	if(!Tags.GetNb())
		InitValidTags();
}


//------------------------------------------------------------------------------
RXMLParser::RXMLParser(const RURI& uri,const RCString& encoding)
 : RTextFile(uri,encoding), Namespaces(20), DefaultNamespace(5), Attributes(20),
   AvoidSpaces(false), InvalidXMLCodes(false), HTMLMode(false)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	if(!Tags.GetNb())
		InitValidTags();
}


//------------------------------------------------------------------------------
RXMLParser::RXMLParser(RIOFile& file,const RCString& encoding)
 : RTextFile(file,encoding), Namespaces(20), DefaultNamespace(5), Attributes(20),
   AvoidSpaces(false), /*Attrs(10),*/ InvalidXMLCodes(false), HTMLMode(false)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	if(!Tags.GetNb())
		InitValidTags();
}


//------------------------------------------------------------------------------
void RXMLParser::InitValidTags(void)
{
	// Global Tags
	Tags.InsertPtr(new HTMLTag("html",0,false));
	Tags.InsertPtr(new HTMLTag("body",10,false));
	Tags.InsertPtr(new HTMLTag("head",10,false));

	// Heading Tags
	Tags.InsertPtr(new HTMLTag("meta",500,true));
	Tags.InsertPtr(new HTMLTag("title",500,false));
	Tags.InsertPtr(new HTMLTag("base",500,false));
	Tags.InsertPtr(new HTMLTag("link",500,true));
	Tags.InsertPtr(new HTMLTag("style",500,false));

	// Division Tags
	Tags.InsertPtr(new HTMLTag("div",150,false,true));
	Tags.InsertPtr(new HTMLTag("span",160,false,true));

	// Frame
	Tags.InsertPtr(new HTMLTag("frameset",100,false));
	Tags.InsertPtr(new HTMLTag("frame",360,false));
	Tags.InsertPtr(new HTMLTag("noframe",360,false));
	Tags.InsertPtr(new HTMLTag("iframe",360,false));

	//info to contact author
	Tags.InsertPtr(new HTMLTag("address",360,false));

	// Forms
	Tags.InsertPtr(new HTMLTag("form",360,false));
	Tags.InsertPtr(new HTMLTag("input",500,false));
	Tags.InsertPtr(new HTMLTag("button",500,false));
	Tags.InsertPtr(new HTMLTag("label",500,false));
	Tags.InsertPtr(new HTMLTag("select",500,false));
	Tags.InsertPtr(new HTMLTag("option",500,false));
	Tags.InsertPtr(new HTMLTag("optgroup",500,false));
	Tags.InsertPtr(new HTMLTag("textarea",500,false));
	Tags.InsertPtr(new HTMLTag("isindex",500,false));
	Tags.InsertPtr(new HTMLTag("fieldset",500,false));
	Tags.InsertPtr(new HTMLTag("legend",500,false));

	// Structure Tags
	Tags.InsertPtr(new HTMLTag("table",200,false));
	Tags.InsertPtr(new HTMLTag("caption",210,false));
	Tags.InsertPtr(new HTMLTag("thead",210,false));
	Tags.InsertPtr(new HTMLTag("tfoot",210,false));
	Tags.InsertPtr(new HTMLTag("tbody",210,false));
	Tags.InsertPtr(new HTMLTag("tr",220,false));
	Tags.InsertPtr(new HTMLTag("th",220,false));
	Tags.InsertPtr(new HTMLTag("td",230,false));
	Tags.InsertPtr(new HTMLTag("ul",240,false));
	Tags.InsertPtr(new HTMLTag("ol",240,false));
	Tags.InsertPtr(new HTMLTag("dl",240,false));
	Tags.InsertPtr(new HTMLTag("li",250,false));
	Tags.InsertPtr(new HTMLTag("dt",250,false));
	Tags.InsertPtr(new HTMLTag("dd",250,false));
	Tags.InsertPtr(new HTMLTag("colgroup",500,false));
	Tags.InsertPtr(new HTMLTag("col",500,false));


	// Paragraph Tag
	Tags.InsertPtr(new HTMLTag("h1",300,false));
	Tags.InsertPtr(new HTMLTag("h2",310,false));
	Tags.InsertPtr(new HTMLTag("h3",320,false));
	Tags.InsertPtr(new HTMLTag("h4",330,false));
	Tags.InsertPtr(new HTMLTag("h5",340,false));
	Tags.InsertPtr(new HTMLTag("h6",350,false));
	Tags.InsertPtr(new HTMLTag("p",360,false));


	// Low level tags
	Tags.InsertPtr(new HTMLTag("sup",500,false));
	Tags.InsertPtr(new HTMLTag("sub",500,false));
	Tags.InsertPtr(new HTMLTag("br",500,true));
	Tags.InsertPtr(new HTMLTag("nobr",500,false));
	Tags.InsertPtr(new HTMLTag("hr",500,true));
	Tags.InsertPtr(new HTMLTag("font",500,false));
	Tags.InsertPtr(new HTMLTag("pre",500,false));
	Tags.InsertPtr(new HTMLTag("img",500,true)); //? Depreciated
	Tags.InsertPtr(new HTMLTag("object",500,false));
	Tags.InsertPtr(new HTMLTag("param",500,false));
	Tags.InsertPtr(new HTMLTag("embed",500,false));
	Tags.InsertPtr(new HTMLTag("noembed",500,false));
	Tags.InsertPtr(new HTMLTag("map",500,false));
	Tags.InsertPtr(new HTMLTag("area",500,false));
	Tags.InsertPtr(new HTMLTag("center",500,false));
	Tags.InsertPtr(new HTMLTag("em",500,false));
	Tags.InsertPtr(new HTMLTag("strong",500,false));
	Tags.InsertPtr(new HTMLTag("dfn",500,false));
	Tags.InsertPtr(new HTMLTag("code",500,false));
	Tags.InsertPtr(new HTMLTag("samp",500,false));
	Tags.InsertPtr(new HTMLTag("kbd",500,false));
	Tags.InsertPtr(new HTMLTag("var",500,false));
	Tags.InsertPtr(new HTMLTag("cite",500,false));
	Tags.InsertPtr(new HTMLTag("abbr",500,false));
	Tags.InsertPtr(new HTMLTag("acronym",500,false));
	Tags.InsertPtr(new HTMLTag("i",500,false));
	Tags.InsertPtr(new HTMLTag("tt",500,false));
	Tags.InsertPtr(new HTMLTag("big",500,false));
	Tags.InsertPtr(new HTMLTag("small",500,false));
	Tags.InsertPtr(new HTMLTag("strike",500,false));
	Tags.InsertPtr(new HTMLTag("b",500,false));
	Tags.InsertPtr(new HTMLTag("u",500,false));  //Depreciated
	Tags.InsertPtr(new HTMLTag("s",500,false));  //Depreciated
	Tags.InsertPtr(new HTMLTag("applet",500,false));  //Depreciated
	Tags.InsertPtr(new HTMLTag("a",500,false));
	Tags.InsertPtr(new HTMLTag("blockquote",500,false));
	Tags.InsertPtr(new HTMLTag("q",500,false));
	Tags.InsertPtr(new HTMLTag("script",500,false));
	Tags.InsertPtr(new HTMLTag("noscript",500,false));
}


//------------------------------------------------------------------------------
char RXMLParser::GetCurrentDepth(void) const
{
	if(Mode!=RIO::Read)
		ThrowRIOException(this,"File not in read mode");
	return(CurDepth);
}


//------------------------------------------------------------------------------
size_t RXMLParser::GetLastTokenPos(void) const
{
	if(Mode!=RIO::Read)
		ThrowRIOException(this,"File not in read mode");
	return(LastTokenPos);
}


//------------------------------------------------------------------------------
void RXMLParser::InitParser(void)
{
	TreatEncoding=false;
	CurHTMLTag=0;
	Namespaces.Clear();
	DefaultNamespace.Clear();
	Namespace* ptr=Namespaces.GetInsertPtr("xmlns");
	ptr->URI.Push(new RString("http://www.w3.org/2000/xmlns"));
	FoundClosingHTML=false;
	if(HTMLMode)
	{
		Namespace* ptr=Namespaces.GetInsertPtr("xml");
		ptr->URI.Push(new RString("http://www.w3.org/1999/xhtml"));
	}
}


//------------------------------------------------------------------------------
void RXMLParser::SetHTMLMode(bool html)
{
	HTMLMode=html;
	SetInvalidXMLCodes(html);
}


//------------------------------------------------------------------------------
void RXMLParser::Open(RIO::ModeType mode)
{
	RTextFile::Open(mode);

	// Create default namespaces
	InitParser();

	// Treat the different modes
	switch(Mode)
	{
		case RIO::Read:
			CurDepth=0;
			LoadHeader();
			TreatEncoding=false;
			Section=Body;
			LoadNextTag();
			CurDepth=0;
			break;

		case RIO::Append:
		case RIO::Create:
			ThrowRIOException(this,"A XML Parser is read-only");
			break;

		default:
			ThrowRIOException(this,"No Valid Mode");
	};
}


//------------------------------------------------------------------------------
void RXMLParser::Open(const RURI& uri,RIO::ModeType mode,const RCString& encoding)
{
	RTextFile::Open(uri,mode,encoding);
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
			Code.SetLen(0);
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
					if(InvalidXMLCodes)
						return(Code);
					ThrowRIOException(this,"Invalid XML/HTML code \""+Code+"\"");
				}
				else
					res+=What;

				// Skip ;
				if(!Cur.End())
					Cur.Next();
			}
			else
			{
				if(InvalidXMLCodes)
					return(Code);
				ThrowRIOException(this,"Invalid XML/HTML code \""+Code+"\"");
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
const RXMLParser::HTMLTag* RXMLParser::GetHTMLTag(const RString& name) const
{
	HTMLTag* tag(Tags.GetPtr(name));
	if(!tag)
		ThrowRIOException(this,"Unknown HTML Tag '"+name+"'.");
	return(tag);
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
			ThrowRIOException(this,"Wrong XML header");
		SkipSpaces();

		// Search for parameters until ?> is found
		bool PopDefault;
		RContainer<Namespace,false,false> PopURI(5);
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
		LoadAttributes(PopDefault,PopURI,'?','>');

		// Skip '>' and the spaces after that
		Next(); SkipSpaces();
	}


	// Search after "!DOCTYPE"
	if(CurString("<!"))
	{
		Section=DOCTYPE;
		if(!CurString("DOCTYPE",false))
			ThrowRIOException(this,"Wrong DOCTYPE command");

		RString Content(GetToken(">["));
		SetDocType(XMLToString(Content));
		SkipSpaces();

		if(Content=="HTML")
		{
			// Skip Everything until end of the header
			while((!End())&&(GetNextChar()!='>'))
				GetChar();
		}
		else
		{
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
							ThrowRIOException(this,"Wrong entities formating");

						RString ns(GetWord());
						Cur=GetChar();
						if(Cur!='"')
							ThrowRIOException(this,"Wrong entities formating");
						RString uri(GetToken(RString('"')));
						Cur=GetChar();
						if(Cur!='"')
							ThrowRIOException(this,"Wrong entities formating");

						// Add the entity
						AddEntity(ns,uri);

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
						ThrowRIOException(this,"Wrong entities formating");

					// Skip ] and spaces
					Next();
					SkipSpaces();
					Cont=true;
				}
				else if(CurString("SYSTEM"))
				{
					SkipSpaces();
					RChar What=GetChar();
					bool Quotes=((What==RChar('\''))||(What==RChar('"')));
					if(!Quotes)
						ThrowRIOException(this,"Bad XML file");
					SetDTD(GetToken(RString(What)));
					SkipSpaces();
					if(GetChar()!=What)
						ThrowRIOException(this,"Bad XML file");
					SkipSpaces();
					Cont=true;
				}
				else
				{
					// Perhaps a string defining directly the DTD
					RChar What=GetNextChar();
					bool Quotes=((What==RChar('\''))||(What==RChar('"')));
					if(Quotes)
					{
						Next();  // Skip the quote
						SetDTD(GetToken(RString(What)));
						SkipSpaces();
						if(GetChar()!=What)
							ThrowRIOException(this,"Bad XML file");
						SkipSpaces();
						Cont=true;
					}
				}
			}
		}
		if(GetChar()!='>') // Skip >
			ThrowRIOException(this,"Bad XML file");
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
	bool Resolve(false);

	// If not a tag -> Error
	if(GetChar()!='<')
		ThrowRIOException(this,"Not a tag");

	// Read name of the tag
	LastTokenPos=GetPos();
	size_t OpenTagLine(GetLineNb());
	TagName=GetToken(">/");
	CurTagClosing=false;  // Suppose it is not a closing tag

	// Treat the tag
	// Search if it has a namespace
	RString uri,lname;
	RString prefix;
	int i=TagName.Find(':');
	if(i!=-1)
	{
		prefix=TagName.Mid(0,i);
		Namespace* ptr=Namespaces.GetPtr(prefix);
		if(!ptr)
			Resolve=true;       // Search after the attribute
		else
			uri=(*ptr->URI());
		lname=TagName.Mid(i+1,TagName.GetLen()-i);
	}
	else
	{
		if(DefaultNamespace.GetNb())
			uri=(*DefaultNamespace()); // Default namespace.
		else
			Resolve=true;    // Search after the attribute
		lname=TagName;
	}

	if(HTMLMode)
	{
		lname=lname.ToLower();
		TagName=TagName.ToLower();

		// Valid HTML Tag?
		CurHTMLTag=Tags.GetPtr(lname);
		if(!CurHTMLTag)
			ThrowRIOException(this,"Unknown opening HTML Tag '"+lname+"'.");
	}
	BeginTag(uri,lname,TagName);
	if(HTMLMode)
	{
		// Verify that is not a single tag in HTML
		if(CurHTMLTag->Single)
			CurTagClosing=true;
		CurHTMLTag=0;
	}

	// Read Attributes
	LoadAttributes(PopDefault,PopURI);
	if(Resolve)
	{
		if(!prefix.IsEmpty())
		{
			Namespace* ptr=Namespaces.GetPtr(prefix);
			if(!ptr)
				ThrowRIOException(this,"Namespace with prefix '"+prefix+"' no defined");
			uri=(*ptr->URI());
			ResolveNamespace(uri);
		}
		else if(DefaultNamespace.GetNb())
		{
			uri=(*DefaultNamespace()); // Default namespace.
			ResolveNamespace(uri);
		}
	}
	BeginTagParsed(uri,lname,TagName);

	// If the parser does not now if it is a closing tag -> test it.
	if(!CurTagClosing)
	{
		// It is a closing tag? -> Skip / and >
		CurTagClosing=(Cur==RChar('/'));
		if(CurTagClosing)
			Next();  // Skip / from "/>"
	}
	SkipSpaces();

	// Verify if it has sub-tags
	if(CurTagClosing)
	{
		if(HTMLMode)
		{
			lname=lname.ToLower();
			TagName=TagName.ToLower();

			// Valid HTML Tag?
			CurHTMLTag=Tags.GetPtr(lname);
			if(!CurHTMLTag)
				ThrowRIOException(this,"Unknown closing HTML Tag '"+lname+"'.");
		}
		EndTag(uri,lname,TagName);
		if(HTMLMode)
		{
			CurHTMLTag=0;
			if(lname=="html")
				FoundClosingHTML=true;
		}
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

				while(!CurString("]]>",false))
				{
					if(End())
						ThrowRIOException(this,"Invalid '[CDATA[]]>'.");
					LastTokenPos=GetPos();
					RString tmp(GetTokenString("]]>"));
					if(tmp.GetLen())
						Text(tmp);

					// Add all spaces
					while(GetNextChar().IsSpace())
						Text(RString(GetChar()));
				}
				Text("]]>");
				SetParseSpace(RTextFile::SkipAllSpaces);
				SkipSpaces();
			}
			else
			{
				CurDepth++;   	// Increase the depth
				LoadNextTag();
				CurDepth--;   	// Decrease the depth
			}
		}
		else
		{
			// It is content -> read it as long as there is no open tag.
			Contains.SetLen(0);
			CDATA=true; // Suppose that first '<' found is a "<![CDATA["
			SetParseSpace(RTextFile::LeaveSpaces);
			while(CDATA)
			{
				// Get the next word
				LastTokenPos=GetPos();
				RString tmp(GetToken("<"));
				if(tmp.GetLen())
					Text(XMLToString(tmp));

				// Add all spaces
				while(GetNextChar().IsSpace())
					Text(RString(GetChar()));

				// Look if the next '<' is the beginning of "<![CDATA["
				CDATA=CurString("<![CDATA[",true);
				if(CDATA)
				{
					Text("<![CDATA[");
					SkipSpaces();
					SetParseSpace(RTextFile::LeaveSpaces);

					while(!CurString("]]>"))
					{
						if(End())
							ThrowRIOException(this,"Invalid '[CDATA[]]>'.");
						LastTokenPos=GetPos();
						RString tmp(GetTokenString("]]>"));
						if(tmp.GetLen())
							Text(tmp);

						// Add all spaces
						while(GetNextChar().IsSpace())
							Text(RString(GetChar()));
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
	RString EndTagName(GetToken(RString('>')));
	if((EndTagName!=TagName))
		ThrowRIOException(this,"Found closing tag '"+EndTagName+"' while closing tag '"+TagName+"' ("+RString::Number(OpenTagLine)+") was expected.");
	SkipSpaces();
	if(HTMLMode&&(lname=="html"))
			FoundClosingHTML=true;
	else
		Next(); // Skip >
	if(HTMLMode)
	{
		lname=lname.ToLower();
		TagName=TagName.ToLower();

		// Valid HTML Tag?
		CurHTMLTag=Tags.GetPtr(lname);
		if(!CurHTMLTag)
			ThrowRIOException(this,"Unknown closing HTML Tag '"+lname+"'.");
	}
	EndTag(uri,lname,TagName);
	if(HTMLMode)
		CurHTMLTag=0;
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
//	size_t NbAttrs(0);

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
		RString attrn=GetToken("=>");

		// Verify if the attribute name has a namespace
		int i=attrn.Find(':');
		if(i!=-1)
		{
			// Namespace defined
			RString prefix=attrn.Mid(0,i);
			lname=attrn.Mid(i+1,attrn.GetLen()-i-1);
			if(prefix=="xmlns")
			{
				uri="http://www.w3.org/2000/xmlns";  // New namespace declared
				GetNs=true;
			}
			else
			{
				Namespace* ptr=Namespaces.GetPtr(prefix);
				if(!ptr)
					ThrowRIOException(this,"Namespace with prefix '"+prefix+"' no defined");
				uri=(*ptr->URI());
			}
		}
		else
		{
			lname=attrn;
			if(attrn=="xmlns")
			{
				uri="http://www.w3.org/2000/xmlns";  // New namespace declared
				GetNs=true;
			}
			else
			{
				if(DefaultNamespace.GetNb())
					uri=(*DefaultNamespace()); // Default namespace.
			}
		}

		// Add the attribute
		if(Section==Header)
			HeaderAttribute(uri,lname,attrn);
		else
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
					RString tmp(GetToken(RString(What)));
					if(tmp.GetLen())
					{
						if(Section==Header)
							HeaderValue(XMLToString(tmp));
						else
							Value(XMLToString(tmp));
					}
					if(GetNs)
						ns+=tmp;

					// Add all spaces
					while(GetNextChar().IsSpace())
					{
						if(Section==Header)
							HeaderValue(tmp);
						else
							Value(RString(GetChar()));
					}
				}
				SetParseSpace(RTextFile::SkipAllSpaces);
				Next();
			}
			else
			{
				// If Quote must be used -> generate an exception
				if(OnlyQuote())
					ThrowRIOException(this,"Quote must be used to delimit the parameter value in a tag.");
				RString tmp(What+GetToken(">"));
				if(Section==Header)
					HeaderValue(tmp);
				else
					Value(tmp);
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
void RXMLParser::HeaderAttribute(const RString&,const RString& lName,const RString&)
{
	if(lName=="encoding")
		TreatEncoding=true;
	else
		TreatEncoding=false;
}


//------------------------------------------------------------------------------
void RXMLParser::HeaderValue(const RString& value)
{
	if(TreatEncoding)
		SetEncoding(value.Latin1());
}


//------------------------------------------------------------------------------
void RXMLParser::SetDocType(const RString& docType)
{
	DocType=docType;
}


//------------------------------------------------------------------------------
void RXMLParser::AddEntity(const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::BeginTag(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::BeginTagParsed(const RString&,const RString&,const RString&)
{
}


//------------------------------------------------------------------------------
void RXMLParser::ResolveNamespace(const RString&)
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
RXMLParser::~RXMLParser(void)
{
}
