/*
	R Project Library

	RGenericVectorCursor.h

	Cursor for RGenericSparseMatrix  - Header.

	Copyright 2003 by the Université Libre de Bruxelles.

	Authors
		 Vandaele Valery(vavdaele@ulb.ac.be)

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
#ifndef RGenericVectorCursor_H
#define RGenericVectorCursor_H

//-----------------------------------------------------------------------------
// include files for R Project
#include <rmath/rgenericsparsevector.h>
#include <rmath/rgenericsparsematrix.h>
#include <rstd/rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* @param C                  The class of the elements that are contained.
*
* This class represent a cursor to iterate a RGenericSparseMatrix. The parameters are
* those of the container.
*
* @author  Vandaele Valery
* @short Cursor for Generic Cell Elements.
*/
template<class C>
class RGenericVectorCursor : public R::RCursor<RGenericSparseVector<RGenericCell<C>,true>,unsigned int>
{
public :
	/**
	* Construct the cursor.
	*/
	RGenericVectorCursor(void);

	/**
	* Construct the cursor.
	* param v               The GenericSparseMatrix to iterate.
	*/
	RGenericVectorCursor(RGenericSparseMatrix<RGenericCell<C>,true>& v);

	/**
	* Construct the cursor.
	* param v               The GenericSparseMatrix  to iterate.
	*/
	RGenericVectorCursor(RGenericSparseMatrix<RGenericCell<C>,true>* v);

	/**
	* Construct the cursor.
	* param v               The GenericSparseMatrix  to iterate.
	*/
	RGenericVectorCursor(RGenericSparseMatrix<RGenericCell<C>,false>& v);

	/**
	* Construct the cursor.
	* param v               The GenericSparseMatrix to iterate.
	*/
	RGenericVectorCursor(RGenericSparseMatrix<RGenericCell<C>,false>* v);

	/**
	* Assignment operator using a "GenericCellCursor".
	*/
	RGenericVectorCursor<C>& operator=(const RGenericVectorCursor<C>& c) throw(std::bad_alloc);

	/**
	* Set the container.
	* param v               GenericSparseMatrix to iterate.
	*/
	void Set(RGenericSparseMatrix<RGenericCell<C>,true>& v);

	/**
	* Set the container.
	* param v               GenericSparseMatrix  to iterate.
	*/
	void Set(RGenericSparseMatrix<RGenericCell<C>,true>* v);

	/**
	* Set the container.
	* param v               GenericSparseMatrix to iterate.
	*/
	void Set(RGenericSparseMatrix<RGenericCell<C>,false>& v);

	/**
	* Set the container.
	* param v               GenericSparseMatrix to iterate.
	*/
	void Set(RGenericSparseMatrix<RGenericCell<C>,false>* v);

};


#include <rmath/rgenericvectorcursor.hh> // implementation


}  //-------- End of namespace R --------------------------------

//-----------------------------------------------------------------------------
/**
* The CLASSGENERICCELLCURSOR macro defines a way to create a cursor for a given class with
* a name.
* @param name            Name of the class representing the cursor.
* @param C                  The class of the elements that are contained.
*/
#define CLASSGENERICVECTORCURSOR(name,C)                                        \
class name : public R::RGenericVectorCursor<C>                              \
{                                                                    \
public:                                                              \
	name(void) : R::RGenericVectorCursor<C>() {}                             \
	static name* GetTmpCursor(void)                                  \
	{                                                                \
		return(R::GetTemporaryObject<name,20>());                 \
	}                                                                \
	name& operator=(const name& c) throw(std::bad_alloc)                  \
	{                                                                \
		R::RGenericVectorCursor<C>::operator=(c);                            \
		return(*this);                                               \
	}                                                                \
	name& operator=(const R::RGenericVectorCursor<C>& c) throw(std::bad_alloc)    \
	{                                                                \
		R::RGenericVectorCursor<C>::operator=(c);                            \
		return(*this);                                               \
	}                                                                \
};


//-----------------------------------------------------------------------------
#endif
