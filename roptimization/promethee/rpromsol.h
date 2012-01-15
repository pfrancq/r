/*

	R Project Library

	RPromSol.h

	PROMETHEE Solutions - Header.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2000 by Thomas L'Eglise.
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RPromSol_H
#define RPromSol_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromethee.h>
#include <rpromcritvalue.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPromSol class provides a representation of a solution in the PROMETHEE
* method.
* @short PROMETHEE Solution.
* @author Pascal Francq & Thomas L'Eglise
*/
class RPromSol : protected RContainer<RPromCritValue,true,false>
{
	/**
	* The identifier of the solution.
	*/
	size_t Id;

	/**
	* Name of the solution.
	*/
	RString Name;

	/**
	* The fi of the solution.
	*/
	double Fi;

	/**
	* The fi+ of the solution.
	*/
	double FiPlus;

	/**
	* The fi- of the solution.
	*/
	double FiMinus;

public:

	/**
	* Construct a PROMETHEE solution.
	* @param id             Identifier of the solution.
	* @param nbcrit         Number of criteria.
	*/
	RPromSol(const size_t id,const size_t nbcrit=30);

	/**
	* Construct a PROMETHEE solution.
	* @param id             Identifier of the solution.
	* @param name           Name of the solution
	* @param nbcrit         Number of criteria.
	*/
	RPromSol(const size_t id,const char* name,const size_t nbcrit=30);

	/**
	* Compare two PROMETHEE solutions.
	* @param sol            The solution used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const RPromSol& sol) const {return(CompareIds(Id,sol.Id));}

	/**
	* Compare two PROMETHEE solutions.
	* @param sol            The solution used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const RPromSol* sol) const {return(CompareIds(Id,sol->Id));}

	/**
	* Compare the identifier of the PROMETHEE solution with another
	* identifier.
	* @param id             The identifier used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const size_t id) const {return(CompareIds(Id,id));}

	/**
	* Compare the name of the PROMETHEE solution with another string.
	* @param name           The string used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* Compare the name of the PROMETHEE solution with another string.
	* @param name           The string used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

	/**
	* @return Fi of the solution.
	*/
	double GetFi(void) const {return(Fi);}

	/**
	* @return Fi+ of the solution.
	*/
	double GetFiPlus(void) const {return(FiPlus);}

	/**
	* @return Fi- of the solution.
	*/
	double GetFiMinus(void) const {return(FiMinus);}

	/**
	* @return Identifier of the solution.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Return the name of the solution.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Destruct a PROMETHEE solution.
	*/
	virtual ~RPromSol(void);

	// friend classes
	friend class RPromKernel;
	friend class RPromCriterion;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
