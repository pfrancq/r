/*

	R Project Library

	RPromKernel.h

	Promethee Kernel - Header.

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
#ifndef RPromKernelH
#define RPromKernelH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rpromethee/rpromethee.h>
#include <rpromethee/rpromsol.h>
#include <rpromethee/rpromcriterion.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RPromethee{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPromKernel provides a kernel to manage Prométhée session.
* @short Prométhée Kernel.
* @author Pascal Francq & Thomas L'Eglise.
*/
class RPromKernel
{
	/**
	* The name of the kernel.
	*/
	RString Name;

	/**
	* Solutions.
	*/
	RContainer<RPromSol,unsigned int,true,true> Solutions;

	/**
	* Criteria.
	*/
	RContainer<RPromCriterion,unsigned int,true,true> Criteria;

	/**
	* Define if a normalisation of the values for the different criteria must be
	* done.
	*/
   bool Normalize;

public:

	/**
	* Construct the kernel.
	* @param name			The name of the kernel.
	* @param sol			Number of solutions.
	* @param crit			Number of criteria.
	* @param norm			Normlisation activ.
	*/
	RPromKernel(const RString& name,const unsigned int sol,const unsigned int crit,const bool norm=true);

	/**
	* Applicates the evaluation functions for the solution.
	*/
	void ComputeEvalFunc(void);

	/**
	* Calculate Prométhée II
	*/
	virtual void ComputePrometheeII(void);

	/**
	* Create a new criterion.
	* @param type	Type of the criterion.
	* @param p		Indifference's threshold.
	* @param q		Preference's threshold.
	* @param w		Weight of the criterion.
	*/
	virtual RPromCriterion* NewCriterion(const CriteriaType t,const double p,const double q,const double w) throw(bad_alloc);
	
	/**
	* Create a new criterion.
	* @param type		Type of the criterion.
	* @param params		Parameters.
	*/
	virtual RPromCriterion* NewCriterion(const CriteriaType t,const RPromCriterionParams& params) throw(bad_alloc);
	
	/**
	* Create a new criterion.
	* @param type	Type of the criterion.
	* @param name	Name of the criterion.
	* @param p		Indifference's threshold.
	* @param q		Preference's threshold.
	* @param w		Weight of the criterion.
	*/
	virtual RPromCriterion* NewCriterion(const CriteriaType t,const RString& name,const double p,
			const double q,const double w) throw(bad_alloc);
	
	/**
	* Create a new criterion.
	* @param type		Type of the criterion.
	* @param name		Name of the criterion.	
	* @param params		Parameters.
	*/
	virtual RPromCriterion* NewCriterion(const CriteriaType t,const RString& name,
			const RPromCriterionParams& params) throw(bad_alloc);
		
	/**
	* Create a new solution.
	*/
	virtual RPromSol* NewSol(void) throw(bad_alloc);
	
	/**
	* Create a new solution.
	* @param name		Name of the solution.	
	*/
	virtual RPromSol* NewSol(const RString& name) throw(bad_alloc);
	
	/**
	* Assign a value to a criterion.
	* @param sol	Pointer to the solution.
	* @param crit	Pointer to the criterion.
	* @param v		Value.
	*/
	virtual void Assign(RPromSol *sol,RPromCriterion *crit,const double v) throw(bad_alloc);
	
	/**
	* Assign a value to a criterion.
	* @param sol	Name of to the solution.
	* @param crit	Pointer to the criterion.
	* @param v		Value.
	*/
	virtual void Assign(const RString& sol,RPromCriterion *crit,const double v) throw(bad_alloc);
	
	/**
	* Assign a value to a criterion.
	* @param sol	Pointer to the solution.
	* @param crit	name of the criterion.
	* @param v		Value.
	*/
	virtual void Assign(RPromSol *sol,const RString& crit,const double v) throw(bad_alloc);
	
	/**
	* Assign a value to a criterion.
	* @param sol	name of the solution.
	* @param crit	name of the criterion.
	* @param v		Value.
	*/
	virtual void Assign(const RString& sol,const RString& crit,const double v) throw(bad_alloc);
	
	/**
	* Return the best solution.
	*/
	RPromSol* GetBestSol(void);
	
	/**
	* Function used to sort the solutions by fit.
	*/
	static int sort_function_solutions( const void *a, const void *b);	
	
	/**
	* Return the solutions order by fit increase.
	*/
	RPromSol** GetSols(void);

	/**
	* Return the solutions order by fit increase.
	*/
	RContainer<RPromSol,unsigned int,false,false>* GetContSols(void);

	/**
	* Return the best solution identifier.
	*/
	unsigned int GetBestSolId(void) {return(GetBestSol()->Id);}
	
	/**
	* Clear the kernel.
	*/
	void Clear(void);

   /**
	* Destruct the kernel.
	*/
	virtual ~RPromKernel(void);
	
	friend RPromCriterion;
};


}  //-------- End of namespace RPromethee -------------------------------------


//-----------------------------------------------------------------------------
#endif
