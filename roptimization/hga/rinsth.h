/*

	R Project Library

	RInstH.h

	Class representing an instance of a HGA - Header

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
#ifndef RInstHH
#define RInstHH


//------------------------------------------------------------------------------
// include files for R Project
#include <rinst.h>
#include <rtreeheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for a Hierarchical Genetic
* Algorithm (HGA).
* @author Pascal Francq
* @short HGA "thread-dependent" Data.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	class RThreadDataH : public RThreadData<cInst,cChromo>
{
protected:

	/**
	* Use for Nodes manipulation functions.
	*/
	cNode** tmpNodes1;

	/**
	* Use for Nodes manipulation functions.
	*/
	cNode** tmpNodes2;

	/**
	* Use for objects manipulation functions.
	*/
	RVectorInt<size_t,true> tmpObjects;

	/**
	* Heuristic Used.
	*/
	RTreeHeuristic<cNode,cObj,cChromo>* Heuristic;

public:

	/**
	* Construct the data.
	* @param owner           Owner of the data.
	*/
	RThreadDataH(cInst *owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadDataH(void);

	friend class RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>;
	friend class RInstH<cInst,cChromo,cFit,cThreadData,cNode,cObj>;
};


//------------------------------------------------------------------------------
/**
 * The RInstH class proposes a generic instance for a Hierarchical Genetic
 * Algorithm (HGA).
 * @author Pascal Francq
 * @short HGA Instance.
 */
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	class RInstH : public RInst<cInst,cChromo,cFit,cThreadData>
{
protected:

	/**
	* Type of the heuristic that used.
	*/
	RString Heuristic;

	/**
	* Objects to group.
	*/
	RCursor<cObj> Objs;

	/**
	 * Maximum number of attributes for an object.
	 */
	size_t MaxAttr;

	/**
	 * Highest identifier of an attribute.
	 */
	size_t ControlAttr;

	/**
	 * Maximum number of nodes to allocate by default.
	 */
	size_t MaxNodes;

public:

	/**
	* Construct the instance.
	* @param popsize        Size of the population.
	* @param objs           Objects to place in the tree.
	* @param h              The heuristic that has to be used.
	* @param name           Name of the genetic algorithm.
	* @param debug          Debugger.
	*/
	RInstH(size_t popsize,RCursor<cObj> objs,const RString& h,const RString& name,RDebug* debug=0);

	/**
	 * Return the name of the class.
	 */
	virtual RCString GetClassName(void) const {return("RInstH");}

	/**
	* Initialization of the instance.
	*/
	virtual void Init(void);

	/**
	* Return the heuristic type.
	*/
	inline RString GetHeuristic(void) const {return(Heuristic);}

	/**
	 * Get the maximum number of attributes associated to an object.
	 */
	size_t GetMaxAttr(void) const {return(MaxAttr);}

	/**
	 * Get the heuristic of a given thread.
	 * @param thread         Number of the thread.
	 */
	inline RTreeHeuristic<cNode,cObj,cChromo>* GetHeuristic(size_t thread) {return(thDatas[thread]->Heuristic);}

	/**
	* Create a heuristic object.
	*/
	virtual RTreeHeuristic<cNode,cObj,cChromo>* CreateHeuristic(void);

	/**
	* This function do a traitement after the GA stops.
	*/
	virtual void PostRun(void);

	/**
	* Display information about the current population.
	*/
	virtual void DisplayInfos(void);

	/**
	* Destruct the instance.
	*/
	virtual ~RInstH(void);

	friend class RThreadDataH<cInst,cChromo,cFit,cThreadData,cNode,cObj>;
	friend class RChromoH<cInst,cChromo,cFit,cThreadData,cNode,cObj>;
};


//------------------------------------------------------------------------------
// template implementation
#include <rinsth.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
