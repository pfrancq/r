/*

	Rainbow Library Project

	RDebug.cpp

	Debugging file in XML format (eXtended Markup Language) - Implementation

	(C) 1998-2000 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <new.h>
#include <string.h>
#ifdef unix
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rdebug.h"
using namespace RGA;



//-----------------------------------------------------------------------------
//
// Class RDebug
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RDebug::RDebug(void) : Deep(-1)
{
}


//-----------------------------------------------------------------------------
void RDebug::BeginTag(char *Text,unsigned NbAttr,...)
{
	va_list ap;

	LevelOutput[++Deep]=false;	// For the moment nothing as child
	NbOptions=0;
	(*tmpOpt)=0;
	va_start(ap, NbAttr);
	while(NbAttr--)
		AddAttribute(tmpOpt,va_arg(ap,char*),va_arg(ap,char*));
	va_end(ap);
	WriteBeginTag(Text,tmpOpt);
}


//-----------------------------------------------------------------------------
void RDebug::AddAttribute(char *buf,char *Value,char *Attr)
{
	if(NbOptions++) strcat(buf," ");
	strcat(buf,Attr);
	strcat(buf,"=\"");
	strcat(buf,Value);
	strcat(buf,"\"");
}


//-----------------------------------------------------------------------------
void RDebug::PrintComment(char *Text)
{
	LevelOutput[Deep]=true;	
	WriteText(Text);
}


//-----------------------------------------------------------------------------
void RDebug::EndTag(char *Text)
{
	if(!LevelOutput[Deep])
		WriteText("No Special Information");
	if(Deep) LevelOutput[Deep-1]=true;
	WriteEndTag(Text);
	Deep--;
}


//-----------------------------------------------------------------------------
void RDebug::PrintInfo(char *Text)
{
	BeginTag("Info");
	PrintComment(Text);
	EndTag("Info");
}


//-----------------------------------------------------------------------------
void RDebug::BeginFunc(char *Name,char *Object)
{
	BeginTag(Name,1,"Object",Object);
}


//-----------------------------------------------------------------------------
void RDebug::EndFunc(char *Name,char *)
{
	EndTag(Name);
}

                               
//-----------------------------------------------------------------------------
void RDebug::BeginApp(char *App,char *Author)
{
	time_t t;
	char TempString[50];

	time(&t);
	strcpy(TempString,ctime(&t));
	TempString[strlen(TempString)-1]=0;
	BeginTag(App,2,"Author",Author,"Date",TempString);
}


//-----------------------------------------------------------------------------
void RDebug::EndApp(char *App,char *)
{
	EndTag(App);
}


//-----------------------------------------------------------------------------
RDebug::~RDebug(void)
{
}



//-----------------------------------------------------------------------------
//
// Class RDebugXML
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RDebugXML::RDebugXML(const RString &name) throw(bad_alloc)
	: RDebug(),Name(name)
{
	unsigned int i;
	char *ptr;

	Handle=open(Name(),O_WRONLY	| O_CREAT | O_TRUNC);
	if(!Handle) throw(new bad_alloc);
	strcpy(tmpNL,"\n");
	tmpLenNL=strlen(tmpNL);
	for(i=50,ptr=tmpTab;--i;ptr++)
		(*ptr)='\t';
	(*ptr)=0;
}


//-----------------------------------------------------------------------------
void RDebugXML::WriteBeginTag(char *tag,char *options)
{
	if(Deep)
	{
		write(Handle,tmpNL,tmpLenNL);
		write(Handle,tmpTab,Deep);
	}
	write(Handle,"<",1);
	write(Handle,tag,strlen(tag));	
	if(options&&(*options))
	{
		write(Handle," ",1);
		write(Handle,options,strlen(options));
	}
	write(Handle,">",1);
}


//-----------------------------------------------------------------------------
void RDebugXML::WriteEndTag(char *tag)
{
	if(Deep&&LevelOutput[Deep-1])
	{
		write(Handle,tmpNL,tmpLenNL);
		write(Handle,tmpTab,Deep);
	}
	write(Handle,"</",2);
	write(Handle,tag,strlen(tag));
	write(Handle,">",1);
}


//-----------------------------------------------------------------------------
void RDebugXML::WriteText(char *text)
{	
	write(Handle,tmpNL,tmpLenNL);
	write(Handle,tmpTab,Deep+1);
	write(Handle,text,strlen(text));
}


//-----------------------------------------------------------------------------
RDebugXML::~RDebugXML(void)
{
	close(Handle);
	int access=S_IREAD | S_IWRITE;
	#ifdef unix
		access|= S_IRGRP | S_IROTH;
	#endif
	chmod(Name(),access);
}