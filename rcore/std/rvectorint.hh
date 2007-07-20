/*

	R Project Library

	RVectorInt.cpp

	Class representing a list of Integer value - Implementation

	Copyright 1998-2007 by the Université Libre de Bruxelles.

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
	RVectorInt<bOrder>::RVectorInt(size_t max)
	: MaxInt(max)
{
	NbInt = 0;
	if(!max)
		MaxInt=100;
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
	void RVectorInt<bOrder>::Verify(size_t max)
{
	if(max>MaxInt)
	{
		unsigned int* ptr;
		size_t OldSize;
		
		OldSize=MaxInt;
		MaxInt+=(MaxInt/2);
		if(max>MaxInt)
			MaxInt=max;
		ptr=new unsigned int[MaxInt];
		memcpy(ptr,List,OldSize*sizeof(unsigned int));
		delete[] List;
		List=ptr;
		memset(&List[OldSize],0,(MaxInt-OldSize)*sizeof(unsigned int));
	}
}

	
//------------------------------------------------------------------------------
template<bool bOrder>
	size_t RVectorInt<bOrder>::GetId(unsigned int nb,bool& find) const
{
	size_t NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	unsigned int ptr,*ptr2;

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
	size_t i;
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
	size_t i;
	unsigned int *ptr;

	for(i=NbInt+1,ptr=List;--i;ptr++)
		if((*ptr)==value) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Insert(unsigned int ins)
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
template<bool bOrder>
	void RVectorInt<bOrder>::Insert(const RVectorInt& ins)
{
	unsigned int* ptr;
	size_t i;

	Verify(NbInt+ins.NbInt);
	for(i=ins.NbInt+1,ptr=ins.List;--i;ptr++)
		Insert(*ptr);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::InsertAt(unsigned int ins,size_t pos)
{
	unsigned int* ptr;
	
	if(pos+1>MaxInt)
		Verify(pos+1);
	ptr=&List[pos];
	if(pos<NbInt)
		memmove(ptr+1,ptr,(NbInt-pos)*sizeof(unsigned int));
	(*ptr)=ins;
	NbInt++;	
}


//------------------------------------------------------------------------------
template<bool bOrder>
	void RVectorInt<bOrder>::Delete(unsigned int del)
{
	unsigned int *ptr=List;
	size_t i=0;

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
	unsigned int RVectorInt<bOrder>::operator[](size_t i) const
{
	return(List[i]);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	size_t RVectorInt<bOrder>::GetNbInt(void) const
{
	return(NbInt);
}


//------------------------------------------------------------------------------
template<bool bOrder>
	RVectorInt<bOrder>::~RVectorInt(void)
{
	delete[] List;
}

