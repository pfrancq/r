/*

  RString.cpp

  String - Implementation.

  (C) 1999-2000 by P. Francq.

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


//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#if unix
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <time.h>


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rtextfile.h"
using namespace RStd;


//---------------------------------------------------------------------------
//
// RTextFile
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RTextFile::RTextFile(const RString &name,int mode) throw(bad_alloc,RString)
  : Name(name),Mode(mode), NewLine(true)
{
  switch(Mode)
  {
    case modRead:
      mode=O_RDONLY;
      break;

    case modAppend:
      mode=O_WRONLY | O_CREAT | O_APPEND;
      break;

    case modCreate:
      mode=O_WRONLY | O_CREAT | O_TRUNC;
      break;

    default:
      throw(RString("No Valid Mode"));
  };
  if(Mode==modRead)
    handle=open(Name(),mode);
  else
    handle=open(Name(),mode,S_IREAD|S_IWRITE);
  if(handle==-1)
    throw(RString("Can't open file """+Name+""""));
  Init();
}


//---------------------------------------------------------------------------
RTextFile::RTextFile(const RString &name,bool all) throw(bad_alloc,RString)
  : Name(name),All(all),Mode(modRead)
{
	handle=open(Name(),O_RDONLY);
  Init();
}


//---------------------------------------------------------------------------
void RTextFile::Init(void) throw(bad_alloc,RString)
{
	struct stat statbuf;

	ptr=Buffer=NULL;
  switch(Mode)
  {
    case modRead:
      if(All)
      {
        fstat(handle, &statbuf);
        Buffer=new char[statbuf.st_size+1];
        read(handle,Buffer,statbuf.st_size);
        Buffer[statbuf.st_size]=0;
      }
      else
      {
        Buffer=new char[1001];
        read(handle,Buffer,1000);
        Buffer[1000]=0;
      }
      Begin();
      SkipSpaces();
      break;
  }
}


//---------------------------------------------------------------------------
void RTextFile::Begin(void) throw(RString)
{
  if(Mode!=modRead)
    throw(RString("File Mode is not modRead"));
  ptr=Buffer;
}


//---------------------------------------------------------------------------
void RTextFile::SkipSpaces(void)
{
	while((*ptr)&&((*ptr)==' '||(*ptr)=='\t'||(*ptr)=='\n'||(*ptr)=='\r')) ptr++;
	if((*ptr)=='%')
		while((*ptr)&&(*ptr)!='\n'&&(*ptr)!='\r') ptr++;
	while((*ptr)&&((*ptr)==' '||(*ptr)=='\t'||(*ptr)=='\n'||(*ptr)=='\r')) ptr++;
}


//---------------------------------------------------------------------------
long int RTextFile::GetInt(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=modRead)
    throw(RString("File Mode is not modRead"));
	while((*ptr)&&(*ptr)!=' '&&(*ptr)!='\t'&&(*ptr)!='\n'&&(*ptr)!='\r')
		if(!isdigit(*(ptr++))) throw(RString("No Int"));
	if(*ptr)
	{
		(*(ptr++))=0;
		SkipSpaces();
	}
	return(atol(ptr2));
}


//---------------------------------------------------------------------------
float RTextFile::GetFloat(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=modRead)
    throw(RString("File Mode is not modRead"));
	while((*ptr)&&(*ptr)!=' '&&(*ptr)!='\t'&&(*ptr)!='\n'&&(*ptr)!='\r')
	{
		if((!isdigit(*ptr))&&(*ptr)!='.'&&(*ptr)!='e'&&(*ptr)!='E')
      throw(RString("No float"));
		ptr++;
	}
	if(*ptr)
  {
		(*(ptr++))=0;
		SkipSpaces();
	}
	return(atof(ptr2));
}


//---------------------------------------------------------------------------
char* RTextFile::GetWord(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=modRead)
    throw(RString("File Mode is not modRead"));
	while((*ptr)&&(*ptr)!=' '&&(*ptr)!='\t'&&(*ptr)!='\n'&&(*ptr)!='\r') ptr++;
	if(*ptr)
	{
		(*(ptr++))=0;
		SkipSpaces();
	}
	return(ptr2);
}


//---------------------------------------------------------------------------
char* RTextFile::GetLine(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=modRead)
    throw(RString("File Mode is not modRead"));
	while((*ptr)&&(*ptr)!='\n'&&(*ptr)!='\r') ptr++;
	if(*ptr)
	{
		(*(ptr++))=0;
		SkipSpaces();
	}
	return(ptr2);
}


//---------------------------------------------------------------------------
void RTextFile::WriteLine(void) throw(RString)
{
  if(Mode==modRead)
    throw(RString("File Mode is modRead"));
  write(handle,"\n",strlen("\n"));
  flushall();
  NewLine=true;
}


//---------------------------------------------------------------------------
void RTextFile::WriteLong(long nb) throw(RString)
{
  char Str[25];

  if(Mode==modRead)
    throw(RString("File Mode is modRead"));
  if(!NewLine)
    write(handle," ",1);
  sprintf(Str,"%li",nb);
  write(handle,Str,strlen(Str));
  flushall();
  NewLine=false;
}


//---------------------------------------------------------------------------
void RTextFile::WriteULong(unsigned long nb) throw(RString)
{
  char Str[25];

  if(Mode==modRead)
    throw(RString("File Mode is modRead"));

  if(!NewLine)
    write(handle," ",1);
  sprintf(Str,"%lu",nb);
  write(handle,Str,strlen(Str));
  flushall();
  NewLine=false;
}


//---------------------------------------------------------------------------
void RTextFile::WriteStr(char *c) throw(RString)
{
  int l;

  if(Mode==modRead)
    throw(RString("File Mode is modRead"));
  if(!NewLine)
    write(handle," ",1);
  l=strlen(c);
  write(handle,c,l);
  flushall();
  if(c[l-1]!=' '&&c[l-1]!='\n'&&c[l-1]!='\t'&&c[l-1]!='\r')
    NewLine=false;
  else
    NewLine=true;
}


//---------------------------------------------------------------------------
void RTextFile::WriteBool(bool b) throw(RString)
{
  char Str[10];

  if(Mode==modRead)
    throw(RString("File Mode is modRead"));
  if(!NewLine)
    write(handle," ",1);
  if(b) strcpy(Str,"true"); else strcpy(Str,"false");
  write(handle,Str,strlen(Str));
  flushall();
  NewLine=false;
}


//---------------------------------------------------------------------------
void RTextFile::WriteTime(void) throw(RString)
{
  char Str[20];
  time_t timer;
  struct tm *tblock;


  if(Mode==modRead)
    throw(RString("File Mode is modRead"));
  timer = time(NULL);
  tblock = localtime(&timer);
  if(!NewLine)
    write(handle," ",1);
  flushall();
  strcpy(Str,asctime(tblock));
  Str[strlen(Str)-1]=0;
  write(handle,Str,strlen(Str));
  NewLine=false;
}


//---------------------------------------------------------------------------
void RTextFile::WriteLog(char *entry) throw(RString)
{
  char Str[30];
  time_t timer;
  struct tm *tblock;

  if(Mode==modRead)
    throw(RString("File Mode is modRead"));
  if(!NewLine) WriteLine();
  strcpy(Str,"[");
  timer = time(NULL);
  tblock = localtime(&timer);
  strcat(Str,asctime(tblock));
  Str[strlen(Str)-1]=0;
  strcat(Str,"] : ");
  write(handle,Str,strlen(Str));
  write(handle,entry,strlen(entry));
  WriteLine();
}


//---------------------------------------------------------------------------
RTextFile::~RTextFile(void)
{
	if(Buffer) delete[] Buffer;
	if(handle!=-1) close(handle);
}
