/*

	R Project Library

	RPromCriterion.h

	Promethee Criterion - Header.

	Copyright 2000-2003 by the Université Libre de Bruxelles.

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
#ifndef RPromCriterion_H
#define RPromCriterion_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rpromethee/rpromethee.h>
#include <rpromethee/rpromcritvalue.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
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
	* Value for the preference threshold of the Prométhée method.
	*/
	double P;

	/**
	* Value for the indifference threshold of the Prométhée method.
	*/
	double Q;

	/**
	* Weight of this criterion.
	*/
	double Weight;

	/**
	* Construct a list of parameters for a criterion.
	*/
	RPromCriterionParams(void);

	/**
	* Construct a list of parameters for a criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	*/
	RPromCriterionParams(double p,double q,double w);

	/**
	* Construct a list of parameters for acriterion.
	* @param p               Params used for the initialisation.
	*/
	RPromCriterionParams(const RPromCriterionParams& p);

	/**
	* Construct a list of parameters for acriterion.
	* @param p               Params used for the initialisation.
	*/
	RPromCriterionParams(const RPromCriterionParams* p);

	/**
	* Return a pointer to a temporary object of class point.
	*/
	static RPromCriterionParams* GetParams(void);

	/**
	* Set the parameters using a string with the format "P Q W".
	* @param values         Values of the parameter.
	*/
	void Set(const char* values);

	/**
	* Assignment operator.
	*/
	RPromCriterionParams& operator=(const RPromCriterionParams &params);
};



//------------------------------------------------------------------------------
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
	* Name of the criterion
	*/
	RString Name;

	/**
	* The type of criteria.
	*/
	CriteriaType Type;

	/**
	* Value for the preference threshold of the Prométhée method.
	*/
	double P;

	/**
	* Value for the indifference threshold of the Prométhée method.
	*/
	double Q;

	/**
	* Weight of this criterion.
	*/
	double Weight;

public:

	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	* @param id             Identifier of the criterion.
	* @param nb             Number of solution.
	*/
	RPromCriterion(CriteriaType type,double p,double q,double w,unsigned int id,unsigned int nb=30);

	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param params         Parameters.
	* @param id             Identifier of the criterion.
	* @param nb             Number of solution.
	*/
	RPromCriterion(CriteriaType type,const RPromCriterionParams& params,unsigned int id,unsigned int nb=30);

	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param p              Preference's threshold.
	* @param q              Indifference's threshold.
	* @param w              Weight of the criterion.
	* @param id             Identifier of the criterion.
	* @param name           Name of the criterion.
	* @param nb             Number of solution.
	*/
	RPromCriterion(CriteriaType type,double p,double q,double w,unsigned int id,const char* name,unsigned int nb=30);

	/**
	* Construct a criterion.
	* @param type           Type of the criterion.
	* @param params         Parameters.
	* @param id             Identifier of the criterion.
	* @param name           Name of the criterion.
	* @param nb             Number of solution.
	*/
	RPromCriterion(CriteriaType type,const RPromCriterionParams& params,unsigned int id,const char* name,unsigned int nb=30);

	/**
	* Compare the identifier of two criteria.
	* @param crit           The criterion used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const RPromCriterion& crit) const {return(Id-crit.Id);}

	/**
	* Compare the identifier of two criteria.
	* @param crit           The criterion used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const RPromCriterion* crit) const {return(Id-crit->Id);}

	/**
	* Compare the identifier of the criterion with a parameter.
	* @param id             The identifier used for the comparaison.
	* @returnThe function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const unsigned int id) const {return(Id-id);}

	/**
	* Compare the name of the criterion with a string.
	* @param name           The string used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* Compare the name of the criterion with a string.
	* @param name           The string used for the comparaison.
	* @return The function returns -1,0,+1 depends if the identifier of the
	* current criteria is less, equal or greater than the one given as parameter.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

	/**
	* Set the preference's threshold.
	*/
	void SetP(double p) {P=p;}

	/**
	* Get the preference's threshold.
	*/
	double GetP(void) const {return(P);}

	/**
	* Set the indifference's threshold.
	*/
	void SetQ(double q) {Q=q;}

	/**
	* Get the indifference's threshold.
	*/
	double GetQ(void) const {return(Q);}

	/**
	* Set the parameters.
	*/
	void SetParams(const RPromCriterionParams& params);

	/**
	* Get the parameters.
	*/
	RPromCriterionParams& GetParams(void);

	/**
	* Set the weight.
	*/
	void SetWeight(double w) {Weight=w;}

	/**
	* Get the weight.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	* Normalize the values for the solution.
	*/
	void Normalize(void);

	/**
	* Compare two values representing two different solutions.
	* @param u              Value of the first solution.
	* @param v              Value of the second solution.
	* @return The result of the preference function defined.
	*/
	double ComputePref(double u,double v) const;

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
* The RPromCriterionCursor class provides a way to go trough a set of criterion.
* @short Criterion Cursor.
*/
CLASSCURSOR(RPromCriterionCursor,RPromCriterion,unsigned int)


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
