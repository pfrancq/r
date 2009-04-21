/*

	R Project Library

	RDebug.cpp

	Debugging file in XML format (eXtended Markup Language) - Implementation

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
void RDebug::BeginTag(const RString& tag,const RString& attrs)
{
	LevelOutput[++Deep]=false;      // For the moment nothing as child
	WriteBeginTag(tag,attrs);
	CurTag=tag;
}


//------------------------------------------------------------------------------
void RDebug::PrintComment(const RString& text)
{
	LevelOutput[Deep]=true;
	WriteText(text);
}


//------------------------------------------------------------------------------
void RDebug::EndTag(const RString& text)
{
	if(!LevelOutput[Deep])
		WriteText("No Special Information");
	if(Deep) LevelOutput[Deep-1]=true;
	WriteEndTag(text);
	Deep--;
}


//------------------------------------------------------------------------------
void RDebug::PrintInfo(const RString& text)
{
	BeginTag("info");
	PrintComment(text);
	EndTag("info");
}


//------------------------------------------------------------------------------
void RDebug::BeginFunc(const RString& name,const RString& object)
{
	BeginTag(name,"object=\""+object+"\"");
}


//------------------------------------------------------------------------------
void RDebug::EndFunc(const RString& name,const RString&)
{
	EndTag(name);
}


//------------------------------------------------------------------------------
void RDebug::BeginApp(const RString& app)
{
	time_t t;
	char TempString[40];
	time(&t);
	strcpy(TempString,ctime(&t));
	TempString[strlen(TempString)-1]=0;
	BeginTag(app,"date=\""+RString(TempString)+"\"");
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
RDebugXML::RDebugXML(const RURI& name)
	: RDebug(), Name(name), File(name,"utf-8")
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
RURI RDebugXML::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
void RDebugXML::WriteBeginTag(const RString& tag,const RString& attrs)
{
	if(Deep)
	{
		File.WriteLine();
		File.WriteStr(tmpTab,Deep);
	}
	File.WriteStr("<",1);
	File.WriteStr(tag,tag.GetLen());
	if(attrs.GetLen())
	{
		File.WriteStr(" ",1);
		File.WriteStr(attrs);
	}
	File.WriteStr(">",1);
}


//------------------------------------------------------------------------------
void RDebugXML::WriteEndTag(const RString& tag)
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
void RDebugXML::WriteText(const RString& text)
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
