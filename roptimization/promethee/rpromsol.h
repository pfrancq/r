/*

	R Project Library

	RPromSol.h

	Promethee Solutions - Header.

	Copyright 2000-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

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
* The RPromSol class provides a representation of a solution in the Prom�h�
* method.
* @short Prom�h� Solution.
* @author Pascal Francq & Thomas L'Eglise
*/
class RPromSol : protected RContainer<RPromCritValue,true,false>
{
	/**
	* The identifier of the solution.
	*/
	unsigned int Id;

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
	* Construct a prom�h� solution.
	* @param id             Identifier of the solution.
	* @param nbcrit         Number of criterions.
	*/
	RPromSol(const unsigned int id,const unsigned int nbcrit=30);

	/**
	* Construct a prom�h� solution.
	* @param id             Identifier of the solution.
	* @param name           Name of the solution
	* @param nbcrit         Number of criterions.
	*/
	RPromSol(const unsigned int id,const char* name,const unsigned int nbcrit=30);

	/**
	* Compare two prom�h� solutions.
	* @param sol            The solution used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const RPromSol& sol) const {return(Id-sol.Id);}

	/**
	* Compare two prom�h� solutions.
	* @param sol            The solution used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const RPromSol* sol) const {return(Id-sol->Id);}

	/**
	* Compare the identifier of the prom�h� solution with another identificator.
	* @param id             The identifier used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const unsigned int id) const {return(Id-id);}

	/**
	* Compare the name of the prom�h� solution with another string.
	* @param name           The string used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current solution is less, equal or greater than the one passed as argument.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* Compare the name of the prom�h� solution with another string.
	* @param name           The string used for the comparaison.
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
	* @return Identificator of the solution.
	*/
	unsigned int GetId(void) const {return(Id);}

	/**
	* Return the name of the solution.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Destruct a prom�h� solution.
	*/
	virtual ~RPromSol(void);

	// friend classes
	friend class RPromKernel;
	friend class RPromCriterion;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
