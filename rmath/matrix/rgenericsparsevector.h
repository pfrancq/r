/*
	R Project Library

	RGenericSparseVector.h

	Generic Sparse Vector containing generic Cells- Header.

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
#ifndef RGenericSparseVector_H
#define RGenericSparseVector_H


//-----------------------------------------------------------------------------
//include file for R Project
#include <rgenericcell.h>
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a container of GenericCell. This elements are
* store in an array of pointers which will be increase when necessary. The
* container can be responsible for the desallocation of the elements
* (bAlloc), and the elements are always ordered. It is also possible to
* indicate an iterator (class T) for the container.
* @author  Vandaele Valery
* @short [short description].
*/
template<class C,bool bAlloc>
	class RGenericSparseVector : public R::RContainer<C,bAlloc,true>
{
private :

	/**
	* The id of the vector.
	*/
	size_t Id;

public :

	/**
	* constructor of RGenericSparseVector
	* @param id                The Id of the vector.
	*/
	RGenericSparseVector(size_t id);

	/**
	* constructor of RGenericSparseVector
	* @param id                The Id of the vector
	* @param nbElem            The initial number of element in the container
	* @param increment         The number of new element each increase
	*/
	RGenericSparseVector(size_t id, size_t nbElem,size_t  increment);

	/**
	* Construct the generic cell from another one.
	* @param src            Generic Sparse Vector used as source.
	*/
	RGenericSparseVector(const RGenericSparseVector<C,bAlloc>& src);

	/**
	* The assignement operator.
	* @param src            Generic Sparse Vector used as source.
	*/
	RGenericSparseVector& operator=(const RGenericSparseVector<C,bAlloc>& src);

	/**
	* Method used by the container
	* param v                  The pointer to the RGenericSparseVector to be compared.
	*/
	int Compare(const RGenericSparseVector<C,bAlloc>* v) const;

	/**
	* Method used by the container
	* param v                  The RGenericSparseVector to be compared.
	*/
	int Compare(const RGenericSparseVector<C,bAlloc>& v) const;

	/**
	* Method used by the container
	* param id                  The id to be compared.
	*/
	int Compare(const size_t id) const;

	/**
	* Get the id of the cell.
	* @return id                Int descibing the id of the cell.
	*/
	size_t GetId(void) const {return Id;}

	/**
	* destructor of RGenericSparseVector
	*/
	virtual ~RGenericSparseVector(void);
};


//---------------------------------------------------------------------------
// inline implementation
#include <rgenericsparsevector.hh>


}  //-------- End of namespace R -----------------------------------------


//-----------------------------------------------------------------------------
#endif
