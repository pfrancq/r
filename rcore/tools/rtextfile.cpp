/*

	R Project Library

	RTextFile.cpp

	Text File - Implementation.

	Copyright 1999-2005 by the Université libre de Bruxelles.

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
// include files for R Project
#include <rstd.h>
#include <rtextfile.h>
#include <rstring.h>
#include <rtextencoding.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RTextFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTextFile::RTextFile(const RURI& uri,const RString& encoding)
  : RIOFile(uri), Buffer(0), All(true), NewLine(true), Rem("%"), BeginRem("/*"),
	EndRem("*/"), CommentType(SingleLineComment), ActivComment(NoComment),
	Separator(" "), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
{
}


//------------------------------------------------------------------------------
RTextFile::RTextFile(RIOFile& file,const RString& encoding)
	: RIOFile(file), Buffer(0), All(true), NewLine(true), Rem("%"), BeginRem("/*"),
	EndRem("*/"), CommentType(SingleLineComment), ActivComment(NoComment),
	Separator(" "), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
{
}


//------------------------------------------------------------------------------
void RTextFile::Open(RIO::ModeType mode)
{
	RIOFile::Open(mode);
	LastLine=Line=0;
	ptr=Buffer=0;
	if(CanRead&&mode!=RIO::Append)
	{
		TotalLen=GetSize();
		if(All)
		{
			Buffer=new char[GetSize()+1];
			Read(Buffer,GetSize());
			Buffer[GetSize()]=0;
		}
		else
		{
			Buffer=new char[1001];
			Read(Buffer,1000);
			Buffer[1000]=0;
		}
		Begin();
	}
}


//------------------------------------------------------------------------------
void RTextFile::Close(void)
{
	if(Buffer)
	{
		delete[] Buffer;
		Buffer=0;
	}
}


//------------------------------------------------------------------------------
void RTextFile::Begin(void)
{
	if(!CanRead)
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
unsigned int RTextFile::ReadCar(char* &text,unsigned int &len,RChar& read) const
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
			throw RIOException(this,e.GetMsg());
		}
	}
	return(skip);
}


//------------------------------------------------------------------------------
void RTextFile::Next(void)
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
	//Read char if nor correct one -> skip it
	CurLen=Len;
	Len-=ReadCar(ptr,CurLen,Cur);
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
	if((CaseSensitive&&(Cur!=str[static_cast<size_t>(0)]))||(!CaseSensitive&&(RChar::ToLower(Cur)!=RChar::ToLower(str[static_cast<size_t>(0)]))))
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
		catch(RIOException) // Invalid File
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
	do
	{
		oldLen=Len;
		while((!Cur.IsNull())&&(Cur.IsSpace()))
		{
			if(Eol(Cur))
				SkipEol();
			else
				Next();
		}
		SkipComments();
	}
	while((!Cur.IsNull())&&(oldLen!=Len));
}


//------------------------------------------------------------------------------
void RTextFile::SkipEmptyLines(void)
{
	unsigned int oldLen;
	bool GoNextLine;
	RChar oldCur;
	char* oldptr;
	do
	{
		// Look until end of current line
		oldLen=Len;
		oldCur=Cur;
		oldptr=ptr;
		GoNextLine=false;
		while((!Cur.IsNull())&&(Cur.IsSpace())&&(!Eol(Cur)))
			Next();

		if((!Cur.IsNull())&&(Eol(Cur)))
		{
			// Only spaces -> If end-of-line -> go trough next line
			if(!Cur.IsNull())
			{
				if(Eol(Cur))
					SkipEol();
				GoNextLine=true;
			}
		}
		else
		{
			// Not only spaces -> perhaps comments
			if(BeginComment())
			{
				SkipComments();
				SkipEmptyLines();
			}
			else
			{
				// Normal sequence -> old position
				Len=oldLen;
				Cur=oldCur;
				ptr=oldptr;
			}
		}
	}
	while((!Cur.IsNull())&&(GoNextLine));
}


//------------------------------------------------------------------------------
void RTextFile::SetEncoding(const RString& name)
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
bool RTextFile::Eof(void)
{
	return(Len==0);
}


//------------------------------------------------------------------------------
RString RTextFile::GetWord(void)
{
	RString res;
	
	res.SetLen(500);
	if(!CanRead)
		throw(RIOException(this,"File Mode is not Read"));
	SkipSpaces();
	while((!Eol(Cur))&&(!Cur.IsNull())&&(!Cur.IsSpace())&&(!BeginComment()))
	{
		res+=Cur;
		Next();
	}
	SkipEmptyLines();
	return(res);
}


