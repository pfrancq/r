/*

	R Project Library

	RTextFile.cpp

	Text File - Implementation.

	Copyright 1999-2004 by the Université libre de Bruxelles.

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


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rtextfile.h>
#include <rstd/rtextencoding.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RTextFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTextFile::RTextFile(const RString &name,const RString& encoding) throw(std::bad_alloc,RIOException, RException)
  : RFile(name), All(true), NewLine(true), Rem("%"), BeginRem("/*"),
	EndRem("*/"), CommentType(SingleLineComment), ActivComment(NoComment),
	Separator(" "), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
{
}


//------------------------------------------------------------------------------
void RTextFile::Open(ModeType mode)
{
	struct stat statbuf;

	RFile::Open(mode);
	LastLine=Line=0;
	ptr=Buffer=0;
	if(Mode==Read)
	{
		if(All)
		{
			fstat(handle, &statbuf);
			TotalLen=statbuf.st_size;
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


//------------------------------------------------------------------------------
void RTextFile::Close(void)
{
	RFile::Close();
	if(!Buffer) return;
	delete[] Buffer;
	Buffer=0;
}


//------------------------------------------------------------------------------
void RTextFile::Begin(void) throw(RIOException)
{
	if(Mode!=Read)
		throw(RIOException(this,"File Mode is not Read"));
	ptr=Buffer;
	Len=TotalLen;
	CurLen=0;
	LastLine=Line=1;
	if(Buffer[0]== char(0xfe) && Buffer[1] == char(0xff) )
		{
			cout<<"networkOrder = TRUE"<<endl;
			ptr+=2;
			Len-=2;
		}
		else if ( Buffer[0] == char(0xff) && Buffer[1] == char(0xfe) )
		{
			cout<<"networkOrder = FALSE"<<endl;
			ptr+=2;
			Len-=2;
		}
	Next();
}


//------------------------------------------------------------------------------
unsigned int RTextFile::ReadCar(char* &text,unsigned int &len,RChar& read) const throw(InvalidFile)
{
	RString str;
	unsigned int skip=0;

	for(bool end=false;!end;)
	{
		try
		{
			read=Codec->NextUnicode(text,len);
			end=true;
		}
		catch(InvalidByteSequence)
		{
			text++;
			skip++;
		}
		catch(RException e)
		{
			throw InvalidFile(this,e.GetMsg());
		}
	}
	return(skip);
}


//------------------------------------------------------------------------------
void RTextFile::Next(void) throw(RIOException)
{
	RString str;

	// Skip the current character
	if(CurLen)
	{
		ptr+=CurLen;
		Len-=CurLen;
	}

	// If Len is null -> end of file
	if(!Len)
	{
		Cur=0;
		CurLen=0;
		return;
	}
	try
	{
		//Read char if nor correct one -> skip it
		CurLen=Len;
		Len-=ReadCar(ptr,CurLen,Cur);
	}
	catch(InvalidFile e)
	{
		throw RIOException(e.GetMsg());
	}
	catch(RException e)
	{
		throw RException(e.GetMsg());
	}
}


//------------------------------------------------------------------------------
RChar RTextFile::GetNextCur(void)
{
	unsigned holdLen;
	unsigned holdCurLen;
	char* holdPtr;
	RChar holdCur;
	RChar NextCur;

	// Hold Current situation
	holdCurLen=CurLen;
	holdLen=Len;
	holdPtr=ptr;
	holdCur=Cur;

	// Get Next character
	Next();
	NextCur=Cur;

	// Go back to initial situation
	CurLen=holdCurLen;
	Len=holdLen;
	ptr=holdPtr;
	Cur=holdCur;

	return(NextCur);
}


//------------------------------------------------------------------------------
RString RTextFile::GetUntilEnd(void)
{
	RString res;
	RChar Buffer[513];
	RChar* tmp;
	unsigned int len;

	while(*ptr)
	{
		// Read a block of maximum 512 characters in Buffer
		len=0;
		tmp=Buffer;
		while((*ptr)&&(len<512))
		{
			(*(tmp++))=(*(ptr++));
			len++;
		}
		(*(tmp))=0;
		res+=Buffer;
	}
	return(res);
}


//------------------------------------------------------------------------------
bool RTextFile::Eol(RChar car)
{
	return((car==10)||(car==13));
}


//------------------------------------------------------------------------------
void RTextFile::SkipEol(void)
{
	// UNIX case
	if(Cur==10)
	{
		Next();
		Line++;
	}

	// MAC and DOS
	if(Cur==13)
	{
		Next();
		Line++;

		// DOS only
		if(Cur==10) Next();
	}
}


//------------------------------------------------------------------------------
bool RTextFile::CurString(const RString& str,bool CaseSensitive) const
{
	unsigned int len,total;
	char* parse;
	RChar tmp;

	// Test the first character
	if((CaseSensitive&&(Cur!=str[0]))||(!CaseSensitive&&(RChar::ToLower(Cur)!=RChar::ToLower(str[0]))))
		return(false);
	if(str.GetLen()==1) return(true);

	// Test the next characters
	RCharCursor CurStr(str);
	for(CurStr.GoTo(1),parse=ptr+CurLen,total=Len-CurLen;!CurStr.End();CurStr.Next())
	{
		try
		{
			// Try to read a new character. if not a correct one -> skip it
			len=Len;
  			ReadCar(parse,len,tmp);
		}
		catch(InvalidFile)
		{
			// Whatever occurs -> It is not the string
			return false;
		}

		// If the character read is not the same -> return false
		if((CaseSensitive&&(CurStr()!=tmp))||(!CaseSensitive&&(RChar::ToLower(CurStr())!=RChar::ToLower(tmp))))
			return(false);

		// Next character to read
		parse+=len;
		total-=len;
	}
	// OK
	return(true);
}


//------------------------------------------------------------------------------
bool RTextFile::BeginComment(void)
{
	bool ret=false;

	if((CommentType==SingleLineComment)||(CommentType==SingleMultiLineComment))
	{
		if(CurString(Rem))
		{
			ActivComment=SingleLineComment;
			ret=true;
		}
	}
	if((CommentType==MultiLineComment)||(CommentType==SingleMultiLineComment))
	{
		if(CurString(BeginRem))
		{
			ActivComment=MultiLineComment;
			ret=true;
		}
	}

	return(ret);
}


//------------------------------------------------------------------------------
bool RTextFile::EndComment(void)
{
	if(ActivComment==NoComment) return(false);
	if(ActivComment==SingleLineComment)
	{
		if(Eol(Cur))
		{
			ActivComment=NoComment;
			SkipEol();
			return(true);
		}
	}
	if(ActivComment==MultiLineComment)
	{
		if(CurString(EndRem))
		{
			// Skip the number of characters
			for(unsigned int skip=EndRem.GetLen()+1;--skip;)
				Next();
			ActivComment=NoComment;
			return(true);
		}
	}

	return(false);
}


//------------------------------------------------------------------------------
void RTextFile::SkipComments(void)
{
	// Read Comments
	while(BeginComment())
	{
		// Skip Begin Comment
		if(ActivComment==SingleLineComment)
		{
			// Skip the number of characters
			for(unsigned int skip=Rem.GetLen()+1;--skip;)
				Next();
		}
		if(ActivComment==MultiLineComment)
		{
			// Skip the number of characters
			for(unsigned int skip=BeginRem.GetLen()+1;--skip;)
				Next();
		}

		// Read a Comment
		while((!Cur.IsNull())&&(!EndComment()))
		{
			if(Eol(Cur))
				SkipEol();
			else
				Next();
		}
	}
}


//------------------------------------------------------------------------------
void RTextFile::SkipSpaces(void)
{
	unsigned int oldLen;
	do{
		oldLen=Len;
		while((!Cur.IsNull())&&(Cur.IsSpace()))
		{
			if(Eol(Cur))
				SkipEol();
			else
				Next();
		}
		SkipComments();

	}while((!Cur.IsNull())&&(oldLen!=Len));
}


//------------------------------------------------------------------------------
void RTextFile::SetEncoding(const RString& name) throw(RIOException)
{
	try
	{
		Codec=RTextEncoding::GetTextEncoding(name);
	}
	catch(...)
	{
		throw RIOException("Encoding "+name+" not supported");
	}
}


//------------------------------------------------------------------------------
RString RTextFile::GetEncoding(void) const
{
	if(!Codec)
		return("utf-16");
	return(Codec->GetName());
}


//------------------------------------------------------------------------------
RString RTextFile::GetWord(void) throw(RIOException)
{
	RString res;

	if(Mode!=Read)
		throw(RIOException(this,"File Mode is not Read"));
	SkipSpaces();
	while((!Cur.IsNull())&&(!Cur.IsSpace())&&(!BeginComment()))
	{
		res+=Cur;
		Next();
	}
	return(res);
}


//------------------------------------------------------------------------------
RString RTextFile::GetLine(bool SkipEmpty) throw(RIOException)
{
	RString res;

	if(Mode!=Read)
		throw(RIOException(this,"File Mode is not Read"));
	if(Cur.IsNull()) return(res);
	while((!Cur.IsNull())&&(!Eol(Cur))&&(!BeginComment()))
	{
		res+=Cur;
		Next();
	}
	LastLine=Line;

	if(!Cur.IsNull())
	{
		SkipEol();
		SkipComments();
		if(SkipEmpty)
		{
			while(Eol(Cur))
			{
				SkipEol();
			}
		}
	}
	// If the line is empty or contains only spaces -> read next line
	if((res.IsEmpty())&&(SkipEmpty))
		return(GetLine());

	// Return readed line
	return(res);
}


//------------------------------------------------------------------------------
long RTextFile::GetInt(void) throw(RIOException)
{
	long nb;
	char* ptr;

	RString str("");
	//SkipSpaces
	while((GetCur()==' '))
		Next();
	//Get the sign
	if((GetCur()=='+')||(GetCur()=='-'))
	{
		str+=GetCur();
		Next();
	}
	//Get number
	while(GetCur().IsDigit())
	{
		str+=GetCur();
		Next();
	}
	nb=strtol(str,&ptr,10);
	if(ptr==str)
		throw RIOException(this,"No Int");
	return(nb);
}


//------------------------------------------------------------------------------
unsigned long RTextFile::GetUInt(void) throw(RIOException)
{
	unsigned long nb;
	char* ptr;

	RString str("");
	//SkipSpaces
	while((GetCur()==' '))
		Next();
	//Get the sign
	if((GetCur()=='+')||(GetCur()=='-'))
	{
		str+=GetCur();
		Next();
	}
	//Get Number
	while(GetCur().IsDigit()||(GetCur()=='+')||(GetCur()=='-'))
	{
		str+=GetCur();
		Next();
	}
	nb=strtoul(str,&ptr,10);
	if(ptr==str)
		throw RIOException(this,"No unsigned Int");
	return(nb);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(RString& str) throw(RIOException)
{
	str=GetWord();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(char &nb) throw(RIOException)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned char &nb) throw(RIOException)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(short &nb) throw(RIOException)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned short &nb) throw(RIOException)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(int &nb) throw(RIOException)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned int &nb) throw(RIOException)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(long &nb) throw(RIOException)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned long &nb) throw(RIOException)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
float RTextFile::GetFloat(void) throw(RIOException)
{
	//Patern [whitespaces][+|-][nnnnn][.nnnnn][e|E[+|-]nnnn]
	float nb;
	char* ptr;

	RString str("");
	//SkipSpaces
	while((GetCur()==' '))
		Next();
	//Check for sign
	if((GetCur()=='-')||(GetCur()=='+'))
	{
		str+=GetCur();
		Next();
	}
	//Read digits
	while(GetCur().IsDigit())
	{
		str+=GetCur();
		Next();
	}
	//Read digit after '.'
	if(GetCur()=='.')
	{
		str+=GetCur();
		Next();
		while(GetCur().IsDigit())
		{
			str+=GetCur();
			Next();
		}
	}
	if((GetCur()=='e')||(GetCur()=='E'))
	{
		cout<<"passe dans e"<<endl;
		str+=GetCur();
		Next();
		//Check for sign
		if((GetCur()=='-')||(GetCur()=='+'))
		{
			str+=GetCur();
			Next();
		}
		while(GetCur().IsDigit())
		{
			str+=GetCur();
			Next();
		}
	}

	nb=strtof(str,&ptr);
	if(ptr==str)
		throw RIOException(this,"No Float");
	return(nb);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(float& nb) throw(RIOException)
{
	nb=GetFloat();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(double& nb) throw(RIOException)
{
	nb=GetFloat();
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteSeparator(void)
{
	if(NewLine) return;
	RCString str=Codec->FromUnicode(Separator);
	write(handle,str,str.GetLen());
}


//------------------------------------------------------------------------------
void RTextFile::WriteLine(void) throw(RIOException)
{
	if(Mode==Read)
		throw(RIOException(this,"File Mode is Read"));
	RString endofline("\n");
	RCString str=Codec->FromUnicode(endofline);
	write(handle,str,str.GetLen());
	LastLine=Line++;
	#ifdef windows
		flushall();
	#endif
	NewLine=true;
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const RString& str) throw(RIOException)
{
	if(Mode==Read)
		throw(RIOException(this,"File Mode is Read"));
	RReturnIfFail(str.GetLen()>0);
	WriteSeparator();
	RCString res=Codec->FromUnicode(str);
	write(handle,res,res.GetLen());
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const char* c) throw(RIOException)
{
	RString str(c);
	WriteStr(str);
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const char* c,unsigned int l) throw(RIOException)
{
	if(Mode==Read)
		throw(RIOException(this,"File Mode is Read"));
	if(!l) return;
	WriteSeparator();
	write(handle,c,l);
	#ifdef windows
		flushall();
	#endif
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const char* c) throw(RIOException)
{
	RString str(c);
	WriteStr(str);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const RString& str) throw(RIOException)
{
	WriteStr(str);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLong(const long nb) throw(RIOException)
{
	RString res=ltou(nb);
	WriteStr(res);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const short nb) throw(RIOException)
{
	WriteLong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const int nb) throw(RIOException)
{
	WriteLong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const long nb) throw(RIOException)
{
	WriteLong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteULong(const unsigned long nb) throw(RIOException)
{
	RString res=ltou(nb);
	WriteStr(res);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned char nb) throw(RIOException)
{
	WriteULong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned int nb) throw(RIOException)
{
	WriteULong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned long nb) throw(RIOException)
{
	WriteULong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteBool(const bool b) throw(RIOException)
{
	if(b)
	{
		RString res('1');
		WriteStr(res);
	}
	else
	{
		RString res('0');
		WriteStr(res);
	}
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const bool b) throw(RIOException)
{
	WriteBool(b);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteChar(const char c) throw(RIOException)
{
	RString res(c);
	WriteStr(res);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const char c) throw(RIOException)
{
	WriteChar(c);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteDouble(const double d) throw(RIOException)
{
	RString str=dtou(d);
	WriteStr(str);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const double d) throw(RIOException)
{
	WriteDouble(d);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteTime(void) throw(RIOException)
{
	char tmp[30];
	time_t timer;
	struct tm *tblock;

	if(Mode==Read)
		throw(RIOException(this,"File Mode is Read"));
	timer = time(NULL);
	tblock = localtime(&timer);
	asctime_r(tblock,tmp);
	tmp[strlen(tmp)-1]=0;
	RString str=tmp;
	WriteStr(str);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLog(const RString& entry) throw(RIOException)
{
	char tmp[30];
	time_t timer;
	struct tm *tblock;

	if(Mode==Read)
		throw(RIOException(this,"File Mode is Read"));
	RReturnIfFail(entry.GetLen()>0);
	RString str;
	if(!NewLine) WriteLine();
	timer = time(NULL);
	tblock = localtime(&timer);
	asctime_r(tblock,tmp);
	tmp[strlen(tmp)-1]=0;
	str=RString("[")+tmp+"] : "+entry;
	WriteStr(str);
	WriteLine();
}


//------------------------------------------------------------------------------
RChar RTextFile::GetDirSeparator(void)
{
#ifdef _BSD_SOURCE
    return('/');
#else
    return('\\');
#endif

}


//------------------------------------------------------------------------------
RTextFile::~RTextFile(void)
{
}



//------------------------------------------------------------------------------
//
// General functions
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTextFile& std::endl(RTextFile &file)
{
	file.WriteLine();
	return(file);
}


//------------------------------------------------------------------------------
RTextFile& std::Time(RTextFile &file)
{
	file.WriteTime();
	return(file);
}
