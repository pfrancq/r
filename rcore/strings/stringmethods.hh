/*

	R Project Library

	StringMethods.cpp

	Generic String Methods - Implementation.

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
// StringMethods
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C>
	bool R::StringMethods::ContainOnlySpaces(const C* src,size_t len)
{
	if(!len)
		return(false);
	const C* ptr(src);
	while((*ptr)!=0)
	{
		if(!isspace(*(ptr++)))
			return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
template<class C>
	int R::StringMethods::Find(const C* src,size_t len,const C car,int pos,bool CaseSensitive)
{
	const C* start;
	bool left;
	size_t max;        // Maximal number of character to search.
	C search;

	// Initialize the search
	if(!CaseSensitive)
		search=static_cast<C>(toupper(car));
	else
		search=car;
	if(pos<0)
	{
		// From right
		left=false;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=static_cast<int>(len)+pos;
		if(pos<=0) return(-1);
		start=&src[pos];
		max=pos+1;
	}
	else
	{
		// From left
		left=true;

		// Start from 0 with maximal Len-pos+1 character to test.
		start=&src[pos];
		max=len-pos+1;
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
template<class C>
	int R::StringMethods::FindStr(const C* src,size_t len,const C* str,size_t strlen,int pos,bool CaseSensitive)
{
	const C* start;
	const C* toFind;
	size_t max;        // Maximal number of character to search.
	size_t avanct;
	size_t maxlen;  //max number of char contained in the string to search
	int incr;

	// Initialize the search
	if(pos<0)
	{
		// From right
		incr=-1;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=static_cast<int>(len)+pos;
		if(pos<=0) return(-1);
		start=&src[pos];
		max=pos+1;

		// Init string to find (here the last character)
		toFind=str;
		toFind+=strlen-1;
	}
	else
	{
		// From left
		incr=+1;

		// Start from 0 with maximal Len-pos+1 character to test.
		start=&src[pos];
		max=len-pos+1;

		// Init string to find
		toFind=str;
	}

	// If string to find is longer than the string return -1
	if (strlen>max)
		return(-1);

	// Search for the maximal number of character
	for(max++;--max;)
	{
		if(((CaseSensitive)&&((*start)==(*toFind))) || ((!CaseSensitive)&&(toupper(*start)==(*toFind))))
		{
			if(max>=strlen)
			{
				avanct=0;
				maxlen=strlen;
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
								return(pos-static_cast<int>(strlen)+1);
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
						return(pos-static_cast<int>(strlen+1));
				}
			}
		}
		start+=incr;
		pos+=incr;
	}
	return(-1);
}


//-----------------------------------------------------------------------------
template<class C>
	int R::StringMethods::FindAnyStr(const C* src,size_t len,const C* str,size_t strlen,int pos,bool CaseSensitive)
{
	const C* start;
	bool left;
	size_t max;        // Maximal number of character to search.

	// Initialize the search
	if(pos<0)
	{
		// From right
		left=false;

		// Start from Length-(-pos) with maximal pos+1 character to test.
		pos=static_cast<int>(len)+pos;
		if(pos<=0) return(-1);
		start=&src[pos];
		max=pos+1;
	}
	else
	{
		// From left
		left=true;

		// Start from 0 with maximal Len-pos+1 character to test.
		start=&src[pos];
		max=len-pos;
	}

	// Search for the maximal number of character
	for(max++;--max;)
	{
		if(StringMethods::Find(str,strlen,*start,0,CaseSensitive)!=-1)
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
template<class C>
	bool R::StringMethods::Begins(const C* src,size_t len,const C* begin,size_t beginlen,bool CaseSensitive,bool skip)
{
	// If the string is shorter -> Nothing to do
	if(len<beginlen)
		return(false);

	// Set the pointers to the characters to compare
	const C* Ptr1(src);
	const C* Ptr2(begin);

	// If spaces must be skipped -> do it
	if(skip)
	{
		while(((*Ptr1)!=0)&&(isspace(*Ptr1)))
			Ptr1++;
		while(((*Ptr2)!=0)&&(isspace(*Ptr2)))
			Ptr2++;
	}

	// Verify if all characters of Ptr2 are in the correct order
	while((*Ptr2)!=0)
	{
		if(((CaseSensitive)&&((*Ptr1)!=(*Ptr2))) || ((!CaseSensitive)&&(toupper(*Ptr1)!=toupper(*Ptr2))))
			return(false);
		Ptr1++;
		Ptr2++;
	}

	return(true);
}


//-----------------------------------------------------------------------------
template<class C>
	bool R::StringMethods::Ends(const C* src,size_t len,const C* end,size_t endlen,bool CaseSensitive,bool skip)
{
	// If the string is shorter -> Nothing to do
	if(len<endlen)
		return(false);

	// Set the pointers to the characters to compare
	const C* Ptr1(&src[len-1]);
	const C* Ptr2(&end[endlen-1]);
	size_t Pos1(len);
	size_t Pos2(endlen);

	// If spaces must be skipped -> do it
	if(skip)
	{
		while(Pos1&&(isspace(*Ptr1)))
		{
			Ptr1--;
			Pos1--;
		}
		while(Pos2&&(isspace(*Ptr2)))
		{
			Ptr2--;
			Pos2--;
		}
	}

	// Verify if all characters of Ptr2 are in the correct order
	while(Pos2)
	{
		if(((CaseSensitive)&&((*Ptr1)!=(*Ptr2))) || ((!CaseSensitive)&&(toupper(*Ptr1)!=toupper(*Ptr2))))
			return(false);
		Ptr1--;
		Pos1--;
		Ptr2--;
		Pos2--;
	}

	return(true);
}


//-----------------------------------------------------------------------------
template<class C>
	bool R::StringMethods::IsAt(const C* src,size_t len,const C* sub,size_t sublen,int pos,bool CaseSensitive)
{
	// Verify first if the length are compatible
	size_t Pos;
	if(pos<0)
	{
		Pos=abs(pos);
		if(Pos>=len)
			return(false);
		Pos=len-Pos;
	}
	else
		Pos=pos;
	if(Pos+sublen>len)
		return(false);

	const C* ptr(&src[Pos]);
	const C* ptr2(sub);
	for(size_t len(sublen+1);--len;ptr++,ptr2++)
		if(((CaseSensitive)&&((*ptr)!=(*ptr2))) || ((!CaseSensitive)&&(toupper(*ptr)!=(*ptr2))))
			return(false);
	return(true);
}
