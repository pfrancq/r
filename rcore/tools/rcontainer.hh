/*

	R Project Library

	RContainer.hh

	Container - Inline Implementation.

	Copyright 1999-2011 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
// class RContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<bool b>
	void RContainer<C,bAlloc,bOrder>::Create(const RContainer<C,b,bOrder>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	MaxPtr=src.MaxPtr;
	IncPtr=src.IncPtr;
	NbPtr=src.NbPtr;
	LastPtr=src.LastPtr;
	if(!MaxPtr)
	{
		MaxPtr=10;
		if(!IncPtr) IncPtr=10;
	}
	Tab = new C*[MaxPtr];
	memset(Tab,0,MaxPtr*sizeof(C*));
	if(bAlloc&&b)
	{
		for(i=src.LastPtr+1,tab=src.Tab,tab2=Tab;--i;tab++,tab2++)
		{
			if(*tab)
				(*tab2)=new C(**tab);
		}
	}
	else
	{
		memcpy(Tab,src.Tab,src.LastPtr*sizeof(C*));
	}
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<bool b>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::NormalCopy(const RContainer<C,b,bOrder>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	Clear();
	if(!src.NbPtr)
		return(*this);
	VerifyTab(src.LastPtr);
	if(bAlloc&&b)
	{
		for(i=src.LastPtr+1,tab=src.Tab,tab2=Tab;--i;tab++,tab2++)
		{
			if(*tab)
				(*tab2)=new C(**tab);
		}
	}
	else
	{
		memcpy(Tab,src.Tab,src.LastPtr*sizeof(C*));
	}
	NbPtr=src.NbPtr;
	LastPtr=src.LastPtr;
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<bool b>
	void RContainer<C,bAlloc,bOrder>::DeepCopy(const RContainer<C,b,bOrder>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	Clear();
	if(!src.NbPtr)
		return;
	VerifyTab(src.LastPtr);
	for(i=src.LastPtr+1,tab=src.Tab,tab2=Tab;--i;tab++,tab2++)
	{
		if(*tab)
			(*tab2)=new C(**tab);
	}
	NbPtr=src.NbPtr;
	LastPtr=src.LastPtr;
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<bool b,bool o>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::Add(const RContainer<C,b,o>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	if(!src.NbPtr)
		return(*this);
	VerifyTab(src.LastPtr+LastPtr);
	if(bOrder&&NbPtr)
	{
		for(i=src.LastPtr+1,tab=src.Tab;--i;tab++)
		{
			if(*tab)
			{
				if(bAlloc)
					InsertPtr(new C(**tab));
				else
					InsertPtr(*tab);
			}
		}
	}
	else
	{
		for(i=src.LastPtr+1,tab=src.Tab,tab2=&Tab[LastPtr];--i;tab++)
		{
			if(*tab)
			{
				if(bAlloc)
					(*(tab2++))=new C(**tab);
				else
					(*(tab2++))=*tab;
				LastPtr++;
				NbPtr++;
			}
		}
	}
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(size_t m,size_t i)
		: Tab(0), NbPtr(0), MaxPtr(m), LastPtr(0), IncPtr(i)
{
	if(!IncPtr)
	{
		if(MaxPtr) IncPtr=MaxPtr/2;
		if(IncPtr<10) IncPtr=10;
	}
	Tab = new C*[MaxPtr];
	memset(Tab,0x0,MaxPtr*sizeof(C*));
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,true,bOrder>& src)
	: Tab(0), NbPtr(0), MaxPtr(0), LastPtr(0), IncPtr(10)
{
	Create<true>(src);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,false,bOrder>& src)
		: Tab(0), NbPtr(0), MaxPtr(0), LastPtr(0), IncPtr(10)
{
	Create<false>(src);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::VerifyTab(size_t max)
{
	if((!max)&&(LastPtr==MaxPtr))
		max=MaxPtr+IncPtr;
	if(max>MaxPtr)
	{
		C** ptr;
		size_t OldSize;

		OldSize=MaxPtr;
		MaxPtr=max;
		ptr=new C*[MaxPtr];
		memcpy(ptr,Tab,OldSize*sizeof(C*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[OldSize],0x0,(MaxPtr-OldSize)*sizeof(C*));
	}
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Clear(size_t m,size_t i)
{
	if(bAlloc)
	{
		C **ptr;

		for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
		{
			if(*ptr)
				delete(*ptr);
		}
	}
	memset(Tab,0x0,MaxPtr*sizeof(C*));
	LastPtr=NbPtr=0;
	if(i)
		IncPtr=i;
	if(m)
		VerifyTab(m);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Exchange(size_t pos1,size_t pos2)
{
	if((pos1>LastPtr)||(pos2>LastPtr))
		return;
	C* ptr=Tab[pos1];
	Tab[pos1]=Tab[pos2];
	Tab[pos2]=ptr;
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	template<bool a,bool o>	void RContainer<C,bAlloc,bOrder>::Transfer(RContainer<C,a,o>& src)

{
	Clear(src.LastPtr,0);
	if(!src.NbPtr)
		return;
	memcpy(Tab,src.Tab,src.LastPtr*sizeof(C*));
	NbPtr=src.NbPtr;
	LastPtr=src.LastPtr;
	src.NbPtr=0;
	src.LastPtr=0;
	memset(src.Tab,0x0,src.MaxPtr*sizeof(C*));
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	template<class TUse> size_t RContainer<C,bAlloc,bOrder>::GetIndex(bool order,const TUse& tag,bool& find,size_t min,size_t max) const
{
	size_t NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	C *ptr,**ptr2;

	if(!NbPtr)
	{
		find=false;
		return(0);
	}
	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if((!max)||(max>=LastPtr)||(max<min))
		NbMax=LastPtr-1;
	else
		NbMax=max;
	if(order)
	{
		find=false;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				ptr=Tab[i];
				if(!ptr)
					ThrowRException("Problem!");
				Comp=ptr->Compare(tag);
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
			Comp=(*Tab)->Compare(tag);
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
		for(i=NbMin,ptr2=&Tab[NbMin];i<=NbMax;ptr2++,i++)
			if((*ptr2)&&(!(*ptr2)->Compare(tag))) return(i);
		find=false;
		return(i);
	}
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	const C* RContainer<C,bAlloc,bOrder>::operator[](size_t idx) const
{
	if(idx>=LastPtr)
	{
		if(LastPtr)
			throw std::range_error("RContainer<C,bAlloc,bOrder>::operator[] const : idx "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]");
		else
			throw std::range_error("RContainer<C,bAlloc,bOrder>::operator[] const : no elements");
	}
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	C* RContainer<C,bAlloc,bOrder>::operator[](size_t idx)
{
	if(idx>=LastPtr)
	{
		if(LastPtr)
			throw std::range_error("RContainer<C,bAlloc,bOrder>::operator[] : idx "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]");
		else
			throw std::range_error("RContainer<C,bAlloc,bOrder>::operator[] : no elements");
	}
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	template<class TUse> C* RContainer<C,bAlloc,bOrder>::GetPtr(const TUse& tag,bool sortkey,size_t min,size_t max) const
{
	bool Find;
	size_t Index=GetIndex(bOrder&&sortkey,tag,Find,min,max);
	if(Find)
		return(Tab[Index]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,bAlloc,bOrder>::GetInsertPtr(const TUse& tag,bool sortkey,size_t min,size_t max)
{
	C* ptr;
	bool Find;
	size_t Index=GetIndex(bOrder&&sortkey,tag,Find,min,max);
	if(!Find)
		InsertPtrAt(ptr=new C(tag),Index,false);
	else
		ptr=Tab[Index];
	return(ptr);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,bAlloc,bOrder>::GetInsertPtrAt(const TUse& tag,size_t pos)
{
	C* ptr;

	// Look if it is in the container
	if(pos<MaxPtr)
		ptr=Tab[pos];
	else
		ptr=0;  // If the position is higher than the maximal, it cannot be in

	// If no valid pointer -> Create the element.
	if(!ptr)
		InsertPtrAt(ptr=new C(tag),pos,false);

	return(ptr);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	size_t RContainer<C,bAlloc,bOrder>::GetTab(const void** tab,size_t min,size_t max) const
{
	if(!LastPtr)
		return(0);

	size_t NbMin,NbMax;
	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if((!max)||(max>=LastPtr)||(max<min))
		NbMax=LastPtr-1;
	else
		NbMax=max;
	size_t ToCopy(NbMax-NbMin+1);
	if(ToCopy)
		memcpy(tab,&Tab[NbMin],ToCopy*sizeof(C*));
	return(ToCopy);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	size_t RContainer<C,bAlloc,bOrder>::GetTab(void** tab,size_t min,size_t max)
{
	if(!LastPtr)
		return(0);

	size_t NbMin,NbMax;
	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if((!max)||(max>=LastPtr)||(max<min))
		NbMax=LastPtr-1;
	else
		NbMax=max;
	size_t ToCopy(NbMax-NbMin+1);
	if(ToCopy)
		memcpy(tab,&Tab[NbMin],ToCopy*sizeof(C*));
	return(ToCopy);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
  template<class TUse> RContainer<C,false,bOrder>*
    RContainer<C,bAlloc,bOrder>::GetPtrs(const TUse& tag,size_t min,size_t max) const
{
	C **ptr;
	size_t i;
	RContainer<C,false,bOrder> *tmp;
	size_t NbMin,NbMax;

	if(min<LastPtr-1)
		NbMin=min;
	else
		NbMin=0;
	if(!max)
		max=LastPtr-1;
	if((max<LastPtr-1)&&(max>min))
		NbMax=max-1;
	else
		NbMax=LastPtr-1;
	tmp=new RContainer<C,false,bOrder>(MaxPtr,IncPtr);
	for(i=NbMin,ptr=reinterpret_cast<C**>(&Tab[NbMin]);i<=NbMax;ptr++,i++)
		if((*ptr)&&(!((*ptr)->Compare(tag))))
			tmp->InsertPtr(*ptr);
	return(tmp);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::InsertPtrAt(C* ins,size_t pos,bool del)
{
	C** ptr;

	if(!ins)
		throw std::invalid_argument("RContainer<C,bAlloc,bOrder>::InsertPtrAt : Cannot insert a null pointer");
	if(pos+1>MaxPtr)
	{
		VerifyTab(pos+1+IncPtr);
	}
	else
	{
		VerifyTab(0);
	}
	ptr=&Tab[pos];
	if(pos<LastPtr)
	{
		if(del)
		{
			if(bAlloc&&(*ptr))
				delete (*ptr);
		}
		else
		{
			memmove(ptr+1,ptr,(LastPtr-pos)*sizeof(C*));
			NbPtr++;
		}
	}
	else
		NbPtr++;
	(*ptr)=ins;
	if(pos+1>LastPtr)
	{
		LastPtr=pos+1;
	}
	else
	{
		if(!del)
			LastPtr++;
	}
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::InsertPtr(C* ins,bool del,size_t min,size_t max)
{
	if(!ins)
		throw std::invalid_argument("RContainer<C,bAlloc,bOrder>::InsertPtr : Cannot insert a null pointer");

	if(bOrder)
	{
		bool Find;
		size_t Index=GetIndex(*ins,Find,min,max);
		InsertPtrAt(ins,Index,Find&&del);
	}
	else
	{
		VerifyTab(0);
		Tab[LastPtr++]=ins;
		NbPtr++;
	}
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::DeletePtrAt(size_t pos,bool shift,bool del)
{
	C** ptr;

	if(pos>=LastPtr)
		throw std::range_error("RContainer<C,bAlloc,bOrder>::DeletePtrAt : idx "+RString::Number(pos)+" outside range [0,"+RString::Number(LastPtr-1)+"]");
	ptr=&Tab[pos];
	if(del)
		delete(*ptr);

	// If the position is the last one -> find the last position
	if(pos==LastPtr-1)
	{
		(*ptr)=0;
		if(NbPtr>1)
			for(;(!(*ptr))&&LastPtr;ptr--,LastPtr--) ;
		else
			LastPtr=0;
	}
	else
	{
		// Not the last element -> Verify if the elements must be moved
		if(shift)
		{
			memmove(ptr,ptr+1,((--LastPtr)-pos)*sizeof(C*));
			Tab[LastPtr]=0;
		}
		else
			(*ptr)=0;
	}

	// Decrease number of elements
	NbPtr--;
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	template<class TUse> void RContainer<C,bAlloc,bOrder>::DeletePtr(const TUse& tag,bool sortkey,bool del,size_t min,size_t max)
{
	if(!(&tag))
		throw std::invalid_argument("RContainer<C,bAlloc,bOrder>::DeletePtr : Cannot delete a null pointer");

	bool Find;
	size_t Index=GetIndex(bOrder&&sortkey,tag,Find,min,max);
	if(!Find)
		return;
	DeletePtrAt(Index,true,del);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::~RContainer(void)
{
	if(bAlloc)
	{
		C** ptr;
		for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
			delete(*ptr);
	}
	delete[] Tab;
}
