/*

	R Project Library

	RContainer.hh

	Container - Inline Implementation.

	(C) 1999-2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
template<class C,class T,bool bAlloc,bool bOrder>
	RStd::RContainer<C,T,bAlloc,bOrder>::RContainer(T M,T I) throw(bad_alloc)
		: Current(0), ActPtr(0), NbPtr(0), MaxPtr(M), LastPtr(0), IncPtr(I)
{
	if(MaxPtr)
	{
		Tab = new C*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(C*));
		if(!IncPtr) IncPtr=10;
	}
	else
	{
		MaxPtr=10;
		Tab = new C*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(C*));
		if(!IncPtr) IncPtr=10;
		RReturnIfFail(M>0);
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RStd::RContainer<C,T,bAlloc,bOrder>::RContainer(const RContainer<C,T,bAlloc,bOrder> *container) throw(bad_alloc)
		: Current(0), ActPtr(0), NbPtr(0), MaxPtr(container->MaxPtr),
		  LastPtr(0), IncPtr(container->IncPtr)
{
	T i;
	C **tab;

	if(container)
	{
		Tab = new C*[MaxPtr];
		memset(Tab,0,MaxPtr*sizeof(C*));
		for(i=container->NbPtr+1,tab=container->Tab;--i;tab++)
			InsertPtr(new C(*tab));
	}
	else
	{
		RContainer(10,10);
		RReturnIfFail(container);
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RStd::RContainer<C,T,bAlloc,bOrder>::RContainer(const RContainer<C,T,bAlloc,bOrder> &container) throw(bad_alloc)
		: Current(0), ActPtr(0), NbPtr(0), MaxPtr(0),
		  LastPtr(0), IncPtr(container.IncPtr)
{
	T i;
	C **tab;

	MaxPtr=container.MaxPtr;
	Tab = new C*[MaxPtr];
	memset(Tab,0,MaxPtr*sizeof(C*));
	for(i=container.NbPtr+1,tab=container.Tab;--i;tab++)
		InsertPtr(new C(*tab));
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RStd::RContainer<C,T,bAlloc,bOrder>& RContainer<C,T,bAlloc,bOrder>::
		operator=(const RContainer<C,T,bAlloc,bOrder>& container) throw(bad_alloc)
{
	T i;
	C **tab;

	Clear();
	VerifyTab(container.NbPtr);
	for(i=container.NbPtr+1,tab=container.Tab;--i;tab++)
		InsertPtr(new C(*tab));
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RStd::RContainer<C,T,bAlloc,bOrder>& RContainer<C,T,bAlloc,bOrder>::
		operator+=(const RContainer<C,T,bAlloc,bOrder>& container) throw(bad_alloc)
{
	T i;
	C **tab;

	VerifyTab(container.NbPtr+NbPtr);
	for(i=container.NbPtr+1,tab=container.Tab;--i;tab++)
		InsertPtr(new C(*tab));
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	T RStd::RContainer<C,T,bAlloc,bOrder>::GetId(const TUse &tag,bool &Find)
{
	T NbMin,NbMax,i;
	int Comp;
	bool Cont=true,NotLast=true;
	C *ptr,**ptr2;

	if(bOrder)
	{
		Find=false;
		if(!NbPtr)
		return(0);
		NbMax=NbPtr-1;
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
					Find=true;
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
				Find=true;
				return(0);
			}
		}
		if(Comp<0) i++;
		return(i);
	}
	else
	{
		Find=true;
		for(i=0,ptr2=Tab;i<NbPtr;ptr2++,i++)
			if(!((*ptr2)->Compare(tag))) return(i);
		Find=false;
		return(i);
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::VerifyTab(void) throw(bad_alloc)
{
	if(LastPtr==MaxPtr)
	{
		C **ptr;
		MaxPtr+=IncPtr;
		ptr=new C*[MaxPtr];
		memcpy(ptr,Tab,LastPtr*sizeof(C*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[LastPtr],0,IncPtr*sizeof(C*));
		if(Current&&(ActPtr<LastPtr))
			Current=&Tab[ActPtr];
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::VerifyTab(T MaxSize) throw(bad_alloc)
{
	RReturnIfFail(MaxSize>0);
	if(MaxSize>MaxPtr)
	{
		C **ptr;
		T OldSize;

		OldSize=MaxPtr;
		MaxPtr=MaxSize;
		ptr=new C*[MaxPtr];
		memcpy(ptr,Tab,LastPtr*sizeof(C*));
		delete[] Tab;
		Tab=ptr;
		memset(&Tab[OldSize],0,(MaxPtr-OldSize)*sizeof(C*));
		if(Current&&(ActPtr<LastPtr))
			Current=&Tab[ActPtr];
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::Clear(void)
{
	if(bAlloc)
	{
		C **ptr;

		for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
			if(*ptr)
				delete (*ptr);
	}
	LastPtr=ActPtr=NbPtr=0;
	Current=0;
	memset(Tab,0,MaxPtr*sizeof(C*));
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::Clear(T M,T I)
{
	if(bAlloc)
	{
		C **ptr;

		for(LastPtr++,ptr=Tab;--LastPtr;ptr++)
		{
			if(*ptr)
				delete (*ptr);
		}
	}
	LastPtr=ActPtr=NbPtr=0;
	Current=0;
	IncPtr=I;
	VerifyTab(M);
	memset(Tab,0,MaxPtr*sizeof(C*));
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::InsertPtrOrderAt(C *ins,T Pos) throw(bad_alloc)
{
	C **ptr;

	RReturnIfFail(ins);
	RReturnIfFail(Pos<=LastPtr);
	VerifyTab();
	ptr=&Tab[Pos];
	if(Pos<LastPtr)
		memmove(ptr+1,ptr,(LastPtr-Pos)*sizeof(C*));
	(*ptr)=ins;
	NbPtr++;
	LastPtr++;
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::InsertPtrAt(C *ins,T Pos,bool del) throw(bad_alloc)
{
	C **ptr;

	RReturnIfFail(ins);
	if(!ins) return;
	if(Pos>MaxPtr)
	{
		VerifyTab(Pos+IncPtr);
	}
	else
	{
		VerifyTab();
	}
	ptr=&Tab[Pos];
	if(Pos<LastPtr)
	{
		if(del)
		{
			if(*ptr)
				delete(*ptr);
		}
		else
		{
			memmove(ptr+1,ptr,(LastPtr-Pos)*sizeof(C*));
		}
	}
	(*ptr)=ins;
	NbPtr++;
	if(Pos+1>LastPtr)
	{
		LastPtr=Pos+1;
	}
	else
	{
		if(!del)
			LastPtr++;
	}
	
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::InsertPtr(C* ins) throw(bad_alloc)
{
	RReturnIfFail(ins);
	if(!ins) return;
	if(bOrder)
	{
		bool Find;
		T Index=GetId<C*>(ins,Find);
		if(Find)
		{
			if(bAlloc)
			{
				delete(ins);
				ins=NULL;
			}
		}
		else
			InsertPtrOrderAt(ins,Index);
	}
	else
	{
		VerifyTab();
		Tab[LastPtr++]=ins;
		NbPtr++;
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	bool RStd::RContainer<C,T,bAlloc,bOrder>::IsIn(const TUse &tag,bool sortkey)
{
	if(bOrder&&sortkey)
	{
		bool Find;
		GetId<TUse>(tag,Find);
		return(Find);
	}
	else
	{
		T i;
		C **ptr;

		for(i=LastPtr+1,ptr=Tab;--i;ptr++)
			if((*ptr)&&(!((*ptr)->Compare(tag))))
				return(true);
		return(false);
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	C* RStd::RContainer<C,T,bAlloc,bOrder>::GetPtr(const TUse &tag,bool sortkey)
{
	if(bOrder&&sortkey)
	{
		bool Find;
		T Index=GetId<TUse>(tag,Find);
		if(Find)
			return(Tab[Index]);
		else
			return(0);
	}
	else
	{
		T i;
		C **ptr;

		for(i=LastPtr+1,ptr=Tab;--i;ptr++)
			if((*ptr)&&(!((*ptr)->Compare(tag))))
				return(*ptr);
		return(0);
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	C* RStd::RContainer<C,T,bAlloc,bOrder>::GetInsertPtr(const TUse &tag,bool sortkey) throw(bad_alloc)
{
	if(bOrder&&sortkey)
	{
		bool Find;
		T Index=GetId<TUse>(tag,Find);
		if(!Find)
			InsertPtrOrderAt(new C(tag),Index);
		return(Tab[Index]);
	}
	else
	{
		T i;
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


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
  template<class TUse> RStd::RContainer<C,T,false,bOrder>* RContainer<C,T,bAlloc,bOrder>::
		GetPtrs(const TUse &tag) throw(bad_alloc)
{
	C **ptr;
	T i;
	RContainer<C,T,false,bOrder> *tmp;

	tmp=new RContainer<C,T,false,bOrder>(MaxPtr,IncPtr);
	for(i=LastPtr+1,ptr=Tab;--i;ptr++)
		if((*ptr)&&(!((*ptr)->Compare(tag))))
			tmp->InsertPtr(*ptr);
	return(tmp);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	void RStd::RContainer<C,T,bAlloc,bOrder>::DeletePtr(C* del) throw(bad_alloc)
{
	C **ptr;
	T Index;

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
	memcpy(ptr,ptr+1,((--LastPtr)-Index)*sizeof(C*));
	Tab[LastPtr]=0;
	NbPtr--;
	if(bAlloc) delete(del);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder> template<class TUse>
	void RStd::RContainer<C,T,bAlloc,bOrder>::DeletePtr(const TUse &tag,bool sortkey) throw(bad_alloc)
{
	C **ptr,*del;
	T Index;

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
	memcpy(ptr,ptr+1,((--LastPtr)-Index)*sizeof(C*));
	Tab[LastPtr]=0;
	NbPtr--;
	if(bAlloc)
		delete(del);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	inline void RStd::RContainer<C,T,bAlloc,bOrder>::Start(void)
{
	if(!NbPtr)
	{
		Current=0;
		return;
	}
	Current=Tab;
	ActPtr=0;
	while(!(*Current))
		Next();
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	inline bool RStd::RContainer<C,T,bAlloc,bOrder>::End(void) const
{
	return(ActPtr==LastPtr);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	inline void RStd::RContainer<C,T,bAlloc,bOrder>::Next(void)
{
	if(!NbPtr) return;
	if(ActPtr==LastPtr)
		Start();
	else
	{
		ActPtr++;
		Current++;
		while((!(*Current))&&(ActPtr<LastPtr))
		{
			ActPtr++;
			Current++;
		}
	}
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	inline C* RStd::RContainer<C,T,bAlloc,bOrder>::operator()(void) const
{
	return(*Current);
}


//-----------------------------------------------------------------------------
template<class C,class T,bool bAlloc,bool bOrder>
	RContainer<C,T,bAlloc,bOrder>::~RContainer(void)
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
	delete[] Tab;
}
