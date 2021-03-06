/*

	R Project Library

	RTextFile.cpp

	Text File - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifdef WIN32
	#include <sys/time.h> //for asctime_s
	#ifdef  __MINGW32__
		#include <time.h>
	#else
		#include <c99_support.h>
#endif
#endif
#include <rstd.h>
#include <rtextfile.h>
#include <rstring.h>
#include <rtextencoding.h>
#include <rstringbuilder.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RTextFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTextFile::RTextFile(void)
  : RIOFile(), NewLine(true),
    Rem("%"), BeginRem("/*"), EndRem("*/"),
    CommentType(NoComment), ActivComment(NoComment), ParseSpace(SkipAllSpaces),
	Separator(" "), SkipSeparator(false), Line(0), LastLine(0), Codec(0)
{
}


//------------------------------------------------------------------------------
RTextFile::RTextFile(const RURI& uri,const RCString& encoding)
  : RIOFile(uri), NewLine(true),
    Rem("%"), BeginRem("/*"), EndRem("*/"),
    CommentType(NoComment), ActivComment(NoComment), ParseSpace(SkipAllSpaces),
	Separator(" "), SkipSeparator(false), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
{
}


//------------------------------------------------------------------------------
RTextFile::RTextFile(RIOFile& file,const RCString& encoding)
	: RIOFile(file), NewLine(true),
	Rem("%"), BeginRem("/*"), EndRem("*/"),
	CommentType(NoComment), ActivComment(NoComment), ParseSpace(SkipAllSpaces),
	Separator(" "), SkipSeparator(false), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
{
}


//------------------------------------------------------------------------------
void RTextFile::Open(RIO::ModeType mode)
{
	RIOFile::Open(mode);
	LastLine=Line=0;
	if(CanRead&&mode!=RIO::Append)
	{
		Begin();
	}
}


//------------------------------------------------------------------------------
void RTextFile::Open(const RURI& uri,RIO::ModeType mode,const RCString& encoding)
{
	SetEncoding(encoding);
	RIOFile::Open(uri,mode);
}


//------------------------------------------------------------------------------
void RTextFile::Close(void)
{
	RIOFile::Close();
}


//------------------------------------------------------------------------------
void RTextFile::Seek(off_t pos)
{
	RIOFile::Seek(pos);

	// Initialize the buffer
	NextWrite=NextRead=Chars;
	SizeNextWrite=SizeNextRead=SizeChars;
	PosChars=0;
	SkipBytes=0;
	Resting=0;

	// Read the first characters at the new position
	ReadChars();
}


//------------------------------------------------------------------------------
void RTextFile::SeekRel(off_t pos)
{
	RIOFile::SeekRel(pos);

	// Initialize the buffer
	NextWrite=NextRead=Chars;
	SizeNextWrite=SizeNextRead=SizeChars;
	PosChars=0;
	SkipBytes=0;
	Resting=0;

	// Read the first characters at the new position
	ReadChars();
}


//------------------------------------------------------------------------------
void RTextFile::Begin(void)
{
	if(!CanRead)
		throw(RIOException(this,"File Mode is not Read"));
	LastLine=Line=1;
	NextWrite=NextRead=Chars;
	SizeNextWrite=SizeNextRead=SizeChars;
	PosChars=0;
	SkipBytes=0;
	Resting=0;
	Read(Buffer,2,false);
	if((Buffer[0]==char(0xfe))&&(Buffer[1]==char(0xff)))
	{
		cout<<"networkOrder = TRUE"<<endl;
		RIOFile::SeekRel(2);
	}
	else if((Buffer[0]==char(0xff))&&(Buffer[1]==char(0xfe)))
	{
		cout<<"networkOrder = FALSE"<<endl;
		RIOFile::SeekRel(2);
	}

	// Read the first characters
	ReadChars();

	// If necessary, skip leading comments and spaces
	if(ParseSpace==SkipAllSpaces)
		SkipSpaces();
}


