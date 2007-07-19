/*

	R Project Library

	RTextEnconding.cpp

	Genereic Text Encoding Scheme - Implementation.

	Copyright 2004-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R library
#include <rcontainer.h>
#include <rcursor.h>
#include <rtextencoding.h>
#include <errno.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// Constant values
const size_t BufSize=2048;


//------------------------------------------------------------------------------
// General Variables
RContainer<RTextEncoding,true,true> Encodings(20,10);



//------------------------------------------------------------------------------
// All encoding names and aliases
struct EncodingName // Structure representing an official encoding
{
	RString Name;
	unsigned int MIBenum;
};
struct EncodingAlias // Structure representing an alias
{
	RString Name;
	RString Alias;
};
#include "rtextencoding_data.cpp"   // Include all possibilities



//------------------------------------------------------------------------------
//
// General Function
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RString GetOfficialName(const RString& alias)
{
	unsigned int NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	const EncodingAlias* ptr;

	NbMax=sizeof(EncodingAliases)/sizeof(EncodingAlias)-1;
	NbMin=0;

	while(Cont)
	{
		i=(NbMax+NbMin)/2;
		ptr=&EncodingAliases[i];
		Comp=ptr->Name.Compare(alias);
		if(!Comp)
		{
			return(ptr->Name);
		}
		if(Comp>0)
		{
			NbMax = i;
			if(i) NbMax--;
		}
		else
			NbMin = i+1;
		Cont = NotLast;
		if(NbMin>=NbMax) NotLast=false;
	}
	return("");
}



//------------------------------------------------------------------------------
//
// class RTextUTF16Encoding
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RTextUTF16Encoding : public RTextEncoding
{
public:
	RTextUTF16Encoding(void)
		: RTextEncoding("utf-16")
	{}

	RString ToUnicode(const char* text,unsigned int len) const
	{
		const char* ptr=text;

		RString res;
		if((text[0]==char(0xfe))&&(text[1]==char(0xff)))
		{
			ptr+=2;
			len-=2;
		}
		else if((text[0]==char(0xff))&&(text[1]==char(0xfe)))
		{
			ptr+=2;
			len-=2;
		}
		res.Copy((RChar*)(text),len/2);
		return(res);
	}

	RCString FromUnicode(const RString& text) const
	{
		const RChar* ptr=text.UTF16();
		RCString res;
		res.Copy((char*)(ptr),text.GetLen()*2);
		return(res);
	}
};



//------------------------------------------------------------------------------
//
// class RTextLatin1Encoding
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RTextLatin1Encoding : public RTextEncoding
{
public:
	RTextLatin1Encoding(void)
		: RTextEncoding("latin1")
		{}

	RString ToUnicode(const char* text,unsigned int) const
	{
		return(RString(text));
	}

	RCString FromUnicode(const RString& text) const
	{
		return(RCString(text.Latin1()));
	}
};



//------------------------------------------------------------------------------
//
// class RTextEncoding
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTextEncoding::RTextEncoding(const RString& name)
	: Name(name.ToLower())
{
	ToUTF16=iconv_open("utf-16",Name);
	if(ToUTF16==(iconv_t)-1)
		throw EncodingNotSupported(Name+" encoding not supported");
	FromUTF16=iconv_open(Name,"utf-16");
	if(FromUTF16==(iconv_t)-1)
		throw EncodingNotSupported(Name+" encoding not supported");
	
	// Test the order
	char Tab[20];
	char Test[]="test";
	size_t s1,s2,err;
	char *ptr1,*ptr2;
	s1=4;
	s2=12;
	ptr1=Test;
	ptr2=Tab;
	#ifdef _LIBICONV_VERSION	
		err=iconv(ToUTF16,const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
	#else
		err=iconv(ToUTF16,&ptr1,&s1,&ptr2,&s2);
	#endif
	if(s2>6)
		Order=true;
	else
	{
		if((Tab[0]==char(0xfe))&&(Tab[1]==char(0xff)))
		{
			Order=true;
		}
		else if((Tab[0]==char(0xff))&&(Tab[1]==char(0xfe)))
		{
			Order=false;
		}
	}
}


//------------------------------------------------------------------------------
void RTextEncoding::Init(void) const
{
}


//------------------------------------------------------------------------------
int RTextEncoding::Compare(const RTextEncoding& enc) const
{
	return(Name.Compare(enc.Name));
}


//------------------------------------------------------------------------------
int RTextEncoding::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
RString RTextEncoding::ToUnicode(const char* text,unsigned int len) const
{
	char* ptr;
	char* ptr2;
	char Tab[BufSize*sizeof(UChar)];
	size_t s1,s2,err;
	RString out;
	bool ToFill=true;

	ptr=(char*)(text);
	s1=len;
	while(ToFill)
	{
		ptr2=Tab;
		s2=BufSize;
		#ifdef _LIBICONV_VERSION	
			err=iconv(ToUTF16,const_cast<const char**>(&ptr),&s1,&ptr2,&s2);
		#else
			err=iconv(ToUTF16,&ptr,&s1,&ptr2,&s2);
		#endif
		if(err==(size_t)-1)
		{
			switch(errno)
			{
				case EILSEQ:
					throw InvalidByteSequence("Invalid byte sequence for encoding "+Name);
					break;
				case E2BIG:
					ToFill=true;
					break;
				case EINVAL:
					throw IncompleteByteSequence("Incomplete byte sequence for encoding "+Name);
					break;
				case EBADF:
					throw RException("Invalid descriptor for encoding "+Name);
					break;
			}
		}
		else
			ToFill=false;
		s2=BufSize-s2;
		(*((UChar*)(ptr2)))=0; // Terminate by a null string
		#warning Does iconv correctly handle encoding with different bytes order?
		out+=(RChar*)(Tab);
	}
	return(out);
}


//------------------------------------------------------------------------------
RChar RTextEncoding::NextUnicode(const char* text,unsigned int& len) const
{
	RChar Code;
	char Tab[sizeof(UChar)];
	char* ptr2;
	char* ptr1;
	size_t s1,s2,err;

	ptr1=(char*)text;
	s1=len;
	ptr2=Tab;
	s2=sizeof(UChar);
	#ifdef _LIBICONV_VERSION	
		err=iconv(ToUTF16,const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
	#else
		err=iconv(ToUTF16,&ptr1,&s1,&ptr2,&s2);
	#endif
	if(err==(size_t)-1)
	{
		switch(errno)
		{
			case EILSEQ:
				throw InvalidByteSequence("Invalid byte sequence for encoding "+Name);
				break;
			case EINVAL:
				throw IncompleteByteSequence("Incomplete byte sequence for encoding "+Name);
				break;
			case EBADF:
				throw RException("Invalid descriptor for encoding  "+Name);
				break;
		}
	}
	len-=s1;
	Code=RChar((*((UChar*)(Tab))));
	return(Code);
}


//------------------------------------------------------------------------------
RCString RTextEncoding::FromUnicode(const RString& text) const
{
	char *ptr,*ptr2;
	char Tab[BufSize*sizeof(char)];
	size_t s1,s2,err;
	bool ToFill=true;
	RCString out;

	ptr=(char*)(text.UTF16());
	s1=text.GetLen()*sizeof(UChar);
	while(ToFill)
	{
		ptr2=Tab;
		s2=BufSize;
		#ifdef _LIBICONV_VERSION	
			err=iconv(FromUTF16,const_cast<const char**>(&ptr),&s1,&ptr2,&s2);
		#else
			err=iconv(FromUTF16,&ptr,&s1,&ptr2,&s2);
		#endif
		if(err==(size_t)-1)
		{
			switch(errno)
			{
				case EILSEQ:
					throw InvalidByteSequence("Invalid byte sequence for encoding UFT-16");
					break;
				case E2BIG:
					ToFill=true;
					break;
				case EINVAL:
					throw IncompleteByteSequence("Incomplete byte sequence for encoding UTF-16");
					break;
				case EBADF:
					throw RException("Invalid descriptor for encoding "+Name);
					break;
			}
		}
		else
			ToFill=false;
		s2=BufSize-s2;
		out+=RCString(Tab,s2);
	}
	return(out);
}


//------------------------------------------------------------------------------
RTextEncoding* RTextEncoding::GetTextEncoding(const RString& name)
{
	RTextEncoding* ptr;
	RString search(name.ToLower());

	// Insert UTF16 and latin1 if necessary
	if(Encodings.GetNb()==0)
	{
		ptr=new RTextUTF16Encoding();
		Encodings.InsertPtr(ptr);
		ptr->Init();
		ptr=new RTextLatin1Encoding();
		Encodings.InsertPtr(ptr);
		ptr->Init();
	}

	// Find the official name
	search=GetOfficialName(search);
	if(search.IsEmpty())
		throw EncodingNotSupported("Encoding "+name+" is not supported");

	// Search it
	ptr=Encodings.GetPtr<const RString>(search);
	if(ptr)
		return(ptr);
	ptr=new RTextEncoding(search);
	Encodings.InsertPtr(ptr);
	ptr->Init();
	return(ptr);
}


//------------------------------------------------------------------------------
RTextEncoding::~RTextEncoding(void)
{
	iconv_close(ToUTF16);
	iconv_close(FromUTF16);
}
