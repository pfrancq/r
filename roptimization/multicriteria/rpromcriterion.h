/*

	R Project Library

	RPromCriterion.h

	PROMETHEE Criterion - Header.

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
#ifndef RPromCriterion_H
#define RPromCriterion_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rpromcritvalue.h>
#include <rparam.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

	
//------------------------------------------------------------------------------
// forward declaration
class RPromKernel;


//------------------------------------------------------------------------------
/**
* The RPromCriterion class provides a representation for a Promethee Criterion.
* @short PROMETHEE Criterion.
* @author Pascal Francq & Thomas L'Eglise.
*/
class RPromCriterion : protected RContainer<RPromCritValue,false,false>
{
public:

	/**
	* Different types of criteria.
	* @short Criteria Type.
	*/
	enum tCriteriaType
	{
		Minimize                 /** The criteria must be maximized.*/,
		Maximize                 /** The criteria must be minimized.*/
	};

protected:

	/**
	* The identifier of the criterion.
	*/
	size_t Id;

	/**
	* Name of the criterion
	*/
	RString Name;

	/**
	* The type of criteria.
	*/
	tCriteriaType Type;

	/**
	* Weight of this criterion.
	*/
	double Weight;

	/**
	 * Is the criteria active ?
    */
	bool Active;

public:

	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param w              Weight of the criterion.
	* @param name           Name of the criterion.
	* @param nb             Number of solution.
	*/
	RPromCriterion(tCriteriaType type,double w,const RString& name=RString::Null,size_t nb=30);

	/**
	* Compare the identifier of two criteria.
	* @param crit           The criterion used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const RPromCriterion& crit) const {return(CompareIds(Id,crit.Id));}

	/**
	* Compare the identifier of two criteria.
	* @param crit           The criterion used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const RPromCriterion* crit) const {return(CompareIds(Id,crit->Id));}

	/**
	* Compare the identifier of the criterion with a parameter.
	* @param id             The identifier used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const size_t id) const {return(CompareIds(Id,id));}

	/**
	* Compare the name of the criterion with a string.
	* @param name           The string used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* Compare the name of the criterion with a string.
	* @param name           The string used for the comparison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

	/**
    * @return if the criteria is active.
    */
	bool IsActive(void) const {return(Active);}

	/**
	 * Get the type of criteria.
	 */
	tCriteriaType GetType(void) const {return(Type);}

	/**
	* Get the weight.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	 * Get the name of the criterion.
	 */
	RString GetName(void) const {return(Name);}

	/**
	* Set the parameter of the parameter.
	* @param param           Pointer to a structure parameter.
	*/
	virtual void Set(const RParam* param);

private:

	/**
	 * Set the identifier of the criterion. Can only be done by RPromKernel.
	 * @param id             Identifier of the criterion.
	 */
	void SetId(size_t id) {Id=id;}

public:

	/**
	* Normalize the values for the solutions such that they are in [0,1].
	*/
	void Normalize(void);

	/**
	* Compute the preference of a solution with the value u and a solution with
	* the value v. This preference must be in [0,1].
	*/
	virtual double ComputePref(double u,double v)=0;

	/**
	* Calculate the Fi Crit for the different solutions.
	*/
	void ComputeFiCrit(RPromKernel *kern);

	/**
	* Destruct the Criterion.
	*/
	~RPromCriterion(void);

	// friend classes
	friend class RPromKernel;
};


//------------------------------------------------------------------------------
/**
* The RPromLinearCriterion class provides a representation for a PROMETHEE Criterion.
* @short Linear PROMETHEE Criterion.
* @author Pascal Francq.
*/
class RPromLinearCriterion : public RPromCriterion
{
protected:

	/**
	* Value for the preference threshold of the PROMETHEE method.
	*/
	double P;

	/**
	* Value for the indifference threshold of the PROMETHEE method.
	*/
	double Q;

public:
	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param p              Preference threshold.
	* @param q              Indifference threshold.
	* @param w              Weight of the criterion.
	* @param name           Name of the criterion.
	* @param nb             Number of solution.
	*/
	RPromLinearCriterion(tCriteriaType type,double p,double q,double w,const RString& name=RString::Null,size_t nb=30);

	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param params         Parameters.
	* @param name           Name of the criterion.
	* @param nb             Number of solution.
	*/
	RPromLinearCriterion(tCriteriaType type,const RParam* params,const RString& name=RString::Null,size_t nb=30);

 	/**
 	* Set the parameters.
 	* @param p              Preference threshold.
 	* @param q              Indifference threshold.
 	* @param w              Weight of the criterion.
	* @param active         Is the criterion active ?
 	*/
	void Set(double p,double q,double w,bool active=true);

	/**
	* Set the parameter of the parameter.
	* @param param           Pointer to a structure parameter.
	*/
	virtual void Set(const RParam* param);

	/**
	* @return the preference threshold.
	*/
	double GetP(void) const {return(P);}

	/**
	* @return the indifference threshold.
	*/
	double GetQ(void) const {return(Q);}

	/**
	* Compute the preference of a solution with the value u and a solution with
	* the value v. This preference must be in [0,1].
	*
	* This method compares the normalized difference of values, d:
	* -# If d<Q=: No solutions must be preferred to the other one. The method
	*            returns 0.
	* -# If Q<d<P: One of the solution has a degree of preference computed as
	*              (d-Q)/(P-Q).
	* -# If d>=P: One of the solution must always be preferred. The method
	*            returns 1.
	* @param u              Value of the first solution.
	* @param v              Value of the second solution.
	* @return the result of the preference function defined.
	*/
	virtual double ComputePref(double u,double v);

	/**
	* Create a parameter corresponding to a PROMETHEE criteria.
	* @param name            Name of the criteria.
	* @param desc            Description of the criteria.
	*/
	static RParam* CreateParam(const RString& name,const RString& desc=RString::Null);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