//------------------------------------------------------------------------------
void RTextFile::ReadChars(void)
{
	// Verify that there is something to read
	if(End())
		return;

	// Initialize some local variables
	size_t treat,len=20*4+SkipBytes,s;
	char* ptr=Buffer+SkipBytes;
	bool end;
	RTextEncoding::UnicodeCharacter NextCar;

	// Read next 20 Unicode characters
	len=Read(Buffer,len,false);
	len-=SkipBytes;
	SkipBytes=0;
	for(size_t n=21;(--n)&&len;NextWrite++,SizeNextWrite++)
	{
		for(end=false,treat=0,s=len;!end;)
		{
			try
			{
				if(Resting.Unicode())
				{
					(*NextWrite)=Resting;
					(*SizeNextWrite)=0;;
					Resting=0;
					end=true;
					continue;
				}

				// Try to read an unicode character with accepting invalid characters
				NextCar=Codec->NextUnicode(ptr,s,true);
				if(!NextCar.Valid)
				{
					ptr++;
					treat++;
					continue;
				}

				// Valid Unicode character
				(*NextWrite)=NextCar.Codes[0];
				ptr+=s;
				len-=treat+s;
				if(n<11)
					SkipBytes+=treat+s;
				if(s>2)
					Resting=NextCar.Codes[1];
				else
					Resting=0;
				(*SizeNextWrite)=treat+s;
				end=true;
			}
			catch(RException e)
			{
				throw RIOException(this,e.GetMsg());
			}
		}
	}
}


//------------------------------------------------------------------------------
inline void RTextFile::FillBuffer(void)
{
	if(PosChars==10)
		ReadChars();
	else if(PosChars==30)
	{
		NextWrite=Chars;
		SizeNextWrite=SizeChars;
		ReadChars();
	}
	else if(PosChars==40)
	{
		PosChars=0;
		NextRead=Chars;
		SizeNextRead=SizeChars;
	}
}


//------------------------------------------------------------------------------
void RTextFile::Next(void)
{
	bool ReadTwice(false);

	Cur=(*(NextRead++));
	PosChars++;

	if(Cur==10)          // UNIX case
	{
		Line++;
	}
	else if(Cur==13) 	 // MAC and DOS
	{
		Line++;

		// DOS only
		if((!End())&&(*NextRead)==10) ReadTwice=true;
	}

	if(End())
		return;

	RIOFile::SeekRel(*(SizeNextRead++)); // Seek file

	FillBuffer();

	if(ReadTwice)
	{
		Cur=(*(NextRead++));
		RIOFile::SeekRel(*(SizeNextRead++));
		PosChars++;

		FillBuffer();
	}
}


//------------------------------------------------------------------------------
RString RTextFile::GetChars(size_t size)
{
	RStringBuilder Res;

	while((!End())&&(size--))
		Res+=GetChar();
	return(Res());
}


//------------------------------------------------------------------------------
RString RTextFile::GetUntilEnd(void)
{
	RStringBuilder Res;
	while(!End())
		Res+=GetChar();
	return(Res());
}


//------------------------------------------------------------------------------
bool RTextFile::CurString(const RString& str,bool CaseSensitive,bool skip)
{
	size_t max(str.GetLen());

	if((!max)||End())
		return(false);

	const RChar* tofind(str()); // String to find

	// Test if the first characters are the same -> return false
	if((CaseSensitive&&((*tofind)!=(*NextRead)))||(!CaseSensitive&&(RChar::ToLower(*tofind)!=RChar::ToLower(*NextRead))))
		return(false);

	// Place the second character
	RChar* search;
	size_t* ssearch;
	size_t possearch(PosChars+1);
	off_t end(GetPos()+(*SizeNextRead));
	if(possearch==40)
	{
		search=Chars;
		ssearch=SizeChars;
		possearch=0;
	}
	else
	{
		search=NextRead+1;
		ssearch=SizeNextRead+1;
	}

	for(tofind++;(--max)&&(end<GetSize());tofind++)
	{
		// If current characters are the same -> return false -> nothing to do
		if((CaseSensitive&&((*tofind)!=(*search)))||(!CaseSensitive&&(RChar::ToLower(*tofind)!=RChar::ToLower(*search))))
			return(false);

		// Look for next character
		end+=(*ssearch);
		possearch++;
		if(possearch==40)
		{
			search=Chars;
			ssearch=SizeChars;
			possearch=0;
		}
		else
		{
			search++;
			ssearch++;
		}
	}

	if(end==GetSize())
		return(false);

	// String found -> skip it
	if(skip)
	{
		for(max=str.GetLen()+1;--max;)
			Next();
	}

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
	switch(ActivComment)
	{
		case NoComment:
			return(false);
		case SingleLineComment:
			if(Eol(*NextRead))
			{
				ActivComment=NoComment;
				Next();
				SkipEol();
				return(true);
			}
			break;
		case MultiLineComment:
			if(CurString(EndRem))
			{
				ActivComment=NoComment;
				return(true);
			}
			break;
		default:
			break;
	}

	return(false);
}


