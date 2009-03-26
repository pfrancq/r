/*

	R Project Library

	RInstSC.h

	Similarity-based Clustering Instance - Header

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RInstSC_H
#define RInstSC_H


//-----------------------------------------------------------------------------
// Defines
#define BESTSOLSVERIFICATION 0


//-----------------------------------------------------------------------------
// include files for R Project
#include <rinstg.h>
#include <scga.h>
#include <rpromkernelsc.h>
#include <rheuristicsc.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RThreadDataSC class provides a representation for "thread-dependent" data
* for the similarity-based clustering algorithm.
* @author Pascal Francq
* @short Similarity-based Clustering "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	class RThreadDataSC : public RThreadDataG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>
{
	using RThreadDataG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Owner;

public:

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
	*/
	size_t NbSols;

	/**
	* Construct the data.
	* @param owner           Instance of the problem.
	*/
	RThreadDataSC(cInst* owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataSC(void);
};


//-----------------------------------------------------------------------------
/**
* The instance of the similarity-based clustering genetic algorithm.
* @author Pascal Francq
* @short Similarity-based Clustering Instance.
*/
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	class RInstSC : public RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>, public RPromKernelSC<cChromo>
{
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::SetMutationParams;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::GetPopSize;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Objs;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Random;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Debug;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::GetGen;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::BestChromosome;
	using RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Chromosomes;
	using RPromKernelSC<cChromo>::NewSol;
	using RPromKernelSC<cChromo>::ComputePrometheeII;
	using RPromKernelSC<cChromo>::GetSols;

protected:

	/**
	* Parameter of the GA.
	*/
	RParamsSC* Params;

	/**
	* Solutions corresponding to the chromosome.
	*/
	RPromSol** Sols;

	/**
	* Social objects.
	*/
	RContainer<cObj,false,true> NoSocialObjs;

	/**
	 * Incremental mode.
	 */
	bool Incremental;

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
	* @param inc            Incremental mode (The GCA constructs the
	*                       chromosomes from existing solutions).
	*/
	RInstSC(RCursor<cObj> objs,RParamsSC* p,RDebug* debug,bool inc);

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
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetDisagreementRatio(size_t element1,size_t element2) const=0;

	/**
	* Get the disagreement ratio between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetAgreementRatio(size_t element1,size_t element2) const=0;

	/**
	* Compute the similarity between two elements.
	* @param element1        First element.
	* @param element2        Second element.
	*/
	virtual double GetSim(size_t element1,size_t element2) const=0;

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
	virtual ~RInstSC(void);

	// friend classes
	friend class RGroupSC<cGroup,cObj,cChromo>;
	friend class RThreadDataSC<cInst,cChromo,cThreadData,cGroup,cObj>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rinstsc.hh>


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif

