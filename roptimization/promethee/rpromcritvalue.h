/*

	Rainbow Library Project

	RPromCritValue.h

	Promethee Criterion Value - Header.

	(C) 2000-2001 by P. Francq.

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
#ifndef RPromCritValueH
#define RPromCritValueH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rpromethee/rpromethee.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RPromethee{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPromCritValue class provides a representation for a value of a solution
* for a given criterion.
* @short Promethee Solution value.
* @author Pascal Francq & Thomas L'Eglise.
*/
class RPromCritValue
{
   /**
	* The value of the solution/criterion.
	*/
	double Value;

	/**
	* The Fi crit used by Prométhée.
	*/
	double FiCrit;

public:

	/**
	* Construct a value for a solution.
	*/
	RPromCritValue(void);

	/**
	* Construct a value for a solution.
	* @param value		The value for the solution/criterion.
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

	friend RPromKernel;
	friend RPromCriterion;
};


}  //-------- End of namespace RPromethee -------------------------------------


//-----------------------------------------------------------------------------
#endif
