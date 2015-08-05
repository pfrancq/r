/*

	R Project Library

	iRContainer.hh

	Container Interface - Inline Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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
// class iRContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Create(const iRContainer<C>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	IncPtr=src.IncPtr;
	if(!src.NbPtr)
		return;
	VerifyTab(src.LastPtr);
	NbPtr=src.NbPtr;
	LastPtr=src.LastPtr;
	if(Dealloc&&src.Dealloc)
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
template<class C>
	iRContainer<C>& iRContainer<C>::NormalCopy(const iRContainer<C>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	Clear();
	if(!src.NbPtr)
		return(*this);
	VerifyTab(src.LastPtr);
	if(Dealloc&&src.Dealloc)
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
template<class C>
	void iRContainer<C>::DeepCopy(const iRContainer<C>& src)
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
template<class C>
	iRContainer<C>& iRContainer<C>::Add(const iRContainer<C>& src)
{
	size_t i;
	C** tab;
	C** tab2;

	if(!src.NbPtr)
		return(*this);
	VerifyTab(src.LastPtr+LastPtr);
	if(Order&&NbPtr)
	{
		for(i=src.LastPtr+1,tab=src.Tab;--i;tab++)
		{
			if(*tab)
			{
				if(Dealloc)
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
				if(Dealloc)
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
template<class C>
	iRContainer<C>::iRContainer(bool dealloc,bool order,size_t m,size_t i)
		: Dealloc(dealloc), Order(order),Tab(0), NbPtr(0), MaxPtr(m), LastPtr(0), IncPtr(i)
{
	if(!IncPtr)
	{
		if(MaxPtr) IncPtr=MaxPtr/2;
		if(IncPtr<10) IncPtr=10;
	}
	if(MaxPtr)
	{
		Tab = new C*[MaxPtr];
		memset(Tab,0x0,MaxPtr*sizeof(C*));
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::VerifyTab(size_t max)
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
		if(Tab)
		{
			memcpy(ptr,Tab,OldSize*sizeof(C*));
			delete[] Tab;
		}
		Tab=ptr;
		memset(&Tab[OldSize],0x0,(MaxPtr-OldSize)*sizeof(C*));
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Clear(size_t m,size_t i)
{
	if(Tab)
	{
		if(Dealloc)
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
	}
	if(i)
		IncPtr=i;
	if(m)
		VerifyTab(m);
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Exchange(size_t pos1,size_t pos2)
{
	if(!LastPtr)
		throw std::range_error("iRContainer<C>::operator[] const : no elements");
	if(pos1>=LastPtr)
        throw std::range_error(R::RString("iRContainer<C>::operator[] const : idx "+RString::Number(pos1)+" outside range [0,"+RString::Number(LastPtr-1)+"]").ToString());
	if(pos2>=LastPtr)
        throw std::range_error(R::RString("iRContainer<C>::operator[] const : idx "+RString::Number(pos2)+" outside range [0,"+RString::Number(LastPtr-1)+"]").ToString());
	C* ptr=Tab[pos1];
	Tab[pos1]=Tab[pos2];
	Tab[pos2]=ptr;
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Shift(size_t idx,bool reverse)
{
	if(!LastPtr)
		throw std::range_error("iRContainer<C>::operator[] const : no elements");
	if(idx>=LastPtr)
        throw std::range_error(R::RString("iRContainer<C>::operator[] const : idx "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]").ToString());

	if(reverse)
	{
		// Divide the container in two parts:
		// [0-idx]: Make (idx+1)/2 exchanges (idx,0), (idx-1,1), etc.
		// [idx+1,LastPtr-1]: Make (LastPtr-idx-1)/2 exchanges (idx+1,LastPtr-1), (idx+2,LastPtr-2), etc.
		size_t LeftExchanges((idx+1)/2),RightExchanges((LastPtr-idx-1)/2);
		for(size_t i=0;i<LeftExchanges;i++)
			Exchange(i,idx-i);
		for(size_t i=0;i<RightExchanges;i++)
			Exchange(idx+i+1,LastPtr-1-i);
	}
	else
	{
		// An intermediate array is create (maximal size is half of the container)
		C** tmp;
		if(idx>LastPtr/2)
		{
			tmp=new C*[LastPtr-idx];
			memcpy(tmp,&Tab[idx],(LastPtr-idx)*sizeof(C*));
			memmove(&Tab[LastPtr-idx],Tab,idx*sizeof(C*));
			memcpy(Tab,tmp,(LastPtr-idx)*sizeof(C*));
		}
		else
		{
			tmp=new C*[idx];
			memcpy(tmp,Tab,idx*sizeof(C*));
			memmove(Tab,&Tab[idx],(LastPtr-idx)*sizeof(C*));
			memcpy(&Tab[LastPtr-idx],tmp,idx*sizeof(C*));
		}
		delete[] tmp;
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Transfer(iRContainer<C>& src)

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
	if((!src.Order)&&Order)
		ReOrder();
}


//-----------------------------------------------------------------------------
template<class C>
	template<class TUse> size_t iRContainer<C>::GetIndex(bool order,const TUse& tag,bool& find,size_t min,size_t max) const
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
					mThrowRException("Problem!");
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
template<class C>
	const C* iRContainer<C>::operator[](size_t idx) const
{
	if(!LastPtr)
		throw std::range_error("iRContainer<C>::operator[] const : no elements");
	if(idx>=LastPtr)
        throw std::range_error(R::RString("iRContainer<C>::operator[] const : idx "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]").ToString());
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
template<class C>
	C* iRContainer<C>::operator[](size_t idx)
{
	if(!LastPtr)
		throw std::range_error("iRContainer<C>::operator[] : no elements");
	if(idx>=LastPtr)
        throw std::range_error(R::RString("iRContainer<C>::operator[] : idx "+RString::Number(idx)+" outside range [0,"+RString::Number(LastPtr-1)+"]").ToString());
	return(Tab[idx]);
}


//-----------------------------------------------------------------------------
template<class C>
	template<class TUse> C* iRContainer<C>::GetPtr(const TUse& tag,bool sortkey,size_t min,size_t max) const
{
	bool Find;
	size_t Index=GetIndex(Order&&sortkey,tag,Find,min,max);
	if(Find)
		return(Tab[Index]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
template<class C> template<class TUse>
	C* iRContainer<C>::GetInsertPtr(const TUse& tag,bool sortkey,size_t min,size_t max)
{
	C* ptr;
	bool Find;
	size_t Index=GetIndex(Order&&sortkey,tag,Find,min,max);
	if(!Find)
		InsertPtrAt(ptr=new C(tag),Index,false);
	else
		ptr=Tab[Index];
	return(ptr);
}


//-----------------------------------------------------------------------------
template<class C> template<class TUse>
	C* iRContainer<C>::GetInsertPtrAt(const TUse& tag,size_t pos)
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
template<class C>
	size_t iRContainer<C>::GetTab(const void** tab,size_t min,size_t max) const
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
template<class C>
	size_t iRContainer<C>::GetTab(void** tab,size_t min,size_t max)
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
template<class C>
	void iRContainer<C>::InsertPtrAt(C* ins,size_t pos,bool del)
{
	C** ptr;

	if(!ins)
		throw std::invalid_argument("iRContainer<C>::InsertPtrAt : Cannot insert a null pointer");
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
			if(Dealloc&&(*ptr))
				delete (*ptr);
		}
		else
		{
			// Moving data "to the right"
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
template<class C>
	void iRContainer<C>::InsertPtr(C* ins,bool del,size_t min,size_t max)
{
	if(!ins)
		throw std::invalid_argument("iRContainer<C>::InsertPtr : Cannot insert a null pointer");

	if(Order)
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
template<class C>
	void iRContainer<C>::DeletePtrAt(size_t pos,bool shift,bool del)
{
	C** ptr;

	if(pos>=LastPtr)
        throw std::range_error(R::RString("iRContainer<C>::DeletePtrAt : idx "+RString::Number(pos)+" outside range [0,"+RString::Number(LastPtr-1)+"]").ToString());
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
template<class C>
	template<class TUse> void iRContainer<C>::DeletePtr(const TUse& tag,bool sortkey,bool del,size_t min,size_t max)
{
	if(!(&tag))
		throw std::invalid_argument("iRContainer<C>::DeletePtr : Cannot delete a null pointer");

	bool Find;
	size_t Index=GetIndex(Order&&sortkey,tag,Find,min,max);
	if(!Find)
		return;
	DeletePtrAt(Index,true,del);
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Inter(const iRContainer<C>& src1,const iRContainer<C>& src2)
{
	Clear();

	if(src1.Order&&src2.Order)
	{
		C** ptr1(src1.Tab);
		int Nb1(src1.LastPtr+1);
		C** ptr2(src2.Tab);
		int Nb2(src2.LastPtr);

		// Parse the elements of each container once
		for(;--Nb1;ptr1++)
		{
			if(!(*ptr1))
				continue;

			while(Nb2&&((!(*ptr2))||((*ptr2)&&((*ptr1)->Compare(**ptr2)>0))))
			{
				Nb2--;
				ptr2++;
			}

			if(Nb2&&((*ptr1)->Compare(**ptr2)==0))
			{
				if(Dealloc)
					InsertPtr(new C(**ptr1));
				else
					InsertPtr(*ptr1);
			}
		}
	}
	else
	{
		C** ptr1(src1.Tab);
		int Nb1(src1.LastPtr+1);

		// Look for each element of src1 if it is in src2
		for(;--Nb1;ptr1++)
		{
			if(!(*ptr1))
				continue;

			if(src2.IsIn(**ptr1))
			{
				if(Dealloc)
					InsertPtr(new C(**ptr1));
				else
					InsertPtr(*ptr1);
			}
		}
	}
}


//-----------------------------------------------------------------------------
template<class C>
	void iRContainer<C>::Union(const iRContainer<C>& src1,const iRContainer<C>& src2)
{
	Clear();

	if(src1.Order&&src2.Order)
	{
		C** ptr1(src1.Tab);
		int Nb1(src1.LastPtr+1);
		C** ptr2(src2.Tab);
		int Nb2(src2.LastPtr);

		// Parse the elements of each container once
		for(;--Nb1;ptr1++)
		{
			if(!(*ptr1))
				continue;

			while(Nb2&&((!(*ptr2))||((*ptr2)&&((*ptr1)->Compare(**ptr2)>0))))
			{
				if(Dealloc)
					InsertPtr(new C(**ptr2));
				else
					InsertPtr(*ptr2);
				Nb2--;
				ptr2++;
			}

			if(Dealloc)
				InsertPtr(new C(**ptr1));
			else
				InsertPtr(*ptr1);


			if(Nb2&&(*ptr2)&&(*ptr1)->Compare(**ptr2)==0)
			{
				Nb2--;
				ptr2++;
			}
		}
		while(Nb2)
		{
			if(Dealloc)
				InsertPtr(new C(**ptr2));
			else
				InsertPtr(*ptr2);
			Nb2--;
			ptr2++;
		}
	}
	else
	{
		C** ptr1(src1.Tab);
		int Nb1(src1.LastPtr+1);

		// Copy one container and add the elements of the second one that are not already in
		NormalCopy(src2);
		for(;--Nb1;ptr1++)
		{
			if(!(*ptr1))
				continue;

			if(!src2.IsIn(**ptr1))
			{
				if(Dealloc)
					InsertPtr(new C(**ptr1));
				else
					InsertPtr(*ptr1);
			}
		}
	}
}


//-----------------------------------------------------------------------------
template<class C>
	iRContainer<C>::~iRContainer(void)
{
	if(Tab)
	{
		if(Dealloc)
		{
			C** ptr;
			for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
				delete(*ptr);
		}
		delete[] Tab;
	}
}
