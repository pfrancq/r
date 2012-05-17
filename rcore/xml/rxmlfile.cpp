/*

	R Project Library

	RXMLFile.cpp

	XML file - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
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
#include <rxmlfile.h>
#include <rnodecursor.h>
#include <rstringbuilder.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RXMLFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLFile::RXMLFile(void)
 : RXMLParser(), XMLStruct(0), NewStruct(false), CurTag(0)
{
}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RURI& uri,RXMLStruct* xmlstruct,const RCString& encoding)
 : RXMLParser(uri,encoding), XMLStruct(xmlstruct), NewStruct(false), CurTag(0)
{
	XMLStruct->SetEncoding(encoding);
}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RCString& encoding)
 : RXMLParser(file,encoding), XMLStruct(xmlstruct), NewStruct(false),CurTag(0)
{
	XMLStruct->SetEncoding(encoding);
}


//------------------------------------------------------------------------------
void RXMLFile::SetEncoding(const RCString& name)
{
	XMLStruct->SetEncoding(name);
	RXMLParser::SetEncoding(name);
}


//------------------------------------------------------------------------------
void RXMLFile::Open(RIO::ModeType mode)
{
	RTextFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			CurTag=0;
			CurAttr=0;
			if(!XMLStruct)
			{
				XMLStruct=new RXMLStruct();
				NewStruct=true;
			}
			else
				XMLStruct->Clear(); // Make sure the xml structure is empty
			RXMLParser::Open(mode);
			break;

		case RIO::Append:
		case RIO::Create:
		{
			if(!XMLStruct)
				throw RIOException("A XML structure must be assign");
			RXMLTag* top(XMLStruct->GetTop());
			RStringBuilder Header;
			Header+="<?xml";
			if(!XMLStruct->GetVersion().IsEmpty())
				Header+=" version=\""+XMLStruct->GetVersion()+"\"";
			if(!XMLStruct->GetEncoding().IsEmpty())
				Header+=" encoding=\""+XMLStruct->GetEncoding()+"\"";
			Header+="?>";
			(*this)<<Header();
			if(!MustAvoidSpaces())
				(*this)<<endl;
			Header.Clear();
			Header+="<!DOCTYPE "+top->GetName();
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
				if(!MustAvoidSpaces())
					Header+="\n";
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					if(!MustAvoidSpaces())
						Header+="\t";
					else
						Header+=" ";
					Header+="<!ENTITY "+Cur()->GetName()+" \""+Cur()->GetValue()+"\">";
					if(!MustAvoidSpaces())
						Header+="\n";
				}
				Header+=" ]>";
			}
			else
				Header+=">";
			(*this)<<Header();
			if(!MustAvoidSpaces())
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
void RXMLFile::Open(const RURI& uri,RXMLStruct* xmlstruct,RIO::ModeType mode,const RCString& encoding)
{
	XMLStruct=xmlstruct;
	RXMLParser::Open(uri,mode,encoding);
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
	RXMLParser::Close();
}


//------------------------------------------------------------------------------
void RXMLFile::SaveNextTag(int depth)
{
	RStringBuilder line;
	RXMLTag* Current(CurTag);

	if(!MustAvoidSpaces())
		for(int i=0;i<depth;i++) line+="\t";

	// Write the first line : <tag attrs> or <tag attrs/>
	line+="<"+CurTag->GetName();
	if(CurTag->GetNbAttrs())
	{
		// Add attributes
		RCursor<RXMLAttr> Cur(CurTag->GetAttrs());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			line+=" "+Cur()->GetName();
			RString Value(Cur()->GetValue());
			if(!Value.IsEmpty())
				line+="=\""+StringToXML(Value)+"\"";
		}
	}
	if(CurTag->GetNbNodes()||CurTag->HasContent())
		line+=">"; // There is some content -> add '>'
	else
		line+="/>"; // No content -> add '/>'
	(*this)<<line()<<endl; // Write the line
	line.Clear();        // Start a new line

	// Wrote the content (if any)
	if(CurTag->HasContent())
	{
		if(!MustAvoidSpaces())
			for(int i=0;i<depth+1;i++) line+="\t";
		line+=StringToXML(CurTag->GetContent().Trim(),false);
		(*this)<<line()<<endl;
		line.Clear();
	}

	// Write the tags (if any)
	if(CurTag->GetNbNodes())
	{
		RNodeCursor<RXMLStruct,RXMLTag> Tags(CurTag);
		for(Tags.Start();!Tags.End();Tags.Next())
		{
			CurTag=Tags();
			if(CurTag->GetParent()!=Current)
				cerr<<"RXMLFile::SaveNextTag(int) : Big Problem, parent '"<<
						                Current->GetName()+"' expected and '"<<CurTag->GetParent()->GetName()<<
						                "' found for tag '"+CurTag->GetName()<<"'"<<endl;
			SaveNextTag(depth+1);
			if(CurTag->GetParent()!=Current)
				cerr<<"RXMLFile::SaveNextTag(int) : Big Problem, parent '"<<
						                Current->GetName()+"' expected and '"<<CurTag->GetParent()->GetName()<<
						                "' found for tag '"+CurTag->GetName()<<"'"<<endl;
		}
	}

	if(Current->GetNbNodes()||Current->HasContent())
	{
		if(!MustAvoidSpaces())
			for(int i=0;i<depth;i++) line+="\t";
		line+="</"+Current->GetName()+">";
		(*this)<<line()<<endl;
	}

	CurTag=Current;
}


//------------------------------------------------------------------------------
void RXMLFile::AddEntity(const RString& name,const RString& value)
{
	XMLStruct->InsertEntity(name,value);
}


//------------------------------------------------------------------------------
void RXMLFile::BeginTag(const RString& namespaceURI, const RString&, const RString& name)
{
	RXMLTag* tag;

	// Create the tag and add it to the XML structure
	tag=XMLStruct->NewTag(name,namespaceURI);
	XMLStruct->AddTag(CurTag,tag);

	// If no top tag -> insert it
	if(!XMLStruct->GetTop())
	{
		// Test if is a XML document (DocType=true) or a HTML document (DocType=false)
		if(!GetDocType().IsEmpty())
		{
			// Name of the Tag must be name of the DocType
			if(tag->GetName()!=GetDocType())
				throw RIOException(this,"Not a valid XML file");
		}
		else
		{
			RString TopName=tag->GetName().ToLower();

			// Is it a HTML file?
			// -> If Not, consider the first tag is the DOCTYPE
			if(TopName!="html")
				SetDocType(TopName);
				//throw RIOException(this,"Not a valid HTML file");
		}
	}

	// Make the tag the current one
	CurTag=tag;
}


//------------------------------------------------------------------------------
void RXMLFile::ResolveNamespace(const RString& namespaceURI)
{
	CurTag->SetNamespace(XMLStruct->GetNamespace(namespaceURI));
}


//------------------------------------------------------------------------------
void RXMLFile::AddAttribute(const RString& namespaceURI,const RString& lName, const RString&)
{
	if(CurTag)
		CurTag->InsertAttr(CurAttr=XMLStruct->NewAttr(lName,namespaceURI));
	else
		AttrName=lName;
}


//------------------------------------------------------------------------------
void RXMLFile::Value(const RString& value)
{
	if(CurAttr)
		CurAttr->AddValue(value);
	else
	{
		if(AttrName=="version")
			XMLStruct->SetVersion(value);
		if(AttrName=="encoding")
			SetEncoding(value.Latin1());
	}
}


//------------------------------------------------------------------------------
void RXMLFile::EndTag(const RString&, const RString&, const RString&)
{
	CurTag=CurTag->GetParent();
}


//------------------------------------------------------------------------------
void RXMLFile::Text(const RString& text)
{
	CurTag->AddContent(text);
}


//------------------------------------------------------------------------------
void RXMLFile::SetDTD(const RString& dtd)
{
	if(XMLStruct)
		XMLStruct->SetDTD(dtd);
}


//------------------------------------------------------------------------------
RXMLFile::~RXMLFile(void)
{
}
