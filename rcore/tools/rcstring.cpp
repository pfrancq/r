/*

	R Project Library

	RCString.cpp

	C String - Implementation.

	Copyright 1999-2004 by the Université libre de Bruxelles.

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
#include <rstd/rcstring.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// Static data
//
//------------------------------------------------------------------------------

RCString::CharBuffer* RCString::DataNull=0;
const RCString RCString::Null;



//------------------------------------------------------------------------------
//
// RCString::CharBuffer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void RCString::CharBuffer::Verify(const unsigned int maxlen) throw(std::bad_alloc)
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
//
// RCString
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCString::RCString(void) throw(std::bad_alloc)
	: Data(GetDataNull())
{
}


//------------------------------------------------------------------------------
RCString::RCString(const char* text) throw(std::bad_alloc)
	: Data(0)
{
	if(text)
	{
		unsigned int len,maxlen;
		len=maxlen=strlen(text);
		char* ptr=new char[maxlen+1];
		memcpy(ptr,text,sizeof(char)*(len+1));
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RCString::RCString(const char* text,unsigned int len) throw(std::bad_alloc)
	: Data(0)
{
	if(text)
	{
		unsigned int maxlen;
		maxlen=len;
		char* ptr=new char[maxlen+1];
		memcpy(ptr,text,sizeof(char)*len);
		ptr[len]=0;
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RCString::RCString(const std::string& text) throw(std::bad_alloc)
	: Data(0)
{
	const char* tab=text.c_str();
	unsigned int len=strlen(tab);

	if(len)
	{
		unsigned int maxlen=len;
		char* ptr=new char[maxlen+1];
		memcpy(ptr,tab,sizeof(char)*(len+1));
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RCString::RCString(const unsigned int maxlen) throw(std::bad_alloc)
	: Data(0)
{
	if(maxlen)
	{
		char* ptr=new char[maxlen+1];
		(*ptr)=0;
		Data=new CharBuffer(ptr,0,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RCString::RCString(const RCString& str) throw(std::bad_alloc)
	: Data(str.Data)
{
	RIncRef<CharBuffer>(Data);
}


//------------------------------------------------------------------------------
RCString::CharBuffer* RCString::GetDataNull(void)
{
	if(!RCString::DataNull)
	{
		char* ptr2=new char[1];
		(*ptr2)=0;
		RCString::DataNull=new CharBuffer(ptr2,0,0);
		RCString* ptr=const_cast<RCString*>(&RCString::Null);
		ptr->Data = RCString::DataNull;
	}
	else
		RIncRef<CharBuffer>(RCString::DataNull);
	return(RCString::DataNull);
}


//------------------------------------------------------------------------------
RCString& RCString::operator=(const RCString& str) throw(std::bad_alloc)
{
	RIncRef(str.Data);
	RDecRef<CharBuffer>(Data);
	Data=str.Data;
	return(*this);
}


//------------------------------------------------------------------------------
RCString& RCString::operator=(const char* text) throw(std::bad_alloc)
{
	unsigned int len,maxlen;

	RDecRef<CharBuffer>(Data);
	maxlen=len=strlen(text);
	char* ptr=new char[maxlen+1];
	memcpy(ptr,text,sizeof(char)*len);
	ptr[len]=0;
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//------------------------------------------------------------------------------
RCString& RCString::operator=(const std::string& text) throw(std::bad_alloc)
{
	(*this)=text.c_str();
	return(*this);
}


//------------------------------------------------------------------------------
void RCString::Clear(void)
{
	if(Data!=DataNull)
	{
		RDecRef<CharBuffer>(Data);
		RIncRef<CharBuffer>(DataNull);
		Data=DataNull;
	}
}


//------------------------------------------------------------------------------
void RCString::Copy(const char* text,unsigned int nb)
{
	RDecRef<CharBuffer>(Data);
	if(text)
	{
		unsigned int len=strlen(text);
		if(nb<len)
			nb=len;
		char* ptr=new char[nb+1];
		memcpy(ptr,text,sizeof(char)*len);
		ptr[len]=0;
		Data=new CharBuffer(ptr,len,nb);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
void RCString::Copy(void)
{
	if(Data&&(Data->GetRefs()!=1))
	{
		if(Data!=DataNull)
		{
			char* ptr=new char[Data->MaxLen+1];
			unsigned int len=Data->Len,maxlen=Data->MaxLen;
			memcpy(ptr,Data->Text,sizeof(char)*(len+1));
			RDecRef<CharBuffer>(Data);
			Data=new CharBuffer(ptr,len,maxlen);
		}
	}
}


//------------------------------------------------------------------------------
RCString RCString::ToUpper(void) const
{
	unsigned int len = Data->Len;
	char* ptr = Data->Text;

	while(len)
	{
		if((*ptr)!=toupper(*ptr))
		{
			RCString str(*this);
			str.Copy();
			ptr = str.Data->Text+(ptr-Data->Text);
	    	while(len)
			{
				(*ptr) = toupper(*ptr);
				len--;
				ptr++;
			}
			return(str);
		}
		len--;
		ptr++;
	}
	return(*this);
}


//------------------------------------------------------------------------------
RCString RCString::ToLower(void) const
{
	unsigned int len = Data->Len;
	char* ptr = Data->Text;

	while(len)
	{
		if((*ptr)!=tolower(*ptr))
		{
			RCString str(*this);
			str.Copy();
			ptr = str.Data->Text+(ptr-Data->Text);
	    	while(len)
			{
				(*ptr) = tolower(*ptr);
				len--;
				ptr++;
			}
			return(str);
		}
		len--;
		ptr++;
	}
	return(*this);
}


//------------------------------------------------------------------------------
void RCString::SetLen(unsigned int len)
{
	if(len<=Data->Len)
	{
		Data->Len=len;
	}
	else
	{
		if(Data==DataNull)
		{
			char* ptr=new char[len+1];
			RDecRef<CharBuffer>(Data);
			Data=new CharBuffer(ptr,len,len);
		}
		else
			Data->Verify(len+1);
	}
	Data->Text[len]=0;
}


//------------------------------------------------------------------------------
char RCString::operator[](int pos) const
{
	if(pos>=Data->Len) return(0);
	return(Data->Text[pos]);
}


//------------------------------------------------------------------------------
int RCString::Find(char car,int pos,bool CaseSensitive) const
{
	char* start;
	bool left;
	unsigned int max;        // Maximal number of character to search.


	// Initialise the search
	if(!CaseSensitive)
		car=toupper(car);
	if(pos<0)
	{
		// From right
		left=false;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=Data->Len+pos-1;
		if(pos<=0) return(-1);
		start=&Data->Text[pos];
		max=pos+1;
	}
	else
	{
		// From left
		left=true;

		// Start from 0 with maximal Len-pos+1 character to test.
		start=&Data->Text[pos];
		max=Data->Len-pos+1;
	}

	// Search for the maximal number of character
	for(max++;--max;)
	{
		if(((CaseSensitive)&&((*start)==car)) || ((!CaseSensitive)&&(toupper(*start)==car)))
			return(pos);
		if(left)
		{
			start++;
			pos++;
		}
		else
		{
			pos--;
			start--;
		}
	}
	return(-1);
}


//------------------------------------------------------------------------------
int RCString::FindStr(const RCString str,int pos,bool CaseSensitive) const
{
	char* start;
	const char* toFind;
	unsigned int max;        // Maximal number of character to search.
	int avanct;
	int maxlen;  //max number of char contained in the string to search
	int incr;
	RCString search(str);

	// Initialise the search
	if(!CaseSensitive)
		search=search.ToUpper();
	if(pos<0)
	{
		// From right
		incr=-1;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=Data->Len+pos+search.GetLen()-1;
		if(pos<=0) return(-1);
		start=&Data->Text[pos];
		max=pos+1;

		// Init string to find (here the last character)
		toFind=search();
		toFind+=search.GetLen()-1;
	}
	else
	{
		// From left
		incr=+1;

		// Start from 0 with maximal Len-pos+1 character to test.
		start=&Data->Text[pos];
		max=Data->Len-pos+1;

		// Init string to find
		toFind=search();
	}

	// If string to find is longer than the string return -1
	if(search.GetLen()>max)
		return(-1);

	// Search for the maximal number of character
	for(max++;--max;)
	{
		if(((CaseSensitive)&&((*start)==(*toFind))) || ((!CaseSensitive)&&(toupper(*start)==(*toFind))))
		{
			if(max>=search.GetLen())
			{
				avanct=0;
				maxlen=search.GetLen();
				bool found=true;
				for(maxlen++;--maxlen,found;)
				{
					if(((CaseSensitive)&&((*start)==(*toFind))) || ((!CaseSensitive)&&(toupper(*start)==(*toFind))))
					{
						if(!(maxlen-1))
						{
							// String found
							if(incr>0)
								return(pos);
							else
								return(pos-search.GetLen());
						}
						start+=incr;
						toFind+=incr;
						avanct+=incr;
					}
					else
					{
						start-=avanct;
						toFind-=avanct;
						found=false;
					}
				}
				if(found)
				{
					// String found
					if(incr>0)
						return(pos);
					else
						return(pos-search.GetLen());
				}
			}
		}
		start+=incr;
		pos+=incr;
	}
	return(-1);

}


//------------------------------------------------------------------------------
RCString RCString::Mid(unsigned int idx,unsigned int len) const
{
	RCString res;
    const char* ptr1;
	char* ptr2;

	// If the index is greather than the length -> return a null string.
	if(Data->Len<=idx) return(res);

	// Computed the number of caracters to copied
	if(Data->Len-idx+1<len) len=Data->Len-idx+1;

	// Verify the the string can hold the number to copied
	res.Data->Verify(len+1);

	// Copy the number of characters.
	res.Data->Len=len;
	for(len++,ptr1=&Data->Text[idx],ptr2=res.Data->Text;--len;)
		(*(ptr2++))=(*(ptr1++));
	(*ptr2)=0;

	return(res);
}


//------------------------------------------------------------------------------
RCString& RCString::operator+=(const RCString& str) throw(std::bad_alloc)
{
	if(str.Data==DataNull)
		return(*this);
	if(Data==DataNull)
	{
		(*this)=str;
	}
	else
	{
		Copy();
		Data->Verify(str.Data->Len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],str.Data->Text,(str.Data->Len+1)*sizeof(char));
		Data->Len+=str.Data->Len;
	}
	return(*this);
}


//------------------------------------------------------------------------------
RCString& RCString::operator+=(const char* text) throw(std::bad_alloc)
{
	RReturnValIfFail(text,*this);
	if(Data==DataNull)
	{
		(*this)=text;
	}
	else
	{
		unsigned int len=strlen(text);
		Copy();
		Data->Verify(len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],text,sizeof(char)*len+1);
		Data->Len+=len;
	}
	return(*this);
}


//------------------------------------------------------------------------------
RCString& RCString::operator+=(const char c) throw(std::bad_alloc)
{
	if(c)
	{
		if(Data==DataNull)
		{
			unsigned int maxlen=1,len=1;
			RDecRef<CharBuffer>(Data);
			char* ptr=new char[2];
			ptr[0]=c; ptr[1]=0;
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			Copy();
			Data->Verify(Data->Len+1);
			char* ptr=&Data->Text[Data->Len++];
			(*(ptr++))=c;
			(*ptr)=0;
		}
	}
	return(*this);
}


//------------------------------------------------------------------------------
int RCString::HashIndex(const RCString* str)
{
	char c;
	char u;
	char a;

	if((!str)||(!str->Data->Text)) return(26);
	c=(*str->Data->Text);
	if(!c) return(26);
	u=tolower(c);
	a=char('a');
	if((u>=a)&&(u<=char('z')))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RCString::HashIndex(const RCString& str)
{
	char c;
	char u;
	char a;

	if(!str) return(26);
	c=(*str.Data->Text);
	if(!c) return(26);
	u=tolower(c);
	a=char('a');
	if((u>=a)&&(u<=char('z')))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RCString::HashIndex(const char* str)
{
	char c;
	char u;
	char a;

	if(!str) return(26);
	c=(*str);
	if(!c) return(26);
	u=tolower(c);
	a=char('a');
	if((u>=a)&&(u<=char('z')))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RCString::HashIndex2(const RCString* str)
{
	char c;
	char u;
	char a;

	if((!str)||(!str->Data->Text)) return(26);
	c=*(str->Data->Text);
	if(!c) return(26);
	c=(*(str->Data->Text+1));
	if(!c) return(26);
	u=tolower(c);
	a=char('a');
	if((u>=a)&&(u<=char('z')))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RCString::HashIndex2(const RCString& str)
{
	char c;
	char u;
	char a;

	if(!str) return(26);
	c=*(str.Data->Text);
	if(!c) return(26);
	c=(*(str.Data->Text+1));
	if(!c) return(26);
	u=tolower(c);
	a=char('a');
	if((u>=a)&&(u<=char('z')))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
int RCString::HashIndex2(const char* str)
{
	char c;
	char u;
	char a;

	if(!str) return(26);
	c=*str;
	if(!c) return(26);
	c=(*(str+1));
	if(!c) return(26);
	u=tolower(c);
	a=char('a');
	if((u>=a)&&(u<=char('z')))
		return(u-a);
	return(26);
}


//------------------------------------------------------------------------------
RCString::~RCString(void)
{
	RDecRef<CharBuffer>(Data);
}



//------------------------------------------------------------------------------
//
// Global Functions and Operators
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RCString R::operator+(const RCString& arg1,const RCString& arg2)
{
	RCString res(arg1);
	return(res+=arg2);
}


//------------------------------------------------------------------------------
RCString R::operator+(const RCString& arg1,const char* arg2)
{
	RCString res(arg1);
	return(res+=arg2);
}


//------------------------------------------------------------------------------
RCString R::operator+(const char* arg1,const RCString& arg2)
{
	RCString res(arg1);
	return(res+=arg2);
}
