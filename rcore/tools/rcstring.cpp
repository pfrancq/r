/*

	R Project Library

	RCString.cpp

	C String - Implementation.

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
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcstring.h>
#include <basicstring.hh>
using namespace std;
using namespace R;



//-----------------------------------------------------------------------------
//
// Static data
//
//-----------------------------------------------------------------------------
BasicString<char,RCString>::BasicCharBuffer* RCString::DataNull=0;
const RCString RCString::Null;



//-----------------------------------------------------------------------------
//
// RCString
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RCString::RCString(void)
	: BasicString<char,RCString>()
{
	Data=GetDataNull();
}


//-----------------------------------------------------------------------------
RCString::RCString(const char* src)
	: BasicString<char,RCString>()
{
	if(src)
	{
		size_t len,maxlen;
		len=maxlen=strlen(src);
		char* ptr=new char[maxlen+1];
		memcpy(ptr,src,sizeof(char)*(len+1));
		Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
RCString::RCString(const char* src,size_t len)
	: BasicString<char,RCString>()
{
	if(src)
	{
		size_t maxlen;
		maxlen=len;
		char* ptr=new char[maxlen+1];
		memcpy(ptr,src,sizeof(char)*len);
		ptr[len]=0;
		Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
RCString::RCString(const std::string& src)
	: BasicString<char,RCString>()
{
	const char* tab=src.c_str();
	size_t len=strlen(tab);

	if(len)
	{
		size_t maxlen=len;
		char* ptr=new char[maxlen+1];
		memcpy(ptr,tab,sizeof(char)*(len+1));
		Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
RCString::RCString(char src)
	: BasicString<char,RCString>()
{
	size_t len,maxlen=1;
	char* ptr=new char[2];

	ptr[0]=src;
	ptr[1]=0;
	if(src)
		len=1;
	else
		len=0;
	Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
RCString::RCString(const RCString& src)
	: BasicString<char,RCString>(src)
{
}


//-----------------------------------------------------------------------------
BasicString<char,RCString>::BasicCharBuffer* RCString::GetDataNull(void)
{
	if(!RCString::DataNull)
	{
		char* ptr2=new char[1];
		(*ptr2)=0;
		RCString::DataNull=new BasicString<char,RCString>::BasicCharBuffer(ptr2,0,0);
		RCString* ptr=const_cast<RCString*>(&RCString::Null);
		ptr->Data = RCString::DataNull;
	}
	else
		RIncRef<BasicString<char,RCString>::BasicCharBuffer>(RCString::DataNull);
	return(RCString::DataNull);
}


//-----------------------------------------------------------------------------
RCString& RCString::operator=(const RCString& src)
{
	RIncRef(src.Data);
	RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
	Data=src.Data;
	return(*this);
}


//-----------------------------------------------------------------------------
RCString& RCString::operator=(const char* src)
{
	size_t len,maxlen;

	RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
	maxlen=len=strlen(src);
	char* ptr=new char[maxlen+1];
	memcpy(ptr,src,sizeof(char)*len);
	ptr[len]=0;
	Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
	return(*this);
}


//-----------------------------------------------------------------------------
RCString& RCString::operator=(const std::string& src)
{
	(*this)=src.c_str();
	return(*this);
}


//-----------------------------------------------------------------------------
void RCString::Clear(void)
{
	if(Data!=DataNull)
	{
		RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
		RIncRef<BasicString<char,RCString>::BasicCharBuffer>(DataNull);
		Data=DataNull;
	}
}


//-----------------------------------------------------------------------------
void RCString::Copy(const char* src,unsigned int nb)
{
	RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
	if(src)
	{
		size_t len=strlen(src);
		if(nb<len)
			nb=len;
		char* ptr=new char[nb+1];
		memcpy(ptr,src,sizeof(char)*len);
		ptr[len]=0;
		Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,nb);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
void RCString::Copy(void)
{
	if(Data&&(Data->GetRefs()!=1))
	{
		if(Data!=DataNull)
		{
			char* ptr=new char[Data->MaxLen+1];
			size_t len=Data->Len,maxlen=Data->MaxLen;
			memcpy(ptr,Data->Text,sizeof(char)*(len+1));
			RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
			Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
		}
	}
}


//-----------------------------------------------------------------------------
void RCString::SetLen(size_t len)
{
	BasicString<char,RCString>::SetLen<BasicCharBuffer>(len);
}


//-----------------------------------------------------------------------------
void RCString::SetLen(size_t len,const RCString& str)
{
	BasicString<char,RCString>::SetLen<BasicCharBuffer>(len,str);
}


//-----------------------------------------------------------------------------
bool RCString::ContainOnlySpaces(void) const
{
	return(BasicString<char,RCString>::ContainOnlySpaces());
}


//-----------------------------------------------------------------------------
const char& RCString::operator[](size_t idx) const
{
	if(idx>=Data->Len)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RCString::operator[] const : index outside string");
	#endif
	return(Data->Text[idx]);
}


//-----------------------------------------------------------------------------
char& RCString::operator[](size_t idx)
{
	if(idx>=Data->Len)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RCString::operator[] : index outside string");
	#endif
	return(Data->Text[idx]);
}


//-----------------------------------------------------------------------------
RCString RCString::Mid(size_t idx,int len) const
{
	return(BasicString<char,RCString>::Mid<BasicCharBuffer>(idx,len));
}


//-----------------------------------------------------------------------------
RCString& RCString::operator+=(const RCString& src)
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
		memcpy(&Data->Text[Data->Len],src.Data->Text,(src.Data->Len+1)*sizeof(char));
		Data->Len+=src.Data->Len;
	}
	return(*this);
}


//-----------------------------------------------------------------------------
RCString& RCString::operator+=(const char* src)
{
	RReturnValIfFail(src,*this);
	if(Data==DataNull)
	{
		(*this)=src;
	}
	else
	{
		size_t len=strlen(src);
		Copy();
		Data->Verify(len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],src,sizeof(char)*len+1);
		Data->Len+=len;
	}
	return(*this);
}


//-----------------------------------------------------------------------------
RCString& RCString::operator+=(const char src)
{
	if(src)
	{
		if(Data==DataNull)
		{
			size_t maxlen=1,len=1;
			RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
			char* ptr=new char[2];
			ptr[0]=src; ptr[1]=0;
			Data=new BasicString<char,RCString>::BasicCharBuffer(ptr,len,maxlen);
		}
		else
		{
			Copy();
			Data->Verify(Data->Len+1);
			char* ptr=&Data->Text[Data->Len++];
			(*(ptr++))=src;
			(*ptr)=0;
		}
	}
	return(*this);
}


//-----------------------------------------------------------------------------
const char* RCString::operator()(void) const
{
	return(Data->Text);
}


//-----------------------------------------------------------------------------
RCString::operator std::string () const
{
	return(Data->Text);
}


//-----------------------------------------------------------------------------
RCString::operator const char* () const
{
	return(Data->Text);
}


//-----------------------------------------------------------------------------
int RCString::Compare(const RCString& src) const
{
	if(!Data)
	{
		if(!src.Data)
			return(0);
		return(-1);
	}
	else if(!src.Data)
		return(1);
	return(strcmp(Data->Text,src.Data->Text));
}


//-----------------------------------------------------------------------------
int RCString::Compare(const char* src) const
{
	if(!Data)
	{
		if(!src)
			return(0);
		return(-1);
	}
	else if(!src)
		return(1);
	return(strcmp(Data->Text,src));
}


//-----------------------------------------------------------------------------
size_t RCString::HashIndex(size_t idx) const
{
	if((!Data)||(Data->Len<idx)) return(26);
	char c(tolower(Data->Text[idx-1]));
	if((c>=char('a'))&&(c<=char('z')))
		return(c-char('a'));
	return(26);
}


//-----------------------------------------------------------------------------
RCString RCString::ToUpper(void) const
{
	return(BasicString<char,RCString>::ToUpper());
}


//-----------------------------------------------------------------------------
RCString RCString::ToLower(void) const
{
	return(BasicString<char,RCString>::ToLower());
}


//-----------------------------------------------------------------------------
RCString RCString::Trim(void) const
{
	return(BasicString<char,RCString>::Trim());
}


//-----------------------------------------------------------------------------
int RCString::Find(const char car,int pos,bool CaseSensitive) const
{
	return(BasicString<char,RCString>::Find(car,pos,CaseSensitive));
}


//-----------------------------------------------------------------------------
int RCString::FindStr(const RCString& str,int pos,bool CaseSensitive) const
{
	return(BasicString<char,RCString>::FindStr(str,pos,CaseSensitive));
}


//-----------------------------------------------------------------------------
void RCString::Split(RContainer<RCString,true,false>& elements,const char car) const
{
	BasicString<char,RCString>::Split(elements,car);
}


//-----------------------------------------------------------------------------
RCString::~RCString(void)
{
	RDecRef<BasicString<char,RCString>::BasicCharBuffer>(Data);
}
