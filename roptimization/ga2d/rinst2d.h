/*

	Rainbow Library Project

	RInst2D.h

	Instance for 2D placement GA - Header

	(C) 1999-2000 by P. Francq.

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



//-----------------------------------------------------------------------------
#ifndef RInst2DH
#define RInst2DH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rga2d.h"
#include "rplacementheuristic.h"
#include "rplacementbottomleft.h"
#include "rplacementedge.h"
#include "rplacementcenter.h"
using namespace RGA;


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* \ingroup 2DGA
* This is a class that holds all data needed by each thread for the 2D GA.
* @author Pascal Francq
* @short 2D GA "thread-dependent" data.
*/
template<class cInst,class cChromo>
	class RThreadData2D : public RThreadData<cInst,cChromo>
{
public:

	/**
	* Number of objects to place.
	*/
	unsigned int NbObjs;				

	/**
	* Array of identificators for objects.
	*/
	unsigned int *Order;				

	/**
	* Array of identificators for objects.
	*/
	unsigned int *Order2;

	/**
	* Temporary array of pointers to Objects (Crossover & Mutation).
	*/
	RObj2D **tmpObjs;						

	/**
	* Temporary objects container.
	*/
	RObj2DContainer *tmpObj1; 	

	/**
	* Temporary objects container.
	*/
	RObj2DContainer *tmpObj2;

	/**
	* Heuristic Used.
	*/
	RPlacementHeuristic *Heuristic;

	/**
	* Construct the data.
	* @param owner		The instance of the problem.
	*/
	RThreadData2D(cInst *owner) throw(bad_alloc);

	/**
	* Initialise thje data.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Destruct the data.
	*/
	virtual ~RThreadData2D(void);
};


//-----------------------------------------------------------------------------
/**
* \ingroup 2DGA
* Instance for the 2D placement GA.
* @author Pascal Francq
* @short 2D GA Instance.
*/
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo>
	class RInst2D	: public RInst<cInst,cChromo,cFit,cThreaData>
{
public:

	/**
	* Objects to place.
	*/
	RObj2D **Objs;

	/**
	* Number of objects.
	*/
	unsigned int NbObjs;

	/**
	* Local Optimisation.
	*/
	bool bLocalOpti;

protected:

	/**
	* Type of the heuristic that used.
	*/
	HeuristicType Heuristic;

public:

	/**
	* Point representing the limits for the placement.
	*/
	RPoint Limits;

	/**
	* Construct the instance. The instance is not responsible for the desallocation
	* of the objects to place.
	* @param popsize			The size of the population.
	* @param objs				The objects to place.
	* @param nbobjs			Number of objects to place.
	* @param limits			The limits for the placement.
	* @param h					The heuristic that has to be used.
	*/
	RInst2D(unsigned int popsize,RObj2D** objs,unsigned int nbobjs,RPoint &limits,HeuristicType h) throw(bad_alloc);

	/**
	* Return the limits for the placement.
	*/
	inline RPoint& GetLimits(void);

	/**
	* Return the heuristic type.
	*/
	inline HeuristicType GetHeuristic(void) { return(Heuristic); }

	/**
	* Return true if a local optimisation is needed.
	*/
	inline bool LocalOpti(void) { return(bLocalOpti); }
};


}  //------- End of namespace RGA ---------------------------------------------

//-----------------------------------------------------------------------------
#endif
