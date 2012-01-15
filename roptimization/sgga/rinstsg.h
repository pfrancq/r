/*

	R Project Library

	RInstSG.h

	Similarity-based Grouping Genetic Algorithm Instance - Header

	Copyright 2002-2012 by Pascal Francq (pascal@francq.info).
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
#ifndef RInstSG_H
#define RInstSG_H


//-----------------------------------------------------------------------------
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstg.h>
#include <sgga.h>
#include <rpromkernelsg.h>
#include <rheuristicsg.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RThreadDataSG class provides a representation for "thread-dependent" data
* for the similarity-based grouping genetic algorithm.
* @author Pascal Francq
* @short Similarity-based Grouping Genetic Algorithm "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	class RThreadDataSG : public RThreadDataG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>
{
public:

	using RThreadDataG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Owner;

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
	* Test Chromosomes.
	*/
	cChromo** Tests;

	/**
	* PROMETHE  Kernel used by the chromosome.
	*/
	RPromKernelSC<cChromo> Prom;

	/**
	* Array of solutions to create in PROMETHEE Kernel.
	*/
	RPromSol** Sols;

	/**
	 * Number of derived solutions for the k-Means.
	 */
	size_t NbSols;

	/**
	* Construct the data.
	* @param owner           Instance of the problem.
	*/
	RThreadDataSG(cInst* owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataSG(void);
};


//-----------------------------------------------------------------------------
/**
* The instance of the similarity-based grouping genetic algorithm.
* @author Pascal Francq
* @short SSimilarity-based Grouping Genetic Algorithm Instance.
*/
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	class RInstSG : public RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>, public RPromKernelSC<cChromo>
{
public:

	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::SetMutationParams;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::GetPopSize;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Objs;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Random;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Debug;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::GetGen;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::BestChromosome;
	using RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Chromosomes;
	using RPromKernelSC<cChromo>::NewSol;
	using RPromKernelSC<cChromo>::ComputePrometheeII;
	using RPromKernelSC<cChromo>::GetSols;

protected:

	/**
	* Parameter of the GA.
	*/
	RParamsSG* Params;

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
	RInstSG(RCursor<cObj> objs,RParamsSG* p,RDebug* debug);

	/**
	* Initialization of the instance.
	*/
	virtual void Init(void);

	/**
	* Create a specific heuristic for the algorithm.
	*/
	virtual R::RGroupingHeuristic<cGroup,cObj,cChromo>* CreateHeuristic(void);

	/**
	* @returns The object corresponding to an element.
	* @param id              Identifier of the element.
	*/
	cObj* GetObj(size_t id) const;

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
	* Get the disagreement ratio between two elements.
	* @param obj1            First element.
	* @param obj2            Second element.
	*/
	virtual double GetDisagreementRatio(const cObj* obj1,const cObj* obj2) const=0;

	/**
	* Get the disagreement ratio between two elements.
	* @param obj1            First element.
	* @param obj2            Second element.
	*/
	virtual double GetAgreementRatio(const cObj* obj1,const cObj* obj2) const=0;

	/**
	* Compute the similarity between two elements.
	* @param obj1            First element.
	* @param obj2            Second element.
	*/
	virtual double GetSim(const cObj* obj1,const cObj* obj2) const=0;

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
	virtual ~RInstSG(void);

	// friend classes
	friend class RGroupSG<cGroup,cObj,cChromo>;
	friend class RThreadDataSG<cInst,cChromo,cThreadData,cGroup,cObj>;
	friend class RChromoSG<cInst,cChromo,cThreadData,cGroup,cObj>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rinstsg.hh>


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif

