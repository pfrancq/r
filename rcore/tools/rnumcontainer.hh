/*

	R Project Library

	RNumContainer.hh

	Class representing a container of numbers - Inline implementation

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2001 by Valery Vandaele.
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
//
// class RNumContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RNumContainer<I,bOrder>::RNumContainer(size_t max)
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
	RNumContainer<I,bOrder>::RNumContainer(const RNumContainer& vec)
	: MaxInt(vec.MaxInt)
{
	NbInt = vec.NbInt;
	List = new I[MaxInt];
	memcpy(List,vec.List,vec.MaxInt*sizeof(I));
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Verify(size_t max)
{
	if(max>=MaxInt)
	{
		size_t OldSize(MaxInt);
		MaxInt+=(MaxInt/2);
		if(max>MaxInt)
			MaxInt=max;
		I* ptr(new I[MaxInt]);
		memcpy(ptr,List,OldSize*sizeof(I));
		delete[] List;
		List=ptr;
		memset(&List[OldSize],0,(MaxInt-OldSize)*sizeof(I));
	}
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Init(size_t nb,I val)
{
	Verify(nb);
	NbInt = nb;
	I* ptr(List);
	for(++nb;--nb;ptr++)
		(*ptr)=val;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	size_t RNumContainer<I,bOrder>::GetId(I nb,bool& find) const
{
	if(bOrder)
	{
		size_t NbMin,NbMax,i;
		I ptr;
		bool CompNeg(true);
		bool Cont=true,NotLast=true;

		find=false;
		if(!NbInt)
			return(0);
		NbMax=NbInt-1;
		NbMin=0;
		i=0;
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
					CompNeg=false;
					NbMax = i;
					if(i) NbMax--;
				}
				else
				{
					NbMin = i+1;
					CompNeg=true;
				}
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
		I* ptr(List);
		size_t i(0);
		for(;i<MaxInt;ptr++,i++)
			if((*ptr)&&(((*ptr)==nb))) return(i);
		find=false;
		return(i);
	}
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	bool RNumContainer<I,bOrder>::IsSame(const RNumContainer& vi) const
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
	bool RNumContainer<I,bOrder>::IsIn(I value) const
{
	bool find;
	GetId(value,find);
	return(find);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Insert(I ins)
{
	Verify(NbInt+1);
	if(bOrder)
	{
		bool Find;
		size_t Index=GetId(ins,Find);
		if(!Find)
			InsertAt(ins,Index);
	}
	else
		List[NbInt++]=ins;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::InsertAt(I ins,size_t pos,bool del)
{
	I* ptr;

	if(pos+1>MaxInt)
		Verify(pos+1);
	ptr=&List[pos];
	if((!del)&&(pos<NbInt))
	{
		memmove(ptr+1,ptr,(NbInt-pos)*sizeof(I));
		NbInt++;
	}
	else if(pos>=NbInt)
		NbInt=pos+1;
	(*ptr)=ins;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Delete(I del)
{
	bool Find;
	size_t Index=GetId(del,Find);
	if(Find)
		DeleteAt(Index,true);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::DeleteAt(size_t pos,bool shift)
{
	I* ptr;

	RReturnIfFail(pos<NbInt);
	ptr=&List[pos];

	// Decrease number of elements
	NbInt--;

	// If the position is the not last one and the element must be shifted
	if((pos<NbInt)&&shift)
	{
		memmove(ptr,ptr+1,sizeof(I)*(NbInt-pos));
		List[NbInt]=0;
	}
	else
		(*ptr)=0;
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Clear(void)
{
	NbInt=0;
	memset(List,0,MaxInt*sizeof(I));
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	int RNumContainer<I,bOrder>::ReOrderFunction(const void* num1, const void* num2)
{
	const I *a=static_cast<const I*>(num1);
	const I *b=static_cast<const I*>(num2);
	return((*a)-(*b));
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::ReOrder(void)
{
	qsort(static_cast<void*>(List),NbInt,sizeof(I),ReOrderFunction);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Randomize(RRandom* rand,size_t nb)
{
	if((!nb)||(nb>NbInt))
		nb=NbInt;
	rand->RandOrder<I>(List,nb);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RNumContainer<I,bOrder>& RNumContainer<I,bOrder>::operator=(const RNumContainer& src)
{
	Verify(src.NbInt);
	NbInt = src.NbInt;
	memcpy(List,src.List,sizeof(I)*NbInt);
	return(*this);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	void RNumContainer<I,bOrder>::Add(const RNumContainer& ins)
{
	I* ptr;
	size_t i;

	Verify(NbInt+ins.NbInt);
	for(i=ins.NbInt+1,ptr=ins.List;--i;ptr++)
		Insert(*ptr);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	I RNumContainer<I,bOrder>::operator[](size_t i) const
{
	if(i>=NbInt)
	{
		if(NbInt)
			throw std::range_error("RNumContainer<I,bOrder>::operator[] const : idx "+RString::Number(i)+" outside range [0,"+RString::Number(NbInt-1)+"]");
		else
			throw std::range_error("RNumContainer<I,bOrder>::operator[] const : no elements");
	}
	return(List[i]);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	I& RNumContainer<I,bOrder>::operator[](size_t i)
{
	Verify(i);
	if(i>=NbInt)
	{
		NbInt=i+1;
	}
	return(List[i]);
}


//------------------------------------------------------------------------------
template<class I,bool bOrder>
	RNumContainer<I,bOrder>::~RNumContainer(void)
{
	delete[] List;
}
