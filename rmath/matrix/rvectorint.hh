/*

	R Project Library

	RVectorInt.cpp

	Class representing a list of Integer value - Implementation

	Copyright 1998-2005 by the Université Libre de Bruxelles.

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
template<bool bOrder>
	RVectorInt<bOrder>::RVectorInt(const unsigned int max)
	: MaxInt(max)
{
	NbInt = 0;
	List = new unsigned int[MaxInt];
	memset(List,0,MaxInt*sizeof(unsigned int));
}


//------------------------------------------------------------------------------
template<bool bOrder>
	RVectorInt<bOrder>::RVectorInt(const RVectorInt& vec)
	: MaxInt(vec.MaxInt)
{
	NbInt = vec.NbInt;
	List = new unsigned int[MaxInt];
	memcpy(List,vec.List,vec.MaxInt*sizeof(unsigned int));
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Verify(unsigned int max)
{
	RReturnIfFail(max>0);
	if(max>MaxInt)
	{
		unsigned int OldSize=MaxInt;
		MaxInt+=(MaxInt/2);
		if(max>MaxInt)
			MaxInt=max;
		unsigned int* ptr=new unsigned int[MaxInt];
		memcpy(ptr,List,OldSize*sizeof(unsigned int));
		memset(&ptr[OldSize],0,(MaxInt-OldSize)*sizeof(unsigned int));
		delete[] List;
		List=ptr;
	}
}


//------------------------------------------------------------------------------
template<bool bOrder>
	unsigned int RVectorInt<bOrder>::GetId(unsigned int nb,bool& find) const
{
	unsigned int NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	unsigned int ptr,*ptr2;

	if(bOrder)
	{
		find=false;
		if(!MaxInt)
			return(0);
		NbMax=NbInt-1;
		NbMin=0;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				ptr=List[i];
				Comp=ptr-nb;
				if(!Comp)
				{
					find=true;
					return(i);
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
		}
		else
		{
			i = 0;
			Comp=(*List)-nb;
			if(!Comp)
			{
				find=true;
				return(0);
			}
		}
		if(Comp<0) i++;
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
template<bool bOrder>
	bool RVectorInt<bOrder>::IsSame(const RVectorInt& vi) const
{
	unsigned int i;
	unsigned int *ptr1,*ptr2;

	if(NbInt!=vi.NbInt) return(false);
	for(i=NbInt+1,ptr1=List,ptr2=vi.List;--i;ptr1++,ptr2++)
		if(*ptr1!=*ptr2) return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	bool RVectorInt<bOrder>::IsIn(unsigned int value) const
{
	unsigned int i;
	unsigned int *ptr;

	for(i=NbInt+1,ptr=List;--i;ptr++)
		if((*ptr)==value) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Insert(unsigned int ins)
{
	unsigned int *ptr=List;
	unsigned int i=NbInt;

	Verify(NbInt+1);
	if(bOrder)
	{
		while(i&&((*ptr)<ins))
		{
			i--;
			ptr++;
		}
		if(i)
			memmove(ptr+1,ptr,sizeof(unsigned int)*i);
		NbInt++;
		(*ptr) = ins;
	}
	else
		List[NbInt++]=ins;
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Insert(const RVectorInt& ins)
{
	unsigned int *ptr,i;

	Verify(NbInt+ins.NbInt);
	for(i=ins.NbInt+1,ptr=ins.List;--i;ptr++)
		Insert(*ptr);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::InsertAt(unsigned int ins,unsigned int pos)
{
	Verify(pos);
	if(NbInt<pos+1)
		NbInt=pos+1;
	List[pos]=ins;
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Delete(const unsigned int del)
{
	unsigned int *ptr=List;
	unsigned int i=0;

	while((*ptr)!=del)
	{
		i++;
		ptr++;
	}
	NbInt--;
	memcpy(ptr,ptr+1,sizeof(unsigned int)*(NbInt-i));
	List[NbInt]=0;
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Clear(void)
{
	NbInt=0;
	memset(List,0,MaxInt*sizeof(unsigned int));
}


//------------------------------------------------------------------------------
template<bool bOrder>
	RVectorInt<bOrder>& RVectorInt<bOrder>::operator=(const RVectorInt& src)
{
	Verify(src.NbInt);
	NbInt = src.NbInt;
	memcpy(List,src.List,sizeof(unsigned int)*NbInt);
	return(*this);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	unsigned int RVectorInt<bOrder>::operator[](int i) const
{
	return(List[i]);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	unsigned int RVectorInt<bOrder>::GetNbInt(void) const
{
	return(NbInt);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	RVectorInt<bOrder>::~RVectorInt(void)
{
	delete[] List;
}

