/*

	R Project Library

	RPromCritValue.h

	Promethee Criterion Value - Header.

	Copyright 2000-2003 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

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



//------------------------------------------------------------------------------
#ifndef RPromCritValue_H
#define RPromCritValue_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromethee.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPromCritValue class provides a representation for a value of a solution
* for a given criterion.
* @short Promethee Solution value.
* @author Pascal Francq & Thomas L'Eglise.
*/
class RPromCritValue
{
	/**
	* The Fi crit used by Prom�th�e.
	*/
	double FiCrit;

	/**
	* The Fi+ crit used by Prom�th�e.
	*/
	double FiCritPlus;

	/**
	* The Fi- crit used by Prom�th�e.
	*/
	double FiCritMinus;

protected:

	/**
	* The value of the solution/criterion.
	*/
	double Value;

public:

	/**
	* Construct a value for a solution.
	*/
	RPromCritValue(void);

	/**
	* Construct a value for a solution.
	* @param value          The value for the solution/criterion.
	*/
	RPromCritValue(double value);

	/**
	* Compare two value of criterion.
	*/
	int Compare(const RPromCritValue&) {return(-1);}

	/**
	* Compare two value of criterion.
	*/
	int Compare(const RPromCritValue*) {return(-1);}

	/**
	* Destruct the value for a solution.
	*/
	~RPromCritValue(void);

	friend class RPromKernel;
	friend class RPromCriterion;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
