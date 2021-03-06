 /*

	R Project Library

	RString.cpp

	Unicode String - Implementation.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rtextencoding.h>
#include <rcontainer.h>
#include <rcursor.h>
#include <rstack.h>
using namespace std;
using namespace R;



//-----------------------------------------------------------------------------
//
// Static data
//
//-----------------------------------------------------------------------------
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
RString::RString(char car)
	: BasicString<RChar,RString>(car)
{
}


//-----------------------------------------------------------------------------
RString::RString(RChar car)
	: BasicString<RChar,RString>(car)
{
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
const char* RString::ToLatin1(void) const
{
	if(Data==DataNull)
		return("");
	if(!static_cast<CharBuffer*>(Data)->Latin1)
	{
		static_cast<CharBuffer*>(Data)->Latin1=UnicodeToLatin1();
	}
	return(static_cast<CharBuffer*>(Data)->Latin1);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const char* src)
{
	mReturnValIfFail(src,*this);
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
void RString::Split(iRContainer<RString>& elements,const RChar car,const RChar del) const
{
	BasicString<RChar,RString>::Split(elements,car,del);
}


//------------------------------------------------------------------------------
void RString::GuessWords(iRContainer<RString>& elements,bool hyphen) const
{
	BasicString<RChar,RString>::GuessWords(elements,hyphen);
}


//-----------------------------------------------------------------------------
void RString::Concat(const iRContainer<RString>& elements,const RChar car)
{
	BasicString<RChar,RString>::Concat(elements,car);
}


//-----------------------------------------------------------------------------
size_t RString::HashCode(size_t max) const
{
    size_t hash(5381);

	 if(!Data)
		 return(0);
    for(const RChar* ptr=Data->Text;!ptr->IsNull();ptr++)
	 {
        hash =((hash<<5)+hash)+ptr->Unicode();   // hash * 33 + c
	 }
    return (hash%max);
}


//-----------------------------------------------------------------------------
int RString::SortOrder(const void* a,const void* b)
{
	RString* as(*((RString**)a));
	RString* bs(*((RString**)b));
	if(!as)
	{
		if(!bs)
			return(0);
		return(-1);
	}
	else if(!bs)
		return(1);
	return(as->Compare(*bs));
}


//-----------------------------------------------------------------------------
int RString::SortDesOrder(const void* a,const void* b)
{
	RString* as(*((RString**)a));
	RString* bs(*((RString**)b));
	if(!bs)
	{
		if(!as)
			return(0);
		return(-1);
	}
	else if(!as)
		return(1);
	return(bs->Compare(*as));
}


//-----------------------------------------------------------------------------
RChar* RString::Latin1ToUnicode(const char* src,size_t& len,size_t& maxlen)
{
	if(!src)
	{
		len=0;
		return(0);
	}

	if(!maxlen)
		maxlen=strlen(src);
	RString Str(RTextEncoding::GetUTF8Encoding()->ToUnicode(src,maxlen));
	size_t lenutf16(Str.Data->Len+1);
	RChar* res=new RChar[lenutf16];
	memcpy(res,Str.Data->Text,lenutf16*sizeof(RChar));
	len=maxlen=Str.Data->Len;
	return(res);
}


//-----------------------------------------------------------------------------
char* RString::UnicodeToLatin1(bool escape) const
{
	if(!Data)
		return(0);

	RCString Str(RTextEncoding::GetUTF8Encoding()->FromUnicode(*this,escape));
	size_t lenlatin1(Str.GetLen()+1);
	char* res=new char[lenlatin1];
	memcpy(res,Str(),lenlatin1*sizeof(char));
	return(res);
}


//------------------------------------------------------------------------------
void RString::GenerateException(const RString& msg) const
{
	throw RException("'"+(*this)+"': "+msg);
}


//------------------------------------------------------------------------------
char RString::ToChar(bool& valid) const
{
	int v;
	valid=(sscanf(ToLatin1(),"%d",&v)>0);
	return(static_cast<char>(v));
}


//------------------------------------------------------------------------------
int RString::ToInt(bool& valid) const
{
	int v;
	valid=(sscanf(ToLatin1(),"%d",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
unsigned int RString::ToUInt(bool& valid) const
{
	unsigned int v;
	valid=(sscanf(ToLatin1(),"%u",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
long RString::ToLong(bool& valid) const
{
	long v;
	valid=(sscanf(ToLatin1(),"%ld",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
unsigned long RString::ToULong(bool& valid) const
{
	unsigned long v;
	valid=(sscanf(ToLatin1(),"%lu",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
size_t RString::ToSizeT(bool& valid) const
{
	size_t v;
	#if __WORDSIZE == 32
		valid=(sscanf(ToLatin1(),"%u",&v)>0);
	#elif __WORDSIZE == 64
		valid=(sscanf(ToLatin1(),"%lu",&v)>0);
	#endif
	return(v);
}


//------------------------------------------------------------------------------
off_t RString::ToOffT(bool& valid) const
{
	off_t v;
	#ifdef _FILE_OFFSET_BITS
		#if _FILE_OFFSET_BITS == 64
			valid=(sscanf(ToLatin1(),"%lld",&v)==1);
		#else
			valid=(sscanf(ToLatin1(),"%d",&v)==1);
		#endif
	#else
		valid=(sscanf(ToLatin1(),"%ld",&v)==1);
	#endif
	return(v);
}


//------------------------------------------------------------------------------
float RString::ToFloat(bool& valid) const
{
	float v;
	valid=(sscanf(ToLatin1(),"%f",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
double RString::ToDouble(bool& valid) const
{
	double v;
	valid=(sscanf(ToLatin1(),"%lf",&v)>0);
	return(v);
}


//------------------------------------------------------------------------------
bool RString::ToBool(bool& valid,bool strict) const
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
	char Tmp[30];

	snprintf(Tmp,30,"%i",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const unsigned int nb)
{
	char Tmp[30];

	snprintf(Tmp,30,"%u",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const long nb)
{
	char Tmp[30];

	snprintf(Tmp,30,"%li",nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const unsigned long nb)
{
	char Tmp[30];

	snprintf(Tmp,30,"%lu",nb);
	return(RString(Tmp));
}


//------------------------------------------------------------------------------
RString RString::Number(const long long nb)
{
	char Tmp[30];
	RString res;

	snprintf(Tmp,30,"%lld",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString RString::Number(const unsigned long long nb)
{
	char Tmp[30];
	RString res;

	snprintf(Tmp,30,"%llu",nb);
	res=Tmp;
	return(res);
}


//-----------------------------------------------------------------------------
RString RString::Number(const float nb,const char* format)
{
	char Tmp[30];

	snprintf(Tmp,30,format,nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const double nb,const char* format)
{
	char Tmp[30];

	snprintf(Tmp,30,format,nb);
	return(RString(Tmp));
}


//-----------------------------------------------------------------------------
RString RString::Number(const long double nb,const char* format)
{
	char Tmp[30];

	snprintf(Tmp,30,format,nb);
	return(RString(Tmp));
}



//-----------------------------------------------------------------------------
//
// class RCharCursor
//
//-----------------------------------------------------------------------------

///----------------------------------------------------------------------------
RCharCursor::RCharCursor(void)
	: Current(0), ActPtr(0), Str(0), CurNbPtr(0), FirstPtr(0), LastPtr(0)
{
}


//-----------------------------------------------------------------------------
RCharCursor::RCharCursor(const RString& src,size_t min,size_t max)
{
	Set(src,min,max);
}


//-----------------------------------------------------------------------------
void RCharCursor::Set(const RString& src,size_t min,size_t max)
{
	mAssert(min<=max);
	Str=&src;
	if((max!=SIZE_MAX)&&(max<src.GetLen()))
		LastPtr=max+1;
	else
		LastPtr=src.GetLen();
	if((min<=max)&&(min<src.GetLen()))
		FirstPtr=min;
	else
		FirstPtr=0;
	Current=0;
	ActPtr=0;
	if((min==0)&&(max==SIZE_MAX))
		CurNbPtr=src.GetLen();   // The whole string is covered -> The exact number of characters is known.
	else
		CurNbPtr=SIZE_MAX;  // We must delay the computation until it is necessary.
}


//-----------------------------------------------------------------------------
void RCharCursor::Start(void)
{
	if(!Str)
		mThrowRException("No string defined");
	if(!Str->Data->Len)
	{
		Current=0;
		ActPtr=0;
		return;
	}
	Current=Str->Data->Text;
	ActPtr=FirstPtr;
	Current=&Str->Data->Text[FirstPtr];
}


//-----------------------------------------------------------------------------
void RCharCursor::StartFromEnd(void)
{
	if(!Str)
		mThrowRException("No string defined");
	if(!Str->Data->Len)
	{
		Current=0;
		ActPtr=0;
		return;
	}
	ActPtr=LastPtr-1;
	Current=&Str->Data->Text[LastPtr-1];
}



//-----------------------------------------------------------------------------
void RCharCursor::GoTo(size_t idx)
{
	if(!Str)
		mThrowRException("No string defined");
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
		mThrowRException("No string defined");
	if(CurNbPtr==SIZE_MAX)
	{
		const_cast<RCharCursor*>(this)->CurNbPtr=0;
		if(Str->Data->Text)
		{
			RChar* ptr(&Str->Data->Text[FirstPtr]);
			for(size_t i=FirstPtr;i<LastPtr;i++,ptr++)
				const_cast<RCharCursor*>(this)->CurNbPtr++;
		}
	}
	return(CurNbPtr);
}


//-----------------------------------------------------------------------------
size_t RCharCursor::GetPos(void)
{
	if(!Str)
		mThrowRException("No string defined");
	return(ActPtr-FirstPtr);
}


//-----------------------------------------------------------------------------
bool RCharCursor::Begin(void) const
{
	if(!Str)
		mThrowRException("No string defined");
	return(ActPtr==cNoRef);
}


//-----------------------------------------------------------------------------
bool RCharCursor::End(void) const
{
	if(!Str)
		mThrowRException("No string defined");
	return(ActPtr==LastPtr);
}


//-----------------------------------------------------------------------------
void RCharCursor::Next(size_t inc)
{
	if(!Str)
		mThrowRException("No string defined");
	if(!Str->Data->Len) return;
	if(ActPtr==LastPtr)
		Start();
	else
	{
		ActPtr+=inc;
		Current+=inc;
	}
}


//-----------------------------------------------------------------------------
void RCharCursor::Prev(size_t inc)
{
	if(!Str)
		mThrowRException("No string defined");
	if(!Str->Data->Len) return;
	if(ActPtr==cNoRef)
	{
		StartFromEnd();
		if(inc>1)
			Prev(inc);
	}
	else
	{
		if(inc>ActPtr)
			ActPtr=cNoRef;
		else
		{
			ActPtr-=inc;
			Current-=inc;
		}
	}
}


//-----------------------------------------------------------------------------
RChar RCharCursor::operator()(void) const
{
	if(!Str)
		mThrowRException("No string defined");
	if(!Current)
		return(0);
	return(*Current);
}


//-----------------------------------------------------------------------------
RChar RCharCursor::operator[](size_t idx) const
{
	if(!Str)
		mThrowRException("No string defined");
	if(idx>=LastPtr)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RCursor::operator[] : index outside string");
	#endif
	return(Str->Data->Text[idx]);
}
