/*

	R Project Library

	BasicString.cpp

	Generic String - Implementation.

	Copyright 2005-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2005-2008 by the Universit√© Libre de Bruxelles (ULB).

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
//
// Static Data
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,class S> class R::BasicString<C,S>::CharBuffer* R::BasicString<C,S>::DataNull=0;



//-----------------------------------------------------------------------------
//
// BasicCharBuffer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,class S> void R::BasicString<C,S>::CharBuffer::Verify(size_t maxlen)
{
	if(MaxLen<maxlen)
	{
		C* tmp;
		MaxLen=maxlen;
		tmp=new C[MaxLen+1];
		if(Text)
		{
			memcpy(tmp,Text,(Len+1)*sizeof(C));
			delete[] Text;
		}
		Text=tmp;
	}
}



//-----------------------------------------------------------------------------
//
// BasicString
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,class S>
	R::BasicString<C,S>::BasicString(void)
	: Data(0)
{
}


//-----------------------------------------------------------------------------
template<class C,class S>
	R::BasicString<C,S>::BasicString(const BasicString& str)
	:  Data(str.Data)
{
	RIncRef<CharBuffer>(Data);
}


//-----------------------------------------------------------------------------
template<class C,class S>
	R::BasicString<C,S>::BasicString(const C car)
{
	size_t len,maxlen=1;
	C* ptr=new C[2];

	ptr[0]=car;
	ptr[1]=0;
	if(!car)
		len=0;
	else
		len=1;
	Data=new CharBuffer(ptr,len,maxlen);
}


//-----------------------------------------------------------------------------
template<class C,class S>
	R::BasicString<C,S>::BasicString(const C* src)
{
	if(src)
	{
		size_t len=strlen(src);
		C* ptr=new C[len+1];
		memcpy(ptr,src,sizeof(C)*len);
		ptr[len]=0;
		Data=new CharBuffer(ptr,len,len);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
template<class C,class S>
	R::BasicString<C,S>::BasicString(const C* src,size_t len)
{
	if(src)
	{
		size_t maxlen;
		maxlen=len;
		C* ptr=new C[maxlen+1];
		memcpy(ptr,src,sizeof(C)*len);
		ptr[len]=0;
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
template<class C,class S>
	class R::BasicString<C,S>::CharBuffer* R::BasicString<C,S>::GetDataNull(void)
{
	if(!S::DataNull)
	{
		C* ptr2=new C[1];
		(*ptr2)=0;
		R::BasicString<C,S>::DataNull=new CharBuffer(ptr2,0,0);
	}
	else
		RIncRef<CharBuffer>(R::BasicString<C,S>::DataNull);
	return(static_cast<class R::BasicString<C,S>::CharBuffer*>(DataNull));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Copy(void)
{
	if(Data&&(Data->GetRefs()!=1)&&(Data->Len))
	{
		C* ptr=new C[Data->MaxLen+1];
		size_t len=Data->Len,maxlen=Data->MaxLen;
		memcpy(ptr,Data->Text,sizeof(C)*(len+1));
		RDecRef<CharBuffer>(Data);
		Data=new CharBuffer(ptr,len,maxlen);
	}
	else if(Data)
		Data->InvalidLatin1();
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Copy(const C* text,size_t nb)
{
	RDecRef<CharBuffer>(Data);
	if(text)
	{
		size_t len=strlen(text);
		if(nb>len)
			nb=len;
		C* ptr=new C[nb+1];
		memcpy(ptr,text,sizeof(C)*nb);
		ptr[nb]=0;
		Data=new CharBuffer(ptr,nb,nb);
	}
	else
		Data=GetDataNull();
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S& R::BasicString<C,S>::operator=(const S& src)
{
	RIncRef(src.Data);
	RDecRef<CharBuffer>(Data);
	Data=src.Data;
	return(*static_cast<S*>(this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S& R::BasicString<C,S>::operator=(const C* src)
{
	Copy(src,size_t(-1));
	return(*static_cast<S*>(this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::SetLen(size_t len)
{
	// If length is already correct -> Nothing to do
	if(len==Data->Len)
		return;

	// Duplicate the string because it will change
	Copy();

	// Modify the length (eventually expand the string)
	if(len<=Data->MaxLen)
	{
		Data->Len=len;
	}
	else
	{
		if(Data==S::DataNull)
		{
			C* ptr=new C[len+1];
			RDecRef<CharBuffer>(Data);
			Data=new CharBuffer(ptr,len,len);
		}
		else
			Data->Verify(len+1);
	}
	if(Data!=S::DataNull)
		Data->Text[len]=0;
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::SetLen(size_t len,const S& str)
{
	// If length is already correct -> Nothing to do
	if(len==Data->Len)
		return;

	// Duplicate the string because it will change
	Copy();

	// Modify the length (eventually expand the string) and fill it
	size_t oldsize=Data->Len;
	SetLen(len);
	Data->Len=len;
	if(oldsize<len)
	{
		C* ptr=&Data->Text[oldsize];
		C* ptr2=str.Data->Text;
		for(;oldsize<len;oldsize++,ptr++,ptr2++)
		{
			if(!(*ptr2))
				ptr2=str.Data->Text;
			if((*ptr2)!=0)
				(*ptr)=(*ptr2);
		}
	}
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Clear(void)
{
	if(Data!=DataNull)
	{
		RDecRef<CharBuffer>(Data);
		RIncRef<CharBuffer>(DataNull);
		Data=DataNull;
	}
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S R::BasicString<C,S>::ToUpper(void) const
{
	size_t len = Data->Len;
	C* ptr = Data->Text;

	while(len)
	{
		if((*ptr)!=toupper(*ptr))
		{
			S str(*static_cast<const S*>(this));
			str.BasicString<C,S>::Copy();
			ptr = str.Data->Text+(ptr-Data->Text);
	    	while(len)
			{
				(*ptr) = static_cast<C>(toupper(*ptr));
				len--;
				ptr++;
			}
			return(str);
		}
		len--;
		ptr++;
	}
	return(*static_cast<const S*>(this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S R::BasicString<C,S>::ToLower(void) const
{
	size_t len = Data->Len;
	C* ptr = Data->Text;

	while(len)
	{
		if((*ptr)!=tolower(*ptr))
		{
			S str(*static_cast<const S*>(this));
			str.BasicString<C,S>::Copy();
			ptr = str.Data->Text+(ptr-Data->Text);
	    	while(len)
			{
				(*ptr) = static_cast<C>(tolower(*ptr));
				len--;
				ptr++;
			}
			return(str);
		}
		len--;
		ptr++;
	}
	return(*static_cast<const S*>(this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S R::BasicString<C,S>::Trim(void) const
{
	size_t pos(0);
	size_t len(Data->Len);
	C* ptr;

	// Skip ending spaces
	if(len)
	{
		ptr=&Data->Text[len-1];
		while(len&&isspace(*ptr))
		{
			len--;
			ptr--;
		}
	}

	// Skip beginning spaces
	if(len)
	{
		ptr=Data->Text;
		while(len&&isspace(*ptr))
		{
			len--;
			ptr++;
			pos++;
		}
	}

	return(Mid(pos,len));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S R::BasicString<C,S>::Trim(const S& str) const
{
	size_t pos(0);
	size_t len(Data->Len);
	size_t slen(str.Data->Len);
	C* ptr;

	// Skip ending spaces
	if(len>=slen)
	{
		ptr=&Data->Text[len-slen];
		while(len&&(!strncmp(str.Data->Text,ptr,slen)))
		{
			len-=slen;
			ptr-=slen;
		}
	}

	// Skip beginning spaces
	if(len>=slen)
	{
		ptr=Data->Text;
		while(len&&(!strncmp(str.Data->Text,ptr,slen)))
		{
			len-=slen;
			ptr+=slen;
			pos+=slen;
		}
	}

	return(Mid(pos,len));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S& R::BasicString<C,S>::operator+=(const S& src)
{
	if(src.Data==DataNull)
		return(static_cast<S&>(*this));
	if(Data==DataNull)
	{
		(*this)=src;
	}
	else
	{
		Copy();
		Data->Verify(src.Data->Len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],src.Data->Text,(src.Data->Len+1)*sizeof(C));
		Data->Len+=src.Data->Len;
	}
	return(static_cast<S&>(*this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S& R::BasicString<C,S>::operator+=(const C* src)
{
	mReturnValIfFail(src,static_cast<S&>(*this));
	if(Data==DataNull)
	{
		static_cast<S&>(*this)=src;
	}
	else
	{
		size_t len=strlen(src);
		Copy();
		Data->Verify(len+Data->Len+1);
		memcpy(&Data->Text[Data->Len],src,sizeof(C)*len+1);
		Data->Len+=len;
	}
	return(static_cast<S&>(*this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Replace(const C search,const C rep,bool first,int pos)
{
	C* start;
	bool left;
	size_t max;        // Maximal number of character to search.
	bool found(false);

	if(pos<0)
	{
		// From right
		left=false;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=static_cast<int>(Data->Len)+pos;
		if(pos<=0) return;
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
		if((*start)==search)
		{
			if(!found)
			{
				Copy();
				start=&Data->Text[pos];
				found=true;
			}
			(*start)=rep;
			if(first)
				return;
		}
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
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::ReplaceStr(const S& search,const S& rep,bool first,int pos)
{
	if(!GetLen())
		return;

	int skip=static_cast<int>(search.GetLen());         // Number of character to skip (+1)

	if(pos<0)
	{
		// From right
		pos=FindStr(search,pos);
		while(pos!=-1)
		{
			// Find one -> replace
			Copy(); // Make a deep copy if necessary
			S tmp(Mid(0,pos));
			tmp+=rep;
			tmp+=Mid(pos+skip);
			(*static_cast<S*>(this))=tmp;

			// If first -> stop
			if(first)
				break;

			// Find the next occurrence
			pos=FindStr(search,-pos);
		}
	}
	else
	{
		// From left
		int Add=static_cast<int>(rep.GetLen())-static_cast<int>(search.GetLen());  // Number of character added.
		pos=FindStr(search,pos);
		while(pos!=-1)
		{
			// Find one -> replace
			Copy(); // Make a deep copy if necessary
			S tmp(Mid(0,pos));
			tmp+=rep;
			tmp+=Mid(pos+skip);
			(*static_cast<S*>(this))=tmp;

			// If first -> stop
			if(first)
				break;

			// Find the next occurrence
			pos+=skip+Add;
			pos=FindStr(search,pos);
		}
	}
}


//-----------------------------------------------------------------------------
template<class C,class S>
	const C& R::BasicString<C,S>::operator[](size_t idx) const
{
	if(idx>=Data->Len)
		throw std::range_error("const C& R::BasicString<C,S>::operator[](size_t) const : index outside the string");
	return(Data->Text[idx]);
}


//-----------------------------------------------------------------------------
template<class C,class S>
	C& R::BasicString<C,S>::operator[](size_t idx)
{
	if(idx>=Data->Len)
		throw std::range_error("C& R::BasicString<C,S>::operator[](size_t) : index outside the string");
	Copy();
	return(Data->Text[idx]);
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S R::BasicString<C,S>::Mid(size_t idx,size_t len) const
{
	S res;
	size_t Len;

	// If the index is greater than the length -> return a null string.
	if(Data->Len<=idx) return(S::Null);

	// Computed the number of characters to copied
	if(len==(size_t)-1)
		Len=Data->Len-idx;
	else
		Len=len;
	if(Data->Len-idx+1<Len)
		Len=Data->Len-idx+1;

	// Verify the the string can hold the number to copied
	C* ptr=new C[Len+1];
	memcpy(ptr,&Data->Text[idx],sizeof(C)*Len);
	ptr[Len]=0;
	res.Data=new CharBuffer(ptr,Len,Len);
	return(res);
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Insert(const S& sub,int pos,size_t del)
{
	if((!del)&&(!sub.Data->Len))
		return;

	// Verify first if the length are compatible
	size_t Pos;
	if(pos<0)
	{
		Pos=abs(pos);
		if(Pos>=Data->Len)
			throw std::range_error("BasicString::Insert(const S&,int,size_t) : position outside the string");
		Pos=Data->Len-Pos;
	}
	else
	{
		Pos=pos;
		if(Pos>=Data->Len)
			throw std::range_error("BasicString::Insert(const S&,int,size_t) : position outside the string");
	}

	// Compute the number of characters to delete
	if(del>Data->Len-Pos)
		del=Data->Len-Pos;

	Copy();  // make a deep copy

	// Verify the size
	Data->Verify(Data->Len-del+sub.Data->Len);

	// Move the string to leave
	if(del<=Data->Len-Pos)
		memmove(&Data->Text[Pos+sub.Data->Len],&Data->Text[Pos+del],sizeof(C)*(Data->Len-Pos-del+1));

	// Copy the sub-string in the current one at position Pos
	memcpy(&Data->Text[Pos],sub.Data->Text,sizeof(C)*sub.Data->Len);
	Data->Len=Data->Len-del+sub.Data->Len;
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Split(R::iRContainer<S>& elements,const C car,const C del) const
{
	S element;
	size_t len;
	C* ptr;
	bool Skip(false);

	// Skip ending spaces
	for(len=Data->Len+1,ptr=Data->Text;--len;ptr++)
	{
		// Delimiters
		if((*ptr)==del)
			Skip=!Skip; // First call, skip=true
		else if(((*ptr)==car)&&(!Skip))
		{
			// Insert element
			if(!element.IsEmpty())
			{
				elements.InsertPtr(new S(element));
				element="";
			}
		}
		else
			element+=(*ptr);
	}
	if(!element.IsEmpty())
		elements.InsertPtr(new S(element));
}


//------------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::GuessWords(R::iRContainer<S>& elements,bool hyphen) const
{
	S New;
	bool Lower(false);  // A lower case must be next character for a word ?
	size_t len(Data->Len+1);
	C* Car(Data->Text);
	bool Start(true);

	// Treat the rest
	for(;--len;Car++)
	{
		if(Start)
		{
			// Skip leading non-alphanumber characters
			if(!isalnum(*Car))
				continue;
			else
				Start=false;
		}

		if(isspace(*Car)||(ispunct(*Car)&&(hyphen||((!hyphen)&&(*Car)!='-'))))
		{
			elements.InsertPtr(new S(New));
			New.Clear();
			Lower=false;
			Start=true;
		}
		else
		{
			bool CurLower(*Car==tolower(*Car));
			if(Lower)
			{
				if(CurLower)
					New+=(*Car);
				else
				{
					elements.InsertPtr(new S(New));
					New.Clear();
					Lower=false;
					if(isalnum(*Car))
						New+=(*Car);
					else
						Start=true;
				}
			}
			else
			{
				Lower=true;
				New+=(*Car);
			}
		}
	}
	elements.InsertPtr(new S(New));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	void R::BasicString<C,S>::Concat(const R::iRContainer<S>& elements,const C car)
{
	Clear();
	if(!elements.GetNb())
		return;
	C Sep[2];
	bool Add(false);
	for(size_t i=0;i<=elements.GetMaxPos();i++) // Cannot use a RCursor because of header dependencies
	{
		if(Add)
			(*this)+=Sep;
		else
		{
			if(car!=C(0))
			{
				Sep[0]=car;
				Sep[1]=C(0);
			}
			else
				Sep[0]=C(0);
			Add=true;
		}
		const S* Cur(elements[i]);
		if(Cur)
			(*this)+=(*Cur);
	}
}


//-----------------------------------------------------------------------------
template<class C,class S>
	R::BasicString<C,S>::~BasicString(void)
{
	RDecRef<CharBuffer>(Data);
}
