 /*

	R Project Library

	RString.cpp

	Unicode String - Implementation.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rshareddata.h>
using namespace std;
using namespace R;



//-----------------------------------------------------------------------------
//
// Static data
//
//-----------------------------------------------------------------------------
RString::CharBuffer* RString::DataNull=0;
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
	: BasicString<RChar,RString>()
{
	if(src&&(!src->IsNull()))
	{
		size_t len=RChar::StrLen(src);
		RChar* ptr=new RChar[len+1];
		memcpy(ptr,src,sizeof(RChar)*len);
		ptr[len]=0;
		Data=new CharBuffer(ptr,len,len);
	}
	else
		Data=GetDataNull();
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
	len=src.IsNull();
	Data=new CharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
RString::RString(const RString& src)
	: BasicString<RChar,RString>(src)
{
}


//-----------------------------------------------------------------------------
RString::CharBuffer* RString::GetDataNull(void)
{
	if(!RString::DataNull)
	{
		RChar* ptr2=new RChar[1];
		(*ptr2)=0;
		RString::DataNull=new CharBuffer(ptr2,0,0);
	}
	else
		RIncRef<BasicCharBuffer>(RString::DataNull);
	return(static_cast<RString::CharBuffer*>(DataNull));
}


//-----------------------------------------------------------------------------
RString& RString::operator=(const RString& src)
{
	RIncRef(src.Data);
	RDecRef<BasicCharBuffer>(Data);
	Data=src.Data;
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator=(const char* src)
{
	size_t len,maxlen=0;
	RDecRef<BasicCharBuffer>(Data);
	RChar* ptr=Latin1ToUnicode(src,len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator=(const std::string& src)
{
	size_t len,maxlen=0;
	RDecRef<BasicCharBuffer>(Data);
	RChar* ptr=Latin1ToUnicode(src.c_str(),len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//-----------------------------------------------------------------------------
void RString::Clear(void)
{
	if(Data!=DataNull)
	{
		RDecRef<BasicCharBuffer>(Data);
		RIncRef<BasicCharBuffer>(DataNull);
		Data=DataNull;
	}
}


//-----------------------------------------------------------------------------
void RString::Copy(const char* text,size_t nb)
{
	RDecRef<BasicCharBuffer>(Data);
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
void RString::Copy(const RChar* text,size_t nb)
{
	RDecRef<BasicCharBuffer>(Data);
	if(text&&(!text->IsNull()))
	{
		size_t len=RChar::StrLen(text);
		if(nb>len)
			nb=len;
		RChar* ptr=new RChar[nb+1];
		memcpy(ptr,text,sizeof(RChar)*nb);
		ptr[nb]=0;
		Data=new CharBuffer(ptr,nb,nb);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
void RString::Copy(void)
{
	if(Data&&(Data->GetRefs()!=1))
	{
		if(Data!=DataNull)
		{
			RChar* ptr=new RChar[Data->MaxLen+1];
			size_t len=Data->Len,maxlen=Data->MaxLen;
			memcpy(ptr,Data->Text,sizeof(RChar)*(len+1));
			RDecRef<BasicCharBuffer>(Data);
			Data=new CharBuffer(ptr,len,maxlen);
		}
	}
}


//-----------------------------------------------------------------------------
const char* RString::Latin1(void) const
{
	if(!static_cast<CharBuffer*>(Data)->Latin1)
	{
		static_cast<CharBuffer*>(Data)->Latin1=UnicodeToLatin1(Data->Text,Data->Len);
	}
	return(static_cast<CharBuffer*>(Data)->Latin1);
}


//-----------------------------------------------------------------------------
const RChar& RString::operator[](size_t idx) const
{
	if(idx>=Data->Len)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RString::operator[] const : index outside string");
	#endif
	return(Data->Text[idx]);
}


//-----------------------------------------------------------------------------
RChar& RString::operator[](size_t idx)
{
	if(idx>=Data->Len)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RString::operator[] : index outside string");
	#endif
	return(Data->Text[idx]);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const RString& src)
{
	if(src.Data==DataNull)
		return(*this);
	if(Data==DataNull)
	{
		(*this)=src;
	}
	else
	{
		Copy();
		Data->Verify(src.Data->Len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],src.Data->Text,(src.Data->Len+1)*sizeof(RChar));
		Data->Len+=src.Data->Len;
		static_cast<CharBuffer*>(Data)->InvalidLatin1();
	}
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const RChar* src)
{
	RReturnValIfFail(src,*this);
	if(Data==DataNull)
	{
		(*this)=src;
	}
	else
	{
		size_t len=RChar::StrLen(src);
		Copy();
		Data->Verify(len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],src,sizeof(RChar)*len+1);
		Data->Len+=len;
		static_cast<CharBuffer*>(Data)->InvalidLatin1();
	}
	return(*this);
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
		Copy();
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
			RDecRef<BasicCharBuffer>(Data);
			RChar* ptr=Latin1ToUnicode(tab,len,maxlen);
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			Copy();
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
			RDecRef<BasicCharBuffer>(Data);
			RChar* ptr=new RChar[2];
			ptr[0]=src;
			ptr[1]=0;
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			Copy();
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
		text+=ptr->Unicode();
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
int RString::HashIndex(const RString& src)
{
	RChar c;
	UChar u;
	UChar a;

	if(!src.Data->Text) return(26);
	c=(*src.Data->Text);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//-----------------------------------------------------------------------------
int RString::HashIndex(const char* src)
{
	RChar c;
	UChar u;
	UChar a;

	if(!src) return(26);
	c=(*src);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//-----------------------------------------------------------------------------
int RString::HashIndex(const RChar* src)
{
	RChar c;
	UChar u;
	UChar a;

	if(!src) return(26);
	c=(*src);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//-----------------------------------------------------------------------------
int RString::HashIndex2(const RString& src)
{
	RChar c;
	UChar u;
	UChar a;

	if(!src.Data->Text) return(26);
	c=*(src.Data->Text);
	if(c.IsNull()) return(26);
	c=(*(src.Data->Text+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//-----------------------------------------------------------------------------
int RString::HashIndex2(const char* src)
{
	RChar c;
	UChar u;
	UChar a;

	if(!src) return(26);
	c=*src;
	if(c.IsNull()) return(26);
	c=(*(src+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//-----------------------------------------------------------------------------
int RString::HashIndex2(const RChar* src)
{
	RChar c;
	UChar u;
	UChar a;

	if(!src) return(26);
	c=*src;
	if(c.IsNull()) return(26);
	c=(*(src+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
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

	len=strlen(src);
	if(maxlen<len)
		maxlen=len;
	res=ptr=new RChar[maxlen+1];
	i = len;
	while(i--)
	   	(*(ptr++)) = (*(src++));
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


//-----------------------------------------------------------------------------
RString::~RString(void)
{
	RDecRef<BasicCharBuffer>(Data);
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
RString RString::Number(const unsigned char c)
{
	char Tmp[2];

	(*Tmp)=c;
	(*(Tmp+1))=0;
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
RString RString::Number(const unsigned long long nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%llu",nb);
	res=Tmp;
	return(res);
}


//-----------------------------------------------------------------------------
RString RString::Number(const float nb)
{
	char Tmp[20];

	sprintf(Tmp,"%f",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const double nb)
{
	char Tmp[20];

	sprintf(Tmp,"%f",nb);
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
unsigned int RCharCursor::GetNb(void)
{
	if(!Str)
		throw RException("No string defined");
	return(Str->Data->Len);
}


//-----------------------------------------------------------------------------
unsigned int RCharCursor::GetPos(void)
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
