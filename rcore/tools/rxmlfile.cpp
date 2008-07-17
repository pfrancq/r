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
// class RXMLFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RURI& uri,RXMLStruct* xmlstruct,const RString& encoding)
 : RXMLParser(uri,encoding), XMLStruct(xmlstruct), NewStruct(false), CurTag(0)
{
}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const RURI& uri,RXMLStruct& xmlstruct,const RString& encoding)
 : RXMLParser(uri,encoding), XMLStruct(&xmlstruct), NewStruct(false),CurTag(0)
{
}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RString& encoding)
 : RXMLParser(file,encoding), XMLStruct(xmlstruct), NewStruct(false),CurTag(0)
{
}


//------------------------------------------------------------------------------
RXMLFile::RXMLFile(RIOFile& file,RXMLStruct& xmlstruct,const RString& encoding)
 : RXMLParser(file,encoding), XMLStruct(&xmlstruct), CurTag(0)
{
}


//------------------------------------------------------------------------------
void RXMLFile::SetEncoding(const RString& name)
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
		line+=StringToXML(CurTag->GetContent().Trim(),false);
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
			SetEncoding(value);
	}
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
void RXMLFile::SetDTD(const RString& dtd)
{
	if(XMLStruct)
		XMLStruct->SetDTD(dtd);
}


//------------------------------------------------------------------------------
RXMLFile::~RXMLFile(void)
{
}
