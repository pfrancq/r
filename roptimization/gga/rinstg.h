/*

	R Project Library

	RInstg.h

	Class representing an instance of a GGA - Header

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RInstG_H
#define RInstG_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>
#include <rga/robjs.h>
#include <rga/rinst.h>
#include <rga/robjg.h>
#include <rga/rfirstfitheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the GGA.
* @author Pascal Francq
* @short GGA "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	class RThreadDataG : public RThreadData<cInst,cChromo>
{
protected:

	/**
	* Heuristic Used.
	*/
	RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* Heuristic;

public:

	/**
	* Construct the data.
	* @param owner          Owner of the data.
	*/
	RThreadDataG(cInst* owner) throw(std::bad_alloc);

	/**
	* Initialise thje data.
	*/
	virtual void Init(void) throw(std::bad_alloc);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataG(void);

	// friend class
	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>;
};


//------------------------------------------------------------------------------
/**
* Generic instance for a GGA.
* @author Pascal Francq
* @short Generic GGA instance.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	class RInstG : public RInst<cInst,cChromo,cFit,cThreadData>
{
protected:

	/**
	* Type of the heuristic that used.
	*/
	HeuristicType Heuristic;

	/**
	* Objects to group.
	*/
	RCursor<cObj>* Objs;

	/**
	* Maximal number of groups to allocate by default.
	*/
	unsigned int MaxGroups;

public:

	/**
	* Construct the instance.
	* @param popsize        Size of the population.
	* @param objs           Objects to groups.
	* @param h              The heuristic that has to be used.
	* @param debug          Debugger.
	*/
	RInstG(unsigned int popsize,RObjs<cObj>* objs,HeuristicType h,RDebug *debug=0) throw(std::bad_alloc);

	/**
	* Construct the instance.
	* @param popsize        Size of the population.
	* @param objs           Objects to groups.
	* @param h              The heuristic that has to be used.
	* @param debug          Debugger.
	*/
	RInstG(unsigned int popsize,RContainer<cObj,false,true>* objs,HeuristicType h,RDebug *debug=0) throw(std::bad_alloc);

	/**
	* Construct the instance.
	* @param popsize        Size of the population.
	* @param objs           Objects to groups.
	* @param h              The heuristic that has to be used.
	* @param debug          Debugger.
	*/
	RInstG(unsigned int popsize,RContainer<cObj,true,true>* objs,HeuristicType h,RDebug *debug=0) throw(std::bad_alloc);

	/**
	* Initialisation of the instance.
	* @param gdata          The Data to use for the construction of the groups.
	*/
	virtual void Init(cGroupData* gdata) throw(std::bad_alloc);

	/**
	* Return the heuristic type.
	*/
	inline HeuristicType GetHeuristic(void) {return(Heuristic);}

	/**
	* Get the Objects to group.
	* @return Pointer to the objects.
	*/
	RCursor<cObj>* GetObjs(void) const {return(Objs);}

	/**
	* Create a heuristic object.
	*/
	virtual RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* CreateHeuristic(void) throw(std::bad_alloc);

	/**
	* Handle the chromosomes that are representing the same solutions. If two
	* same chromosomes are detect, one of them is replace a random based one
	* with a probability of 90%.
	*/
	virtual void HandleSameChromosomes(void) throw(eGA);

	/**
	* Random construction of the chromosomes. Call the method of RInst and
	* handles the same solutions after.
	*/
	virtual void RandomConstruct(void) throw(eGA);

	/**
	* This function does the crossovers for a generation. Call the method of
	* RInst and handles the same solutions after.
	*/
	virtual void Crossover(void) throw(eGA);

	/**
	* This function does the necessary mutations for a generation. Call the
	* method of RInst and handles the same solutions after.
	*/
	virtual void Mutation(void) throw(eGA);

	/**
	* This function does the necessary inversions for a generation. Call the
	* method of RInst and handles the same solutions after.
	*/
	virtual void Inversion(void) throw(eGA);

	/**
	* Destruct the instance.
	*/
	virtual ~RInstG(void);

	// friend classes
	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>;
	friend class RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rga/rinstg.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
