/*

	R Project Library

	RVectorInt.cpp

	Class representing a list of Integer value - Implementation

	Copyright 1998-2008 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery(vvandaele@ulb.ac.be).

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <string.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rvectorint.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RVectorInt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RVectorInt<I,bOrder>::RVectorInt(size_t max)
	: MaxInt(max)
{
	NbInt = 0;
	if(!max)
		MaxInt=100;
	List = new I[MaxInt];
	memset(List,0,MaxInt*sizeof(I));
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RVectorInt<I,bOrder>::RVectorInt(const RVectorInt& vec)
	: MaxInt(vec.MaxInt)
{
	NbInt = vec.NbInt;
	List = new I[MaxInt];
	memcpy(List,vec.List,vec.MaxInt*sizeof(I));
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::Verify(size_t max)
{
	if(max>MaxInt)
	{
		I* ptr;
		size_t OldSize;

		OldSize=MaxInt;
		MaxInt+=(MaxInt/2);
		if(max>MaxInt)
			MaxInt=max;
		ptr=new I[MaxInt];
		memcpy(ptr,List,OldSize*sizeof(I));
		delete[] List;
		List=ptr;
		memset(&List[OldSize],0,(MaxInt-OldSize)*sizeof(I));
	}
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	size_t RVectorInt<I,bOrder>::GetId(I nb,bool& find) const
{
	size_t NbMin,NbMax,i=0;
	bool CompNeg(true);
	bool Cont=true,NotLast=true;
	I ptr,*ptr2;

	if(bOrder)
	{
		find=false;
		if(!NbInt)
			return(0);
		NbMax=NbInt-1;
		NbMin=0;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				ptr=List[i];
				if(ptr==nb)
				{
					find=true;
					return(i);
				}
				if(ptr>nb)
				{
					NbMax = i;
					if(i) NbMax--;
				}
				else
					NbMin = i+1;
				Cont = NotLast;
				if(NbMin>=NbMax) NotLast=false;
			}
		}
		else
		{
			i = 0;
			if((*List)==nb)
			{
				find=true;
				return(0);
			}
			else if((*List)>nb)
				CompNeg=false;
		}
		if(CompNeg) i++;
		return(i);
	}
	else
	{
		find=true;
		for(i=0,ptr2=List;i<MaxInt;ptr2++,i++)
			if((*ptr2)&&(((*ptr2)!=nb))) return(i);
		find=false;
		return(i);
	}
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	bool RVectorInt<I,bOrder>::IsSame(const RVectorInt& vi) const
{
	size_t i;
	I *ptr1,*ptr2;

	if(NbInt!=vi.NbInt) return(false);
	for(i=NbInt+1,ptr1=List,ptr2=vi.List;--i;ptr1++,ptr2++)
		if((*ptr1)!=(*ptr2)) return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	bool RVectorInt<I,bOrder>::IsIn(I value) const
{
	size_t i;
	I *ptr;

	for(i=NbInt+1,ptr=List;--i;ptr++)
		if((*ptr)==value) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::Insert(I ins)
{
	Verify(NbInt+1);
	if(bOrder)
	{
		bool Find;
		size_t Index=GetId(ins,Find);
		InsertAt(ins,Index);
	}
	else
		List[NbInt++]=ins;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::Insert(const RVectorInt& ins)
{
	I* ptr;
	size_t i;

	Verify(NbInt+ins.NbInt);
	for(i=ins.NbInt+1,ptr=ins.List;--i;ptr++)
		Insert(*ptr);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::InsertAt(I ins,size_t pos)
{
	I* ptr;

	if(pos+1>MaxInt)
		Verify(pos+1);
	ptr=&List[pos];
	if(pos<NbInt)
		memmove(ptr+1,ptr,(NbInt-pos)*sizeof(I));
	(*ptr)=ins;
	NbInt++;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::Delete(I del)
{
	I *ptr=List;
	size_t i=0;

	while((*ptr)!=del)
	{
		i++;
		ptr++;
	}
	NbInt--;
	memmove(ptr,ptr+1,sizeof(I)*(NbInt-i));
	List[NbInt]=0;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::Clear(void)
{
	NbInt=0;
	memset(List,0,MaxInt*sizeof(I));
}

//------------------------------------------------------------------------------
template<class I,bool bOrder>
	int RVectorInt<I,bOrder>::ReOrderFunction(const void* num1, const void* num2)
{
	const I *a=static_cast<const I*>(num1);
	const I *b=static_cast<const I*>(num2);
	return((*a)-(*b));
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::ReOrder(void)
{
	qsort(static_cast<void*>(List),NbInt,sizeof(I),ReOrderFunction);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RVectorInt<I,bOrder>::Randomize(RRandom* rand,size_t nb)
{
	if((!nb)||(nb>NbInt))
		nb=NbInt;
	rand->RandOrder<I>(List,nb);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RVectorInt<I,bOrder>& RVectorInt<I,bOrder>::operator=(const RVectorInt& src)
{
	Verify(src.NbInt);
	NbInt = src.NbInt;
	memcpy(List,src.List,sizeof(I)*NbInt);
	return(*this);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RVectorInt<I,bOrder>::~RVectorInt(void)
{
	delete[] List;
}
