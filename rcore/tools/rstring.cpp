/*

	R Project Library

	RString.cpp

	Unicode String - Implementation.

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
#include <rstd/rstd.h>
#include <rstd/rstring.h>
#include <rstd/rshareddata.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// Static data
//
//------------------------------------------------------------------------------

RString::CharBuffer* RString::DataNull=0;
const RString RString::Null;



//------------------------------------------------------------------------------
//
// RString::CharBuffer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RString::CharBuffer : public RSharedData
{
public:
	RChar* Text;         // Text
	unsigned int Len;    // Actual length
	unsigned int MaxLen; // Maximum length
	char* Latin1;        // Latin1 version of the string.

	CharBuffer(void)
		: RSharedData(), Text(0), Len(0), MaxLen(0), Latin1(0) {}
	CharBuffer(RChar* tab,unsigned int len,unsigned int maxlen)
		: RSharedData(), Text(tab), Len(len), MaxLen(maxlen), Latin1(0) {}
	void InvalidLatin1(void) {if(Latin1) {delete[] Latin1; Latin1=0;}}
	void Verify(const unsigned int maxlen);
	~CharBuffer(void)
		{if(Text) delete[] Text;
		 if(Latin1) delete[] Latin1;}
};


//------------------------------------------------------------------------------
void RString::CharBuffer::Verify(const unsigned int maxlen)
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
//
// RString
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RString::RString(void)
	: Data(GetDataNull())
{
}


//------------------------------------------------------------------------------
RString::RString(const char* text)
	: Data(0)
{
	if(text)
	{
		unsigned int len,maxlen=0;
		RChar* ptr=Latin1ToUnicode(text,len,maxlen);
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RString::RString(const RChar* text)
	: Data(0)
{
	if(text&&(!text->IsNull()))
	{
		unsigned int len=RChar::StrLen(text);
		RChar* ptr=new RChar[len+1];
		memcpy(ptr,text,sizeof(RChar)*len);
		ptr[len]=0;
		Data=new CharBuffer(ptr,len,len);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RString::RString(const std::string& text)
	: Data(0)
{
	unsigned int len,maxlen=0;
	RChar* ptr;

	ptr=Latin1ToUnicode(text.c_str(),len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
}


//------------------------------------------------------------------------------
RString::RString(unsigned int maxlen)
	: Data(0)
{
	if(maxlen)
	{
		RChar* ptr=new RChar[maxlen+1];
		(*ptr)=0;
		Data=new CharBuffer(ptr,0,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RString::RString(const RString& str)
	: Data(str.Data)
{
	RIncRef<CharBuffer>(Data);
}


//------------------------------------------------------------------------------
RString::RString(const RString* str)
	: Data(0)
{
	if(str)
	{
		Data=str->Data;
		RIncRef<CharBuffer>(Data);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
RString::CharBuffer* RString::GetDataNull(void)
{
	if(!RString::DataNull)
	{
		RChar* ptr2=new RChar[1];
		(*ptr2)=0;
		RString::DataNull=new CharBuffer(ptr2,0,0);
		RString* ptr=const_cast<RString*>(&RString::Null);
		ptr->Data = RString::DataNull;
	}
	else
		RIncRef<CharBuffer>(RString::DataNull);
	return(RString::DataNull);
}


//------------------------------------------------------------------------------
RString& RString::operator=(const RString& str)
{
	RIncRef(str.Data);
	RDecRef<CharBuffer>(Data);
	Data=str.Data;
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator=(const char* text)
{
	unsigned int len,maxlen=0;
	RDecRef<CharBuffer>(Data);
	RChar* ptr=Latin1ToUnicode(text,len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator=(const std::string& text)
{
	unsigned int len,maxlen=0;
	RDecRef<CharBuffer>(Data);
	RChar* ptr=Latin1ToUnicode(text.c_str(),len,maxlen);
	Data=new CharBuffer(ptr,len,maxlen);
	return(*this);
}


//------------------------------------------------------------------------------
void RString::Clear(void)
{
	if(Data!=DataNull)
	{
		RDecRef<CharBuffer>(Data);
		RIncRef<CharBuffer>(DataNull);
		Data=DataNull;
	}
}


//------------------------------------------------------------------------------
void RString::Copy(const char* text,unsigned int nb)
{
	RDecRef<CharBuffer>(Data);
	if(text)
	{
		unsigned int len,maxlen=nb;
		RChar* ptr=Latin1ToUnicode(text,len,maxlen);
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//------------------------------------------------------------------------------
void RString::Copy(const RChar* text,unsigned int nb)
{
	RDecRef<CharBuffer>(Data);
	if(text&&(!text->IsNull()))
	{
		unsigned int len=RChar::StrLen(text);
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


//------------------------------------------------------------------------------
void RString::Copy(void)
{
	if(Data&&(Data->GetRefs()!=1))
	{
		if(Data!=DataNull)
		{
			RChar* ptr=new RChar[Data->MaxLen+1];
			unsigned int len=Data->Len,maxlen=Data->MaxLen;
			memcpy(ptr,Data->Text,sizeof(RChar)*(len+1));
			RDecRef<CharBuffer>(Data);
			Data=new CharBuffer(ptr,len,maxlen);
		}
	}
}


//------------------------------------------------------------------------------
RString RString::ToUpper(void) const
{
	unsigned int len = Data->Len;
	RChar* ptr = Data->Text;

	while(len)
	{
		if((*ptr)!=RChar::ToUpper(*ptr))
		{
			RString str(*this);
			str.Copy();
			ptr = str.Data->Text+(ptr-Data->Text);
	    	while(len)
			{
				(*ptr) = RChar::ToUpper(*ptr);
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
RString RString::ToLower(void) const
{
	unsigned int len = Data->Len;
	RChar* ptr = Data->Text;

	while(len)
	{
		if((*ptr)!=RChar::ToLower(*ptr))
		{
			RString str(*this);
			str.Copy();
			ptr = str.Data->Text+(ptr-Data->Text);
	    	while(len)
			{
				(*ptr) = RChar::ToLower(*ptr);
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
unsigned int RString::GetLen(void) const
{
	return(Data->Len);
}


//------------------------------------------------------------------------------
void RString::SetLen(unsigned int len)
{
	if(len<=Data->Len)
	{
		Data->Len=len;
	}
	else
	{
		if(Data==DataNull)
		{
			RChar* ptr=new RChar[len+1];
			RDecRef<CharBuffer>(Data);
			Data=new CharBuffer(ptr,len,len);
		}
		else
			Data->Verify(len+1);
	}
	if(Data!=DataNull)
		Data->Text[len]=0;
}


//------------------------------------------------------------------------------
unsigned int RString::GetMaxLen(void) const
{
	return(Data->MaxLen);
}


//------------------------------------------------------------------------------
bool RString::IsEmpty(void) const
{
	return(!Data->Len);
}


//------------------------------------------------------------------------------
const char* RString::Latin1(void) const
{
	if(!Data->Latin1)
	{
		Data->Latin1=UnicodeToLatin1(Data->Text,Data->Len);
	}
	return(Data->Latin1);
}


//------------------------------------------------------------------------------
RString RString::Trim(void) const
{
	RString res;
	unsigned int len = Data->Len;
	RChar* ptr = Data->Text;

	// Skip ending spaces
	if(!len)
		return(res);
	ptr=&Data->Text[len-1];
	while(len&&ptr->IsSpace())
	{
		len--;
		ptr--;
	}

	// Skip beginning spaces
	if(!len)
		return(res);
	ptr=Data->Text;
	while(len&&ptr->IsSpace())
	{
		len--;
		ptr++;
	}

	// Get the rest of the string
	while(len)
	{
		res+=(*(ptr++));
		len--;
	};

	return(res);
}


//------------------------------------------------------------------------------
RChar RString::operator[](int pos) const
{
	if(pos>=Data->Len) return(0);
	return(Data->Text[pos]);
}


//------------------------------------------------------------------------------
int RString::Find(const RChar car,int pos,bool CaseSensitive) const
{
	RChar* start;
	bool left;
	unsigned int max;        // Maximal number of character to search.
	RChar search;

	// Initialise the search
	if(!CaseSensitive)
		search=RChar::ToUpper(car);
	else
		search=car;
	if(pos<0)
	{
		// From right
		left=false;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=Data->Len+pos;
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
		if(((CaseSensitive)&&((*start)==search)) || ((!CaseSensitive)&&(RChar::ToUpper(*start)==search)))
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
int RString::FindStr(const RString str,int pos,bool CaseSensitive) const
{
	RChar* start;
	const RChar* toFind;
	unsigned int max;        // Maximal number of character to search.
	int avanct;
	int maxlen;  //max number of char contained in the string to search
	int incr;
	RString search(str);

	// Initialise the search
	if(!CaseSensitive)
		search=search.ToUpper();
	if(pos<0)
	{
		// From right
		incr=-1;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=Data->Len+pos;
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
	if (search.GetLen()>max)
		return(-1);

	// Search for the maximal number of character
	for(max++;--max;)
	{
		if(((CaseSensitive)&&((*start)==(*toFind))) || ((!CaseSensitive)&&(RChar::ToUpper(*start)==(*toFind))))
		{
			if(max>=search.GetLen())
			{
				avanct=0;
				maxlen=search.GetLen();
				bool found=true;
				for(maxlen++;--maxlen,found;)
				{
					if(((CaseSensitive)&&((*start)==(*toFind))) || ((!CaseSensitive)&&(RChar::ToUpper(*start)==(*toFind))))
					{
						if(!(maxlen-1))
						{
							// String found
							if(incr>0)
								return(pos);
							else
								return(pos-search.GetLen()+1);
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
						return(pos-search.GetLen()+1);
				}
			}
		}
		start+=incr;
		pos+=incr;
	}
	return(-1);
}


//------------------------------------------------------------------------------
RString RString::Mid(unsigned int idx,unsigned int len) const
{
	RString res;

	// If the index is greather than the length -> return a null string.
	if(Data->Len<=idx) return("");

	// Computed the number of caracters to copied
	if(len==0xFFFFFFFF)
		len=Data->Len-idx;
	if(Data->Len-idx+1<len) len=Data->Len-idx+1;

	// Verify the the string can hold the number to copied
	RChar* ptr=new RChar[len+1];
	memcpy(ptr,&Data->Text[idx],sizeof(RChar)*len);
	ptr[len]=0;
	res.Data=new CharBuffer(ptr,len,len);
	return(res);
}


//------------------------------------------------------------------------------
void RString::Split(RContainer<RString,true,false>& elements,const RChar car) const
{
	RString element;
	unsigned int len;
	RChar* ptr;

	// Skip ending spaces
	for(len=Data->Len+1,ptr=Data->Text;--len;ptr++)
	{
		if((*ptr)==car)
		{
			// Insert element
			if(!element.IsEmpty())
				elements.InsertPtr(new RString(element));
			element="";
		}
		else
			element+=(*ptr);
	}
	if(!element.IsEmpty())
		elements.InsertPtr(new RString(element));
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RString& str)
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
		memcpy(&Data->Text[Data->Len],str.Data->Text,(str.Data->Len+1)*sizeof(RChar));
		Data->Len+=str.Data->Len;
		Data->InvalidLatin1();
	}
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RChar* text)
{
	RReturnValIfFail(text,*this);
	if(Data==DataNull)
	{
		(*this)=text;
	}
	else
	{
		unsigned int len=RChar::StrLen(text);
		Copy();
		Data->Verify(len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],text,sizeof(RChar)*len+1);
		Data->Len+=len;
		Data->InvalidLatin1();
	}
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const char* text)
{

	RReturnValIfFail(text,*this);
	if(Data==DataNull)
	{
		(*this)=text;
	}
	else
	{
		const char* ptr1=text;
		unsigned int len=strlen(text);
		Copy();
		Data->Verify(len+Data->Len+1);
		RChar* ptr2=&Data->Text[Data->Len];
		while(*ptr1)
			(*(ptr2++))=(*(ptr1++));
		(*ptr2)=0;
		Data->Len+=len;
		Data->InvalidLatin1();
	}
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const char c)
{
	if(c)
	{
		if(Data==DataNull)
		{
			unsigned int maxlen=1,len=1;
			char tab[2]={c,0};
			RDecRef<CharBuffer>(Data);
			RChar* ptr=Latin1ToUnicode(tab,len,maxlen);
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			Copy();
			Data->Verify(Data->Len+1);
			RChar* ptr=&Data->Text[Data->Len++];
			(*(ptr++))=c;
			(*ptr)=0;
			Data->InvalidLatin1();
		}
	}
	return(*this);
}


//------------------------------------------------------------------------------
RString& RString::operator+=(const RChar c)
{
	if(!c.IsNull())
	{
		if(Data==DataNull)
		{
			unsigned int maxlen=1,len=1;
			RDecRef<CharBuffer>(Data);
			RChar* ptr=new RChar[2];
			ptr[0]=c;
			ptr[1]=0;
			Data=new CharBuffer(ptr,len,maxlen);
		}
		else
		{
			Copy();
			Data->Verify(Data->Len+1);
			RChar* ptr=&Data->Text[Data->Len++];
			(*(ptr++))=c;
			(*ptr)=0;
			Data->InvalidLatin1();
		}
	}
	return(*this);
}


//------------------------------------------------------------------------------
const RChar* RString::UTF16(void) const
{
	return(Data->Text);
}


//------------------------------------------------------------------------------
const RChar* RString::operator()(void) const
{
	return(Data->Text);
}


//------------------------------------------------------------------------------
RString::operator const char* ()
{
	return(Latin1());
}


//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
std::string RString::ToString(void) const
{
	return(operator std::string());
}


//------------------------------------------------------------------------------
bool RString::operator==(const RString& str) const
{
	return(Compare(str)==0);
}


//------------------------------------------------------------------------------
bool RString::operator==(const char* str) const
{
	return(Compare(str)==0);
}


//------------------------------------------------------------------------------
bool RString::operator==(const RChar* str) const
{
	return(Compare(str)==0);
}


//------------------------------------------------------------------------------
bool RString::operator!=(const RString& str) const
{
	return(Compare(str));
}


//------------------------------------------------------------------------------
bool RString::operator!=(const char* str) const
{
	return(Compare(str));
}


//------------------------------------------------------------------------------
bool RString::operator!=(const RChar* str) const
{
	return(Compare(str));
}


//------------------------------------------------------------------------------
int RString::Compare(const RString& str) const
{
	if(!Data)
	{
		if(!str.Data)
			return(0);
		return(-1);
	}
	else if(!str.Data)
		return(1);
	return(RChar::StrCmp(Data->Text,str.Data->Text));
}

//------------------------------------------------------------------------------
int RString::Compare(const RString* str) const
{
	if(!Data)
	{
		if((!str)||(!str->Data))
			return(0);
		return(-1);
	}
	else if((!str)||(!str->Data))
		return(1);
	return(RChar::StrCmp(Data->Text,str->Data->Text));
}


//------------------------------------------------------------------------------
int RString::Compare(const char* str) const
{
	if(!Data)
	{
		if(!str)
			return(0);
		return(-1);
	}
	else if(!str)
		return(1);
	return(RChar::StrCmp(Data->Text,str));
}


//------------------------------------------------------------------------------
int RString::Compare(const RChar* str) const
{
	if(!Data)
	{
		if(!str)
			return(0);
		return(-1);
	}
	else if(!str)
		return(1);
	return(RChar::StrCmp(Data->Text,str));
}


//------------------------------------------------------------------------------
int RString::HashIndex(const RString* str)
{
	RChar c;
	UChar u;
	UChar a;

	if((!str)||(!str->Data->Text)) return(26);
	c=(*str->Data->Text);
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

	if(!str.Data->Text) return(26);
	c=(*str.Data->Text);
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

	if((!str)||(!str->Data->Text)) return(26);
	c=*(str->Data->Text);
	if(c.IsNull()) return(26);
	c=(*(str->Data->Text+1));
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

	if(!str.Data->Text) return(26);
	c=*(str.Data->Text);
	if(c.IsNull()) return(26);
	c=(*(str.Data->Text+1));
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
RChar* RString::Latin1ToUnicode(const char *str,unsigned int& len, unsigned int& maxlen)
{
	if(!str)
	{
		len=0;
		return(0);
	}

	RChar* res=0;
	unsigned int i;
	RChar* ptr;

	len=strlen(str);
	if(maxlen<len)
		maxlen=len;
	res=ptr=new RChar[maxlen+1];
	i = len;
	while(i--)
	   	(*(ptr++)) = (*(str++));
	(*ptr)=0;
	return(res);
}


//------------------------------------------------------------------------------
char* RString::UnicodeToLatin1(const RChar* tab,unsigned int len)
{
	if(!tab)
		return(0);

	char* res=new char[len+1];
	char* ptr=res;

	while(len--)
	{
		(*(ptr++))=(tab->Unicode() > 0xff) ? '?' : (char)tab->Unicode();
		tab++;
	}
	(*ptr)=0;
	return(res);
}


//------------------------------------------------------------------------------
RString::~RString(void)
{
	RDecRef<CharBuffer>(Data);
}



//------------------------------------------------------------------------------
//
// Global Functions and Operators
//
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RString R::operator+(const RString& arg1,const RString& arg2)
{
	RString res(arg1);
	return(res+=arg2);
}


//------------------------------------------------------------------------------
RString R::operator+(const RString& arg1,const char* arg2)
{
	RString res(arg1);
	return(res+=arg2);
}


//------------------------------------------------------------------------------
RString R::operator+(const char* arg1,const RString& arg2)
{
	RString res(arg1);
	return(res+=arg2);
}


//------------------------------------------------------------------------------
RString R::itou(const int nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%i",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::itou(const unsigned int nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%u",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::ltou(const long nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%li",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::chrtou(const unsigned char c)
{
	char Tmp[2];
	RString res;

	(*Tmp)=c;
	(*(Tmp+1))=0;
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::ltou(const unsigned long nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%lu",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::lltou(const unsigned long long nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%llu",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::ftou(const float nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%f",nb);
	res=Tmp;
	return(res);
}


//------------------------------------------------------------------------------
RString R::dtou(const double nb)
{
	char Tmp[20];
	RString res;

	sprintf(Tmp,"%f",nb);
	res=Tmp;
	return(res);
}



//------------------------------------------------------------------------------
//
// class RCharCursor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCharCursor::RCharCursor(void)
	: Current(0), ActPtr(0), Str(0)
{
}


//------------------------------------------------------------------------------
RCharCursor::RCharCursor(const RString& str)
	: Current(str.Data->Text), ActPtr(0), Str(&str)
{
}


//------------------------------------------------------------------------------
RCharCursor::RCharCursor(const RString* str)
	: Current(str->Data->Text), ActPtr(0), Str(str)
{
}


//------------------------------------------------------------------------------
void RCharCursor::Start(void) throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	Current=Str->Data->Text;
	ActPtr=0;
}


//------------------------------------------------------------------------------
void RCharCursor::Set(const RString& str)
{
	Str=&str;
	Current=Str->Data->Text;
	ActPtr=0;
}


//------------------------------------------------------------------------------
void RCharCursor::Set(const RString* str)
{
	Str=str;
	Current=Str->Data->Text;
	ActPtr=0;
}


//------------------------------------------------------------------------------
void RCharCursor::GoTo(const unsigned int i) throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	if(i>=Str->Data->Len)
		throw RException("Index out of range");
	Current=&Str->Data->Text[i];
	ActPtr=i;
}


//------------------------------------------------------------------------------
unsigned int RCharCursor::GetNb(void) throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	return(Str->Data->Len);
}


//------------------------------------------------------------------------------
unsigned int RCharCursor::GetPos(void) throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	return(ActPtr);
}


//------------------------------------------------------------------------------
bool RCharCursor::End(void) const throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	return(ActPtr==Str->Data->Len);
}


//------------------------------------------------------------------------------
void RCharCursor::Next(void) throw(RException)
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


//------------------------------------------------------------------------------
RChar RCharCursor::operator()(void) const throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	return(*Current);
}


//------------------------------------------------------------------------------
RChar RCharCursor::operator[](unsigned int pos) const throw(RException)
{
	if(!Str)
		throw RException("No string defined");
	if(pos>=Str->Data->Len)
		throw RException("Index out of range");
	return(Str->Data->Text[pos]);
}
