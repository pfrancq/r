/*

	R Project Library

	RInst2D.h

	Instance for 2D placement GA - Header

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RInst2D_H
#define RInst2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rinst.h>
#include <rga2d.h>
#include <rplacementheuristic.h>
#include <rplacementbottomleft.h>
#include <rplacementedge.h>
#include <rplacementcenter.h>
#include <rproblem2d.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This is a class that holds all data needed by each thread for the 2D GA.
* @author Pascal Francq
* @short 2D GA "thread-dependent" data.
*/
template<class cInst,class cChromo>
	class RThreadData2D : public RThreadData<cInst,cChromo>
{
	using RThreadData<cInst,cChromo>::Owner;

public:

	/**
	* Heuristic Used.
	*/
	RPlacementHeuristic* Heuristic;

	/**
	* Temporary array to remember which objects are selected.
	*/
	bool* Selected;

	/**
	 * PROMETHEE Kernel.
	 */
	RPromKernel Kernel;

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	RThreadData2D(cInst* owner);

	/**
	* Initialize the data.
	*/
	virtual void Init(void);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadData2D(void);
};


//------------------------------------------------------------------------------
/**
* Instance for the 2D placement GA.
* @author Pascal Francq
* @short 2D GA Instance.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	class RInst2D : public RInst<cInst,cChromo,cFit,cThreadData>
{
protected:

	using RInst<cInst,cChromo,cFit,cThreadData>::thDatas;
	using RInst<cInst,cChromo,cFit,cThreadData>::BestChromosome;

	/**
	* The problem.
	*/
	RProblem2D* Problem;

	/**
	* Local Optimization.
	*/
	bool bLocalOpti;

	/**
	* Name of the heuristic that used.
	*/
	RString Heuristic;

	/**
	* Calculate free polygons.
	*/
	bool ComputeFreePolygons;

	/**
	* Use free polygons.
	*/
	bool UseFreePolygons;

	/**
	* Test all possible orientation.
	*/
	bool AllOrientations;

	/**
	* PROMETHEE Parameters for Heuristic Distance.
	*/
	RParamStruct* HeurDist;

	/**
	* PROMETHEE Parameters for Heuristic Area.
	*/
	RParamStruct* HeurArea;

	/**
	* PROMETHEE Parameters for Selection Distance.
	*/
	RParamStruct* SelectDist;

	/**
	* PROMETHEE Parameters for Selection Weight.
	*/
	RParamStruct* SelectWeight;

public:

	/**
	* Construct the instance. The instance is not responsible for the deallocation
	* of the objects to place.
	* @param popsize        The size of the population.
	* @param prob           Pointer to the problem.
	* @param h              Name of the heuristic that has to be used.
	* @param name           Name of the genetic algorithm.
	* @param heurdist       Distance criteria parameters for the heuristic.
	* @param heurarea       Area criteria parameters for the heuristic.
	* @param selectdist     Distance criteria parameters for the objects
	*                       selection.
	* @param selectweight   Weight criteria parameters for the objects
	*                       selection.
	* @param debug          Debugger.
	*/
	RInst2D(size_t popsize,RProblem2D* prob,const RString& h,const RString& name,
			RParamStruct* heurdist,RParamStruct* heurarea,RParamStruct* selectdist,RParamStruct* selectweight,
			RDebug* debug=0);

	/**
	* Initialization of the instance.
	*/
	virtual void Init(void);

	/**
	* Return the heuristic type.
	*/
	RString GetHeuristic(void) const {return(Heuristic);}

	/**
	* Return true if a local optimization is needed.
	*/
	inline bool LocalOpti(void) { return(bLocalOpti); }

	friend class RThreadData2D<cInst,cChromo>;
	friend class RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rinst2d.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
