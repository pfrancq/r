/*
	R Project Library

	RGenericCell.h

	Generic Cell Element  - Header.

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
#ifndef RGenericCell_H
#define RGenericCell_H


//-----------------------------------------------------------------------------
//include file for R Project



//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a Generic Cell to create vector or matrix of generic elements.
* This Generic Cell in composed of one Id (size_t) and a value of Generic Type
* This class provides a way to create different type of cells each containing different type of information.
* @author  Vandaele Valery
* @short Generic Cell template.
*/

template<class C>
	class RGenericCell
{
private :

	/**
	* Id of the Cell.
	*/
	size_t Id;

	/**
	* Value of the cell (of type C).
	*/
	C Value;

public :

	/**
	* constructor of RGenericCell
	* @param id             The Id of the cell.
	*/
	RGenericCell(size_t id);

	/**
	* constructor of RGenericCell
	* @param id             The Id of the cell.
	* @param value          The value of type T
	*/
	RGenericCell(size_t id, C value);

	/**
	* Construct the generic cell from another one.
	* @param src            Generic Cell used as source.
	*/
	RGenericCell(const RGenericCell<C>* src);

	/**
	* Construct the generic cell from another one.
	* @param src            Generic Cell used as source.
	*/
	RGenericCell(const RGenericCell<C>& src);

	/**
	* The assignement operator.
	* @param src            Generic Cell used as source.
	*/
	RGenericCell& operator=(const RGenericCell<C>& src);

	/**
	* Method used by the container
	* param c               The pointer to the RGenericCell to be compared.
	*/
	int Compare(const RGenericCell<C>* c) const;

	/**
	* Method used by the container
	* param c               The RGenericCell to be compared.
	*/
	int Compare(const RGenericCell<C>& c) const;

	/**
	* Method used by the container
	* param id              The id to be compared.
	*/
	int Compare(const size_t id) const;

	/**
	* Get the id of the cell.
	* @return id                Int descibing the id of the cell.
	*/
	size_t GetId(void) const {return Id;}

	/**
	* Get The value contained in the cell.
	* @return C             The value of the cell (of type C).
	*/
	C GetValue(void) const {return Value;}

	/**
	* Set the value of the cell.
	* @param value          The value to be contained in the cell (of type C).
	*/
	void SetValue(C value) { Value=value;}

	/**
	* Destructor of RGenericCell
	*/
	virtual ~RGenericCell(void);
};

//---------------------------------------------------------------------------
// inline implementation
#include <rgenericcell.hh>


}  //-------- End of namespace RMath ------------------------------------------


//-----------------------------------------------------------------------------
#endif
