/*

	R Project Library

	RXMLFile.cpp

	XML file - Implementation.

	Copyright 2000-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

	Version $Revision$

	Last Modify: $Date$

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



//------------------------------------------------------------------------------
//
// class RXMLFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLFile::RXMLFile(const char *name,RXMLStruct *xmlstruct,ModeType mode)
 : RTextFile(name,mode), XMLStruct(xmlstruct)
{
	RXMLTag *top;
	RXMLAttrCursor Cur;
	char tmp[500];

	SetRemStyle(MultiLineComment);
	SetRem("<!--","-->");
	switch(Mode)
	{
		case Read:
			while(*ptr)
			{
				SkipSpaces();
				top=new RXMLTag();
				top->Load(this,XMLStruct);
				XMLStruct->AddNode(0,top);
			}
			break;

		case Append:
		case Create:
			top = XMLStruct->Top->Tab[0];
			(*this)<<"<?xml version=\"1.0\"?>"<<endl;
			(*this)<<"<!DOCTYPE"<<top->GetName();
			Cur=XMLStruct->GetXMLEntitiesCursor();
			if(Cur.GetNb())
			{
				(*this)<<"["<<endl;
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					sprintf(tmp,"\t<!ENTITY %s \"%s\">",Cur()->GetName().Latin1(),Cur()->GetValue().Latin1());
					(*this)<<tmp<<endl;
				}
				(*this)<<"  ]>"<<endl;
			}
			else
				(*this)<<">"<<endl;
			top->Save(this,0);
			break;

		default:
			throw(RString("No Valid Mode"));
	};
}


//------------------------------------------------------------------------------
bool RXMLFile::BeginTag(void)
{
	return(((*ptr)=='<')&&((*(ptr+1))!='/'));
}


//------------------------------------------------------------------------------
bool RXMLFile::EndTag(void)
{
	return(((*ptr)=='<')&&((*(ptr+1))=='/'));
}


//------------------------------------------------------------------------------
char* RXMLFile::GetTag(void)
{
	char *tmp=GetCharPtr();
	char c,*ptr2;

	SkipSpaces();
	ptr2=ptr;
	while((*ptr)!='>')
		ptr++;
	ptr++;          // Skip '>'

	// Making ptr2 pointed to a valid C string
	c=(*ptr);
	(*ptr)=0;
	strcpy(tmp,ptr2);
	(*ptr)=c;
	SkipSpaces();

	return(tmp);
}


//------------------------------------------------------------------------------
char* RXMLFile::GetContains(void)
{
	char *tmp=GetCharPtr();
	char c,*ptr2;

	ptr2=ptr;
	while((*ptr)!='<')
		ptr++;

	c=(*ptr);
	(*ptr)=0;
	strcpy(tmp,ptr2);
	(*ptr)=c;

	return(tmp);
}


//------------------------------------------------------------------------------
RXMLFile::~RXMLFile(void)
{
}
