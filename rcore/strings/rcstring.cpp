/*

	R Project Library

	RCString.cpp

	C String - Implementation.

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
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcstring.h>
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
RCString::RCString(char car)
	: BasicString<char,RCString>(car)
{
}


//-----------------------------------------------------------------------------
RCString::RCString(const RCString& src)
	: BasicString<char,RCString>(src)
{
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
size_t RCString::HashCode(size_t max) const
{
    size_t hash(5381);

	 if(!Data)
		 return(0);
    for(const char* ptr=Data->Text;!ptr;ptr++)
	 {
        hash =((hash<<5)+hash)+(*ptr);   // hash * 33 + c
	 }
    return (hash%max);
}


//-----------------------------------------------------------------------------
void RCString::Split(iRContainer<RCString>& elements,const char car,const char del) const
{
	BasicString<char,RCString>::Split(elements,car,del);
}


//------------------------------------------------------------------------------
void RCString::GuessWords(iRContainer<RCString>& elements,bool hyphen) const
{
	BasicString<char,RCString>::GuessWords(elements,hyphen);
}


//-----------------------------------------------------------------------------
void RCString::Concat(const iRContainer<RCString>& elements,const char car)
{
	BasicString<char,RCString>::Concat(elements,car);
}
