/*
	R Project Library

	RGenericVectorCursor.h

	Cursor for RGenericSparseMatrix - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

	Authors
		 Pascal Francq (pfrancq@ulb.ac.be)
		 Vandaele Valery(vavdaele@ulb.ac.be)

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
// RGenericVectorCursor
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C>
	RGenericVectorCursor<C>::RGenericVectorCursor(void)
		:  R::RCursor< RGenericSparseVector<RGenericCell<C>,true> >()
{
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericVectorCursor<C>::RGenericVectorCursor(RGenericSparseMatrix<RGenericCell<C>,true>& v)
		: R::RCursor< RGenericSparseVector<RGenericCell<C>,true> >(v)
{
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericVectorCursor<C>::RGenericVectorCursor(RGenericSparseMatrix<RGenericCell<C>,false>& v)
		: R::RCursor< RGenericSparseVector<RGenericCell<C>,true> >(v)
{
}


//-----------------------------------------------------------------------------
template<class C>
	RGenericVectorCursor<C>& RGenericVectorCursor<C>::operator=(const RGenericVectorCursor<C>& c)
{
	R::RCursor< RGenericSparseVector<RGenericCell<C>,true> >::operator=(c);
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C>
	void RGenericVectorCursor<C>::Set(RGenericSparseMatrix<RGenericCell<C>,true>& c)
{
	R::RCursor< RGenericSparseVector<RGenericCell<C>,true> >::Set(c);
}


//-----------------------------------------------------------------------------
template<class C>
	void RGenericVectorCursor<C>::Set(RGenericSparseMatrix<RGenericCell<C>,false>& c)
{
	R::RCursor< RGenericSparseVector<RGenericCell<C>,true> >::Set(c);
}
