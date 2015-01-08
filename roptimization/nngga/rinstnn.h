/*

	R Project Library

	RInstNN.h

	NNGGA Instance - Header

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
#ifndef RInstNN_H
#define RInstNN_H


//-----------------------------------------------------------------------------
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstg.h>
#include <nngga.h>
#include <rpromkernelnn.h>
#include <rheuristicnn.h>
#include <rmaxvector.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RThreadDataSG class provides a representation for "thread-dependent" data
* for the Nearest Neighbors Grouping Genetic Algorithm.
* @author Pascal Francq
* @short NNGGA "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	class RThreadDataNN : public RThreadDataG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>
{
public:

	using RThreadDataG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::Owner;

	/**
	 * Groups to delete because they contain only one "social" object.
	 */
	R::RContainer<cGroup,false,false> ToDel;

	/**
	* Temporary array of Objects.
	*/
	cObj** tmpObjs1;

	/**
	* Temporary array of Objects.
	*/
	cObj** tmpObjs2;

	/**
	* Construct the data.
	* @param owner           Instance of the problem.
	*/
	RThreadDataNN(cInst* owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataNN(void);
};


//-----------------------------------------------------------------------------
/**
* The instance of the Nearest Neighbors Grouping Genetic Algorithm.
* @author Pascal Francq
* @short NNGGA Instance.
*/
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	class RInstNN : public RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>, public RPromKernelNN<cChromo>
{
public:

	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::SetMutationParams;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::GetPopSize;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::Objs;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::Random;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::Debug;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::GetGen;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::BestChromosome;
	using RInstG<cInst,cChromo,RFitnessNN,cThreadData,cGroup,cObj>::Chromosomes;
	using RPromKernelNN<cChromo>::AssignChromo;
	using RPromKernelNN<cChromo>::NewSol;
	using RPromKernelNN<cChromo>::ComputePrometheeII;
	using RPromKernelNN<cChromo>::GetSols;

protected:

	/**
	* Parameter of the GA.
	*/
	RParamsNN* Params;

	/**
	* Solutions corresponding to the chromosome.
	*/
	RPromSol** Sols;

	/**
	* Social objects.
	*/
	RContainer<cObj,false,true> NoSocialObjs;

#if BESTSOLSVERIFICATION
	/**
	* Container holding the best chromosomes.
	*/
	R::RContainer<cChromo,true,false> BestSols;
#endif

public:

	/**
	* Construct the instance.
	* @param objs           The objects to group.
	* @param p              Parameters.
	* @param debug          Debugger.
	*/
	RInstNN(RCursor<cObj> objs,RParamsNN* p,RDebug* debug);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<cGroup,cObj,cChromo>* CreateHeuristic(void);

	/**
	* This function determines if the GA must be stopped. Actually, it is the case
	* when the maximal number of generation is calculated.
	* @return	The function returns true if the GA must stop.
	*/
	virtual bool StopCondition(void);

	/**
	* Write the information of a given chromosome.
	* @param c              Chromosome.
	*/
	void WriteChromoInfo(cChromo* c);

	/**
	* Apply PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void);

	/**
	* Get the maximum disagreement ratios of an element.
	* @param obj             Element.
	*/
	virtual const RMaxVector* GetDisagreementRatios(const cObj* obj) const=0;

	/**
	* Get the disagreement ratio between two elements.
	* @param obj             Element.
	*/
	virtual const RMaxVector* GetAgreementRatios(const cObj* obj) const=0;

	/**
	* Compute the similarity between two elements.
	* @param obj             Element.
	*/
	virtual const RMaxVector* GetSims(const cObj* obj) const=0;

	/**
	 * Verify if the object is a social one.
	 * @param obj            Object.
	 * @return false by default.
	 */
	virtual bool IsSocial(const cObj* obj);

	/**
	* This function can be used to do a treatment after the GA stops.
	*/
	virtual void PostRun(void);

	/**
	 *  Called when no handler was found for a given notification.
	 */
	virtual void HandlerNotFound(const R::RNotification& notification);

	/**
	* Destruct the instance.
	*/
	virtual ~RInstNN(void);

	// friend classes
	friend class RGroupNN<cGroup,cObj,cChromo>;
	friend class RThreadDataNN<cInst,cChromo,cThreadData,cGroup,cObj>;
	friend class RChromoNN<cInst,cChromo,cThreadData,cGroup,cObj>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rinstnn.hh>


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif

