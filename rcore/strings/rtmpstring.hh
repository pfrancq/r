/*

	R Project Library

	RTmpString.cpp

	Temporary String - Implementation.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
// RTmpString
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::RTmpString(void)
	: Len(0)
{
	Text[0]=0;
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::RTmpString(const RTmpString& str)
	:  Len(str.Len)
{
	mempcy(Text,str.Text,(Len+1)*sizeof(RChar));
	mempcy(Latin1,str.Latin1,(Len+1)*sizeof(char));
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::RTmpString(const RChar car)
{
	Text[0]=car;
	Text[1]=0;
	if(!car)
		Len=0;
	else
		Len=1;
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::RTmpString(const char* src)
	: Len(0)
{
	Latin1[0]=0;
	if(src)
	{
		RChar* ptr(Text);
		while((Len<=MaxLen)&&(*src))
		{
			(*(ptr++))=(*(src++));
			Len++;
		}
		if(*src)
			mThrowRException("Maximal length is not large enough");
	}
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::RTmpString(const RChar* src)
	: Len(0)
{
	Latin1[0]=0;
	if(src)
	{
		RChar* ptr(Text);
		while((Len<=MaxLen)&&(!src->IsNull()))
		{
			(*(ptr++))=(*(src++));
			Len++;
		}
		if(!src->IsNull())
			mThrowRException("Maximal length is not large enough");
	}
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::RTmpString(const RChar* src,size_t len)
	: Len(0)
{
	Latin1[0]=0;
	if(src)
	{
		RChar* ptr(Text);
		while((Len<=MaxLen)&&(Len<=len)&&(!src->IsNull()))
		{
			(*(ptr++))=(*(src++));
			Len++;
		}
		if(!src->IsNull())
			mThrowRException("Maximal length is not large enough");
	}
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Copy(const RChar* text,size_t nb)
{
	Len=0;
	Latin1[0]=0;
	if(text)
	{
		RChar* ptr(Text);
		while((Len<=MaxLen)&&(Len<=nb)&&(!text->IsNull()))
		{
			(*(ptr++))=(*(text++));
			Len++;
		}
		if(!text->IsNull())
			mThrowRException("Maximal length is not large enough");
	}
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::UnicodeToLatin1(void) const
{
	RCString Str(RTextEncoding::GetUTF8Encoding()->FromUnicode(Text,Len));
	if(Str.GetLen()>MaxLen)
		mThrowRException("Maximal length is not large enough");
	size_t lenlatin1(Str.GetLen()+1);
	memcpy(const_cast<char*>(Latin1),Str(),lenlatin1*sizeof(char));
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	const char* R::RTmpString<MaxLen>::ToLatin1(void) const
{
	if(!Len)
		return("");
	if(!(*Latin1))
	{
		UnicodeToLatin1();
	}
	return(Latin1);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::operator std::string () const
{
	RChar* ptr;
	std::string text;

	ptr=Text;
	while(!ptr->IsNull())
	{
		text+=ptr->Latin1();
		ptr++;
	}
	return(text);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	std::string R::RTmpString<MaxLen>::ToString(void) const
{
	return(operator std::string());
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RTmpString<MaxLen>& R::RTmpString<MaxLen>::operator=(const RTmpString& src)
{
	Copy(src.Text,src.Len);
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RTmpString<MaxLen>& R::RTmpString<MaxLen>::operator=(const char* src)
{
	Latin1[0]=0;
	if(src)
	{
		RChar* ptr(Text);
		while((Len<=MaxLen)&&(*src))
		{
			(*(ptr++))=(*(src++));
			Len++;
		}
		if(*src)
			mThrowRException("Maximal length is not large enough");
	}
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RTmpString<MaxLen>& R::RTmpString<MaxLen>::operator=(const RChar* src)
{
	Latin1[0]=0;
	if(src)
	{
		RChar* ptr(Text);
		while((Len<=MaxLen)&&(!src->IsNull()))
		{
			(*(ptr++))=(*(src++));
			Len++;
		}
		if(!src->IsNull())
			mThrowRException("Maximal length is not large enough");
	}
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::SetLen(size_t len)
{
	if(len<=MaxLen)
	{
		Len=len;
		Text[len]=0;
		Latin1[0]=0;
	}
	else
		mThrowRException("Maximal length is not large enough");
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::SetLen(size_t len,const RChar* str)
{
	size_t oldsize=Len;
	SetLen(len);
	if(oldsize<len)
	{
		RChar* ptr=&Text[oldsize];
		RChar* ptr2=str;
		for(;oldsize<len;oldsize++,ptr++,ptr2++)
		{
			if(!(*ptr2))
				ptr2=str;
			if((*ptr2)!=0)
				(*ptr)=(*ptr2);
		}
	}
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Clear(void)
{
	Len=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::ToUpper(void) const
{
	RChar* ptr(Text);
	while(!ptr->IsNull())
		(*(ptr++))=toupper(*(ptr++));
	Latin1[0]=0;
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::ToLower(void) const
{
	RChar* ptr(Text);
	while(!ptr->IsNull())
		(*(ptr++))=tolower(*(ptr++));
	Latin1[0]=0;
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Trim(void) const
{
	size_t pos(0);
	size_t len(Len);
	RChar* ptr;

	// Skip ending spaces
	if(len)
	{
		ptr=&Text[len-1];
		while(len&&ptr->IsSpace())
		{
			len--;
			ptr--;
		}
	}

	// Skip beginning spaces
	if(len)
	{
		ptr=Text;
		while(len&&ptr->IsSpace())
		{
			len--;
			ptr++;
			pos++;
		}
	}

	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Trim(const RChar* str) const
{
	size_t pos(0);
	size_t len(Len);
	size_t slen(str);
	RChar* ptr;

	// Skip ending spaces
	if(len>=slen)
	{
		ptr=&Text[len-slen];
		while(len&&(!RChar::StrNCmp(str,ptr,slen)))
		{
			len-=slen;
			ptr-=slen;
		}
	}

	// Skip beginning spaces
	if(len>=slen)
	{
		ptr=Text;
		while(len&&(!RChar::StrNCmp(str,ptr,slen)))
		{
			len-=slen;
			ptr+=slen;
			pos+=slen;
		}
	}

	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RTmpString<MaxLen>& R::RTmpString<MaxLen>::operator+=(const RTmpString& src)
{
	if(!src.Len)
		return(*this);
	if(src.Len+Len>MaxLen)
		mThrowRException("Maximal length is not large enough");
	memcpy(&Text[Len],src.Text,src.Len*sizeof(RChar));
	Len+=src.Len;
	Text[Len+1]=0;
	Latin1[0]=0;
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RTmpString<MaxLen>& R::RTmpString<MaxLen>::operator+=(const char* src)
{
	size_t len(strlen(src));
	if(len+Len>MaxLen)
		mThrowRException("Maximal length is not large enough");
	RChar* ptr(&Text[Len]);
	const char* ptr2(src);
	while(*ptr2)
		(*(ptr++))=RChar(*(ptr2++));
	Len+=len;
	Text[Len+1]=0;
	Latin1[0]=0;
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RTmpString<MaxLen>& R::RTmpString<MaxLen>::operator+=(const RChar* src)
{
	size_t len(strlen(src));
	if(len+Len>MaxLen)
		mThrowRException("Maximal length is not large enough");
	memcpy(&Text[Len],src,len*sizeof(RChar));
	Len+=len;
	Text[Len+1]=0;
	Latin1[0]=0;
	return(*this);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Replace(const RChar search,const RChar rep,bool first,int pos)
{
	if(!Len)
		return;

	RChar* start;
	bool left;
	size_t max;        // Maximal number of character to search.

	if(pos<0)
	{
		// From right
		left=false;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=static_cast<int>(Len)+pos;
		if(pos<=0) return;
		start=&Text[pos];
		max=pos+1;
	}
	else
	{
		// From left
		left=true;

		// Start from 0 with maximal Len-pos+1 character to test.
		start=&Text[pos];
		max=Len-pos+1;
	}

	// Search for the maximal number of character
	for(max++;--max;)
	{
		if((*start)==search)
		{
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
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::ReplaceStr(const RTmpString& search,const RTmpString& rep,bool first,int pos)
{
	if(!Len)
		return;

	int skip=static_cast<int>(search.GetLen());         // Number of character to skip (+1)

	if(pos<0)
	{
		// From right
		pos=FindStr(search,pos);
		while(pos!=-1)
		{
			// Find one -> replace
			RTmpString tmp,tmp2;
			Mid(tmp,0,pos);
			tmp+=rep;
			Mid(tmp2,pos+skip);
			tmp+=tmp2;
			(*this)=tmp;

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
			RTmpString tmp,tmp2;
			Mid(tmp,0,pos);
			tmp+=rep;
			Mid(tmp2,pos+skip);
			tmp+=tmp2;
			(*this)=tmp;

			// If first -> stop
			if(first)
				break;

			// Find the next occurrence
			pos+=skip+Add;
			pos=FindStr(search,pos);
		}
	}
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	const RChar& R::RTmpString<MaxLen>::operator[](size_t idx) const
{
	if(idx>=Len)
		throw std::range_error("const RChar& R::RTmpString<MaxLen>::operator[](size_t) const : index outside the string");
	return(Text[idx]);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	RChar& R::RTmpString<MaxLen>::operator[](size_t idx)
{
	if(idx>=Len)
		throw std::range_error("RChar& R::RTmpString<MaxLen>::operator[](size_t) : index outside the string");
	Latin1[0]=0;
	return(Text[idx]);
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Mid(RTmpString& to,size_t idx,size_t len) const
{
	size_t CopyLen;

	// If the index is greater than the length -> return
	if(Len<=idx) return;

	// Computed the number of characters to copied
	if(len==(size_t)-1)
		CopyLen=Len-idx;
	else
		CopyLen=len;
	if(Len-idx+1<CopyLen)
		CopyLen=Len-idx+1;

	// Verify the the string can hold the number to copied
	memcpy(to.Text,&Text[idx],sizeof(RChar)*CopyLen);
	to.Text[CopyLen]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Insert(const RTmpString& sub,int pos,size_t del)
{
	if((!del)&&(!sub.Len))
		return;

	// Verify first if the length are compatible
	size_t Pos;
	if(pos<0)
	{
		Pos=abs(pos);
		if(Pos>=Len)
			throw std::range_error("RTmpString::Insert(const S&,int,size_t) : position outside the string");
		Pos=Len-Pos;
	}
	else
	{
		Pos=pos;
		if(Pos>=Len)
			throw std::range_error("RTmpString::Insert(const S&,int,size_t) : position outside the string");
	}

	// Compute the number of characters to delete
	if(del>Len-Pos)
		del=Len-Pos;

	if(sub.Len+Len-del>MaxLen)
		mThrowRException("Maximal length is not large enough");

	// Move the string to leave
	if(del<=Len-Pos)
		memmove(&Text[Pos+sub.Len],&Text[Pos+del],sizeof(RChar)*(Len-Pos-del+1));

	// Copy the sub-string in the current one at position Pos
	memcpy(&Text[Pos],sub.Text,sizeof(RChar)*sub.Len);
	Len=Len-del+sub.Len;
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	void R::RTmpString<MaxLen>::Split(R::RContainer<RTmpString,true,false>& elements,const RChar car,const RChar del) const
{
	RTmpString element;
	size_t len;
	RChar* ptr;
	bool Skip(false);

	// Skip ending spaces
	for(len=Len+1,ptr=Text;--len;ptr++)
	{
		// Delimiters
		if((*ptr)==del)
			Skip=!Skip; // First call, skip=true
		else if(((*ptr)==car)&&(!Skip))
		{
			// Insert element
			elements.InsertPtr(new RTmpString(element));
			element="";
		}
		else
			element+=(*ptr);
	}
	elements.InsertPtr(new RTmpString(element));
}


//-----------------------------------------------------------------------------
template<size_t MaxLen> template<bool a,bool o>
	void R::RTmpString<MaxLen>::Concat(const R::RContainer<RTmpString,a,o>& elements,const RChar car)
{
	Clear();
	if(!elements.GetNb())
		return;
	RChar Sep[2];
	bool Add(false);
	for(size_t i=0;i<=elements.GetMaxPos();i++) // Cannot use a RCursor because of header dependencies
	{
		if(Add)
			(*this)+=Sep;
		else
		{
			if(car!=RChar(0))
			{
				Sep[0]=car;
				Sep[1]=RChar(0);
			}
			else
				Sep[0]=RChar(0);
			Add=true;
		}
		const RTmpString* Cur(elements[i]);
		if(Cur)
			(*this)+=(*Cur);
	}
	Latin1[0]=0;
}


//-----------------------------------------------------------------------------
template<size_t MaxLen>
	R::RTmpString<MaxLen>::~RTmpString(void)
{
}
