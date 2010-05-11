/*

	R Project Library

	SCGA.h

	Similarity-based Clustering Genetic Algorithm - Header.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef SCGA_H
#define SCGA_H


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
template<class cGroup,class cObj,class cGroups> class RGroupSC;
template<class cGroup,class cObj,class cGroups> class RHeuristicSC;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RThreadDataSC;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RChromoSC;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RInstSC;


//-----------------------------------------------------------------------------
/**
* The RSCFitness provides a representation for a fitness function for the
* similarity-based clustering genetic algorithm.
* @author Pascal Francq
* @short Similarity-based Clustering Fitness.
*/
class RFitnessSC : public RFitness<double,true>
{
public:

	/**
	* Constructor of the fitness function used for the Bin Packing.
	*/
	RFitnessSC(void) : RFitness<double,true>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	RFitnessSC& operator=(const RFitnessSC &f)
	{
		RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	RFitnessSC& operator=(const double val)
	{
		RFitness<double,true>::operator=(val);
		return(*this);
	}
};


//-----------------------------------------------------------------------------
/**
* The GCAParam represents all the parameter used in the GCA module.
* @short GCA Parameters.
*/
class RParamsSC
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
	* Incremental mode. The GCA constructs the chromosomes from existing
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
	RParamsSC(void);
};


//------------------------------------------------------------------------------
/**
* The RObjSC class provides a representation of an object to group by the
* similarity-based clustering algorithm.
* @author Pascal Francq
* @short Similarity-based Clustering Object.
*/
class RObjSC : public RObjG
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
	RObjSC(const size_t id,const RString& name,bool social=true,size_t parentid=0);

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
