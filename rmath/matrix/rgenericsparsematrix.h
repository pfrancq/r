/*
	R Project Library

	RGenericSparseMatrix.h

	Generic Sparse Matrix containing generic Sparse vector - Header.

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
#ifndef RGenericSparseMatrix_H
#define RGenericSparseMatrix_H


//-----------------------------------------------------------------------------
//include file for R Project
#include <rgenericcell.h>
#include <rgenericsparsevector.h>
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a container of GenericSparseVector This elements are
* store in an array of pointers which will be increase when necessary. The
* container can be responsible for the desallocation of the elements
* (bAlloc), and the elements are always ordered. It is also possible to
* indicate an iterator (class T) for the container.
* @author  Vandaele Valery
* @short Generic Sparse Matrix.
*/
template<class C,bool bAlloc>
	class RGenericSparseMatrix  :  public R::RContainer<RGenericSparseVector<C,bAlloc>, bAlloc,true>
{
public :

	/**
	* Constructor of RGenericSparseMatrix
	*/
	RGenericSparseMatrix(void);

	/**
	* Constructor of RGenericSparseMatrix
	* @param nbElem            The initial number of element in the container
	* @param increment         The number of new element each increase
	*/
	RGenericSparseMatrix(size_t nbElem,size_t increment);

	/**
	* Construct the generic cell from another one.
	* @param src            Generic Sparse Matrix used as source.
	*/
	RGenericSparseMatrix(const RGenericSparseMatrix<C,bAlloc>* src);

	/**
	* Construct the generic cell from another one.
	* @param src            Generic Sparse Matrix used as source.
	*/
	RGenericSparseMatrix(const RGenericSparseMatrix<C,bAlloc>& src);

	/**
	* The assignement operator.
	* @param src            Generic Sparse Matrix used as source.
	*/
	RGenericSparseMatrix& operator=(const RGenericSparseMatrix<C,bAlloc>& src);

	/**
	* destructor of RGenericSparseMatrix
	*/
	virtual ~RGenericSparseMatrix(void);
};


#include <rgenericsparsematrix.hh>     // Implementation


}  //-------- End of namespace R -----------------------------------------


//-----------------------------------------------------------------------------
#endif
