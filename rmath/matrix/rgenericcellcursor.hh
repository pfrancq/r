/*
	R Project Library

	RGenericCellCursor.cpp

	Cursor for RGenericSparseVector - Implementation.

	Copyright 2003 by the Universit� Libre de Bruxelles.

	Authors
		 Vandaele Valery(name@email.be)

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
// RGenericCursor
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C>
	RGenericCellCursor<C>::RGenericCellCursor(void)
		:  R::RCursor<RGenericCell<C>,unsigned int>()
{
}



//-----------------------------------------------------------------------------
template<class C>
	RGenericCellCursor<C>::RGenericCellCursor(RGenericSparseVector<RGenericCell<C>,true>& v)
		: R::RCursor<RGenericCell<C>,unsigned int>(v)
{
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCellCursor<C>::RGenericCellCursor(RGenericSparseVector<RGenericCell<C>,true>* v)
		: R::RCursor<RGenericCell<C>,unsigned int>(v)
{
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCellCursor<C>::RGenericCellCursor(RGenericSparseVector<RGenericCell<C>,false>& v)
		: R::RCursor<RGenericCell<C>,unsigned int>(v)
{
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericCellCursor<C>::RGenericCellCursor(RGenericSparseVector<RGenericCell<C>,false>* v)
		: R::RCursor<RGenericCell<C>,unsigned int>(v)
{
}

//-----------------------------------------------------------------------------
template<class C>
	RGenericCellCursor<C>& RGenericCellCursor<C>::operator=(const RGenericCellCursor<C>& c) throw(std::bad_alloc)
{
	R::RCursor<RGenericCell<C>,unsigned int>::operator=(c);
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C>
	void RGenericCellCursor<C>::Set(RGenericSparseVector<RGenericCell<C>,true>* c)
{
	R::RCursor<RGenericCell<C>,unsigned int>::Set(c);
}


//-----------------------------------------------------------------------------
template<class C>
	void RGenericCellCursor<C>::Set(RGenericSparseVector<RGenericCell<C>,true>& c)
{
	R::RCursor<RGenericCell<C>,unsigned int>::Set(c);
}


//-----------------------------------------------------------------------------
template<class C>
	void RGenericCellCursor<C>::Set(RGenericSparseVector<RGenericCell<C>,false>* c)
{
	R::RCursor<RGenericCell<C>,unsigned int>::Set(c);
}


//-----------------------------------------------------------------------------
template<class C>
	void RGenericCellCursor<C>::Set(RGenericSparseVector<RGenericCell<C>,false>& c)
{
	R::RCursor<RGenericCell<C>,unsigned int>::Set(c);
}