//------------------------------------------------------------------------------
RString RTextFile::GetLine(bool SkipEmpty)
{
	RString res;

	if(!CanRead)
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
long RTextFile::GetInt(void)
{
	long nb;
	char* ptr;
	RString str;

	SkipSpaces();

	// Get the sign
	if((GetCur()=='+')||(GetCur()=='-'))
	{
		str+=GetCur();
		Next();
	}

	// Get number
	while(GetCur().IsDigit())
	{
		str+=GetCur();
		Next();
	}

	SkipEmptyLines();

	nb=strtol(str,&ptr,10);
	if(ptr==str)
		throw RIOException(this,"No Int");
	return(nb);
}


//------------------------------------------------------------------------------
unsigned long RTextFile::GetUInt(void)
{
	unsigned long nb;
	char* ptr;
	RString str;

	SkipSpaces();

	// Get the sign
	if((GetCur()=='+')||(GetCur()=='-'))
	{
		str+=GetCur();
		Next();
	}

	// Get Number
	while(GetCur().IsDigit())
	{
		str+=GetCur();
		Next();
	}

	SkipEmptyLines();

	nb=strtoul(str,&ptr,10);
	if(ptr==str)
		throw RIOException(this,"No unsigned Int");
	return(nb);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(RString& str)
{
	str=GetWord();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(char &nb)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned char &nb)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(short &nb)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned short &nb)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(int &nb)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned int &nb)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(long &nb)
{
	nb=GetInt();
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(unsigned long &nb)
{
	nb=GetUInt();
	return(*this);
}


//------------------------------------------------------------------------------
RString RTextFile::GetRealNb(void)
{
	RString str;
	
	// Patern [whitespaces][+|-][nnnnn][.nnnnn][e|E[+|-]nnnn]
	SkipSpaces();

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

	SkipEmptyLines();
	
	return(str);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(float& nb)
{
	char* ptr;
	RString str=GetRealNb();
	nb=strtof(str,&ptr);
	if(ptr==str)
		throw RIOException(this,"No Float");
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(double& nb)
{
	char* ptr;
	RString str=GetRealNb();
	nb=strtod(str,&ptr);
	if(ptr==str)
		throw RIOException(this,"No Float");
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator>>(long double& nb)
{
	char* ptr;
	RString str=GetRealNb();
	nb=strtold(str,&ptr);
	if(ptr==str)
		throw RIOException(this,"No Float");
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteSeparator(void)
{
	if(NewLine) return;
	RCString str=Codec->FromUnicode(Separator);
	Write(str,str.GetLen());
}


//------------------------------------------------------------------------------
void RTextFile::WriteLine(void)
{
	RString endofline("\n");
	RCString str=Codec->FromUnicode(endofline);
	Write(str,str.GetLen());
	LastLine=Line++;
	#ifdef windows
		flushall();
	#endif
	NewLine=true;
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const RString& str)
{
	RReturnIfFail(str.GetLen()>0);
	WriteSeparator();
	RCString res=Codec->FromUnicode(str);
	Write(res,res.GetLen());
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const char* c)
{
	RString str(c);
	WriteStr(str);
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const char* c,unsigned int l)
{
	if(!l) return;
	WriteSeparator();
	Write(c,l);
	#ifdef windows
		flushall();
	#endif
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const char* c)
{
	RString str(c);
	WriteStr(str);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const RString& str)
{
	WriteStr(str);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLong(const long nb)
{
	RString res=RString::Number(nb);
	WriteStr(res);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const short nb)
{
	WriteLong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const int nb)
{
	WriteLong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const long nb)
{
	WriteLong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteULong(const unsigned long nb)
{
	RString res=RString::Number(nb);
	WriteStr(res);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned char nb)
{
	WriteULong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned int nb)
{
	WriteULong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const unsigned long nb)
{
	WriteULong(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteBool(const bool b)
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
RTextFile& RTextFile::operator<<(const bool b)
{
	WriteBool(b);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteChar(const char c)
{
	RString res(c);
	WriteStr(res);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const char c)
{
	WriteChar(c);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteFloat(const float nb)
{
	RString str=RString::Number(nb);
	WriteStr(str);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const float nb)
{
	WriteFloat(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteDouble(const double nb)
{
	RString str=RString::Number(nb);
	WriteStr(str);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const double nb)
{
	WriteDouble(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLongDouble(const long double nb)
{
	RString str=RString::Number(nb);
	WriteStr(str);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const long double nb)
{
	WriteLongDouble(nb);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteTime(void)
{
	char tmp[30];
	time_t timer;
	struct tm *tblock;

	timer = time(NULL);
	tblock = localtime(&timer);
	asctime_r(tblock,tmp);
	tmp[strlen(tmp)-1]=0;
	RString str=tmp;
	WriteStr(str);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLog(const RString& entry)
{
	char tmp[30];
	time_t timer;
	struct tm *tblock;

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
RTextFile::~RTextFile(void)
{
	Close();
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
