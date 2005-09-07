/*
	R Project Library

	RGenericSparseMatrix.cpp

	Generic Sparse Matrix containing generic Sparse vector - Implementation.

	Copyright 2003-2005 by the Université Libre de Bruxelles.

	Authors
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
// RGenericSparseMatrix
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseMatrix<C,bAlloc>::RGenericSparseMatrix(void)
		: RContainer<RGenericSparseVector<C,bAlloc>, bAlloc,true>(3,2)
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseMatrix<C,bAlloc>::RGenericSparseMatrix(unsigned int nbElem, unsigned int increase)
		: RContainer<RGenericSparseVector<C,bAlloc>, bAlloc,true>(nbElem,increase)
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseMatrix<C,bAlloc>::RGenericSparseMatrix(const RGenericSparseMatrix<C,bAlloc>* src)
		: RContainer<RGenericSparseVector<C,bAlloc>, bAlloc,true>::RContainer(src)
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseMatrix<C,bAlloc>::RGenericSparseMatrix(const RGenericSparseMatrix<C,bAlloc>& src)
		: RContainer<RGenericSparseVector<C,bAlloc>, bAlloc,true>::RContainer(src)
{}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseMatrix<C,bAlloc>& RGenericSparseMatrix<C,bAlloc>::operator=(const RGenericSparseMatrix<C,bAlloc>& src)
{
	RContainer<RGenericSparseVector<C,bAlloc>,bAlloc,true>::operator=(src);
	return(*this);
}


//-----------------------------------------------------------------------------
template<class C,bool bAlloc>
	RGenericSparseMatrix<C,bAlloc>::~RGenericSparseMatrix(void)
{
}
