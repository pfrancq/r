/*

	Rainbow Library Project

	RString.cpp

	String - Implementation.

	(C) 1999-2000 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstd.h>
#include <rstd/rstring.h>
using namespace RStd;



//-----------------------------------------------------------------------------
//
// RString
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString::RString(void) throw(bad_alloc)
{
	MaxLen=200;
	Len=0;
	Text=new char[MaxLen+1];
	(*Text)=0;
}


//-----------------------------------------------------------------------------
RString::RString(const char *text) throw(bad_alloc)
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


//-----------------------------------------------------------------------------
RString::RString(const int maxlen) throw(bad_alloc)
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


//-----------------------------------------------------------------------------
RString::RString(const RString& str) throw(bad_alloc)
{
	MaxLen=str.MaxLen;
	Len=str.Len;
	Text=new char[MaxLen+1];
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
inline void RString::Verify(const int maxlen) throw(bad_alloc)
{
	if(MaxLen<maxlen)
	{
	  	char *tmp;
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


//-----------------------------------------------------------------------------
RString& RString::operator=(const RString &str) throw(bad_alloc)
{
	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator=(const char *text) throw(bad_alloc)
{
	const char *ptr1;
	char *ptr2;

	RReturnValIfFail(text,*this);
	ptr1=text;
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	return(*this);
}


//-----------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------
inline void RString::StrUpr(void)
{
	char *ptr=Text;
	while(*ptr)
	{
		#if __BORLANDC__
			#pragma warn -sig
		#endif
		if(((*ptr)>='a')&&((*ptr)<='z'))
			(*ptr)-='a'-'A';
		#if __BORLANDC__
			#pragma warn .sig
		#endif
		ptr++;
	}
}


//-----------------------------------------------------------------------------
void RString::StrUpr(const char *text) throw(bad_alloc)
{
	const char *ptr1;
	char *ptr2;

	RReturnIfFail(text);
	ptr1=text;
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	StrUpr();
}


//-----------------------------------------------------------------------------
void RString::StrUpr(const RString &str) throw(bad_alloc)
{
	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
	StrUpr();
}


//-----------------------------------------------------------------------------
inline void RString::StrLwr(void)
{
	char *ptr=Text;

	while(*ptr)
	{
		#if __BORLANDC__
			#pragma warn -sig
		#endif
		if(((*ptr)>='A')&&((*ptr)<='Z'))
			(*ptr)+='a'-'A';
		#if __BORLANDC__
			#pragma warn .sig
		#endif
		ptr++;
	}
}


//-----------------------------------------------------------------------------
void RString::StrLwr(const char *text) throw(bad_alloc)
{
	const char *ptr1=text;
	char *ptr2;

	RReturnIfFail(text);
	Len=strlen(text);
	Verify(Len);
	ptr2=Text;
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	StrLwr();
}


//-----------------------------------------------------------------------------
void RString::StrLwr(const RString &str) throw(bad_alloc)
{
	Verify(str.MaxLen);
	Len=str.Len;
	memcpy(Text,str.Text,(Len+1)*sizeof(char));
	StrLwr();
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const RString &str) throw(bad_alloc)
{
	Verify(str.Len+Len);
	memcpy(&Text[Len],str.Text,(str.Len+1)*sizeof(char*));
	Len+=str.Len;
	return(*this);
}


//-----------------------------------------------------------------------------
RString& RString::operator+=(const char *text) throw(bad_alloc)
{
	const char *ptr1;
	char *ptr2;

	RReturnValIfFail(text,*this);
	ptr1=text;
	int len=strlen(text);
	Verify(len+Len);
	ptr2=&Text[Len];
	while(*ptr1)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;
	Len+=len;
	return(*this);
}


//-----------------------------------------------------------------------------
bool RString::operator==(const RString &str) const
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


//-----------------------------------------------------------------------------
bool RString::operator!=(const RString &str) const
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


//-----------------------------------------------------------------------------
int RString::Compare(const RString &str) const
{
	const char *ptr1=str.Text;
	char *ptr2=Text;

	if(Len!=str.Len)	return(Len-str.Len);
	while((*ptr1)&&(*ptr2)&&(*ptr1)==(*ptr2))
	{
		ptr1++;
		ptr2++;
	}
	return((*ptr2)-(*ptr1));
}


//-----------------------------------------------------------------------------
int RString::Compare(const RString *str) const
{
	const char *ptr1;
	char *ptr2=Text;

	RReturnValIfFail(Text,1);
	if(!ptr1) return(1);
	ptr1=str->Text;
	if(Len!=str->Len) return(Len-str->Len);
	while((*ptr1)&&(*ptr2)&&(*ptr1)==(*ptr2))
	{
		ptr1++;
		ptr2++;
	}
	return((*ptr2)-(*ptr1));
}


//-----------------------------------------------------------------------------
int RString::Compare(const char *str) const
{
	return(strcmp(Text,str));
}


//-----------------------------------------------------------------------------
char RString::HashIndex(void) const
{
	int c=(*Text);
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


//-----------------------------------------------------------------------------
RString* RString::GetString(void)
{
	RString *str=GetTemporaryObject<RString,__RMAXSTRING__>();

	str->Len=0;
	(*str->Text)=0;
	return(str);
}


//-----------------------------------------------------------------------------
char* RString::GetCString(void)
{
	return(*GetTemporaryObject<char*,__RMAXSTRING__>());
}


//-----------------------------------------------------------------------------
RString::~RString(void)
{
	if(Text)
		delete[] Text;
}



//-----------------------------------------------------------------------------
//
// Global Functions and Operators
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString& RStd::operator+(const RString &arg1,const RString &arg2)
{
	RString *res=RString::GetString();
	(*res)=arg1;
	return((*res)+=arg2);
}


//-----------------------------------------------------------------------------
RString& RStd::operator+(const RString &arg1,const char *arg2)
{
	RString *res=RString::GetString();
	(*res)=arg1;
	return((*res)+=arg2);
}


//-----------------------------------------------------------------------------
RString& RStd::operator+(const char *arg1,const RString &arg2)
{
	RString *res=RString::GetString();
	(*res)=arg1;
	return((*res)+=arg2);
}

//-----------------------------------------------------------------------------
RString& RStd::itoa(const int nb)
{
	char Tmp[20];	
	RString *res=RString::GetString();
	
	sprintf(Tmp,"%i",nb);
	(*res)=Tmp;
	return((*res));
}


//-----------------------------------------------------------------------------
RString& RStd::itoa(const unsigned int nb)
{
	char Tmp[20];	
	RString *res=RString::GetString();
	
	sprintf(Tmp,"%u",nb);
	(*res)=Tmp;
	return((*res));
}


//-----------------------------------------------------------------------------
RString& RStd::ltoa(const long nb)
{
	char Tmp[20];	
	RString *res=RString::GetString();
	
	sprintf(Tmp,"%li",nb);
	(*res)=Tmp;
	return((*res));
}


//-----------------------------------------------------------------------------
RString& RStd::chr(const unsigned char c)
{
	char Tmp[2];	
	RString *res=RString::GetString();
	
	(*Tmp)=c;
	(*(Tmp+1))=0;
	(*res)=Tmp;
	return((*res));
}


//-----------------------------------------------------------------------------
RString& RStd::ltoa(const unsigned long nb)
{
	char Tmp[20];	
	RString *res=RString::GetString();
	
	sprintf(Tmp,"%lu",nb);
	(*res)=Tmp;
	return((*res));
}


//-----------------------------------------------------------------------------
RString& RStd::ftoa(const float nb)
{
	char Tmp[20];	
	RString *res=RString::GetString();
	
	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}


//-----------------------------------------------------------------------------
RString& RStd::dtoa(const double nb)
{
	char Tmp[20];	
	RString *res=RString::GetString();
	
	sprintf(Tmp,"%f",nb);
	(*res)=Tmp;
	return((*res));
}
