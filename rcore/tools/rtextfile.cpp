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
RTextFile::RTextFile(const RString &name,ModeType mode) throw(bad_alloc,RString)
  : Mode(mode), Name(name), All(true), NewLine(true), Rem("%"),BeginRem("/*"),
		EndRem("*/"),	CommentType(SingleLineComment), Separator(" "), Line(0)
{
	int localmode;

  switch(Mode)
  {
    case Read:
      localmode=O_RDONLY;
      break;

    case Append:
      localmode=O_WRONLY | O_CREAT | O_APPEND;
      break;

    case Create:
      localmode=O_WRONLY | O_CREAT | O_TRUNC;
      break;

    default:
      throw(RString("No Valid Mode"));
  };
  if(Mode==Read)
    handle=open(Name(),localmode);
  else
    handle=open(Name(),localmode,S_IREAD|S_IWRITE);
  if(handle==-1)
    throw(RString("Can't open file """+Name+""""));
  Init();
}


//---------------------------------------------------------------------------
RTextFile::RTextFile(const RString &name,bool all) throw(bad_alloc,RString)
  : Mode(Read), Name(name), All(all), NewLine(false), Rem("%"), BeginRem("/*"),
		EndRem("*/"),CommentType(SingleLineComment),Line(0)
{
	handle=open(Name(),O_RDONLY);
  Init();
}


//---------------------------------------------------------------------------
void RTextFile::Init(void) throw(bad_alloc,RString)
{
	struct stat statbuf;

	Line=0;
	ptr=Buffer=0;
	if(Mode==Read)
	{
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
  }
}


//---------------------------------------------------------------------------
void RTextFile::Begin(void) throw(RString)
{
  if(Mode!=Read)
    throw(RString("File Mode is not Read"));
  ptr=Buffer;
	Line=0;
}


//---------------------------------------------------------------------------
bool RTextFile::BeginComment(void)
{
	switch(CommentType)
	{
		case NoComment:
			return(false);
			break;

		case SingleLineComment:
			return(!strncmp(ptr,Rem(),Rem.GetLen()));
			break;

		case MultiLineComment:
			return(!strncmp(ptr,BeginRem(),BeginRem.GetLen()));
			break;
	}
	return(false);

}


//---------------------------------------------------------------------------
bool RTextFile::EndComment(void)
{
	switch(CommentType)
	{
		case NoComment:
			return(false);
			break;

		case SingleLineComment:
			if(((*ptr)=='\n')||((*ptr)=='\r')) return(true);
			break;

		case MultiLineComment:
			if(!strncmp(ptr,EndRem(),EndRem.GetLen()))
			{
				ptr+=EndRem.GetLen();
				return(true);
			}
			break;
	}
	return(false);
}


//---------------------------------------------------------------------------
void RTextFile::SkipSpaces(void)
{
	// Read Spaces
	while((*ptr)&&isspace(*ptr))
	{
		switch(*ptr)
		{
			case '\n':
				Line++;
				if((*(ptr+1))=='\r') ptr++;
				break;
			case '\r':
				Line++;
				if((*(ptr+1))=='\n') ptr++;
				break;
		}
		ptr++;
	}

	// Read Comments
	while(BeginComment())
	{
		// Skip Begin Comment
		if(CommentType==SingleLineComment)
			ptr+=Rem.GetLen();	
		if(CommentType==MultiLineComment)
			ptr+=BeginRem.GetLen();

		// Read a Comment
		while((*ptr)&&(!EndComment()))
			ptr++;

		// Read Spaces
		while((*ptr)&&isspace(*ptr))
    {
  		switch(*ptr)
  		{
  			case '\n':
  				Line++;
  				if((*(ptr+1))=='\r') ptr++;
  				break;
  			case '\r':
  				Line++;
  				if((*(ptr+1))=='\n') ptr++;
  				break;
  		}
			ptr++;
		}
	}
}


//---------------------------------------------------------------------------
long RTextFile::GetInt(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=Read)
    throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment()))
		if(!isdigit(*(ptr++))) throw(RString("No Int"));
	if(*ptr)
	{
		(*(ptr++))=0;
		SkipSpaces();
	}
	return(strtol(ptr2,0,10));
}


//---------------------------------------------------------------------------
unsigned long RTextFile::GetUInt(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=Read)
    throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment()))
		if(!isdigit(*(ptr++))) throw(RString("No Int"));
	if(*ptr)
	{
		(*(ptr++))=0;
		SkipSpaces();
	}
	return(strtoul(ptr2,0,10));
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(char &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned char &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(short &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned short &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(int &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned int &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(long &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned long &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//---------------------------------------------------------------------------
float RTextFile::GetFloat(void) throw(RString)
{
	char *ptr2=ptr;

  if(Mode!=Read)
    throw(RString("File Mode is not Read"));
	while((*ptr)&&isspace(*ptr)&&(!BeginComment()))
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

  if(Mode!=Read)
    throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment())) ptr++;
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

  if(Mode!=Read)
    throw(RString("File Mode is not Read"));
	while((*ptr)&&(*ptr)!='\n'&&(*ptr)!='\r'&&(!BeginComment())) ptr++;
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
  if(Mode==Read)
    throw(RString("File Mode is Read"));
  write(handle,"\n",strlen("\n"));
	Line++;
	#ifdef windows
	  flushall();
	#endif
  NewLine=true;
}


//---------------------------------------------------------------------------
void RTextFile::WriteLong(const long nb) throw(RString)
{
  char Str[25];

  if(Mode==Read)
    throw(RString("File Mode is Read"));
  if(!NewLine)
    write(handle,Separator(),Separator.GetLen());
  sprintf(Str,"%li",nb);
  write(handle,Str,strlen(Str));
	#ifdef windows
	  flushall();
	#endif
  NewLine=false;
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const char nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const short nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const int nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const long nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//---------------------------------------------------------------------------
void RTextFile::WriteULong(const unsigned long nb) throw(RString)
{
  char Str[25];

  if(Mode==Read)
    throw(RString("File Mode is Read"));

  if(!NewLine)
    write(handle,Separator(),Separator.GetLen());
  sprintf(Str,"%lu",nb);
  write(handle,Str,strlen(Str));
	#ifdef windows
	  flushall();
	#endif
  NewLine=false;
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned char nb) throw(RString)
{
	WriteULong(nb);
	return(*this);
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned int nb) throw(RString)
{
	WriteULong(nb);
	return(*this);
}

//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned long nb) throw(RString)
{
	WriteULong(nb);
	return(*this);
}


//---------------------------------------------------------------------------
void RTextFile::WriteStr(const char *c) throw(RString)
{
  int l;

  if(Mode==Read)
    throw(RString("File Mode is Read"));
  if(!NewLine)
    write(handle,Separator(),Separator.GetLen());
  l=strlen(c);
  write(handle,c,l);
	#ifdef windows
	  flushall();
	#endif
  if(c[l-1]!=' '&&c[l-1]!='\n'&&c[l-1]!='\t'&&c[l-1]!='\r')
    NewLine=false;
  else
    NewLine=true;
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const char *c) throw(RString)
{
	WriteStr(c);
	return(*this);
}


//---------------------------------------------------------------------------
void RTextFile::WriteStr(const RString &str) throw(RString)
{
  int l;

  if(Mode==Read)
    throw(RString("File Mode is Read"));
  if(!NewLine)
    write(handle,Separator(),Separator.GetLen());
  l=str.GetLen();
  if(str[l-1]!='\n'&&str[l-1]!='\r')
    NewLine=false;
  else
	{
    NewLine=true;
		l--;
		if(str[l-1]!='\n'&&str[l-1]!='\r') l--;
	}
  write(handle,str(),l);
	if(NewLine) WriteLine();
	#ifdef windows
	  flushall();
	#endif
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const RString &str) throw(RString)
{
	WriteStr(str);
	return(*this);
}


//---------------------------------------------------------------------------
void RTextFile::WriteBool(const bool b) throw(RString)
{
  char Str[10];

  if(Mode==Read)
    throw(RString("File Mode is Read"));
  if(!NewLine)
    write(handle,Separator(),Separator.GetLen());
  if(b) strcpy(Str,"1"); else strcpy(Str,"0");
  write(handle,Str,strlen(Str));
	#ifdef windows
	  flushall();
	#endif
  NewLine=false;
}


//---------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const bool b) throw(RString)
{
	WriteBool(b);
	return(*this);
}


//---------------------------------------------------------------------------
void RTextFile::WriteTime(void) throw(RString)
{
  char Str[30];
  time_t timer;
  struct tm *tblock;


  if(Mode==Read)
    throw(RString("File Mode is Read"));
  timer = time(NULL);
  tblock = localtime(&timer);
  if(!NewLine)
    write(handle,Separator(),Separator.GetLen());
  strcpy(Str,asctime(tblock));
  Str[strlen(Str)-1]=0;
  write(handle,Str,strlen(Str));
	#ifdef windows
	  flushall();
	#endif
  NewLine=false;
}


//---------------------------------------------------------------------------
void RTextFile::WriteLog(const char *entry) throw(RString)
{
  char Str[30];
  time_t timer;
  struct tm *tblock;

  if(Mode==Read)
    throw(RString("File Mode is Read"));
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
	#ifdef windows
	  flushall();
	#endif
}


//---------------------------------------------------------------------------
RTextFile::~RTextFile(void)
{
	if(Buffer) delete[] Buffer;
	if(handle!=-1) close(handle);
}


//---------------------------------------------------------------------------
RTextFile& endl(RTextFile &file)
{
	file.WriteLine();
	return(file);
}


//---------------------------------------------------------------------------
RTextFile& Time(RTextFile &file)
{
	file.WriteTime();
	return(file);
}