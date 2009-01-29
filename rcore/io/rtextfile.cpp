/*

	R Project Library

	RTextFile.cpp

	Text File - Implementation.

	Copyright 1999-2008 by the Université libre de Bruxelles.

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
#ifdef WIN32
	#include <sys/time.h> //for asctime_s
	#include <c99_support.h>
#endif
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
RTextFile::RTextFile(void)
  : RIOFile(), NewLine(true),
    Rem("%"), BeginRem("/*"), EndRem("*/"),
    CommentType(NoComment), ActivComment(NoComment), ParseSpace(SkipAllSpaces),
	Separator(" "), Line(0), LastLine(0), Codec(0)
{
}

//------------------------------------------------------------------------------
RTextFile::RTextFile(const RURI& uri,const RString& encoding)
  : RIOFile(uri), NewLine(true),
    Rem("%"), BeginRem("/*"), EndRem("*/"),
    CommentType(NoComment), ActivComment(NoComment), ParseSpace(SkipAllSpaces),
	Separator(" "), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
{
}


//------------------------------------------------------------------------------
RTextFile::RTextFile(RIOFile& file,const RString& encoding)
	: RIOFile(file), NewLine(true),
	Rem("%"), BeginRem("/*"), EndRem("*/"),
	CommentType(NoComment), ActivComment(NoComment), ParseSpace(SkipAllSpaces),
	Separator(" "), Line(0), LastLine(0), Codec(RTextEncoding::GetTextEncoding(encoding))
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
void RTextFile::Open(const RURI& uri,RIO::ModeType mode,const RString& encoding)
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
		SeekRel(2);
	}
	else if((Buffer[0]==char(0xff))&&(Buffer[1]==char(0xfe)))
	{
		cout<<"networkOrder = FALSE"<<endl;
		SeekRel(2);
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
	size_t treat,len=20*4,s;
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
				NextCar=Codec->NextUnicode(ptr,s);
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
			catch(InvalidByteSequence)
			{
				ptr++;
				treat++;
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
	SeekRel(*(SizeNextRead++));
	PosChars++;

	if(Cur==10)          // UNIX case
	{
		Line++;
	}
	else if(Cur==13) 	 // MAC and DOS
	{
		Line++;

		// DOS only
		if((*NextRead)==10) ReadTwice=true;
	}

	if(End())
		return;

	FillBuffer();

	if(ReadTwice)
	{
		Cur=(*(NextRead++));
		SeekRel(*(SizeNextRead++));
		PosChars++;

		FillBuffer();
	}
}


//------------------------------------------------------------------------------
RChar RTextFile::GetNextChar(void) const
{
	if(End())
		return(RChar());
	return(*NextRead);
}


//------------------------------------------------------------------------------
RChar RTextFile::GetChar(void)
{
	if(End())
		return(RChar());
	Next();
	return(Cur);
}


//------------------------------------------------------------------------------
RString RTextFile::GetUntilEnd(void)
{
	RString res;
	char Buffer[513];
	size_t len;

	while(!End())
	{
		// Read a block of maximum 512 characters in Buffer
		len=Read(Buffer,512);
		Buffer[len]=0;
		res+=Buffer;
	}
	return(res);
}


//------------------------------------------------------------------------------
/*inline void RTextFile::SkipEol(void)
{
	if(((*NextRead)==10)||((*NextRead)==13))
		Next();
}*/


//------------------------------------------------------------------------------
bool RTextFile::CurString(const RString& str,bool CaseSensitive,bool skip)
{
	size_t max=str.GetLen();

	if((!max)||End())
		return(false);

	const RChar* tofind=str(); // String to find

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
RString RTextFile::GetWord(void)
{
	if(!CanRead)
		throw RIOException(this,"File Mode is not Read");
	RString res(RString::Null);
	bool FindComment(false);
	SkipSpaces();
	RChar* ptr=TmpBuffer;
	size_t i(0);

	while((!End())&&(!Eol(GetNextChar()))&&(!GetNextChar().IsSpace())&&(!(FindComment=BeginComment())))
	{
		(*(ptr++))=GetChar();
		i++;
		if(i==255)
		{
			(*ptr)=0;
			res+=TmpBuffer;
			ptr=TmpBuffer;
			i=0;
		}
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(FindComment)
			SkipComments();
	}

	if(i)
	{
		(*ptr)=0;
		res+=TmpBuffer;
	}
	return(res);
}


//------------------------------------------------------------------------------
RString RTextFile::GetToken(const RString& endingchar)
{
	if(!CanRead)
		throw RIOException(this,"File Mode is not Read");
	RString res(RString::Null);
	bool FindComment(false);
	SkipSpaces();
	RChar* ptr=TmpBuffer;
	size_t i(0);

	while((!End())&&(!Eol(GetNextChar()))&&(!GetNextChar().IsSpace())&&(!(FindComment=BeginComment()))&&(endingchar.Find(GetNextChar())==-1))
	{
		(*(ptr++))=GetChar();
		i++;
		if(i==255)
		{
			(*ptr)=0;
			res+=TmpBuffer;
			ptr=TmpBuffer;
			i=0;
		}
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(FindComment)
			SkipComments();
	}

	if(i)
	{
		(*ptr)=0;
		res+=TmpBuffer;
	}
	return(res);
}


//------------------------------------------------------------------------------
RString RTextFile::GetTokenString(const RString& endingstr)
{
	if(!CanRead)
		throw RIOException(this,"File Mode is not Read");
	RString res(RString::Null);
	bool FindComment(false);
	SkipSpaces();
	RChar* ptr=TmpBuffer;
	size_t i(0);

	while((!End())&&(!Eol(GetNextChar()))&&(!GetNextChar().IsSpace())&&(!(FindComment=BeginComment()))&&(!CurString(endingstr,true,false)))
	{
		(*(ptr++))=GetChar();
		i++;
		if(i==255)
		{
			(*ptr)=0;
			res+=TmpBuffer;
			ptr=TmpBuffer;
			i=0;
		}
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(FindComment)
			SkipComments();
	}

	if(i)
	{
		(*ptr)=0;
		res+=TmpBuffer;
	}
	return(res);
}


//------------------------------------------------------------------------------
RString RTextFile::GetLine(bool SkipEmpty)
{
	if(!CanRead)
		throw(RIOException(this,"File Mode is not Read"));
	if(End())
		return(RString::Null);
	RString res(RString::Null);
	RChar* ptr=TmpBuffer;
	size_t i(0);

	while((!End())&&(!Eol(*NextRead)))
	{
		if(BeginComment())
			SkipComments();
		(*(ptr++))=GetChar();
		i++;
		if(i==255)
		{
			(*ptr)=0;
			res+=TmpBuffer;
			ptr=TmpBuffer;
			i=0;
		}
	}
	LastLine=Line;

	if(!End())
	{
		SkipEol();

		// Skip spaces and comments if necessary
		if(!End())
		{
			if(ParseSpace==SkipAllSpaces)
				SkipSpaces();
		}
	}

	if(i)
	{
		(*ptr)=0;
		res+=TmpBuffer;
	}

	// If the line is empty or contains only spaces -> read next line
	if((res.IsEmpty())&&(SkipEmpty))
		return(GetLine());

	// Return read line
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
	if((GetNextChar()=='+')||(GetNextChar()=='-'))
	{
		Next();
		str+=Cur;
	}

	// Get number
	while(GetNextChar().IsDigit())
	{
		Next();
		str+=Cur;
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(BeginComment())
			SkipComments();
	}

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
	if((GetNextChar()=='+')||(GetNextChar()=='-'))
	{
		Next();
		str+=Cur;
	}

	// Get Number
	while(GetNextChar().IsDigit())
	{
		Next();
		str+=Cur;
	}

	// Skip spaces and comments if necessary
	if(!End())
	{
		if(ParseSpace==SkipAllSpaces)
			SkipSpaces();
		else if(BeginComment())
			SkipComments();
	}

	nb=strtoul(str,&ptr,10);
	if(ptr==str)
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
	RString str;

	// Patern [whitespaces][+|-][nnnnn][.nnnnn][e|E[+|-]nnnn]
	SkipSpaces();

	//Check for sign
	if((Cur=='-')||(Cur=='+'))
	{
		str+=Cur;
		Next();
	}
	//Read digits
	while(Cur.IsDigit())
	{
		str+=Cur;
		Next();
	}
	//Read digit after '.'
	if(Cur=='.')
	{
		str+=Cur;
		Next();
		while(Cur.IsDigit())
		{
			str+=Cur;
			Next();
		}
	}
	if((Cur=='e')||(Cur=='E'))
	{
		str+=Cur;
		Next();
		//Check for sign
		if((Cur=='-')||(Cur=='+'))
		{
			str+=Cur;
			Next();
		}
		while(Cur.IsDigit())
		{
			str+=Cur;
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
void RTextFile::WriteStr(const char* c,size_t l)
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
