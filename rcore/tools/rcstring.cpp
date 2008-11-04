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
	: BasicString<char,RCString>(src)
{
}


//-----------------------------------------------------------------------------
RCString::RCString(const char* src,size_t len)
	: BasicString<char,RCString>(src,len)
{
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
		Data=new BasicString<char,RCString>::CharBuffer(ptr,len,maxlen);
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
	Data=new BasicString<char,RCString>::CharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
RCString::RCString(const RCString& src)
	: BasicString<char,RCString>(src)
{
}


//-----------------------------------------------------------------------------
RCString& RCString::operator=(const char* src)
{
	size_t len,maxlen;

	RDecRef<BasicString<char,RCString>::CharBuffer>(Data);
	maxlen=len=strlen(src);
	char* ptr=new char[maxlen+1];
	memcpy(ptr,src,sizeof(char)*len);
	ptr[len]=0;
	Data=new BasicString<char,RCString>::CharBuffer(ptr,len,maxlen);
	return(*this);
}


//-----------------------------------------------------------------------------
RCString& RCString::operator=(const std::string& src)
{
	(*this)=src.c_str();
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
			RDecRef<BasicString<char,RCString>::CharBuffer>(Data);
			char* ptr=new char[2];
			ptr[0]=src; ptr[1]=0;
			Data=new BasicString<char,RCString>::CharBuffer(ptr,len,maxlen);
		}
		else
		{
			BasicString<char,RCString>::Copy();
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
	char c(static_cast<char>(tolower(Data->Text[idx-1])));
	if((c>=char('a'))&&(c<=char('z')))
		return(c-char('a'));
	return(26);
}



//-----------------------------------------------------------------------------
//
// Dummy part
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void RCString::Dummy(void)
{
	BasicString<char,RCString>::Dummy();
}
