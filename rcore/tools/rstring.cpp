 /*

	R Project Library

	RString.cpp

	Unicode String - Implementation.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rshareddata.h>
using namespace std;
#include <basicstring.hh>
using namespace R;



//-----------------------------------------------------------------------------
//
// Static data
//
//-----------------------------------------------------------------------------
//RString::CharBuffer* RString::DataNull=0;
const RString RString::Null;



//-----------------------------------------------------------------------------
//
// RString
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString::RString(void)
	: BasicString<RChar,RString>()
{
	Data=GetDataNull();
}


//-----------------------------------------------------------------------------
RString::RString(const char* src)
	: BasicString<RChar,RString>()
{
	if(src)
	{
		size_t len,maxlen=0;
		RChar* ptr=Latin1ToUnicode(src,len,maxlen);
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
RString::RString(const RChar* src)
	: BasicString<RChar,RString>(src)
{
}

//-----------------------------------------------------------------------------
RString::RString(const RChar* src,size_t len)
	: BasicString<RChar,RString>(src,len)
{
}


//-----------------------------------------------------------------------------
RString::RString(const std::string& src)
	: BasicString<RChar,RString>()
{
	size_t len,maxlen=0;
	RChar* ptr;

	ptr=Latin1ToUnicode(src.c_str(),len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
RString::RString(char src)
	: BasicString<RChar,RString>()
{
	size_t len,maxlen=1;
	RChar* ptr=new RChar[2];

	ptr[0]=RChar(src);
	ptr[1]=0;
	len=(src>0);
	Data=new CharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
RString::RString(RChar src)
	: BasicString<RChar,RString>()
{
	size_t len,maxlen=1;
	RChar* ptr=new RChar[2];

	ptr[0]=src;
	ptr[1]=0;
	if(src.IsNull())
		len=0;
	else
		len=1;
	Data=new CharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
RString::RString(const RString& src)
	: BasicString<RChar,RString>(src)
{
}


//-----------------------------------------------------------------------------
RString& RString::operator=(const char* src)
{
	size_t len,maxlen=0;
	RDecRef<CharBuffer>(Data);
	RChar* ptr=Latin1ToUnicode(src,len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator=(const std::string& src)
{
	size_t len,maxlen=0;
	RDecRef<CharBuffer>(Data);
	RChar* ptr=Latin1ToUnicode(src.c_str(),len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//-----------------------------------------------------------------------------
void RString::Copy(const char* text,size_t nb)
{
	RDecRef<CharBuffer>(Data);
	if(text)
	{
		size_t len,maxlen=nb;
		RChar* ptr=Latin1ToUnicode(text,len,maxlen);
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
const char* RString::Latin1(void) const
{
	if(Data==DataNull)
		return("");
	if(!static_cast<CharBuffer*>(Data)->Latin1)
	{
		static_cast<CharBuffer*>(Data)->Latin1=UnicodeToLatin1(Data->Text,Data->Len);
	}
	return(static_cast<CharBuffer*>(Data)->Latin1);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const char* src)
{
	RReturnValIfFail(src,*this);
	if(Data==DataNull)
	{
		(*this)=src;
	}
	else
	{
		const char* ptr1=src;
		size_t len=strlen(src);
		BasicString<RChar,RString>::Copy();
		Data->Verify(len+Data->Len+1);
		RChar* ptr2=&Data->Text[Data->Len];
		while(*ptr1)
			(*(ptr2++))=(*(ptr1++));
		(*ptr2)=0;
		Data->Len+=len;
		static_cast<CharBuffer*>(Data)->InvalidLatin1();
	}
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const char src)
{
	if(src)
	{
		if(Data==DataNull)
		{
			size_t maxlen=1,len=1;
			char tab[2]={src,0};
			RDecRef<CharBuffer>(Data);
			RChar* ptr=Latin1ToUnicode(tab,len,maxlen);
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			BasicString<RChar,RString>::Copy();
			Data->Verify(Data->Len+1);
			RChar* ptr=&Data->Text[Data->Len++];
			(*(ptr++))=src;
			(*ptr)=0;
			static_cast<CharBuffer*>(Data)->InvalidLatin1();
		}
	}
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const RChar src)
{
	if(!src.IsNull())
	{
		if(Data==DataNull)
		{
			size_t maxlen=1,len=1;
			RDecRef<CharBuffer>(Data);
			RChar* ptr=new RChar[2];
			ptr[0]=src;
			ptr[1]=0;
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			BasicString<RChar,RString>::Copy();
			Data->Verify(Data->Len+1);
			RChar* ptr=&Data->Text[Data->Len++];
			(*(ptr++))=src;
			(*ptr)=0;
			static_cast<CharBuffer*>(Data)->InvalidLatin1();
		}
	}
	return(*this);
}


//-----------------------------------------------------------------------------
const RChar* RString::UTF16(void) const
{
	return(Data->Text);
}


//-----------------------------------------------------------------------------
const RChar* RString::operator()(void) const
{
	return(Data->Text);
}


//-----------------------------------------------------------------------------
RString::operator const char* () const
{
	return(Latin1());
}


//-----------------------------------------------------------------------------
RString::operator std::string () const
{
	RChar* ptr;
	std::string text;

	ptr=Data->Text;
	while(!ptr->IsNull())
	{
		text+=ptr->Latin1();
		ptr++;
	}
	return(text);
}


//-----------------------------------------------------------------------------
std::string RString::ToString(void) const
{
	return(operator std::string());
}


//-----------------------------------------------------------------------------
int RString::Compare(const RString& src) const
{
	if(!Data)
	{
		if(!src.Data)
			return(0);
		return(-1);
	}
	else if(!src.Data)
		return(1);
	return(RChar::StrCmp(Data->Text,src.Data->Text));
}


//-----------------------------------------------------------------------------
int RString::Compare(const char* src) const
{
	if(!Data)
	{
		if(!src)
			return(0);
		return(-1);
	}
	else if(!src)
		return(1);
	return(RChar::StrCmp(Data->Text,src));
}


//-----------------------------------------------------------------------------
int RString::Compare(const RChar* src) const
{
	if(!Data)
	{
		if(!src)
			return(0);
		return(-1);
	}
	else if(!src)
		return(1);
	return(RChar::StrCmp(Data->Text,src));
}


//-----------------------------------------------------------------------------
size_t RString::HashIndex(size_t idx) const
{
	if((!Data)||(Data->Len<idx)) return(26);
	UChar c(RChar::ToLower(Data->Text[idx-1]).Unicode());
	UChar a(RChar('a').Unicode());
	if((c>=a)&&(c<=RChar('z').Unicode()))
		return(c-a);
	return(26);
}


//-----------------------------------------------------------------------------
RChar* RString::Latin1ToUnicode(const char* src,size_t& len,size_t& maxlen)
{
	if(!src)
	{
		len=0;
		return(0);
	}

	RChar* res=0;
	size_t i;
	RChar* ptr;

	if(!maxlen)
		maxlen=strlen(src);
	res=ptr=new RChar[maxlen+1];
	i = maxlen+1;
	len=0;
	while((--i)&&(*src))
	{
	   	(*(ptr++)) = (*(src++));
	   	len++;
	}
	(*ptr)=0;
	return(res);
}


//-----------------------------------------------------------------------------
char* RString::UnicodeToLatin1(const RChar* src,size_t len)
{
	if(!src)
		return(0);

	char* res=new char[len+1];
	char* ptr=res;

	while(len--)
	{
		(*(ptr++))=(src->Unicode() > 0xff) ? '?' : (char)src->Unicode();
		src++;
	}
	(*ptr)=0;
	return(res);
}


//------------------------------------------------------------------------------
char RString::ToChar(bool& valid)
{
	int v;
	valid=(sscanf(Latin1(),"%d",&v)>0);
	return(static_cast<char>(v));
}


//------------------------------------------------------------------------------
int RString::ToInt(bool& valid)
{
	int v;
	valid=(sscanf(Latin1(),"%d",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
unsigned int RString::ToUInt(bool& valid)
{
	unsigned int v;
	valid=(sscanf(Latin1(),"%u",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
long RString::ToLong(bool& valid)
{
	long v;
	valid=(sscanf(Latin1(),"%ld",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
unsigned long RString::ToULong(bool& valid)
{
	unsigned long v;
	valid=(sscanf(Latin1(),"%lu",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
size_t RString::ToSizeT(bool& valid)
{
	size_t v;
	#if __WORDSIZE == 32
		valid=(sscanf(Latin1(),"%u",&v)>0);
	#elif __WORDSIZE == 64
		valid=(sscanf(Latin1(),"%lu",&v)>0);
	#endif
	return(v);
}


//------------------------------------------------------------------------------
float RString::ToFloat(bool& valid)
{
	float v;
	valid=(sscanf(Latin1(),"%f",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
double RString::ToDouble(bool& valid)
{
	double v;
	valid=(sscanf(Latin1(),"%lf",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
bool RString::ToBool(bool& valid,bool strict)
{
	RString tmp;
	if(!strict)
		tmp=ToLower();
	else
		tmp=(*this);
	valid=true;
	if((tmp=="true")||(tmp=="1"))
		return(true);
	if((tmp=="false")||(tmp=="0"))
		return(false);
	valid=false;
	return(false);
}



//-----------------------------------------------------------------------------
// Static Conversion functions
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString RString::Number(const int nb)
{
	char Tmp[20];

	sprintf(Tmp,"%i",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const unsigned int nb)
{
	char Tmp[20];

	sprintf(Tmp,"%u",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const long nb)
{
	char Tmp[20];

	sprintf(Tmp,"%li",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const unsigned long nb)
{
	char Tmp[20];

	sprintf(Tmp,"%lu",nb);
	return(RString(Tmp));
}


//------------------------------------------------------------------------------
RString RString::Number(const long long nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%lld",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString RString::Number(const unsigned long long nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%llu",nb);
	res=Tmp;
	return(res);
}


//-----------------------------------------------------------------------------
RString RString::Number(const float nb,const char* format)
{
	char Tmp[20];

	sprintf(Tmp,format,nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const double nb,const char* format)
{
	char Tmp[20];

	sprintf(Tmp,format,nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const long double nb,const char* format)
{
	char Tmp[20];

	sprintf(Tmp,format,nb);
	return(RString(Tmp));
}



//-----------------------------------------------------------------------------
//
// class RCharCursor
//
//-----------------------------------------------------------------------------

///----------------------------------------------------------------------------
RCharCursor::RCharCursor(void)
	: Current(0), ActPtr(0), Str(0)
{
}


//-----------------------------------------------------------------------------
RCharCursor::RCharCursor(const RString& src)
	: Current(src.Data->Text), ActPtr(0), Str(&src)
{
}


//-----------------------------------------------------------------------------
void RCharCursor::Start(void)
{
	if(!Str)
		throw RException("No string defined");
	Current=Str->Data->Text;
	ActPtr=0;
}


//-----------------------------------------------------------------------------
void RCharCursor::Set(const RString& src)
{
	Str=&src;
	Current=Str->Data->Text;
	ActPtr=0;
}


//-----------------------------------------------------------------------------
void RCharCursor::GoTo(size_t idx)
{
	if(!Str)
		throw RException("No string defined");
	if(idx>=Str->Data->Len)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RCursor::GoTo : index outside string");
	#endif
	Current=&Str->Data->Text[idx];
	ActPtr=idx;
}


//-----------------------------------------------------------------------------
size_t RCharCursor::GetNb(void)
{
	if(!Str)
		throw RException("No string defined");
	return(Str->Data->Len);
}


//-----------------------------------------------------------------------------
size_t RCharCursor::GetPos(void)
{
	if(!Str)
		throw RException("No string defined");
	return(ActPtr);
}


//-----------------------------------------------------------------------------
bool RCharCursor::End(void) const
{
	if(!Str)
		throw RException("No string defined");
	return(ActPtr==Str->Data->Len);
}


//-----------------------------------------------------------------------------
void RCharCursor::Next(void)
{
	if(!Str)
		throw RException("No string defined");
	if(ActPtr==Str->Data->Len)
	{
		Current=Str->Data->Text;
		ActPtr=0;
	}
	else
	{
		Current++;
		ActPtr++;
	}
}


//-----------------------------------------------------------------------------
RChar RCharCursor::operator()(void) const
{
	if(!Str)
		throw RException("No string defined");
	return(*Current);
}


//-----------------------------------------------------------------------------
RChar RCharCursor::operator[](size_t idx) const
{
	if(!Str)
		throw RException("No string defined");
	if(idx>=Str->Data->Len)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RCursor::operator[] : index outside string");
	#endif
	return(Str->Data->Text[idx]);
}



//-----------------------------------------------------------------------------
//
// Dummy part
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void RString::Dummy(void)
{
	BasicString<RChar,RString>::Dummy();
}
