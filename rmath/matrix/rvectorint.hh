/*

	R Project Library

	RVectorInt.cpp

	Class representing a list of Integer value - Implementation

	Copyright 1998-2003 by the Universit�Libre de Bruxelles.

	Authors:
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
#include <rmath/rvectorint.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RVectorInt
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<bool bOrder>
	RVectorInt<bOrder>::RVectorInt(const unsigned int MaxSize) throw(std::bad_alloc)
	: MaxInt(MaxSize)
{
	NbInt = 0;
	List = new unsigned int[MaxSize];
}


//------------------------------------------------------------------------------
template<bool bOrder>
	RVectorInt<bOrder>::RVectorInt(const RVectorInt* lst) throw(std::bad_alloc)
	: MaxInt(lst->MaxInt)
{
	NbInt = lst->NbInt;
	List = new unsigned int[MaxInt];
	memcpy(List,lst->List,lst->NbInt*sizeof(NbInt));
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Verify(void) throw(std::bad_alloc)
{
	if(NbInt==MaxInt)
	{
		unsigned int OldSize;
		unsigned int IncPtr;
		unsigned int* ptr;

		OldSize=MaxInt;
		IncPtr=MaxInt/2;
		MaxInt+=IncPtr;
		ptr=new unsigned int[MaxInt];
		memcpy(ptr,List,OldSize*sizeof(unsigned int));
		delete[] List;
		List=ptr;
	}
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Verify(unsigned int max) throw(std::bad_alloc)
{
	RReturnIfFail(max>0);
	if(max>MaxInt)
	{
		unsigned int OldSize;
		unsigned int* ptr;

		OldSize=MaxInt;
		MaxInt=max;
		ptr=new unsigned int[MaxInt];
		memcpy(ptr,List,OldSize*sizeof(unsigned int));
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
	bool RVectorInt<bOrder>::IsIn(const unsigned int In) const
{
	unsigned int i;
	unsigned int *ptr;

	for(i=NbInt+1,ptr=List;--i;ptr++)
		if((*ptr)==In) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Insert(const unsigned int Ins)
{
	unsigned int *ptr=List;
	unsigned int i=NbInt;

	Verify();
	if(bOrder)
	{
		while(i&&((*ptr)<Ins))
		{
			i--;
			ptr++;
		}
		if(i)
			memmove(ptr+1,ptr,sizeof(unsigned int)*i);
		NbInt++;
		(*ptr) = Ins;
	}
	else
		List[NbInt++]=Ins;
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Insert(const RVectorInt& Ins)
{
	unsigned int *ptr,i;

	Verify();
	for(i=Ins.NbInt+1,ptr=Ins.List;--i;ptr++)
		Insert(*ptr);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Delete(const unsigned int Del)
{
	unsigned int *ptr=List;
	unsigned int i=0;

	while((*ptr)!=Del)
	{
		i++;
		ptr++;
	}
	NbInt--;
	memcpy(ptr,ptr+1,sizeof(unsigned int)*(NbInt-i));
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Reset(void)
{
	NbInt=0;
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

