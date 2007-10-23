/*

	R Project Library

	BasicString.cpp

	Generic String - Implementation.

	Copyright 2005 by the Université libre de Bruxelles.

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
//
// BasicCharBuffer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,class S> void R::BasicString<C,S>::BasicCharBuffer::Verify(size_t maxlen)
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
	RIncRef<BasicCharBuffer>(Data);
}


//-----------------------------------------------------------------------------
template<class C,class S>
	S R::BasicString<C,S>::Trim(void) const
{
	S res;
	size_t len = Data->Len;
	C* ptr = Data->Text;

	// Skip ending spaces
	if(!len)
		return(res);
	ptr=&Data->Text[len-1];
	while(len&&isspace(*ptr))
	{
		len--;
		ptr--;
	}

	// Skip beginning spaces
	if(!len)
		return(res);
	ptr=Data->Text;
	while(len&&isspace(*ptr))
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
	return(*static_cast<const S*>(this));
}


//-----------------------------------------------------------------------------
template<class C,class S>
	int R::BasicString<C,S>::Find(const C car,int pos,bool CaseSensitive) const
{
	C* start;
	bool left;
	size_t max;        // Maximal number of character to search.
	C search;

	// Initialise the search
	if(!CaseSensitive)
		search=toupper(car);
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
		if(((CaseSensitive)&&((*start)==search)) || ((!CaseSensitive)&&(toupper(*start)==search)))
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


//-----------------------------------------------------------------------------
template<class C,class S>
	int R::BasicString<C,S>::FindStr(const S& str,int pos,bool CaseSensitive) const
{
	C* start;
	const C* toFind;
	size_t max;        // Maximal number of character to search.
	size_t avanct;
	size_t maxlen;  //max number of char contained in the string to search
	int incr;
	S search(str);

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


//-----------------------------------------------------------------------------
template<class C,class S> template<class B>
	S R::BasicString<C,S>::Mid(size_t idx,int len) const
{
	S res;
	size_t Len;

	// If the index is greather than the length -> return a null string.
	if(Data->Len<=idx) return("");

	// Computed the number of caracters to copied
	if(len<0)
		Len=Data->Len-idx;
	else
		Len=len;
	if(Data->Len-idx+1<Len)
		Len=Data->Len-idx+1;

	// Verify the the string can hold the number to copied
	C* ptr=new C[Len+1];
	memcpy(ptr,&Data->Text[idx],sizeof(C)*Len);
	ptr[Len]=0;
	res.Data=new B(ptr,Len,Len);
	return(res);
}


//-----------------------------------------------------------------------------
template<class C,class S> template<class B>
	void R::BasicString<C,S>::SetLen(size_t len)
{
	if(len<=Data->Len)
	{
		Data->Len=len;
	}
	else
	{
		if(Data==S::DataNull)
		{
			C* ptr=new C[len+1];
			RDecRef<BasicCharBuffer>(Data);
			Data=new B(ptr,len,len);
		}
		else
			Data->Verify(len+1);
	}
	if(Data!=S::DataNull)
		Data->Text[len]=0;
}


//-----------------------------------------------------------------------------
template<class C,class S> template<class B>
	void R::BasicString<C,S>::SetLen(size_t len,const S& str)
{
	size_t oldsize=Data->Len;
	SetLen<B>(len);
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
	void R::BasicString<C,S>::Split(R::RContainer<S,true,false>& elements,const C car) const
{
	S element;
	size_t len;
	C* ptr;

	// Skip ending spaces
	for(len=Data->Len+1,ptr=Data->Text;--len;ptr++)
	{
		if((*ptr)==car)
		{
			// Insert element
			if(!element.IsEmpty())
				elements.InsertPtr(new S(element));
			element="";
		}
		else
			element+=(*ptr);
	}
	if(!element.IsEmpty())
		elements.InsertPtr(new S(element));
}
