/*

	R Project Library

	RTextFile.cpp

	Text File - Implementation.

	(C) 1999-2002 by P. Francq.

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
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <time.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;
#include <rio/rtextfile.h>
using namespace RIO;



//-----------------------------------------------------------------------------
//
// RTextFile
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RIO::RTextFile::RTextFile(const RString &name,RIO::ModeType mode) throw(bad_alloc,RString)
  : Mode(mode), Name(name), All(true), NewLine(true), Rem("%"),BeginRem("/*"),
		EndRem("*/"), CommentType(SingleLineComment), ActivComment(NoComment),
		Separator(" "), Line(0), LastLine(0)
{
	int localmode;

	switch(Mode)
	{
		case RIO::Read:
			localmode=O_RDONLY;
			break;

		case RIO::Append:
			localmode=O_WRONLY | O_CREAT | O_APPEND;
			break;

		case RIO::Create:
			localmode=O_WRONLY | O_CREAT | O_TRUNC;
			break;

		default:
			throw(RString("No Valid Mode"));
	};
	#ifndef _BSD_SOURCE
		localmode|=O_BINARY;
	#endif
	if(Mode==RIO::Read)
		handle=open(Name,localmode);
	else
		handle=open(Name,localmode,S_IREAD|S_IWRITE);
	if(handle==-1)
		throw(RString("Can't open file """+Name+""""));
	Init();
}


//-----------------------------------------------------------------------------
RIO::RTextFile::RTextFile(const RString &name,bool all) throw(bad_alloc,RString)
  : Mode(Read), Name(name), All(all), NewLine(false), Rem("%"), BeginRem("/*"),
		EndRem("*/"),CommentType(SingleLineComment),Line(0)
{
	#ifdef _BSD_SOURCE
		handle=open(Name,O_RDONLY);
	#else
		handle=open(Name,O_RDONLY | O_BINARY);
	#endif
	Init();
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::Init(void) throw(bad_alloc,RString)
{
	struct stat statbuf;

	Line=0;
	ptr=Buffer=0;
	if(Mode==RIO::Read)
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
	}
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::Begin(void) throw(RString)
{
	if(Mode!=RIO::Read)
		throw(RString("File Mode is not Read"));
	ptr=Buffer;
	Line=1;
	SkipSpaces();
}


//-----------------------------------------------------------------------------
bool RIO::RTextFile::BeginComment(void)
{
	bool ret=false;

	if((CommentType==SingleLineComment)||(CommentType==SingleMultiLineComment))
	{
		if(!strncmp(ptr,Rem,Rem.GetLen()))
		{
			ActivComment=SingleLineComment;
			ret=true;
		}
	}
	if((CommentType==MultiLineComment)||(CommentType==SingleMultiLineComment))
	{
		if(!strncmp(ptr,BeginRem,BeginRem.GetLen()))
		{
			ActivComment=MultiLineComment;
			ret=true;
		}
	}

	return(ret);
}


//-----------------------------------------------------------------------------
bool RIO::RTextFile::EndComment(void)
{
	if(ActivComment==NoComment) return(false);
	if(ActivComment==SingleLineComment)
	{
		if(((*ptr)=='\n')||((*ptr)=='\r'))
		{
			ActivComment=NoComment;
			Line++;
			switch(*ptr)
			{
				case '\n':
					if((*(ptr+1))=='\r') ptr++;
					break;
				case '\r':
					if((*(ptr+1))=='\n') ptr++;
					break;
			}
			ptr++;
			return(true);
		}
	}
	if(ActivComment==MultiLineComment)
	{
		if(!strncmp(ptr,EndRem,EndRem.GetLen()))
		{
			ptr+=EndRem.GetLen();
			ActivComment=NoComment;
			return(true);
		}
	}

	return(false);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::SkipComments(void)
{
	// Read Comments
	while(BeginComment())
	{
		// Skip Begin Comment
		if(ActivComment==SingleLineComment)
			ptr+=Rem.GetLen();
		if(ActivComment==MultiLineComment)
			ptr+=BeginRem.GetLen();

		// Read a Comment
		while((*ptr)&&(!EndComment()))
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


//-----------------------------------------------------------------------------
void RIO::RTextFile::SkipSpaces(void)
{
	//while(isspace(*ptr))
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
		SkipComments();
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


//-----------------------------------------------------------------------------
char* RIO::RTextFile::GetCharPtr(void)
{
	static char tab[25][300];
	static long act=0;

	if(act==25) act=0;
	return(tab[act++]);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::GetUntilEnd(char* &buffer)
{
	buffer=strdup(ptr);
}


//-----------------------------------------------------------------------------
long RIO::RTextFile::GetInt(void) throw(RString)
{
	char* ptr2=ptr;
	char* rem;

	if(Mode!=RIO::Read)
		throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment()))
		if(!isdigit(*(ptr++))) throw(RString("No Int"));
	LastLine=Line;
	if(*ptr)
	{
		rem=ptr;
		SkipSpaces();
		(*rem)=0;
	}
	return(strtol(ptr2,0,10));
}


//-----------------------------------------------------------------------------
unsigned long RIO::RTextFile::GetUInt(void) throw(RString)
{
	char* ptr2=ptr;
	char* rem;

	if(Mode!=RIO::Read)
		throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment()))
		if(!isdigit(*(ptr++))) throw(RString("No Int"));
	LastLine=Line;
	if(*ptr)
	{
		rem=ptr;
		SkipSpaces();
		(*rem)=0;
	}
	return(strtoul(ptr2,0,10));
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(char &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(unsigned char &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(short &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(unsigned short &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(int &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(unsigned int &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(long &nb) throw(RString)
{
	nb=GetInt();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(unsigned long &nb) throw(RString)
{
	nb=GetUInt();
	return(*this);
}


//-----------------------------------------------------------------------------
float RIO::RTextFile::GetFloat(void) throw(RString)
{
	char *ptr2=ptr;
	char* rem;

	if(Mode!=RIO::Read)
		throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment()))
	{
		if((!isdigit(*ptr))&&(*ptr)!='-'&&(*ptr)!='+'&&(*ptr)!='.'&&(*ptr)!='e'&&(*ptr)!='E')
			throw(RString("No float"));
		ptr++;
	}
	LastLine=Line;
	if(*ptr)
	{
		rem=ptr;
		SkipSpaces();
		(*rem)=0;
	}
	return(atof(ptr2));
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(float& nb) throw(RString)
{
	nb=GetFloat();
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator>>(double& nb) throw(RString)
{
	nb=GetFloat();
	return(*this);
}


//-----------------------------------------------------------------------------
char* RIO::RTextFile::GetWord(void) throw(RString)
{
	char *ptr2=ptr;
	char* rem;

	if(Mode!=RIO::Read)
		throw(RString("File Mode is not Read"));
	while((*ptr)&&(!isspace(*ptr))&&(!BeginComment())) ptr++;
	LastLine=Line;
	if(*ptr)
	{
		rem=ptr;
		SkipSpaces();
		(*rem)=0;
	}
	return(ptr2);
}


//-----------------------------------------------------------------------------
char* RIO::RTextFile::GetLine(void) throw(RString)
{
	char *ptr2=ptr,*ptr3;

	if(Mode!=RIO::Read)
		throw(RString("File Mode is not Read"));
	if(!(*ptr)) return(0);
	while((*ptr)&&(*ptr)!='\n'&&(*ptr)!='\r'&&(!BeginComment())) ptr++;
	LastLine=Line;
	if(*ptr)
	{
		ptr3=ptr;
		while((*ptr)&&((*ptr)=='\n'||(*ptr)=='\r'))
			ptr++;
		SkipComments();
		while((*ptr)&&((*ptr)=='\n'||(*ptr)=='\r'))
			ptr++;
		(*ptr3)=0;
	}
	// If the line is empty, read next line
	if(!(*ptr2))
	{
		return(GetLine());
	}
	return(ptr2);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteLine(void) throw(RString)
{
	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	write(handle,"\n",strlen("\n"));
	LastLine=Line++;
	#ifdef windows
		flushall();
	#endif
	NewLine=true;
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteLong(const long nb) throw(RString)
{
	char Str[25];

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	sprintf(Str,"%li",nb);
	write(handle,Str,strlen(Str));
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const short nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const int nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const long nb) throw(RString)
{
	WriteLong(nb);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteULong(const unsigned long nb) throw(RString)
{
	char Str[25];

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	sprintf(Str,"%lu",nb);
	write(handle,Str,strlen(Str));
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const unsigned char nb) throw(RString)
{
	WriteULong(nb);
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const unsigned int nb) throw(RString)
{
	WriteULong(nb);
	return(*this);
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const unsigned long nb) throw(RString)
{
	WriteULong(nb);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteStr(const char *c) throw(RString)
{
	int l;

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	l=strlen(c);
	RReturnIfFail(l);
	write(handle,c,l);
	#ifdef windows
		flushall();
	#endif
	if(c[l-1]!=' '&&c[l-1]!='\n'&&c[l-1]!='\t'&&c[l-1]!='\r')
		NewLine=false;
	else
		NewLine=true;
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteStr(const char *c,unsigned int l) throw(RString)
{
	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	if(!l) return;
	write(handle,c,l);
	#ifdef windows
		flushall();
	#endif
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const char *c) throw(RString)
{
	WriteStr(c);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteStr(const RString &str) throw(RString)
{
	int l;

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	RReturnIfFail(str.GetLen()>0);
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	l=str.GetLen();
	if(!l) return;
	if((str()[l-1]!='\n')&&(str()[l-1]!='\r'))
		NewLine=false;
	else
	{
		NewLine=true;
		l--;
		if((str()[l-1]!='\n')&&(str()[l-1]!='\r')) l--;
	}
	write(handle,str,l);
	if(NewLine) WriteLine();
	#ifdef windows
		flushall();
	#endif
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const RString &str) throw(RString)
{
	WriteStr(str);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteBool(const bool b) throw(RString)
{
	char Str[10];

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	if(b) strcpy(Str,"1"); else strcpy(Str,"0");
	write(handle,Str,strlen(Str));
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const bool b) throw(RString)
{
	WriteBool(b);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteChar(const char c) throw(RString)
{
	char Str[10];

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	sprintf(Str,"%c",c);
	write(handle,Str,1);
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const char c) throw(RString)
{
	WriteChar(c);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteDouble(const double d) throw(RString)
{
	char Str[25];

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	sprintf(Str,"%f",d);
	write(handle,Str,strlen(Str));
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//-----------------------------------------------------------------------------
RTextFile& RIO::RTextFile::operator<<(const double d) throw(RString)
{
	WriteDouble(d);
	return(*this);
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteTime(void) throw(RString)
{
	char Str[30];
	time_t timer;
	struct tm *tblock;

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	timer = time(NULL);
	tblock = localtime(&timer);
	if(!NewLine)
		write(handle,Separator,Separator.GetLen());
	strcpy(Str,asctime(tblock));
	Str[strlen(Str)-1]=0;
	write(handle,Str,strlen(Str));
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//-----------------------------------------------------------------------------
void RIO::RTextFile::WriteLog(const char *entry) throw(RString)
{
	char Str[30];
	time_t timer;
	struct tm *tblock;

	if(Mode==RIO::Read)
		throw(RString("File Mode is Read"));
	RReturnIfFail(strlen(entry)>0);
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


//-----------------------------------------------------------------------------
RIO::RTextFile::~RTextFile(void)
{
	if(Buffer) delete[] Buffer;
	if(handle!=-1) close(handle);
}



//-----------------------------------------------------------------------------
//
// General functions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RTextFile& endl(RTextFile &file)
{
	file.WriteLine();
	return(file);
}


//-----------------------------------------------------------------------------
RTextFile& Time(RTextFile &file)
{
	file.WriteTime();
	return(file);
}
