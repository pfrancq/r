/*

    R Project Library

    RXMLFile.cpp

    XML file - Implementation.

	(c) 2000-2001 by P. Francq and T. L'Eglise.

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



//-----------------------------------------------------------------------------
// include files for R PRoject
#include <rxml/rxmlfile.h>
using namespace RXML;
using namespace RIO;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class RXMLFile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RXML::RXMLFile::RXMLFile(const char *name,RXMLStruct *xmlstruct,ModeType mode)
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
					sprintf(tmp,"\t<!ENTITY %s \"%s\">",(Cur()->GetName())(),(Cur()->GetValue())());
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


//-----------------------------------------------------------------------------
bool RXML::RXMLFile::BeginTag(void)
{
	return(((*ptr)=='<')&&((*(ptr+1))!='/'));
}


//-----------------------------------------------------------------------------
bool RXML::RXMLFile::EndTag(void)
{
	return(((*ptr)=='<')&&((*(ptr+1))=='/'));
}


//-----------------------------------------------------------------------------
char* RXML::RXMLFile::GetTag(void)
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


//-----------------------------------------------------------------------------
char* RXML::RXMLFile::GetContains(void)
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


//-----------------------------------------------------------------------------
RXML::RXMLFile::~RXMLFile()
{
}
