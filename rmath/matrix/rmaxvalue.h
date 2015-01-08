/*

	R Project Library

	RMaxValue.h

	Ascending Ordered value associate to an identifier - Header.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RMaxValue_H
#define RMaxValue_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RMaxValue class provides an association of an identifier and a value
* order by ascending or descending order of values.
* @author Pascal Francq
* @short Ordered Value Associated to a Identifier.
*/
class RMaxValue
{
public:

	/**
	 * Represent a null value and an undefined identifier.
	 */
	static const RMaxValue Null;

	/**
	* The identifier of the value.
	*/
	size_t Id;

	/**
	* Value.
	*/
	double Value;

	/**
	 * Default constructor. The identifier is set to cNoRef and the value to
	 * NAN.
	 */
	RMaxValue(void);

	/**
	 * Copy constructor.
	 * @param val            Original value.
	 */
	RMaxValue(const RMaxValue& val);

	/**
	* Construct a default value.
	* @param id             Identifier.
	*/
	explicit RMaxValue(size_t id);

	/**
	* Construct a value.
	* @param id             Identifier.
	* @param val            Value.
	*/
	RMaxValue(size_t id,double val);

	/**
	* Comparison function.
	* @param obj             Value to compare with.
	*/
	int Compare(const RMaxValue& obj) const;

	/**
	* Comparison function.
	* @param val             Value to compare with.
	*/
	int Compare(double val) const;

	/**
	* Compare the identifier of the sparse ordered vector with a given identifier.
	* param id               Identifier to compare with.
	*/
	int Compare(size_t id) const;

	/**
	 * Assignment operator.
	 * @param val            Value to assign.
	 */
	RMaxValue& operator=(const RMaxValue& val);

	/**
	 * Assignment operator.
	 * @param val            Value to assign.
	 */
	RMaxValue& operator=(const double val);

	/**
	 * Automatic conversion to a double (const version).
	 */
	operator double () const {return(Value);}

	/**
	 * Get the value.
    */
	double GetValue(void) const {return(Value);}

	/**
	 * Get the identifier.
    */
	size_t GetId(void) const {return(Id);}

	/**
	* Destruct the object.
	*/
	virtual ~RMaxValue(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
