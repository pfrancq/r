/*

	R Project Library

	RDebug.cpp

	Debugging file in XML format (eXtended Markup Language) - Implementation

	Copyright 1998-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
RDebug::RDebug(RString app,RString author)
	: Deep(-1), App(app), Author(author)
{
}


//------------------------------------------------------------------------------
void RDebug::BeginTag(RString Text,unsigned NbAttr,...)
{
	va_list ap;

	LevelOutput[++Deep]=false;      // For the moment nothing as child
	NbOptions=0;
	tmpOpt.Clear();
	va_start(ap, NbAttr);
	while(NbAttr--)
		AddAttribute(va_arg(ap,RString*),va_arg(ap,RString*));
	va_end(ap);
	WriteBeginTag(Text,tmpOpt);
	CurTag=Text;
}


//------------------------------------------------------------------------------
void RDebug::AddAttribute(RString* Value,RString* Attr)
{
	if(NbOptions++)
		tmpOpt+=" ";
	tmpOpt+=(*Attr)+"=\""+(*Value)+"\"";
}


//------------------------------------------------------------------------------
void RDebug::PrintComment(RString Text)
{
	LevelOutput[Deep]=true;
	WriteText(Text);
}


//------------------------------------------------------------------------------
void RDebug::EndTag(RString Text)
{
	if(!LevelOutput[Deep])
		WriteText("No Special Information");
	if(Deep) LevelOutput[Deep-1]=true;
	WriteEndTag(Text);
	Deep--;
}


//------------------------------------------------------------------------------
void RDebug::PrintInfo(RString Text)
{
	BeginTag("Info");
	PrintComment(Text);
	EndTag("Info");
}


//------------------------------------------------------------------------------
void RDebug::BeginFunc(RString Name,RString Object)
{
	BeginTag(Name,1,&RString("Object"),&Object);
}


//------------------------------------------------------------------------------
void RDebug::EndFunc(RString Name,RString)
{
	EndTag(Name);
}


//------------------------------------------------------------------------------
void RDebug::BeginApp(void)
{
	time_t t;
	char TempString[20];

	time(&t);
	strcpy(TempString,ctime(&t));
	TempString[strlen(TempString)-1]=0;
	BeginTag(App,2,&RString("Author"),&Author,&RString("Date"),&RString(TempString));
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
RDebugXML::RDebugXML(RString name,RString app,RString author) throw(std::bad_alloc)
	: RDebug(app,author),Name(name), File(name)
{
	File.Open(RIO::Create);
	File.WriteStr("<!DOCTYPE "+RString(app)+">");
	File.WriteLine();
	for(int i=51;--i;)
		tmpTab+=RChar('\t');
}


//------------------------------------------------------------------------------
void RDebugXML::WriteBeginTag(RString tag,RString options)
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
void RDebugXML::WriteEndTag(RString tag)
{
	if(Deep&&LevelOutput[Deep-1])
	{
		File.WriteLine();
		File.WriteStr(tmpTab,Deep);
	}
	File.WriteStr("</",2);
	File.WriteStr(tag);
	File.WriteStr(">",1);
}


//------------------------------------------------------------------------------
void RDebugXML::WriteText(RString text)
{
	File.WriteLine();
	File.WriteStr(tmpTab,Deep+1);
	File.WriteStr(text);
}


//------------------------------------------------------------------------------
RDebugXML::~RDebugXML(void)
{
}
