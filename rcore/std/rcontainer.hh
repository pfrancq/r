/*

	R Project Library

	RContainer.hh

	Container - Inline Implementation.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
//
// class RContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<bool b>
	void RContainer<C,bAlloc,bOrder>::Create(const RContainer<C,b,bOrder>& src)
{
	unsigned int i;
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
	Tab = new void*[MaxPtr];
	memset(Tab,0,MaxPtr*sizeof(void*));
	if(bAlloc&&b)
	{
		for(i=src.LastPtr+1,tab=reinterpret_cast<C**>(src.Tab),tab2=reinterpret_cast<C**>(Tab);--i;tab++,tab2++)
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
	unsigned int i;
	C** tab;
	C** tab2;

	Clear();
	if(!src.NbPtr)
		return(*this);
	VerifyTab(src.LastPtr);
	if(bAlloc&&b)
	{
		for(i=src.LastPtr+1,tab=reinterpret_cast<C**>(src.Tab),tab2=reinterpret_cast<C**>(Tab);--i;tab++,tab2++)
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
	unsigned int i;
	C** tab;
	C** tab2;

	Clear();
	if(!src.NbPtr)
		return;
	VerifyTab(src.LastPtr);
	for(i=src.LastPtr+1,tab=reinterpret_cast<C**>(src.Tab),tab2=reinterpret_cast<C**>(Tab);--i;tab++,tab2++)
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
	unsigned int i;
	C** tab;
	C** tab2;

	if(!src.NbPtr)
		return(*this);
	VerifyTab(src.LastPtr+LastPtr);
	if(bOrder&&NbPtr)
	{
		for(i=src.LastPtr+1,tab=reinterpret_cast<C**>(src.Tab);--i;tab++)
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
		for(i=src.LastPtr+1,tab=reinterpret_cast<C**>(src.Tab),tab2=reinterpret_cast<C**>(&Tab[LastPtr]);--i;tab++)
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
		: BasicContainer(m,i)
{
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,true,bOrder>& src)
	: BasicContainer()
{
	Create<true>(src);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,false,bOrder>& src)
		: BasicContainer()
{
	Create<false>(src);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> template<class TUse>
	C* RContainer<C,bAlloc,bOrder>::GetInsertPtr(const TUse& tag,bool sortkey,size_t min, size_t max)
{
	C* ptr;
	bool Find;
	size_t Index=BasicContainer::GetIndex(bOrder&&sortkey,static_cast<const void*>(&tag),Find,min,max,Compare<TUse>);
	if(!Find)
		BasicContainer::InsertPtrAt(bAlloc,ptr=new C(tag),Index,false);
	else
		ptr=static_cast<C*>(Tab[Index]);
	return(ptr);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
  template<class TUse> RContainer<C,false,bOrder>*
    RContainer<C,bAlloc,bOrder>::GetPtrs(const TUse& tag,size_t min, size_t max) const
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
	RContainer<C,bAlloc,bOrder>::~RContainer(void)
{
	if(bAlloc)
	{
		C** ptr;
		for(LastPtr++,ptr=reinterpret_cast<C**>(Tab);--LastPtr;ptr++)
			delete(*ptr);
	}
}
