/*

	R Project Library

	RDebug.cpp

	Debugging file in XML format (eXtended Markup Language) - Implementation

	Copyright 1998-2005 by the Université Libre de Bruxelles.

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
#include <rdebug.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RDebug
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDebug::RDebug(void)
	: Deep(-1)
{
}


//------------------------------------------------------------------------------
void RDebug::BeginTag(RString Text,unsigned NbAttr,...)
{
	va_list ap;

	LevelOutput[++Deep]=false;      // For the moment nothing as child
	size_t NbOptions=0;
	tmpOpt.Clear();
	va_start(ap, NbAttr);
	while(NbAttr--)
		AddAttribute(NbOptions,va_arg(ap,RString*),va_arg(ap,RString*));
	va_end(ap);
	WriteBeginTag(Text,tmpOpt);
	CurTag=Text;
}


//------------------------------------------------------------------------------
void RDebug::AddAttribute(size_t& nboptions,RString* Value,RString* Attr)
{
	if(nboptions++)
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
	RString what("Object");
	RString obj(Object);
	BeginTag(Name,1,&what,&obj);
}


//------------------------------------------------------------------------------
void RDebug::EndFunc(RString Name,RString)
{
	EndTag(Name);
}


//------------------------------------------------------------------------------
void RDebug::BeginApp(const RString& app)
{
	time_t t;
	char TempString[40];
	RString what("Date");
	
	time(&t);
	strcpy(TempString,ctime(&t));
	TempString[strlen(TempString)-1]=0;
	RString Time(TempString);
	BeginTag(app,1,&what,&Time);
}


//------------------------------------------------------------------------------
void RDebug::EndApp(const RString& app)
{
	EndTag(app);
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
RDebugXML::RDebugXML(const RString& name)
	: RDebug(), Name(name), File(name)
{
	LevelOutput[++Deep]=false;      // For the moment nothing as child
	File.SetSeparator("");
	File.Open(RIO::Create);
	File.WriteStr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	File.WriteLine();
	File.WriteStr("<!DOCTYPE RDebug>");
	File.WriteLine();
	File.WriteStr("<RDebug>");
	for(int i=51;--i;)
		tmpTab+=RChar('\t');
}



//------------------------------------------------------------------------------
RString RDebugXML::GetName(void) const
{
	return(Name);
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
	File.WriteStr(tag,tag.GetLen());
	if(options.GetLen())
	{
		File.WriteStr(" ",1);
		File.WriteStr(options);
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
	File.WriteLine();
	File.WriteStr("</RDebug>");
}
