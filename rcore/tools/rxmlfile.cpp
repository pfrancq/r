/*

	R Project Library

	RXMLFile.cpp

	XML file - Implementation.

	Copyright 2000-2004 by the Université Libre de Bruxelles.

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
#include <rstd/rxmlfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RXMLFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RString& name,RXMLStruct* xmlstruct,ModeType mode,const RString& encoding) throw(bad_alloc,RIOException)
 : RTextFile(name,mode,encoding), XMLStruct(xmlstruct), CurTag(0)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");

}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RString& name,RXMLStruct& xmlstruct,ModeType mode,const RString& encoding) throw(bad_alloc,RIOException)
 : RTextFile(name,mode,encoding), XMLStruct(&xmlstruct), CurTag(0)
{
	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");

}


//------------------------------------------------------------------------------
void RXMLFile::SetEncoding(const RString& name) throw(RIOException)
{
	XMLStruct->SetEncoding(name);
	RTextFile::SetEncoding(name);
}


//------------------------------------------------------------------------------
void RXMLFile::SetDocType(const RString& docType) throw(RIOException)
{
	DocType=docType;
}


//------------------------------------------------------------------------------
void RXMLFile::Process(void) throw(RIOException)
{
	RXMLTag *top;
	RXMLAttrCursor Cur;
	RString Header;

	switch(Mode)
	{
		case Read:
			LoadHeader();
			LoadNextTag();
			break;

		case Append:
		case Create:
			top = XMLStruct->GetTop();
			Header="<?xml version=\""+XMLStruct->GetVersion()+"\" encoding=\""+XMLStruct->GetEncoding()+"\"?>";
			(*this)<<Header<<endl;
			Header="<!DOCTYPE "+top->GetName();
			Cur=XMLStruct->GetXMLEntitiesCursor();
			if(Cur.GetNb())
			{
				Header+="[\n";
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					Header+="\t<!ENTITY "+Cur()->GetName()+" \""+Cur()->GetValue()+"\'>\n";
				}
				Header+=" ]>";
			}
			else
				Header+=">";
			(*this)<<Header<<endl;
			CurTag=top;
			SaveNextTag(0);
			break;

		default:
			throw(RString("No Valid Mode"));
	};
}


//------------------------------------------------------------------------------
RChar RXMLFile::CodeToChar(RString& str)
{
	RChar res;

	if(!str.Compare("quo"))
		res='"';
	if(!str.Compare("amp"))
		res='&';
	if(!str.Compare("apos"))
		res='\'';
	if(!str.Compare("lt"))
		res='<';
	if(!str.Compare("gt"))
		res='>';

	if(!res.IsNull())
		return(res);
	else
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
RString RXMLFile::XMLToString(const RString& str) throw(RIOException)
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

	for(Cur.Start();!Cur.End();Cur.Next())
	{
		res+=CharToCode(Cur(),strict);
	}
	return(res);
}


//------------------------------------------------------------------------------
void RXMLFile::LoadHeader(void) throw(RIOException)
{
	RString Content;
	RContainer<RXMLAttr,true,true> Attrs(10);
	RXMLAttr* Attr;

	// Skip Spaces and comments
	SkipSpaces();

	// Search after "<? xml"
	if(CurString("<?"))
	{
		// Skip <?
		Next(); Next();

		// Search for parameters until ?> is found
		LoadAttributes(Attrs,'?','>');
		if((Attr=Attrs.GetPtr<const char*>("version")))
			XMLStruct->SetVersion(Attr->GetValue());
		if((Attr=Attrs.GetPtr<const char*>("encoding")))
			SetEncoding(Attr->GetValue());

		// Skip ?>
		Next(); Next();
	}

	// Skip Spaces
	SkipSpaces();

	// Search after "!DOCTYPE"
	if(CurString("<!"))
	{
		Next(); Next(); // Skip <?
		if(!CurString("DOCTYPE",false))
			throw RIOException(this,"Wrong DOCTYPE command");

		// Skip DOCTYPE
		for(int i=8;--i;)
			Next();

		// Read the DocType
		SkipSpaces();
		while((!Cur.IsNull())&&(Cur!=RChar('>'))&&(!Cur.IsSpace()))
		{
			Content+=Cur;
			Next();
		}
		SetDocType(XMLToString(Content));
		while((!Cur.IsNull())&&(Cur!=RChar('>')))
			Next();
		Next();
	}

	// Skip Spaces and comments
	SkipSpaces();
}


//------------------------------------------------------------------------------
void RXMLFile::LoadNextTag(void) throw(RIOException)
{
	RString attrn,attrv;
	RString TagName;
	RString Contains;
	RContainer<RXMLAttr,true,true> Attrs(10);
	RChar What;
	RString Code;
	bool CDATA;

	// If not a tag -> Error
	try
	{
		if((Cur!=RChar('<'))||(GetNextCur()==RChar('/')))
			throw RIOException(this,"Not a tag");
	}
	catch(InvalidFile e)
	{
		throw RIOException(this,e.GetMsg());
	}
	catch(exception e)
	{
		throw RIOException(this,e.what());
	}
	catch(RException e)
	{
		throw RIOException(this,e.GetMsg());
	}
	catch(...)
	{
		throw RIOException(this,"?");
	}

	// Read name of the tag
	Next(); // Skip <
	SkipSpaces();
	while((!Cur.IsNull())&&(!Cur.IsSpace())&&(Cur!=RChar('>'))&&(Cur!=RChar('/')))
	{
		TagName+=Cur;
		Next();
	}
	SkipSpaces();

	// Read Attributes
	LoadAttributes(Attrs);

	// It is a closing tag?
	CurTagClosing=(Cur==RChar('/'));
	if(CurTagClosing)
		Next();

	// Treat the tag
	BeginTag("","",TagName,Attrs);
	// Verify if it has sub-tags
	if(CurTagClosing)
	{
		EndTag("","",TagName);
		Next(); // Skip >
		SkipSpaces();
		return;
	}
	// Treat sub-tags
	Next();
	SkipSpaces();
	while((!Cur.IsNull())&&((Cur!=RChar('<'))||(GetNextCur()!=RChar('/'))))
	{
		if((Cur==RChar('<'))&&(GetNextCur()!=RChar('/')))
		{
			// It is a tag -> read it
			LoadNextTag();
		}
		else
		{
			// It is content -> read it as long as there is no open tag.
			Contains.Clear();
			CDATA=true; // Suppose that first '<' found is a "<![CDATA["
			while(CDATA)
			{
				// Read text until '<'
				while((!Cur.IsNull())&&(Cur!=RChar('<')))
					AddNextCharacter(Contains);

				// Look if the next '<' is the beginning of "<![CDATA["
				CDATA=CurString("<![CDATA[",true);
				if(CDATA)
				{
					// Skip <![CDATA[
					for(int i=10;--i;)
						Next();

					// Read until ']]>' is found
					while((!Cur.IsNull())&&(!CurString("]]>")))
						AddNextCharacter(Contains);

					// Skip ]]>
					for(int i=4;--i;)
						Next();
				}
			}
			Text(XMLToString(Contains));
			SkipSpaces();
		}
	}

	// Read the close tag
	CurTagClosing=true;
	Next(); Next();  // Normal character
	TagName.Clear();
	SkipSpaces();
	while((!Cur.IsNull())&&(!Cur.IsSpace())&&(Cur!=RChar('>')))
	{
		TagName+=Cur;
		Next();
	}
	SkipSpaces();
	Next(); // Skip >
	EndTag("","",TagName);
	SkipSpaces();
}


//------------------------------------------------------------------------------
void RXMLFile::LoadAttributes(RContainer<RXMLAttr,true,true>& attrs,RChar EndTag1,RChar EndTag2) throw(RIOException)
{
	RString attrn,attrv;
	RChar What;
	bool Quotes;

	while((!Cur.IsNull())&&(Cur!=EndTag1)&&(Cur!=EndTag2))
	{
		// Read the Name
		attrn.Clear();
		while((!Cur.IsNull())&&(!Cur.IsSpace())&&(Cur!=RChar('=')))
		{
			attrn+=Cur;
			Next();
		}

		// Determine if a value is assign
		SkipSpaces();
		if(Cur==RChar('='))
		{
			// A value is assigned
			Next();  // Skip =
			SkipSpaces();

			// Determine if the parameter is delimited by quotes
			What=Cur;
			Quotes=((What==RChar('\''))||(What==RChar('"')));

			// Read the parameter
			attrv.Clear();
			if(Quotes)
			{
				Next(); // Skip the quote
				// Read until the next quote is found
				while((!Cur.IsNull())&&(Cur!=What))
				{
					attrv+=Cur;
					Next();
				}
				Next();
			}
			else
			{
				// If Quote must be used -> generate an exeception
				if(OnlyQuote())
					throw RIOException(this,"Quote must be used to delimit the parameter value in a tag.");

				// Read until a space or the end of the tag
				while((!Cur.IsNull())&&(!Cur.IsSpace())&&(!  (((Cur==EndTag1)&&(GetNextCur()==EndTag2)) || (Cur==EndTag2))))
				{
					attrv+=Cur;
					Next();
				}
			}
			attrs.InsertPtr(new RXMLAttr(attrn,XMLToString(attrv)));
			SkipSpaces();
		}
		else
			attrs.InsertPtr(new RXMLAttr(attrn,""));
	}
}


//------------------------------------------------------------------------------
void RXMLFile::SaveNextTag(int depth) throw(RIOException)
{
	int i;
	RString text;
	RXMLAttrCursor Cur;
	RXMLTagCursor Tags;
	RString line;

	for(int i=0;i<depth;i++) line+="\t";
	Cur=CurTag->GetXMLAttrCursor();
	Tags=CurTag->GetXMLTagsCursor();
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
		for(int i=0;i<depth;i++) line+="\t";
		line+="</"+CurTag->GetName()+">";
		(*this)<<line<<endl;
	}
}


//------------------------------------------------------------------------------
void RXMLFile::AddNextCharacter(RString& str) throw(RIOException)
{
	// If it is an eol character, skip it with the SkipEol
	if(RTextFile::Eol(Cur))
	{
		str+='\n';
		SkipEol();
	}
	else
	{
		// Normal character
		str+=Cur;
		Next();
	}

}


//------------------------------------------------------------------------------
void RXMLFile::BeginTag(const RString&, const RString&, const RString& name,RContainer<RXMLAttr,true,true>& attrs) throw(RIOException)
{
	RXMLTag* tag;
	RXMLAttrCursor Cur;

	// Create the tag
	tag=new RXMLTag(name);
	Cur.Set(attrs);
	for(Cur.Start();!Cur.End();Cur.Next())
		tag->InsertAttr(Cur()->GetName(),Cur()->GetValue());

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

	// Add the tag the XML structure and make it the current tag
	XMLStruct->AddTag(CurTag,tag);
	CurTag=tag;
}


//------------------------------------------------------------------------------
void RXMLFile::EndTag(const RString&, const RString&, const RString& name) throw(RIOException)
{
	if(CurTag->GetName()!=name)
		throw RIOException(this,"Found closing tag '"+name+"' while closing tag '"+CurTag->GetName()+"' was expected.");
	CurTag=CurTag->GetParent();
}


//------------------------------------------------------------------------------
void RXMLFile::Text(const RString& text) throw(RIOException)
{
	RCharCursor Cur(text);

	while((!Cur.End())&&(Cur().IsSpace()))
		Cur.Next();
	if(Cur.End())
		return;
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
