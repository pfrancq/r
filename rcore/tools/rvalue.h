/*

	R Project Library

	RValue.h

	Value associate to an identifier - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RValue_H
#define RValue_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RValue class provides an association of an identifier and a value.
* @author Pascal Francq
* @short Value Associated to a Identifier.
*/
class RValue
{
public:

	/**
	 * Represent a null value and an undefined identifier.
	 */
	static const RValue Null;

	/**
	* The identifier of the value.
	*/
	size_t Id;

	/**
	* Value.
	*/
	double Value;

	/**
	 * Default constructor. The identifier is set to cNoRef and the value to NAN.
	 */
	RValue(void);

	/**
	* Construct a default value.
	* @param id             Identifier.
	*/
	explicit RValue(size_t id);

	/**
	* Construct a value.
	* @param id             Identifier.
	* @param val            Value.
	*/
	RValue(size_t id,double val);

	/**
	* Comparison function.
	* @param id              Identifier to compare with.
	*/
	int Compare(size_t id) const;

	/**
	* Comparison function
	* @param obj             Value to compare with.
	*/
	int Compare(const RValue& obj) const;

	/**
	 * Assignment operator.
	 * @param val            Value to assign.
	 */
	RValue& operator=(const RValue& val);

	/**
	 * Assignment operator.
	 * @param val            Value to assign.
	 */
	RValue& operator=(const double val);

	/**
	 * Automatic conversion to a double (const version).
	 */
	operator double () const {return(Value);}

	/**
	* Destruct the object.
	*/
	virtual ~RValue(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