//------------------------------------------------------------------------------
void RTextFile::SkipComments(void)
{
	// Read Comments
	do
	{
		// Read a Comment
		while((!End())&&(!EndComment()))
		{
			if(Eol(GetNextChar()))
				SkipEol();
			else
				Next();
		}
	}
	while(BeginComment());
}


//------------------------------------------------------------------------------
void RTextFile::SkipSpaces(void)
{
	for(bool Cont=true;Cont;)
	{
		// Suppose nothing was found
		Cont=false;

		// Skip all spaces (by looking if the next character is a space)
		while((!End())&&(NextRead->IsSpace()))
		{
			Next();
			Cont=true;
		}

		// Look if after these spaces, there are not comments
		if(BeginComment())
		{
			SkipComments();
			Cont=true;
		}
	}
}


//------------------------------------------------------------------------------
size_t RTextFile::SkipCountSpaces(RChar car)
{
	size_t nb(0);

	for(bool Cont=true;Cont;)
	{
		// Suppose nothing was found
		Cont=false;

		// Skip all spaces (by looking if the next character is a space)
		while((!End())&&(NextRead->IsSpace()))
		{
			if(Eol(*NextRead))
				nb=0;
			else if((*NextRead)==car)
				nb++;
			Next();
			Cont=true;
		}

		// Look if after these spaces, there are not comments
		if(BeginComment())
		{
			nb=0;
			SkipComments();
			Cont=true;
		}
	}
	return(nb);
}


//------------------------------------------------------------------------------
void RTextFile::SetEncoding(const RCString& name)
{
	try
	{
		Codec=RTextEncoding::GetTextEncoding(name);
	}
	catch(...)
	{
		throw RIOException("Encoding "+RString(name())+" not supported");
	}
}


//------------------------------------------------------------------------------
RCString RTextFile::GetEncoding(void) const
{
	if(!Codec)
		return("utf-16");
	return(Codec->GetName());
}


//------------------------------------------------------------------------------
void RTextFile::SetRem(const RString& c)
{
	Rem=c;
	if(c.GetLen()>10)
		throw RIOException(this,"Maximum of 10 characters for defining comments");
}


//------------------------------------------------------------------------------
void RTextFile::SetRem(const RString& b,const RString& e)
{
	BeginRem=b;
	EndRem=e;
	if((b.GetLen()>10)||(e.GetLen()>10))
		throw RIOException(this,"Maximum of 10 characters for defining comments");
}


//------------------------------------------------------------------------------
void RTextFile::SetAddSeparator(bool add)
{
	SkipSeparator=!add;
}


//------------------------------------------------------------------------------
RString RTextFile::GetWord(void)
{
	if(!CanRead)
		throw RIOException(this,"File Mode is not Read");
	RStringBuilder Res;
	bool FindComment(false);
	SkipSpaces();

	while((!End())&&(!Eol(GetNextChar()))&&(!GetNextChar().IsSpace())&&(!(FindComment=BeginComment())))
		Res+=GetChar();

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(FindComment)
			SkipComments();
	}

	return(Res());
}


//------------------------------------------------------------------------------
RString RTextFile::GetToken(const RString& endingchar)
{
	if(!CanRead)
		throw RIOException(this,"File Mode is not Read");
	RStringBuilder Res;
	bool FindComment(false);
	SkipSpaces();

	while((!End())&&(!Eol(GetNextChar()))&&(!GetNextChar().IsSpace())&&(!(FindComment=BeginComment()))&&(endingchar.Find(GetNextChar())==-1))
		Res+=GetChar();

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(FindComment)
			SkipComments();
	}

	return(Res());
}


//------------------------------------------------------------------------------
RString RTextFile::GetTokenString(const RString& endingstr)
{
	if(!CanRead)
		throw RIOException(this,"File Mode is not Read");
	RStringBuilder Res;
	bool FindComment(false);
	SkipSpaces();

	while((!End())&&(!Eol(GetNextChar()))&&(!GetNextChar().IsSpace())&&(!(FindComment=BeginComment()))&&(!CurString(endingstr,true,false)))
		Res+=GetChar();

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(FindComment)
			SkipComments();
	}

	return(Res());
}


