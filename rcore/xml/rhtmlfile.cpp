/*

	R Project Library

	RHTMLFile.cpp

	HTML file - Implementation.

	Copyright 2004-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rhtmlfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RHTMLFile
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(void)
 : RXMLFile()
{
	SetHTMLMode(true);
}


//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(const RURI& uri,RXMLStruct* xmlstruct,const RCString& encoding)
 : RXMLFile(uri,xmlstruct,encoding)
{
	SetHTMLMode(true);
}


//------------------------------------------------------------------------------
RHTMLFile::RHTMLFile(RIOFile& file,RXMLStruct* xmlstruct,const RCString& encoding)
 : RXMLFile(file,xmlstruct,encoding)
{
	SetHTMLMode(true);
}


//------------------------------------------------------------------------------
void RHTMLFile::Open(RIO::ModeType mode)
{
	RXMLFile::Open(mode);
}


//------------------------------------------------------------------------------
void RHTMLFile::Open(const RURI& uri,RXMLStruct* xmlstruct,RIO::ModeType mode,const RCString& encoding)
{
	RXMLFile::Open(uri,xmlstruct,mode,encoding);
}


//------------------------------------------------------------------------------
void RHTMLFile::SetDocType(const RString& docType)
{
	RString name(docType.ToLower());
	if((name=="xhtml")||(name=="html"))
		RXMLFile::SetDocType("html");
	else
		throw RIOException(this,"DOCTYPE of a HTML file must be either HTML or XHTML");
}


//------------------------------------------------------------------------------
void RHTMLFile::BeginTag(const RString& namespaceURI, const RString& lName, const RString& name)
{
	RXMLTag* Search;

	// if HTML closing tag found -> Nothing to do
	if(HasFoundClosingHTML())
		return;

	// If the current tag is deeper than this one -> close it
	if(CurTag)
	{
		const HTMLTag* tag2(GetHTMLTag(CurTag->GetName()));
		if(!tag2)
			throw RIOException(this,"Unknown opening HTML Tag '"+CurTag->GetName()+"'.");
		if((GetCurHTMLTag()->Level<tag2->Level)||((GetCurHTMLTag()==tag2)&&(!GetCurHTMLTag()->SelfContained)&&(GetCurHTMLTag()->Level!=500)))
		{
			// Find the first parent with a higher Level
			Search=CurTag;
			while(Search&&(GetCurHTMLTag()->Level<=tag2->Level))
			{
				Search=Search->GetParent();
				if(Search)
					tag2=GetHTMLTag(Search->GetName());
			}
			if(Search)
				CurTag=Search;
		}

	}

	// Treat the beginning tag
	RXMLFile::BeginTag(namespaceURI,lName,name);
}


//------------------------------------------------------------------------------
void RHTMLFile::EndTag(const RString& namespaceURI, const RString& lName, const RString& name)
{
	RXMLTag* Search;

	// if HTML closing tag found -> Nothing to do
	if(HasFoundClosingHTML())
		return;

	RString HTMLName(lName);

	// A Current tag must exist
	if(!CurTag)
		throw RIOException(this,"No current tag");

	const HTMLTag* tag2(GetHTMLTag(CurTag->GetName()));

	// If the closing tag is the not same as the current open one
	// -> find the right tag to close
	if((GetCurHTMLTag()!=tag2)&&IsCurTagClosing())
	{
		// If the founding tag is deeper that the the one to close -> Skip it
		// Else -> Find the tag to close
		if(GetCurHTMLTag()->Level<tag2->Level)
		{
			// Find the first parent with a higher Level
			Search=CurTag;
			while(Search&&(GetCurHTMLTag()->Level<=tag2->Level))
			{
				Search=Search->GetParent();
				if(Search)
					tag2=GetHTMLTag(Search->GetName());
			}
			if(Search)
				CurTag=Search;
		}
		else
		{
			// The name of the tag to close is the current one.
			HTMLName=CurTag->GetName();
			Search=CurTag;
		}
	}
	else
		Search=0;

	// Treat the ending tag
	if(!Search)
		RXMLFile::EndTag(namespaceURI,HTMLName,name);
}


//------------------------------------------------------------------------------
void RHTMLFile::Text(const RString& text)
{
	// if HTML closing tag found -> Nothing to do
	if(HasFoundClosingHTML())
		return;

	// A Current tag must exist
	if(!CurTag)
		throw RIOException(this,"No current tag");

	RXMLFile::Text(text);
}


//------------------------------------------------------------------------------
bool RHTMLFile::OnlyQuote(void)
{
	return(false);
}


//------------------------------------------------------------------------------
RHTMLFile::~RHTMLFile(void)
{
}
