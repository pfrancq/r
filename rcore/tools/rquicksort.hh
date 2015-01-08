/*

	R Project Library

	RQuickSort.hh

	Container - Inline Implementation.

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
// class RQuickSort
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C>
	RQuickSort<C>::RQuickSort(C** tab,size_t max)
		: Tab(tab), Max(max)
{
}


//-----------------------------------------------------------------------------
template<class C>
	RQuickSort<C>::RQuickSort(iRContainer<C>& cont)
		: Tab(reinterpret_cast<C**>(cont.Tab)), Max(cont.NbPtr)
{
}


//-----------------------------------------------------------------------------
template<class C>
	int RQuickSort<C>::Compare(C* obj1,C* obj2)
{
	return(obj1->Compare(*obj2));
}


//-----------------------------------------------------------------------------
template<class C>
	size_t RQuickSort<C>::Partition(size_t left,size_t  right,size_t  pivotIndex)
{
	C** pivotPtr(&Tab[pivotIndex]);
	C** rightPtr(&Tab[right]);
	C* pivot(*pivotPtr);
	(*pivotPtr)=(*rightPtr);
	(*rightPtr)=pivot;
	size_t storeIndex(left);
	C** iPtr(&Tab[left]);
	C** Store(iPtr);
	for(size_t i=left;i<right;i++,iPtr++)
	{
		if(Compare(pivot,*iPtr)>0)
		{
			C* tmp(*iPtr);
			storeIndex++;
			(*iPtr)=(*Store);
			(*Store)=tmp;
			Store++;
		}
	}
	C* tmp(*rightPtr);
	(*rightPtr)=(*Store);
	(*Store)=tmp;
	return(storeIndex);
}


//-----------------------------------------------------------------------------
template<class C>
	void RQuickSort<C>::Quicksort(size_t left, size_t right)
{
	if(right<=left) return;
	size_t pivotIndex(left);
	size_t pivotNewIndex(Partition(left,right,pivotIndex));
	if(pivotNewIndex)
		Quicksort(left, pivotNewIndex - 1);
	Quicksort(pivotNewIndex + 1, right);
}


//-----------------------------------------------------------------------------
template<class C>
	RQuickSort<C>::~RQuickSort(void)
{
}
