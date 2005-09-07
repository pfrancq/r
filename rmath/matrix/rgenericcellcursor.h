/*
	R Project Library

	RGenericCellCursor.h

	Cursor for RGenericSparseVector - Header.

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
#ifndef RGenericCellCursor_H
#define RGenericCellCursor_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgenericcell.h>
#include <rgenericsparsevector.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* @param C                  The class of the elements that are contained.
*
* This class represent a cursor to iterate a RGenericSparseVector. The parameters are
* those of the container.
*
* @author  Vandaele Valery
* @short Cursor for Generic Cell Elements.
*/
template<class C>
	class RGenericCellCursor : public R::RCursor< RGenericCell<C> >
{

public :
	/**
	* Construct the cursor.
	*/
	RGenericCellCursor(void);

	/**
	* Construct the cursor.
	* param v               The GenericSparseVector to iterate.
	*/
	RGenericCellCursor(RGenericSparseVector<RGenericCell<C>,true>& v);

	/**
	* Construct the cursor.
	* param v               The GenericSparseVector  to iterate.
	*/
	RGenericCellCursor(RGenericSparseVector<RGenericCell<C>,false>& v);

	/**
	* Assignment operator using a "GenericCellCursor".
	*/
	RGenericCellCursor<C>& operator=(const RGenericCellCursor<C>& c);

	/**
	* Set the container.
	* param v               GenericSparseVector to iterate.
	*/
	void Set(RGenericSparseVector<RGenericCell<C>,true>& v);

	/**
	* Set the container.
	* param v               GenericSparseVector to iterate.
	*/
	void Set(RGenericSparseVector<RGenericCell<C>,false>& v);
};


#include <rgenericcellcursor.hh> // implementation


}  //-------- End of namespace RMath --------------------------------


//-----------------------------------------------------------------------------
/**
* The CLASSGENERICCELLCURSOR macro defines a way to create a cursor for a given
* class with a name.
* @param name            Name of the class representing the cursor.
* @param C                  The class of the elements that are contained.
*/
#define CLASSGENERICCELLCURSOR(name,C)                                          \
class name : public R::RGenericCellCursor<C>                                    \
{                                                                               \
public:                                                                         \
	name(void) : R::RGenericCellCursor<C>() {}                                  \
	name& operator=(const name& c)                                              \
	{                                                                           \
		R::RGenericCellCursor<C>::operator=(c);                                 \
		return(*this);                                                          \
	}                                                                           \
	name& operator=(const R::RGenericCellCursor<C>& c)                          \
	{                                                                           \
		R::RGenericCellCursor<C>::operator=(c);                                 \
		return(*this);                                                          \
	}                                                                           \
};


//-----------------------------------------------------------------------------
#endif
