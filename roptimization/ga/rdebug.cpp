/*

	R Project Library

	RDebug.cpp

	Debugging file in XML format (eXtended Markup Language) - Implementation

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for C/C++ ANSI
#include <stdarg.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rga/rdebug.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RDebug
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDebug::RDebug(const char* app,const char* author)
	: Deep(-1), App(app), Author(author)
{
}


//------------------------------------------------------------------------------
void RDebug::BeginTag(const char* Text,unsigned NbAttr,...)
{
	va_list ap;

	LevelOutput[++Deep]=false;      // For the moment nothing as child
	NbOptions=0;
	(*tmpOpt)=0;
	va_start(ap, NbAttr);
	while(NbAttr--)
		AddAttribute(va_arg(ap,char*),va_arg(ap,char*));
	va_end(ap);
	WriteBeginTag(Text,tmpOpt);
	CurTag=Text;
}


//------------------------------------------------------------------------------
const char* RDebug::GetCurrentTag(void) const
{
	return(CurTag);
}


//------------------------------------------------------------------------------
void RDebug::AddAttribute(const char* Value,const char* Attr)
{
	if(NbOptions++)
		strcat(tmpOpt," ");
	strcat(tmpOpt,Attr);
	strcat(tmpOpt,"=\"");
	strcat(tmpOpt,Value);
	strcat(tmpOpt,"\"");
}


//------------------------------------------------------------------------------
void RDebug::PrintComment(const char* Text)
{
	LevelOutput[Deep]=true;
	WriteText(Text);
}


//------------------------------------------------------------------------------
void RDebug::EndTag(const char* Text)
{
	if(!LevelOutput[Deep])
		WriteText("No Special Information");
	if(Deep) LevelOutput[Deep-1]=true;
	WriteEndTag(Text);
	Deep--;
}


//------------------------------------------------------------------------------
void RDebug::PrintInfo(const char* Text)
{
	BeginTag("Info");
	PrintComment(Text);
	EndTag("Info");
}


//------------------------------------------------------------------------------
void RDebug::BeginFunc(const char* Name,const char* Object)
{
	BeginTag(Name,1,"Object",Object);
}


//------------------------------------------------------------------------------
void RDebug::EndFunc(const char* Name,const char*)
{
	EndTag(Name);
}

                               
//------------------------------------------------------------------------------
void RDebug::BeginApp(void)
{
	time_t t;
	char TempString[50];

	time(&t);
	strcpy(TempString,ctime(&t));
	TempString[strlen(TempString)-1]=0;
	BeginTag(App.Latin1(),2,"Author",Author.Latin1(),"Date",TempString);
}


//------------------------------------------------------------------------------
void RDebug::EndApp(void)
{
	EndTag(App);
}


//------------------------------------------------------------------------------
RDebug::~RDebug(void)
{
}



//------------------------------------------------------------------------------
//
// Class RDebugXML
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDebugXML::RDebugXML(const char* name,const char* app,const char* author) throw(std::bad_alloc)
	: RDebug(app,author),Name(name), File(name,Create)
{
	unsigned int i;
	char *ptr;

	File.WriteStr("<!DOCTYPE "+RString(app)+">");
	File.WriteLine();
	for(i=50,ptr=tmpTab;--i;ptr++)
		(*ptr)='\t';
	(*ptr)=0;
}


//------------------------------------------------------------------------------
void RDebugXML::WriteBeginTag(const char *tag,const char *options)
{
	if(Deep)
	{
		File.WriteLine();
		File.WriteStr(tmpTab,Deep);
	}
	File.WriteStr("<",1);
	File.WriteStr(tag,strlen(tag));
	if(options&&(*options))
	{
		File.WriteStr(" ",1);
		File.WriteStr(options,strlen(options));
	}
	File.WriteStr(">",1);
}


//------------------------------------------------------------------------------
void RDebugXML::WriteEndTag(const char *tag)
{
	if(Deep&&LevelOutput[Deep-1])
	{
		File.WriteLine();
		File.WriteStr(tmpTab,Deep);
	}
	File.WriteStr("</",2);
	File.WriteStr(tag,strlen(tag));
	File.WriteStr(">",1);
}


//------------------------------------------------------------------------------
void RDebugXML::WriteText(const char *text)
{
	File.WriteLine();
	File.WriteStr(tmpTab,Deep+1);
	File.WriteStr(text,strlen(text));
}


//------------------------------------------------------------------------------
RDebugXML::~RDebugXML(void)
{
}
