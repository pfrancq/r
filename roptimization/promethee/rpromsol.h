/*

	Rainbow Library Project

	RPromSol.h

	Promethee Solutions - Header.

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
#ifndef RPromSolH
#define RPromSolH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rpromethee/rpromethee.h>
#include <rpromethee/rpromcritvalue.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RPromethee{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPromSol class provides a representation of a solution in the Prométhée
* method.
* @short Prométhée Solution.
* @author Pascal Francq & Thomas L'Eglise
*/
class RPromSol : protected RContainer<RPromCritValue,unsigned int,true,false>
{
	/**
	* The identifier of the solution.
	*/
	unsigned int Id;

	/**
	* The fi of the solution.
	*/
	double Fi;
	
public:

	/*
	* Construct a prométhée solution.
	* @param id			Identifier of the solution.
	* @param nbcrit	Number of criterions.
	*/
	RPromSol(const unsigned int id,const unsigned int nbcrit=30);

	/**
	* Compare two prométhée solutions.
	* @param sol		The solution used for the comparaison.
	* @return	The function returns -1,0,+1 depends if the identifier of the
	*				current solution is less, equal or greater than the one passed
	*				as argument.
	*/
	int Compare(const RPromSol& sol) {return(Id-sol.Id);}

	/**
	* Compare two prométhée solutions.
	* @param sol		The solution used for the comparaison.
	* @return	The function returns -1,0,+1 depends if the identifier of the
	*				current solution is less, equal or greater than the one passed
	*				as argument.
	*/
	int Compare(const RPromSol* sol) {return(Id-sol->Id);}

	/**
	* Compare the identifier of the prométhée solution with another name.
	* @param id		The identifier used for the comparaison.
	* @return	The function returns -1,0,+1 depends if the identifier of the
	*				current solution is less, equal or greater than the one passed
	*				as	argument.
	*/
	int Compare(const unsigned int id) {return(Id-id);}

	/**
	* @return Identificator of the solution.
	*/
	unsigned int GetId(void) {return(Id);}
	
	/**
	* Destruct a prométhée solution.
	*/
	virtual ~RPromSol(void);

	friend RPromKernel;
	friend RPromCriterion;
};


}  //-------- End of namespace RPromethee -------------------------------------


//-----------------------------------------------------------------------------
#endif
