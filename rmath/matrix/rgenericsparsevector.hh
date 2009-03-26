/*
	R Project Library

	RGenericSparseVector.cpp

	[Description of the class] - Implementation.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by Vandaele Valery.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
// RGenericSparseVector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseVector<C,bAlloc>::RGenericSparseVector(size_t id)
		: RContainer<C,bAlloc,true>(3,2) , Id(id)
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseVector<C,bAlloc>::RGenericSparseVector(size_t id,size_t nbElem, size_t increase)
		: RContainer<C,bAlloc,true>(nbElem,increase) , Id(id)
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseVector<C,bAlloc>::RGenericSparseVector(const RGenericSparseVector<C,bAlloc>& src)
		: RContainer<C,bAlloc,true>::RContainer(src),Id(src.GetId())
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseVector<C,bAlloc>& RGenericSparseVector<C,bAlloc>::operator=(const RGenericSparseVector<C,bAlloc>& src)
{
	RContainer<C,bAlloc,true>::operator=(src);
	Id= src.GetId();
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	int RGenericSparseVector< C,bAlloc>::Compare(const RGenericSparseVector<C,bAlloc>* v) const
{
	return (Id - v->Id);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	int RGenericSparseVector<C,bAlloc>::Compare(const RGenericSparseVector<C,bAlloc>& v) const
{
	return (Id-v.Id);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	int RGenericSparseVector<C,bAlloc>::Compare(const size_t id) const
{
	return (Id-id );
}

//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	 RGenericSparseVector<C,bAlloc>::~RGenericSparseVector(void)
{
}


