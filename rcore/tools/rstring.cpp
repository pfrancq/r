/*

	R Project Library

	RString.cpp

	String - Implementation.

	Copyright 1999-2003 by the Universit� Libre de Bruxelles.

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
//
// RString
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RString::RString(void) throw(bad_alloc)
{
	MaxLen=200;
	Len=0;
	Text=new char[MaxLen+1];
	(*Text)=0;
}


//------------------------------------------------------------------------------
RString::RString(const char* text) throw(bad_alloc)
{
	const char *ptr1=text;
	char *ptr2;

	if(text)
	{
		MaxLen=Len=strlen(text);
		ptr2=Text=new char[MaxLen+1];
		while(*ptr1)
			(*(ptr2++))=(*(ptr1++));
		(*ptr2)=0;
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new char[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(text);
	}
}


//------------------------------------------------------------------------------
RString::RString(const unsigned int maxlen) throw(bad_alloc)
{
	if(maxlen)
	{
		MaxLen=maxlen;
		Len=0;
		Text=new char[MaxLen+1];
		(*Text)=0;
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new char[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(maxlen!=0);
	}
}


//------------------------------------------------------------------------------
RString::RString(const RString& str) throw(bad_alloc)
{
	MaxLen=str.MaxLen;
	Len=str.Len;
	Text=new char[MaxLen+1];
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
}


//------------------------------------------------------------------------------
RString::RString(const RString* str) throw(bad_alloc)
{
	if(str)
	{
		MaxLen=str->MaxLen;
		Len=str->Len;
		Text=new char[MaxLen+1];
		memcpy(Text,str->Text,(Len+1)*sizeof(char));
	}
	else
	{
		MaxLen=200;
		Len=0;
		Text=new char[MaxLen+1];
		(*Text)=0;
		RReturnIfFail(str);
	}
}


//------------------------------------------------------------------------------
inline void RString::Verify(const unsigned int maxlen) throw(bad_alloc)
{
	if(MaxLen<maxlen)
	{
		char* tmp;
		MaxLen=maxlen;
		tmp=new char[MaxLen+1];
		if(Text)
		{
			memcpy(tmp,Text,(Len+1)*sizeof(char));
			delete[] Text;
		}
		Text=tmp;
	}
}


//------------------------------------------------------------------------------
RString& RString::operator=(const RString& str) throw(bad_alloc)
{
	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator=(const char* text) throw(bad_alloc)
{
	const char *ptr1;
	char *ptr2;
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
void RString::Copy(const char* text,unsigned int nb)
{
	Verify(nb);
	Len=nb;
	memcpy(Text,text,nb*sizeof(char));
	Text[nb]=0;
}


//------------------------------------------------------------------------------
char* RString::StrDup(void) const throw(bad_alloc)
{
	char *text,*ptr1;
	const char *ptr2=Text;

	ptr1=text=new char[Len+1];
	while(*ptr2)
		(*(ptr1++))=(*(ptr2++));
	(*ptr1)=0;
	return(text);
}


//------------------------------------------------------------------------------
char RString::ToLower(const char c)
{
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if((c>='A')&&(c<='Z')) return(c+'a'-'A');
	if((c>='�')&&(c<='�')) return(c+'�'-'�');
	if((c>='�')&&(c<='�')) return(c+'�'-'�');
	#ifdef __BORLANDC__
		#pragma warn .sig
	#endif
	return(c);
}


//------------------------------------------------------------------------------
char RString::ToUpper(const char c)
{
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if((c>='a')&&(c<='z')) return(c-'a'+'A');
	if((c>='�')&&(c<='�')) return(c-'�'+'�');
	if((c>='�')&&(c<='�')) return(c-'�'+'�');
	#ifdef __BORLANDC__
		#pragma warn .sig
	#endif
	return(c);
}


//------------------------------------------------------------------------------
void RString::StrUpr(void)
{
	char *ptr=Text;
	while(*ptr)
		(*(ptr++))=ToUpper(*ptr);
}


//------------------------------------------------------------------------------
void RString::StrUpr(const char* text) throw(bad_alloc)
{
	const char *ptr1=text;
	char *ptr2;

	RReturnIfFail(text);
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=ToUpper(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrUpr(const RString& str) throw(bad_alloc)
{
	const char *ptr1=str.Text;
	char *ptr2;

	Verify(str.MaxLen);
	Len=str.Len;
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=ToUpper(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrLwr(void)
{
	char *ptr=Text;

	while(*ptr)
		(*(ptr++))=ToLower(*ptr);
}


//------------------------------------------------------------------------------
void RString::StrLwr(const char* text) throw(bad_alloc)
{
	const char *ptr1=text;
	char *ptr2;

	RReturnIfFail(text);
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=ToLower(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
void RString::StrLwr(const RString& str) throw(bad_alloc)
{
	const char *ptr1=str.Text;
	char *ptr2;

	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=ToLower(*(ptr1++));
	(*ptr2)=0;
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RString& str) throw(bad_alloc)
{
	Verify(str.Len+Len);
	memcpy(&Text[Len],str.Text,(str.Len+1)*sizeof(char));
	Len+=str.Len;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const char* text) throw(bad_alloc)
{
	const char *ptr1;
	char *ptr2;

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
	char* ptr;

	Verify(Len+1);
	ptr=&Text[Len++];
	(*(ptr++))=c;
	(*ptr)=0;
	return(*this);
}


//------------------------------------------------------------------------------
bool RString::operator==(const RString& str) const
{
	const char *ptr1=str.Text;
	char *ptr2=Text;

	if(Len!=str.Len) return(false);
	while((*ptr1)&&(*ptr2))
	{
		if((*ptr1)!=(*ptr2)) return(false);
		ptr1++;
		ptr2++;
	}
	return(true);
}


//------------------------------------------------------------------------------
bool RString::operator==(const char* str) const
{
	return(!strcmp(Text,str));
}


//------------------------------------------------------------------------------
bool RString::operator!=(const RString& str) const
{
	const char *ptr1=str.Text;
	char *ptr2=Text;

	if(Len!=str.Len) return(true);
	while((*ptr1)&&(*ptr2))
	{
		if((*ptr1)!=(*ptr2)) return(true);
		ptr1++;
		ptr2++;
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RString::operator!=(const char* str) const
{
	return(strcmp(Text,str));
}


//------------------------------------------------------------------------------
int RString::Compare(const RString &str) const
{
	return(strcmp(Text,str.Text));
}


//------------------------------------------------------------------------------
int RString::Compare(const RString *str) const
{
	return(strcmp(Text,str->Text));
}


//------------------------------------------------------------------------------
int RString::Compare(const char *str) const
{
	return(strcmp(Text,str));
}


//------------------------------------------------------------------------------
char RString::HashIndex(const RString* str)
{
	int c=(*str->Text);
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if(!c) return(26);
	if(c>='a'&&c<='z') return(c-'a');
	if(c>='A'&&c<='Z') return(c-'A');
	#ifdef __BORLANDC__
		#pragma warn .sig	
	#endif
	return(26);

}


//------------------------------------------------------------------------------
char RString::HashIndex(const RString& str)
{
	int c=(*str.Text);
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if(!c) return(26);
	if(c>='a'&&c<='z') return(c-'a');
	if(c>='A'&&c<='Z') return(c-'A');
	#ifdef __BORLANDC__
		#pragma warn .sig	
	#endif
	return(26);
}


//------------------------------------------------------------------------------
char RString::HashIndex(const char* str)
{
	int c=(*str);
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if(!c) return(26);
	if(c>='a'&&c<='z') return(c-'a');
	if(c>='A'&&c<='Z') return(c-'A');
	#ifdef __BORLANDC__
		#pragma warn .sig	
	#endif
	return(26);
}


//------------------------------------------------------------------------------
char RString::HashIndex2(const RString* str)
{
	int c=*(str->Text);
	if(!c) return(26);
	c=(*(str->Text+1));
	if(!c) return(26);
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if(c>='a'&&c<='z') return(c-'a');
	if(c>='A'&&c<='Z') return(c-'A');
	#ifdef __BORLANDC__
		#pragma warn .sig	
	#endif
	return(26);

}


//------------------------------------------------------------------------------
char RString::HashIndex2(const RString& str)
{
	int c=(*str.Text);
	if(!c) return(26);
	c=(*(str.Text+1));
	if(!c) return(26);
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if(c>='a'&&c<='z') return(c-'a');
	if(c>='A'&&c<='Z') return(c-'A');
	#ifdef __BORLANDC__
		#pragma warn .sig	
	#endif
	return(26);
}


//------------------------------------------------------------------------------
char RString::HashIndex2(const char* str)
{
	int c=(*str);
	if(!c) return(26);
	c=(*(str+1));
	if(!c) return(26);
	#ifdef __BORLANDC__
		#pragma warn -sig
	#endif
	if(c>='a'&&c<='z') return(c-'a');
	if(c>='A'&&c<='Z') return(c-'A');
	#ifdef __BORLANDC__
		#pragma warn .sig	
	#endif
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
char* RString::GetCString(void)
{
	return(*GetTemporaryObject<char*,__RMAXSTRING__>());
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
RString& R::itoa(const int nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%i",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::itoa(const unsigned int nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%u",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::ltoa(const long nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%li",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::chr(const unsigned char c)
{
	char Tmp[2];
	RString *res=RString::GetString();

	(*Tmp)=c;
	(*(Tmp+1))=0;
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::ltoa(const unsigned long nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%lu",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::ftoa(const float nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}


//------------------------------------------------------------------------------
RString& R::dtoa(const double nb)
{
	char Tmp[20];
	RString *res=RString::GetString();

	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}
