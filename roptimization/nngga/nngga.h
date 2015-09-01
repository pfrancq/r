/*

	R Project Library

	NNGGA.h

	Nearest Neighbors Grouping Genetic Algorithm (NNGGA) - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef NNGGA_H
#define NNGGA_H


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rparam.h>
#include <rgga.h>
#include <robjg.h>
#include <rfitness.h>
#include <rmaxmatrix.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RSCObj;
template<class cGroup,class cObj,class cGroups> class RGroupNN;
template<class cGroup,class cObj,class cGroups> class RGroupingHeuristicNN;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RThreadDataNN;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RChromoNN;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RInstNN;


//-----------------------------------------------------------------------------
/**
* The RFitnessNN provides a representation for a fitness function for the
* Nearest Neighbors Grouping Genetic Algorithm.
* @short NNGGA Fitness.
*/
class RFitnessNN : public RFitness<double,true>
{
public:

	/**
	* Constructor of the fitness function used for the Bin Packing.
	*/
	RFitnessNN(void) : RFitness<double,true>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	RFitnessNN& operator=(const RFitnessNN &f)
	{
		RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	RFitnessNN& operator=(const double val)
	{
		RFitness<double,true>::operator=(val);
		return(*this);
	}
};


//-----------------------------------------------------------------------------
/**
* The RParamNN represents all the parameter used in the Nearest Neighbors
* Grouping Genetic Algorithm.
* @short NNGGA Parameters.
*/
class RParamsNN
{
public:

	/**
	* Size of the population of the GA.
	*/
	size_t PopSize;

	/**
	* Maximal number of generation to run.
	*/
	size_t MaxGen;

	/**
	* Is the GA in step mode?
	*/
	bool Step;

	/**
	* Number of generation for each step.
	*/
	size_t StepGen;

	/**
	* Minimum threshold for similarity between the objects.
	*/
	double MinSimLevel;

	/**
	* Minimum threshold for the agreement ratio.
	*/
	double MinAgreement;

	/**
	* Minimum threshold for the disagreement ratio.
	*/
	double MinDisagreement;

	/**
	* Parameter for the criterion "Similarity".
	*/
	RParam* ParamsSim;

	/**
	* Parameter for the criterion "Agreement".
	*/
	RParam* ParamsAgreement;

	/**
	* Parameter for the criterion "Disagreement".
	*/
	RParam* ParamsDisagreement;

	/**
	* Incremental mode. The NNGA constructs the chromosomes from existing
	* solutions.
	*/
	bool Incremental;

	/**
	 * Minimum number of objects per groups.
	 */
	size_t NbMinObjs;

	/**
	 * Maximum number of objects per group.
	 */
	size_t NbMaxObjs;

	/**
	* Constructor.
	*/
	RParamsNN(void);
};


//------------------------------------------------------------------------------
/**
* The RObjNN class provides a representation of an object to group by the
* Nearest Neighbors Grouping Genetic Algorithm.
* @short NNGGA Object.
*/
class RObjNN : public RObjG
{
protected:

	/**
	 * Can the element be grouped in a group alone?
	 */
	bool Social;

	/**
	 * Identifier of a common parent between elements that could prevent to
	 * group them together.
	 */
	size_t ParentId;

public:

	/**
	* Construct the object.
	* @param id             Identifier.
	* @param name           Name of the object.
	* @param social         Is the object social.
	* @param parentid       Identifier of the parent (if any).
	*/
	RObjNN(const size_t id,const RString& name,bool social=true,size_t parentid=0);

	/**
	 * See if the element is social or not.
	 */
	inline bool IsSocial(void) const {return(Social);}

	/**
	 * Get the identifier of the parent or 0 if there is no parent.
	 */
	inline size_t GetParentId(void) const {return(ParentId);}
};


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif
