/*

	R Project Library

	RContainer.hh

	Container - Inline Implementation.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
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
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(size_t m,size_t i)
		: RIContainer<C>(bAlloc,bOrder,m,i)
{
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,true,bOrder>& src)
	: RIContainer<C>(bAlloc,bOrder,0,10)
{
	RIContainer<C>::Create(src);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder>
	RContainer<C,bAlloc,bOrder>::RContainer(const RContainer<C,false,bOrder>& src)
		: RIContainer<C>(bAlloc,bOrder,0,10)
{
	RIContainer<C>::Create(src);
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
	RContainer<C,bAlloc,bOrder>::~RContainer(void)
{
}
