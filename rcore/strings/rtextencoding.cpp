/*

	R Project Library

	RTextEnconding.cpp

	Generic Text Encoding Scheme - Implementation.

	Copyright 2004-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for iconv
#include <iconv.h>


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
RTextEncoding* CodecUTF8(0);


//------------------------------------------------------------------------------
// All encoding names and aliases
struct EncodingName // Structure representing an official encoding
{
	RCString Name;
	size_t MIBenum;
};
struct EncodingAlias // Structure representing an alias
{
	RCString Alias;
	RCString Name;
};
#include "rtextencoding_data.cpp"   // Include all possibilities



//------------------------------------------------------------------------------
//
// General Function
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RCString GetOfficialName(const RCString& alias)
{
	size_t NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	const EncodingAlias* ptr;

	NbMax=sizeof(EncodingAliases)/sizeof(EncodingAlias)-1;
	NbMin=0;

	while(Cont)
	{
		i=(NbMax+NbMin)/2;
		ptr=&EncodingAliases[i];
		Comp=ptr->Alias.Compare(alias);
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
// class RTextEncoding
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTextEncoding::RTextEncoding(const RCString& name)
	: Name(name.ToLower())
{
	ToUTF16=static_cast<void*>(iconv_open("utf-16le",Name));
	if((ToUTF16==(iconv_t)-1)&&(errno==EINVAL))
		throw REncodingException(RString(Name())+" encoding not supported");
	FromUTF16=static_cast<void*>(iconv_open(Name,"utf-16le"));
	if((FromUTF16==(iconv_t)-1)&&(errno==EINVAL))
		throw REncodingException(RString(Name())+" encoding not supported");

	// Test the order
	char Tab[20];
	char Test[]="test";
	size_t s1,s2/*,err*/;
	char *ptr1,*ptr2;
	s1=4;
	s2=12;
	ptr1=Test;
	ptr2=Tab;
	#ifdef _LIBICONV_VERSION
		#if (defined __APPLE__ || defined WIN32)
			iconv(static_cast<iconv_t>(ToUTF16),&ptr1,&s1,&ptr2,&s2);
		#else
			iconv(static_cast<iconv_t>(ToUTF16),const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
		#endif
	#else
		iconv(static_cast<iconv_t>(ToUTF16),&ptr1,&s1,&ptr2,&s2);
	#endif
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
int RTextEncoding::Compare(const RCString& name) const
{
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
RString RTextEncoding::ToUnicode(const char* text,size_t len) const
{
	char* ptr1;
	char* ptr2;
	char Tab[BufSize*sizeof(UChar)];
	size_t s1,s2,err;
	RString out;
	bool ToFill=true;

	ptr1=(char*)(text);
	s1=len;
	while(ToFill)
	{
		ptr2=Tab;
		s2=BufSize;
		#ifdef _LIBICONV_VERSION
			#if (defined __APPLE__ || defined WIN32)
				err=iconv(static_cast<iconv_t>(ToUTF16),&ptr1,&s1,&ptr2,&s2);
			#else
				err=iconv(static_cast<iconv_t>(ToUTF16),const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
			#endif
		#else
			err=iconv(static_cast<iconv_t>(ToUTF16),&ptr1,&s1,&ptr2,&s2);
		#endif
		if(err==(size_t)-1)
		{
			switch(errno)
			{
				case EILSEQ:
					throw RInvalidByteException("Invalid byte sequence for encoding "+RString(Name()));
					break;
				case E2BIG:
					ToFill=true;
					break;
				case EINVAL:
					throw RIncompleteByteException("Incomplete byte sequence for encoding "+RString(Name()));
					break;
				case EBADF:
					throw RException("Invalid descriptor for encoding "+RString(Name()));
					break;
			}
		}
		else
			ToFill=false;
		s2=BufSize-s2;
		(*((UChar*)(ptr2)))=0; // Terminate by a null string
		out+=(RChar*)(Tab);
	}
	return(out);
}


//------------------------------------------------------------------------------
RTextEncoding::UnicodeCharacter RTextEncoding::NextUnicode(const char* text,size_t& len,bool invalid) const
{
	UnicodeCharacter Code;
	char* ptr2;
	char* ptr1;
	size_t s1,s2,err;
	bool ToFill=true;

	// Suppose Unicode character is valid.
	Code.Valid=true;
	ptr1=(char*)text;
	s1=len;
	ptr2=(char*)Code.Codes;
	size_t trytoread(0);
	for(;ToFill;)
	{
		trytoread+=2;
		if(trytoread==6)
			throw RException("Big Error in RTextEncoding::NextUnicode");
		s2=trytoread;
		#ifdef _LIBICONV_VERSION
			#if (defined __APPLE__ || defined WIN32)
				err=iconv(static_cast<iconv_t>(ToUTF16),&ptr1,&s1,&ptr2,&s2);
			#else
				err=iconv(static_cast<iconv_t>(ToUTF16),const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
			#endif
		#else
			err=iconv(static_cast<iconv_t>(ToUTF16),&ptr1,&s1,&ptr2,&s2);
		#endif
		if(err==(size_t)-1)
		{
			switch(errno)
			{
				// Strange behavior with iconv under MinGW (err==-1 and errno==0)
				case 0:
					if(!s2)
						ToFill=false;
					break;

				case EILSEQ:
					if(invalid)
					{
						Code.Valid=false;
						return(Code);
					}
					else
						throw RInvalidByteException("Invalid byte sequence for encoding "+RString(Name()));
					break;

				case E2BIG:
					if(!s2)
						ToFill=false;
					break;

				case EINVAL:
					throw RIncompleteByteException("Incomplete byte sequence for encoding "+RString(Name()));
					break;

				case EBADF:
					throw RException("Invalid descriptor for encoding  "+RString(Name()));
					break;
			}
		}
		else
			ToFill=false;
	}
	len-=s1;
	if(len<2)
		Code.Codes[1]=0;
	return(Code);
}


//------------------------------------------------------------------------------
RCString RTextEncoding::FromUnicode(const RChar* text,size_t len) const
{
	char *ptr1,*ptr2;
	char Tab[BufSize*sizeof(char)];
	size_t s1,s2,err;
	bool ToFill=true;
	RCString out;

	ptr1=(char*)(text);
	s1=len*sizeof(UChar);
	while(ToFill)
	{
		ptr2=Tab;
		s2=BufSize;
		#ifdef _LIBICONV_VERSION
			#if (defined __APPLE__ || defined WIN32)
				err=iconv(static_cast<iconv_t>(FromUTF16),&ptr1,&s1,&ptr2,&s2);
			#else
				err=iconv(static_cast<iconv_t>(FromUTF16),const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
			#endif
		#else
			err=iconv(static_cast<iconv_t>(FromUTF16),&ptr1,&s1,&ptr2,&s2);
		#endif
		if(err==(size_t)-1)
		{
			switch(errno)
			{
				case EILSEQ:
					throw RInvalidByteException("Invalid byte sequence for encoding UFT-16");
					break;
				case E2BIG:
					ToFill=true;
					break;
				case EINVAL:
					throw RIncompleteByteException("Incomplete byte sequence for encoding UTF-16");
					break;
				case EBADF:
					throw RException("Invalid descriptor for encoding "+RString(Name()));
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
RCString RTextEncoding::FromUnicode(const RString& text) const
{
	char *ptr1,*ptr2;
	char Tab[BufSize*sizeof(char)];
	size_t s1,s2,err;
	bool ToFill=true;
	RCString out;

	ptr1=(char*)(text.ToUTF16());
	s1=text.GetLen()*sizeof(UChar);
	while(ToFill)
	{
		ptr2=Tab;
		s2=BufSize;
		#ifdef _LIBICONV_VERSION
			#if (defined __APPLE__ || defined WIN32)
				err=iconv(static_cast<iconv_t>(FromUTF16),&ptr1,&s1,&ptr2,&s2);
			#else
				err=iconv(static_cast<iconv_t>(FromUTF16),const_cast<const char**>(&ptr1),&s1,&ptr2,&s2);
			#endif
		#else
			err=iconv(static_cast<iconv_t>(FromUTF16),&ptr1,&s1,&ptr2,&s2);
		#endif
		if(err==(size_t)-1)
		{
			switch(errno)
			{
				case EILSEQ:
					throw RInvalidByteException("Invalid byte sequence for encoding UFT-16");
					break;
				case E2BIG:
					ToFill=true;
					break;
				case EINVAL:
					throw RIncompleteByteException("Incomplete byte sequence for encoding UTF-16");
					break;
				case EBADF:
					throw RException("Invalid descriptor for encoding "+RString(Name()));
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
RTextEncoding* RTextEncoding::GetTextEncoding(const RCString& name)
{
	RTextEncoding* ptr;
	RCString search(name.ToLower());

	// Shortcut since utf-8 is probably the most used encoding
	if(name=="utf-8")
	{
		if(!CodecUTF8)
			return(GetUTF8Encoding());
		return(CodecUTF8);
	}

	// Find the official name
	search=GetOfficialName(search);
	if(search.IsEmpty())
		throw REncodingException("Encoding "+RString(name())+" is not supported");

	// Search it
	ptr=Encodings.GetPtr(search);
	if(ptr)
		return(ptr);
	ptr=new RTextEncoding(search);
	Encodings.InsertPtr(ptr);
	ptr->Init();
	return(ptr);
}


//------------------------------------------------------------------------------
RTextEncoding* RTextEncoding::GetUTF8Encoding(void)
{
	if(!CodecUTF8)
	{
		CodecUTF8=new RTextEncoding("utf-8");
		CodecUTF8->Init();
	}
	return(CodecUTF8);
}


//------------------------------------------------------------------------------
RTextEncoding::~RTextEncoding(void)
{
	iconv_close(static_cast<iconv_t>(ToUTF16));
	iconv_close(static_cast<iconv_t>(FromUTF16));
}
