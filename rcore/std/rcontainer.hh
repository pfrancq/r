/*

	R Project Library

	RContainer.hh

	Container - Inline Implementation.

	Copyright 1999-2005 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
//
// class RContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(size_t m,size_t i)
		: NbPtr(0), MaxPtr(m), LastPtr(0), IncPtr(i)
{
	if(MaxPtr)
	{
		Tab = new C*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(C*));
		if(!IncPtr) IncPtr=MaxPtr/2;
		if(!IncPtr) IncPtr=10;
	}
	else
	{
		MaxPtr=10;
		Tab = new C*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(C*));
		if(!IncPtr) IncPtr=10;
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,true,bOrder>& src)
		: NbPtr(0), MaxPtr(0), LastPtr(0), IncPtr(0)
{
	Create<true>(src);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,false,bOrder>& src)
		: NbPtr(0), MaxPtr(0), LastPtr(0), IncPtr(0)
{
	Create<false>(src);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::
		operator=(const RContainer<C,true,bOrder>& src)
{
	return(NormalCopy<true>(src));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::
		operator=(const RContainer<C,false,bOrder>& src)
{
	return(NormalCopy<false>(src));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Copy(const RContainer<C,true,bOrder>& src)
{
	DeepCopy<true>(src);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Copy(const RContainer<C,false,bOrder>& src)
{
	DeepCopy<false>(src);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::
		operator+=(const RContainer<C,true,true>& src)
{
	return(Add<true,true>(src));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::
		operator+=(const RContainer<C,true,false>& src)
{
	return(Add<true,false>(src));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::
		operator+=(const RContainer<C,false,true>& src)
{
	return(Add<false,true>(src));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>& RContainer<C,bAlloc,bOrder>::
		operator+=(const RContainer<C,false,false>& src)
{
	return(Add<false,false>(src));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::VerifyTab(void)
{
	if(LastPtr==MaxPtr)
	{
		size_t OldSize;

		OldSize=MaxPtr;
		C **ptr;
		MaxPtr+=IncPtr;
		ptr=new C*[MaxPtr];
		memcpy(ptr,Tab,OldSize*sizeof(C*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[OldSize],0,IncPtr*sizeof(C*));
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::VerifyTab(size_t max)
{
	RReturnIfFail(max>0);
	if(max>MaxPtr)
	{
		C **ptr;
		size_t OldSize;

		OldSize=MaxPtr;
		MaxPtr=max;
		ptr=new C*[MaxPtr];
		memcpy(ptr,Tab,OldSize*sizeof(C*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[OldSize],0,(MaxPtr-OldSize)*sizeof(C*));
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Clear(void)
{
	if(bAlloc)
	{
		C **ptr;

		for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
			if(*ptr)
				delete (*ptr);
	}
	LastPtr=NbPtr=0;
	memset(Tab,0,MaxPtr*sizeof(C*));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Clear(size_t m,size_t i)
{
	Clear();
	IncPtr=i;
	MaxPtr=m;
	if(!IncPtr) IncPtr=MaxPtr/2;
	if(!IncPtr) IncPtr=10;
	VerifyTab(m);
	memset(Tab,0,MaxPtr*sizeof(C*));
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::ReOrder(int sortOrder(const void*,const void*))
{
	if(NbPtr)
		qsort(static_cast<void*>(Tab),LastPtr,sizeof(C*),sortOrder);
}



//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::InsertPtrOrderAt(const C* ins,size_t pos)
{
	C **ptr;

	RReturnIfFail(ins);
	RReturnIfFail(pos<=LastPtr);
	if(pos>MaxPtr)
	{
		VerifyTab(pos+IncPtr);
	}
	else
	{
		VerifyTab();
	}
	ptr=&Tab[pos];
	if(pos<LastPtr)
		memmove(ptr+1,ptr,(LastPtr-pos)*sizeof(C*));
	(*ptr)=(C*)ins;
	NbPtr++;
	LastPtr++;
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::InsertPtrAt(const C* ins,size_t pos,bool del)
{
	C **ptr;

	RReturnIfFail(ins);
	if(!ins) return;
	if(pos+1>MaxPtr)
	{
		VerifyTab(pos+1+IncPtr);
	}
	else
	{
		VerifyTab();
	}
	ptr=&Tab[pos];
	if(pos<LastPtr)
	{
		if(del)
		{
			if((*ptr)&&(bAlloc))
				delete(*ptr);
		}
		else
		{
			memmove(ptr+1,ptr,(LastPtr-pos)*sizeof(C*));
		}
	}
	(*ptr)=(C*)ins;
	NbPtr++;
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


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::InsertPtr(const C* ins)
{
	RReturnIfFail(ins);
	if(!ins) return;
	if(bOrder)
	{
		bool Find;
		size_t Index=GetId<const C*>(ins,Find);
		if(Find)
		{
			if(bAlloc)
				delete(Tab[Index]);
			Tab[Index]=(C*)ins;
		}
		else
			InsertPtrOrderAt(ins,Index);
	}
	else
	{
		VerifyTab();
		Tab[LastPtr++]=(C*)ins;
		NbPtr++;
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::Exchange(size_t pos1,size_t pos2)
{
	if((pos1>LastPtr)||(pos2>LastPtr))
		return;
	C* ptr=Tab[pos1];
	Tab[pos1]=Tab[pos2];
	Tab[pos2]=ptr;
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	size_t RContainer<C,bAlloc,bOrder>::GetId(const TUse& tag,bool& find) const
{
	size_t NbMin,NbMax,i=0;
	int Comp=0;
	bool Cont=true,NotLast=true;
	C *ptr,**ptr2;

	if(bOrder)
	{
		find=false;
		if(!LastPtr)
			return(0);
		NbMax=LastPtr-1;
		NbMin=0;
		if(NbMax)
		{
			while(Cont)
			{
				i=(NbMax+NbMin)/2;
				ptr=Tab[i];
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
			Comp= (*Tab)->Compare(tag);
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
		for(i=0,ptr2=Tab;i<LastPtr;ptr2++,i++)
			if((*ptr2)&&(!((*ptr2)->Compare(tag)))) return(i);
		find=false;
		return(i);
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	bool RContainer<C,bAlloc,bOrder>::IsIn(const TUse& tag,bool sortkey) const
{
	if(bOrder&&sortkey)
	{
		bool Find;
		GetId<TUse>(tag,Find);
		return(Find);
	}
	else
	{
		size_t i;
		C **ptr;

		for(i=LastPtr+1,ptr=Tab;--i;ptr++)
			if((*ptr)&&(!((*ptr)->Compare(tag))))
				return(true);
		return(false);
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	const C* RContainer<C,bAlloc,bOrder>::operator[](size_t idx) const
{
	if(idx>=LastPtr)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RContainer::operator[] const : index outside container");
	#endif
	return(Tab[idx]);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	C* RContainer<C,bAlloc,bOrder>::operator[](size_t idx)
{
	if(idx>=LastPtr)
	#ifdef __GNUC__
		throw std::range_error(__PRETTY_FUNCTION__);
	#else
		throw std::range_error("RContainer::operator[] const : index outside container");
	#endif
	return(Tab[idx]);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,bAlloc,bOrder>::GetPtr(const TUse& tag,bool sortkey) const
{
	if(bOrder&&sortkey)
	{
		bool Find;
		size_t Index=GetId<TUse>(tag,Find);
		if(Find)
			return(Tab[Index]);
		else
			return(0);
	}
	else
	{
		size_t i;
		C **ptr;

		for(i=LastPtr+1,ptr=Tab;--i;ptr++)
			if((*ptr)&&(!((*ptr)->Compare(tag))))
				return(*ptr);
		return(0);
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,bAlloc,bOrder>::GetInsertPtr(const TUse& tag,bool sortkey)
{
	if(bOrder&&sortkey)
	{
		bool Find;
		size_t Index=GetId<TUse>(tag,Find);
		if(!Find)
			InsertPtrOrderAt(new C(tag),Index);
		return(Tab[Index]);
	}
	else
	{
		size_t i;
		C **ptr;

		for(i=LastPtr+1,ptr=Tab;--i;ptr++)
			if((*ptr)&&(!((*ptr)->Compare(tag))))
				return(*ptr);
		VerifyTab();
		(*ptr)=new C(tag);
		InsertPtr(*ptr);
		return(*ptr);
	}
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
  template<class TUse> RContainer<C,false,bOrder>*
    RContainer<C,bAlloc,bOrder>::GetPtrs(const TUse& tag) const
{
	C **ptr;
	size_t i;
	RContainer<C,false,bOrder> *tmp;

	tmp=new RContainer<C,false,bOrder>(MaxPtr,IncPtr);
	for(i=LastPtr+1,ptr=Tab;--i;ptr++)
		if((*ptr)&&(!((*ptr)->Compare(tag))))
			tmp->InsertPtr(*ptr);
	return(tmp);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	size_t RContainer<C,bAlloc,bOrder>::GetTab(C** tab)
{
	memcpy(tab,Tab,LastPtr*sizeof(C*));
	return(LastPtr);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	size_t RContainer<C,bAlloc,bOrder>::GetTab(const C** tab) const
{
	memcpy(tab,Tab,LastPtr*sizeof(C*));
	return(LastPtr);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	void RContainer<C,bAlloc,bOrder>::DeletePtr(C*& del)
{
	C **ptr;
	size_t Index;

	RReturnIfFail(del);
	if(!del) return;
	if(bOrder)
	{
		bool Find;
		Index=GetId<C*>(del,Find);
		if(!Find) return;
		ptr=&Tab[Index];
	}
	else
	{
		for(Index=0,ptr=Tab;((*ptr)!=del)&&(Index<LastPtr);Index++,ptr++);
		if(Index==LastPtr) return;
	}
	memmove(ptr,ptr+1,((--LastPtr)-Index)*sizeof(C*));
	Tab[LastPtr]=0;
	NbPtr--;
	if(bAlloc) delete(del);
}


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	void RContainer<C,bAlloc,bOrder>::DeletePtr(const TUse& tag,bool sortkey)
{
	C **ptr,*del;
	size_t Index;

	if(bOrder&&sortkey)
	{
		bool Find;
		Index=GetId<TUse>(tag,Find);
		if(!Find)
			return;
		ptr=&Tab[Index];
	}
	else
	{
		for(Index=0,ptr=Tab;Index<LastPtr;Index++,ptr++)
		{
			if((*ptr)&&(!((*ptr)->Compare(tag))))
				break;
		}
		if(Index==LastPtr)
			return;
	}
	del=*ptr;
	memmove(ptr,ptr+1,((--LastPtr)-Index)*sizeof(C*));
	Tab[LastPtr]=0;
	NbPtr--;
	if(bAlloc)
		delete(del);
}


//------------------------------------------------------------------------------
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
		NbPtr=src.NbPtr;
		LastPtr=src.LastPtr;
	}
}


//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::~RContainer(void)
{
	if(bAlloc)
	{
		C** ptr(Tab);
		for(LastPtr++;--LastPtr;ptr++)
		{
			delete(*ptr);
		}
	}
	delete[] Tab;
}
