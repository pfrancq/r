/*

	R Project Library

	RString.cpp

	String - Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rstring.h>
using namespace R;


//------------------------------------------------------------------------------
// Generic defines
#ifndef __RMAXSTRING__
	#define __RMAXSTRING__ 30
#endif


//------------------------------------------------------------------------------
// TmpChar
class TmpChar
{
public:
	char* Tab;
	unsigned int Len;

	TmpChar(void) : Tab(0), Len(50) { Tab=new char[Len];}
	~TmpChar(void) {delete[] Tab;}
};


//------------------------------------------------------------------------------
//
// RString
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RString::RString(void) throw(bad_alloc)
{
	MaxLen=200;
	Len=0;
	Text=new RChar[MaxLen+1];
	(*Text)=0;
}


//------------------------------------------------------------------------------
inline void RString::Verify(const unsigned int maxlen) throw(bad_alloc)
{
	if(MaxLen<maxlen)
	{
		RChar* tmp;
		MaxLen=maxlen;
		tmp=new RChar[MaxLen+1];
		if(Text)
		{
			memcpy(tmp,Text,(Len+1)*sizeof(RChar));
			delete[] Text;
		}
		Text=tmp;
	}
}


//------------------------------------------------------------------------------
RString::RString(const char* text) throw(bad_alloc)
{
	const char *ptr1=text;
	RChar *ptr2;

	if(text)
	{
		MaxLen=Len=strlen(text);
		ptr2=Text=new RChar[MaxLen+1];
		while(*ptr1)
			(*(ptr2++))=(*(ptr1++));
		(*ptr2)=0;
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new RChar[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(text);
	}
}


//------------------------------------------------------------------------------
RString::RString(const RChar* text) throw(bad_alloc)
{
	const RChar *ptr1=text;
	RChar *ptr2;

	if(!text->IsNull())
	{
		MaxLen=Len=RChar::StrLen(text);
		ptr2=Text=new RChar[MaxLen+1];
		while(!ptr1->IsNull())
			(*(ptr2++))=(*(ptr1++));
		(*ptr2)=0;
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new RChar[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(text);
	}
}


//------------------------------------------------------------------------------
RString::RString(const std::string& text) throw(bad_alloc)
{
	MaxLen=50;
	Text=Latin1ToUnicode(text.c_str(),Len,MaxLen);
}


//------------------------------------------------------------------------------
RString::RString(const unsigned int maxlen) throw(bad_alloc)
{
	if(maxlen)
	{
		MaxLen=maxlen;
		Len=0;
		Text=new RChar[MaxLen+1];
		(*Text)=0;
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new RChar[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(maxlen!=0);
	}
}


//------------------------------------------------------------------------------
RString::RString(const RString& str) throw(bad_alloc)
{
	MaxLen=str.MaxLen;
	Len=str.Len;
	Text=new RChar[MaxLen+1];
	memcpy(Text,str.Text,(Len+1)*sizeof(RChar));
}


//------------------------------------------------------------------------------
RString::RString(const RString* str) throw(bad_alloc)
{
	if(str)
	{
		MaxLen=str->MaxLen;
		Len=str->Len;
		Text=new RChar[MaxLen+1];
		memcpy(Text,str->Text,(Len+1)*sizeof(RChar));
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new RChar[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(str);
	}
}


//------------------------------------------------------------------------------
RString& RString::operator=(const RString& str) throw(bad_alloc)
{
	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(RChar));
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator=(const char* text) throw(bad_alloc)
{
	const char* ptr1;
	RChar* ptr2;
    unsigned int len;

	RReturnValIfFail(text,*this);
	ptr1=text;
	len=strlen(text);
	Verify(len);
    Len=len;
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator=(const std::string& text) throw(bad_alloc)
{
	const char* ptr1;
	RChar* ptr2;
    unsigned int len;

	ptr1=text.c_str();
	len=strlen(text.c_str());
	Verify(len);
    Len=len;
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	return(*this);
}


//------------------------------------------------------------------------------
void RString::Copy(const char* text,unsigned int nb)
{
	const char* ptr1;
	RChar* ptr2;

	Verify(nb);
	Len=nb;
	for(nb++,ptr1=text,ptr2=Text;(--nb)&&(*ptr1);)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::Copy(const RChar* text,unsigned int nb)
{
	const RChar* ptr1;
	RChar* ptr2;

	Verify(nb);
	Len=nb;
	for(nb++,ptr1=text,ptr2=Text;(--nb)&&(!ptr1->IsNull());)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
RChar* RString::StrDup(void) const throw(bad_alloc)
{
	RChar* text,*ptr1;
	const RChar* ptr2=Text;

	ptr1=text=new RChar[Len+1];
	while(ptr2->IsNull())
		(*(ptr1++))=(*(ptr2++));
	(*ptr1)=0;
	return(text);
}


//------------------------------------------------------------------------------
void RString::StrUpr(void)
{
	RChar* ptr=Text;
	while(!ptr->IsNull())
		(*(ptr++))=RChar::ToUpper(*ptr);
}


//------------------------------------------------------------------------------
RString& RString::GetUpr(void) const
{
	RString *res=RString::GetString();

	(*res)=(*this);
	res->StrUpr();
	return(*res);
}


//------------------------------------------------------------------------------
void RString::StrUpr(const char* text) throw(bad_alloc)
{
	const char* ptr1=text;
	RChar* ptr2;

	RReturnIfFail(text);
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=RChar::ToUpper(*(ptr1++));
	(*ptr2)=0;
}

//------------------------------------------------------------------------------
void RString::StrUpr(const RChar* text) throw(bad_alloc)
{
	const RChar* ptr1=text;
	RChar* ptr2;

	RReturnIfFail(text);
	Len=RChar::StrLen(text);
	Verify(Len);
	ptr2=Text;
	while(!ptr1->IsNull())
		(*(ptr2++))=RChar::ToUpper(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrUpr(const RString& str) throw(bad_alloc)
{
	const RChar* ptr1=str.Text;
	RChar* ptr2;

	Verify(str.MaxLen);
	Len=str.Len;
	ptr2=Text;
	while(!ptr1->IsNull())
		(*(ptr2++))=RChar::ToUpper(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrLwr(void)
{
	RChar* ptr=Text;

	while(!ptr->IsNull())
		(*(ptr++))=RChar::ToLower(*ptr);
}


//------------------------------------------------------------------------------
RString& RString::GetLwr(void) const
{
	RString *res=RString::GetString();

	(*res)=(*this);
	res->StrLwr();
	return(*res);
}


//------------------------------------------------------------------------------
void RString::StrLwr(const RChar* text) throw(bad_alloc)
{
	const RChar* ptr1=text;
	RChar* ptr2;

	RReturnIfFail(text);
	Len=RChar::StrLen(text);
	Verify(Len);
	ptr2=Text;
	while(!ptr1->IsNull())
		(*(ptr2++))=RChar::ToLower(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrLwr(const char* text) throw(bad_alloc)
{
	const char* ptr1=text;
	RChar* ptr2;

	RReturnIfFail(text);
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=RChar::ToLower(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrLwr(const RString& str) throw(bad_alloc)
{
	const RChar* ptr1=str.Text;
	RChar* ptr2;

	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(RChar));
	ptr2=Text;
	while(!ptr1->IsNull())
		(*(ptr2++))=RChar::ToLower(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
char* RString::Latin1(void) const
{
	const RChar* ptr1=Text;
	char* ptr2;
	TmpChar* ptr=GetTemporaryObject<TmpChar,__RMAXSTRING__>();

	if(ptr->Len<Len+1)
	{
		ptr->Len=Len+1;
		char* tmp=new char[Len+1];
		delete[] ptr->Tab;
		ptr->Tab=tmp;
	}
	ptr2=ptr->Tab;
	while(!ptr1->IsNull())
	{
		(*(ptr2++))=ptr1->Latin1();
		ptr1++;
	}
	(*ptr2)=0;
	return(ptr->Tab);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RString& str) throw(bad_alloc)
{
	Verify(str.Len+Len);
	memcpy(&Text[Len],str.Text,(str.Len+1)*sizeof(RChar));
	Len+=str.Len;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RChar* text) throw(bad_alloc)
{
	const RChar* ptr1;
	RChar* ptr2;

	RReturnValIfFail(text,*this);
	ptr1=text;
	unsigned int len=RChar::StrLen(text);
	Verify(len+Len);
	ptr2=&Text[Len];
	while(!ptr1->IsNull())
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	Len+=len;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const char* text) throw(bad_alloc)
{
	const char* ptr1;
	RChar* ptr2;

	RReturnValIfFail(text,*this);
	ptr1=text;
	unsigned int len=strlen(text);
	Verify(len+Len);
	ptr2=&Text[Len];
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	Len+=len;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const char c) throw(bad_alloc)
{
	RChar* ptr;

	Verify(Len+1);
	ptr=&Text[Len++];
	(*(ptr++))=c;
	(*ptr)=0;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RChar c) throw(bad_alloc)
{
	RChar* ptr;

	Verify(Len+1);
	ptr=&Text[Len++];
	(*(ptr++))=c;
	(*ptr)=0;
	return(*this);
}


//------------------------------------------------------------------------------
int RString::HashIndex(const RString* str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=(*str->Text);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex(const RString& str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=(*str.Text);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex(const char* str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=(*str);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex(const RChar* str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=(*str);
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex2(const RString* str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=*(str->Text);
	if(c.IsNull()) return(26);
	c=(*(str->Text+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex2(const RString& str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=*(str.Text);
	if(c.IsNull()) return(26);
	c=(*(str.Text+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex2(const char* str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=*str;
	if(c.IsNull()) return(26);
	c=(*(str+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RString::HashIndex2(const RChar* str)
{
	RChar c;
	UChar u;
	UChar a;

	if(!str) return(26);
	c=*str;
	if(c.IsNull()) return(26);
	c=(*(str+1));
	if(c.IsNull()) return(26);
	u=RChar::ToLower(c).Unicode();
	a=RChar('a').Unicode();
	if((u>=a)&&(u<=RChar('z').Unicode()))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
RString* RString::GetString(void)
{
	RString *str=GetTemporaryObject<RString,__RMAXSTRING__>();

	str->Len=0;
	(*str->Text)=0;
	return(str);
}


//------------------------------------------------------------------------------
RChar* RString::Latin1ToUnicode(const char *str,unsigned int& len, unsigned int& maxlen)
{
	RChar* res=0;
	unsigned int i;
	RChar* ptr;
	len=0;

	if(str)
	{
    	len=strlen(str);
		if(maxlen<len)
			maxlen=len;
		res=ptr=new RChar[maxlen+1];
		i = len;
		while(i--)
	    	(*(ptr++)) = (*(str++));
	}
	return(res);
}


//------------------------------------------------------------------------------
RString::~RString(void)
{
	if(Text)
		delete[] Text;
}



//------------------------------------------------------------------------------
//
// Global Functions and Operators
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString& R::operator+(const RString& arg1,const RString& arg2)
{
	RString *res=RString::GetString();
	(*res)=arg1;
	return((*res)+=arg2);
}


//------------------------------------------------------------------------------
RString& R::operator+(const RString& arg1,const char* arg2)
{
	RString *res=RString::GetString();
	(*res)=arg1;
	return((*res)+=arg2);
}


//------------------------------------------------------------------------------
RString& R::operator+(const char* arg1,const RString& arg2)
{
	RString *res=RString::GetString();
	(*res)=arg1;
	return((*res)+=arg2);
}


//------------------------------------------------------------------------------
RString& R::itou(const int nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%i",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::itou(const unsigned int nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%u",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::ltou(const long nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%li",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::chrtou(const unsigned char c)
{
	char Tmp[2];
	RString *res=RString::GetString();

	(*Tmp)=c;
	(*(Tmp+1))=0;
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::ltou(const unsigned long nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%lu",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::ftou(const float nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::dtou(const double nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}