//------------------------------------------------------------------------------
RString RTextFile::GetLine(bool skipempty)
{
	if(!CanRead)
		throw(RIOException(this,"File Mode is not Read"));
	if(End())
		return(RString::Null);
	RStringBuilder Res;

	while((!End())&&(!Eol(*NextRead)))
	{
		if(BeginComment())
			SkipComments();
		Res+=GetChar();
	}
	LastLine=Line;

	if(!End())
	{
		SkipEol();

		// Skip spaces and comments if necessary
		if(!End())
		{
			if((ParseSpace==SkipAllSpaces)&&skipempty)
				SkipSpaces();
		}
	}

	// If the line is empty or contains only spaces -> read next line
	if((Res().IsEmpty())&&skipempty)
		return(GetLine(true));

	// Return read line
	return(Res());
}


//------------------------------------------------------------------------------
long RTextFile::GetInt(void)
{
	long nb;
	char* ptr;
	RStringBuilder Res;

	SkipSpaces();

	// Get the sign
	if((GetNextChar()=='+')||(GetNextChar()=='-'))
	{
		Next();
		Res+=Cur;
	}

	// Get number
	while(GetNextChar().IsDigit())
	{
		Next();
		Res+=Cur;
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(BeginComment())
			SkipComments();
	}

	nb=strtol(Res(),&ptr,10);
	if(ptr==Res())
		throw RIOException(this,"No Int");
	return(nb);
}


//------------------------------------------------------------------------------
unsigned long RTextFile::GetUInt(void)
{
	unsigned long nb;
	char* ptr;
	RStringBuilder Res;

	SkipSpaces();

	// Get the sign
	if((GetNextChar()=='+')||(GetNextChar()=='-'))
	{
		Next();
		Res+=Cur;
	}

	// Get Number
	while(GetNextChar().IsDigit())
	{
		Next();
		Res+=Cur;
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(BeginComment())
			SkipComments();
	}

	nb=strtoul(Res(),&ptr,10);
	if(ptr==Res())
		throw RIOException(this,"No size_t");
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
	RStringBuilder Res;

	// Pattern [whitespace][+|-][nnnnn][.nnnnn][e|E[+|-]nnnn]
	SkipSpaces();

	//Check for sign
	if((Cur=='-')||(Cur=='+'))
	{
		Res+=Cur;
		Next();
	}
	//Read digits
	while(Cur.IsDigit())
	{
		Res+=Cur;
		Next();
	}
	//Read digit after '.'
	if(Cur=='.')
	{
		Res+=Cur;
		Next();
		while(Cur.IsDigit())
		{
			Res+=Cur;
			Next();
		}
	}
	if((Cur=='e')||(Cur=='E'))
	{
		Res+=Cur;
		Next();
		//Check for sign
		if((Cur=='-')||(Cur=='+'))
		{
			Res+=Cur;
			Next();
		}
		while(Cur.IsDigit())
		{
			Res+=Cur;
			Next();
		}
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(BeginComment())
			SkipComments();
	}

	return(Res());
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
	if(NewLine||SkipSeparator) return;
	RCString str=Codec->FromUnicode(Separator,false);
	Write(str,str.GetLen());
}


//------------------------------------------------------------------------------
void RTextFile::WriteLine(void)
{
	RString endofline("\n");
	RCString str=Codec->FromUnicode(endofline,false);
	Write(str,str.GetLen());
	LastLine=Line++;
	#ifdef windows
		flushall();
	#endif
	NewLine=true;
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const RString& str,bool invalid)
{
	mReturnIfFail(str.GetLen()>0);
	WriteSeparator();
	RCString res=Codec->FromUnicode(str,invalid);
	Write(res,res.GetLen());
	#ifdef windows
		flushall();
	#endif
	NewLine=false;
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const char* c,bool invalid)
{
	WriteStr(RString(c),invalid);
}


