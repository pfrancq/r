/*

	Rainbow Library Project

	RPromCriterion.h

	Promethee Criterion - Header.

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
#ifndef RPromCriterionH
#define RPromCriterionH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rpromethee/rpromethee.h>
#include <rpromethee/rpromcritvalue.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
namespace RPromethee{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPromCriterionParams is use to hold the parameters that can influence a
* criterion in Promethee.
* @short Prométhée Criterion.
* @author Pascal Francq.
*/
class RPromCriterionParams
{

public:

	/**
	* Value for the indifference's threshold of the Prométhée method.
	*/
	double P;

	/**
	* Value for the preference's threshold of the Prométhée method.
	*/
	double Q;
	
	/**
	* Weight of this criterion.
	*/
	double Weight;
	
	/**
	* Return a pointer to a temporary object of class point.
	*/
	static RPromCriterionParams* GetParams(void);
	
	/**
	* Assignment operator.
	*/
	RPromCriterionParams& operator=(const RPromCriterionParams &params);
};



//-----------------------------------------------------------------------------
/**
* The RPromCriterion class provides a representation for a Prométhée Criterion.
* @short Prométhée Criterion.
* @author Pascal Francq & Thomas L'Eglise.
*/
class RPromCriterion : protected RContainer<RPromCritValue,unsigned int,false,false>
{

	/**
	* The identifier of the criterion.
	*/
	unsigned int Id;

	/**
	* The type of criteria.
	*/
   	CriteriaType Type;

	/**
	* Value for the indifference's threshold of the Prométhée method.
	*/
	double P;

	/**
	* Value for the preference's threshold of the Prométhée method.
	*/
	double Q;
	
	/**
	* Weight of this criterion.
	*/
	double Weight;

public:                                                          	
	/**
	* Construct a criterion.
	* @param type	Type of the criterion.
	* @param p		Indifference's threshold.
	* @param q		Preference's threshold.
	* @param w		Weight of the criterion.
	* @param id		Identifier of the criterion.
	* @param nb		Number of solution.
	*/
	RPromCriterion(const CriteriaType type,const double p,const double q,const double w,const unsigned int id,const unsigned int nb=30);
	
	/**
	* Construct a criterion.
	* @param type		Type of the criterion.
	* @param params		Parameters.
	* @param q			Preference's threshold.
	* @param w			Weight of the criterion.
	* @param id			Identifier of the criterion.
	* @param nb			Number of solution.
	*/
	RPromCriterion(const CriteriaType type,const RPromCriterionParams& params,const unsigned int id,const unsigned int nb=30);
	
	/**
	* Compare the identifier of two criteria.
	* @param crit		The criterion used for the comparaison.
	* @return	The function returns -1,0,+1 depends if the identifier of the
	*				current criteria is less, equal or greater than the one given as
	*				parameter.
	*/
	int Compare(const RPromCriterion& crit) { return(Id-crit.Id);}

	/**
	* Compare the identifier of two criteria.
	* @param crit		The criterion used for the comparaison.
	* @return	The function returns -1,0,+1 depends if the identifier of the
	*				current criteria is less, equal or greater than the one given as
	*				parameter.
	*/
	int Compare(const RPromCriterion* crit) { return(Id-crit->Id);}

	/**
	* Compare the identifier of two criteria.
	* @param id		The identifier used for the comparaison.
	* @return	The function returns -1,0,+1 depends if the identifier of the
	*				current criteria is less, equal or greater than the one given as
	*				parameter.
	*/
	int Compare(const unsigned int id) { return(Id-id);}

	/**
	* Set the indifference's threshold.
	*/
	void SetP(double p) {P=p;}
	
	/**
	* Get the indifference's threshold.
	*/
	double GetP(void) {return(P);}	
	
	/**
	* Set the preference's threshold.
	*/
	void SetQ(double q) {Q=q;}

	/**
	* Set the parameters.
	*/
	void SetParams(const RPromCriterionParams& params);
	
	/**
	* Get the parameters.
	*/
	RPromCriterionParams& GetParams(void);
	
	/**
	* Get the Preference's threshold.
	*/
	double GetQ(void) {return(Q);}	
	
	/**
	* Set the weight.
	*/
	void SetWeight(double w) {Weight=w;}
	
	/**
	* Get the weight.
	*/
	double GetWeight(void) {return(Weight);}	
	
	/**
	* Normalize the values for the solution.
	*/
	void Normalize(void);

	/**
	* Compare two values representing two different solutions.
	* @param u		Value of the first solution.
	* @param v		Value of the second solution.
	* @return	The resukt of the preference function defined.
	*/
	double ComputePref(const double u,const double v);

	/**
	* Calculate the Fi Crit for the different solutions.
	*/
	void ComputeFiCrit(RPromKernel *kern);

	/**
	* Destruct the Criterion.
	*/
	~RPromCriterion(void);

	friend RPromKernel;
};


}  //-------- End of namespace RPromethee -------------------------------------


//-----------------------------------------------------------------------------
#endif
