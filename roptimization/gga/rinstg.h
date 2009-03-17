/*

	R Project Library

	RInstg.h

	Class representing an instance of a GGA - Header

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
#include <rcontainer.h>
#include <rcursor.h>
#include <robjs.h>
#include <rinst.h>
#include <robjg.h>
#include <rfirstfitheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the GGA.
* @author Pascal Francq
* @short GGA "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	class RThreadDataG : public RThreadData<cInst,cChromo>
{
	using RThreadData<cInst,cChromo>::Owner;

protected:

	/**
	* Heuristic Used.
	*/
	RGroupingHeuristic<cGroup,cObj,cChromo>* Heuristic;

public:

	/**
	* Construct the data.
	* @param owner          Owner of the data.
	*/
	RThreadDataG(cInst* owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataG(void);

	// friend class
	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
};


//------------------------------------------------------------------------------
/**
* Generic instance for a GGA.
* @author Pascal Francq
* @short Generic GGA instance.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	class RInstG : public RInst<cInst,cChromo,cFit,cThreadData>
{
	using RInst<cInst,cChromo,cFit,cThreadData>::GetPopSize;
	using RInst<cInst,cChromo,cFit,cThreadData>::Chromosomes;
	using RInst<cInst,cChromo,cFit,cThreadData>::BestChromosome;
	using RInst<cInst,cChromo,cFit,cThreadData>::Random;
	using RInst<cInst,cChromo,cFit,cThreadData>::Debug;
	using RInst<cInst,cChromo,cFit,cThreadData>::RRand;
	using RInst<cInst,cChromo,cFit,cThreadData>::PostNotification;

protected:

	/**
	* Name of the heuristic used.
	*/
	RString Heuristic;

	/**
	* Determine if the groups modified during a crossover must be emptied, or
	* only the existing objects should be removed.
	*/
	bool EmptyModifiedGroups;

	/**
	* Objects to group.
	*/
	RCursor<cObj> Objs;

	/**
	* Maximal number of groups to allocate by default.
	*/
	size_t MaxGroups;

	/**
	 * Must a local optimization be performed.
	 */
	bool DoLocalOptimisation;

public:

	/**
	* Construct the instance.
	* @param popsize        Size of the population.
	* @param objs           Objects to groups.
	* @param h              Name of the heuristic that has to be used.
	* @param name           Name of the genetic algorithm.
	* @param debug          Debugger.
	*/
	RInstG(size_t popsize,RCursor<cObj> objs,const RString& h,const RString& name,RDebug *debug=0);

	/**
	* Initialization of the instance.
	*/
	virtual void Init(void);

	/**
	* Return the heuristic type.
	*/
	inline RString GetHeuristic(void) const {return(Heuristic);}

	/**
	 * Look if a local optimization is performed.
	 * @return true or false.
	 */
	inline bool MustLocalOptimisation(void) const {return(DoLocalOptimisation);}

	/**
	 * Set if a local optimization must be done or not. By default, the GGA
	 * does local optimization.
	 * @param localopti      Perform local optimization?
	 */
	void SetLocalOptimisation(bool localopti);

	/**
	* Look if the groups containing objects that are also contained in groups
	* inserted during a crossover must be completely emptied.
	* @return true if the groups must be emptied.
	*/
	bool GetEmptyModifiedGroups(void) const {return(EmptyModifiedGroups);}

	/**
	* Set if the groups containing objects that are also contained in groups
	* inserted during a crossover must be completely emptied.
	* @param empty          If true, the groups must be emptied.
	*/
	void SetEmptyModifiedGroups(bool empty) {EmptyModifiedGroups=empty;}

	/**
	* Get the Objects to group.
	* @return Pointer to the objects.
	*/
	RCursor<cObj> GetObjs(void) const {return(RCursor<cObj>(Objs));}

	/**
	* Create a heuristic object.
	*/
	virtual RGroupingHeuristic<cGroup,cObj,cChromo>* CreateHeuristic(void);

	/**
	* Analyze the population to find the best chromosome of the population and
	* to verify if the best chromosome ever calculated has to replaced. When
	* needed, the chromosomes are evaluated.
	*
	* This method handles the chromosomes that are representing the same solutions. If two
	* same chromosomes are detect, one of them is replace a random based one
	* with a probability of 90%.
	*/
	virtual void AnalysePop(void);

	/**
	* Random construction of the chromosomes. Call the method of RInst and
	* handles the same solutions after.
	*/
	virtual void RandomConstruct(void);

	/**
	* This function does the crossovers for a generation. Call the method of
	* RInst and handles the same solutions after.
	*/
	virtual void Crossover(void);

	/**
	* This function does the necessary mutations for a generation. Call the
	* method of RInst and handles the same solutions after.
	*/
	virtual void Mutation(void);

	/**
	* This function does the necessary inversions for a generation. Call the
	* method of RInst and handles the same solutions after.
	*/
	virtual void Inversion(void);

	/**
	* Destruct the instance.
	*/
	virtual ~RInstG(void);

	// friend classes
	friend class RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
	friend class RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rinstg.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