//------------------------------------------------------------------------------
void RTextFile::WriteStr(const char* c,size_t l,bool)
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
	WriteStr(RString(c),false);
	return(*this);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const RString& str)
{
	WriteStr(str,false);
	return(*this);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLong(const long nb)
{
	WriteStr(RString::Number(nb),false);
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
	WriteStr(RString::Number(nb),false);
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
		WriteStr("0",false);
	else
		WriteStr("1",false);
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
	WriteStr(RString(c),false);
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
	WriteStr(RString::Number(nb),false);
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
	WriteStr(RString::Number(nb),false);
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
	WriteStr(RString::Number(nb),false);
}


//------------------------------------------------------------------------------
RTextFile& RTextFile::operator<<(const long double nb)
{
	WriteLongDouble(nb);
	return(*this);
}


//------------------------------------------------------------------------------
inline void Add(char* tmp,int add,int len)
{
	int idx(0);
	for(len--;len>0;len--)
	{
		int dec(1);
		for(int i=len;i--;)
			dec*=10;
		//int dec(10*len);
		int rest(add/dec);
		tmp[idx++]=48+rest;
		add-=rest*dec;
	}
	tmp[idx]=48+add;
}


//------------------------------------------------------------------------------
void RTextFile::WriteTime(void)
{
	time_t timer;
	struct tm *tblock;

	timer = time(NULL);
	tblock = localtime(&timer);
	char Tmp[25];
	switch(tblock->tm_wday)
	{
		case 0:  Tmp[0]='S'; Tmp[1]='u'; Tmp[2]='n'; break;
		case 1:  Tmp[0]='M'; Tmp[1]='o'; Tmp[2]='n'; break;
		case 2:  Tmp[0]='T'; Tmp[1]='u'; Tmp[2]='e'; break;
		case 3:  Tmp[0]='W'; Tmp[1]='e'; Tmp[2]='d'; break;
		case 4:  Tmp[0]='T'; Tmp[1]='h'; Tmp[2]='u'; break;
		case 5:  Tmp[0]='F'; Tmp[1]='r'; Tmp[2]='i'; break;
		case 6:  Tmp[0]='S'; Tmp[1]='a'; Tmp[2]='t'; break;
	}
	Tmp[3]=' ';
	switch(tblock->tm_mon)
	{
		case 0:  Tmp[4]='J'; Tmp[5]='a'; Tmp[6]='n'; break;
		case 1:  Tmp[4]='F'; Tmp[5]='e'; Tmp[6]='b'; break;
		case 2:  Tmp[4]='M'; Tmp[5]='a'; Tmp[6]='r'; break;
		case 3:  Tmp[4]='A'; Tmp[5]='p'; Tmp[6]='r'; break;
		case 4:  Tmp[4]='M'; Tmp[5]='a'; Tmp[6]='y'; break;
		case 5:  Tmp[4]='J'; Tmp[5]='u'; Tmp[6]='n'; break;
		case 6:  Tmp[4]='J'; Tmp[5]='u'; Tmp[6]='l'; break;
		case 7:  Tmp[4]='A'; Tmp[5]='u'; Tmp[6]='g'; break;
		case 8:  Tmp[4]='S'; Tmp[5]='e'; Tmp[6]='p'; break;
		case 9:  Tmp[4]='O'; Tmp[5]='c'; Tmp[6]='t'; break;
		case 10: Tmp[4]='N'; Tmp[5]='o'; Tmp[6]='v'; break;
		case 11: Tmp[4]='D'; Tmp[5]='e'; Tmp[6]='c'; break;
	}
	Tmp[7]=' ';
	Add(&Tmp[8],tblock->tm_mday,2);
	Tmp[10]=' ';
	Add(&Tmp[11],tblock->tm_year+1900,4);
	Tmp[15]=' ';
	Add(&Tmp[16],tblock->tm_hour,2);
	Tmp[18]=':';
	Add(&Tmp[19],tblock->tm_min,2);
	Tmp[21]=':';
	Add(&Tmp[22],tblock->tm_sec,2);
	Tmp[24]=0;
	WriteStr(Tmp,24,false);
}


//------------------------------------------------------------------------------
void RTextFile::WriteLog(const RString& entry,bool invalid)
{
	mReturnIfFail(entry.GetLen()>0);
	if(!NewLine) WriteLine();
	WriteSeparator();
	bool SkipSep(SkipSeparator);
	SkipSeparator=true;
	WriteStr("[",false);
	WriteTime();
	WriteStr("] : ",false);
	WriteStr(entry,invalid);
	SkipSeparator=SkipSep;
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
