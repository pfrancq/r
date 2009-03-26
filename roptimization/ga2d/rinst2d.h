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
#include <rconnections.h>
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
public:

	/**
	* Number of objects to place.
	*/
	size_t NbObjs;

	/**
	* Array of identifiers for objects.
	*/
	size_t* Order;

	/**
	* Array of identifiers for objects.
	*/
	size_t* Order2;

	/**
	* Temporary array of pointers to Objects (Crossover & Mutation).
	*/
	RObj2D** tmpObjs;

	/**
	* Temporary objects container.
	*/
	RObj2DContainer* tmpObj1;

	/**
	* Temporary objects container.
	*/
	RObj2DContainer* tmpObj2;

	/**
	* Temporary geometric informations.
	*/
	RGeoInfos* tmpInfos;

	/**
	* Heuristic Used.
	*/
	RPlacementHeuristic* Heuristic;

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
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo>
	class RInst2D : public RInst<cInst,cChromo,cFit,cThreaData>
{
public:

	/**
	* The problem.
	*/
	RProblem2D* Problem;

	/**
	* Objects to place.
	*/
	RCursor<RObj2D> Objs;

	/**
	* Number of objects.
	*/
	size_t NbObjs;

	/**
	* Local Optimization.
	*/
	bool bLocalOpti;

	/**
	* Connections for the objects.
	*/
	RConnections* Cons;

protected:

	/**
	* Name of the heuristic that used.
	*/
	RString Heuristic;

public:

	/**
	* Point representing the limits for the placement.
	*/
	RPoint Limits;

	/**
	* Construct the instance. The instance is not responsible for the desallocation
	* of the objects to place.
	* @param popsize        The size of the population.
	* @param prob           Pointer to the problem.
	* @param h              Name of the heuristic that has to be used.
	* @param name           Name of the genetic algorithm.
	* @param debug          Debugger.
	*/
	RInst2D(size_t popsize,RProblem2D* prob,const RString& h,const RString& name,RDebug* debug=0);

	/**
	* Initialization of the instance.
	*/
	virtual void Init(void);

	/**
	* Return the limits for the placement.
	*/
	inline RPoint GetLimits(void);

	/**
	* Return the heuristic type.
	*/
	RString GetHeuristic(void) const {return(Heuristic);}

	/**
	* Set the parameters for the "area" criterion.
	* @param params         The parameters.
	*/
	void SetAreaParams(const RParam& params);

	/**
	* Set the parameters for the "area" criterion.
	* @param params         The parameters.
	*/
	void SetDistParams(const RParam& params);

	/**
	* Return true if a local optimization is needed.
	*/
	inline bool LocalOpti(void) { return(bLocalOpti); }
};


//------------------------------------------------------------------------------
// Template implementation
#include <rinst2d.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
