/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Header

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
#ifndef RPlacementHeuristic_H
#define RPlacementHeuristic_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rpromkernel.h>
#include <rpoint.h>
#include <rrect.h>
#include <rga2d.h>
#include <robj2d.h>
#include <rgeoinfo.h>
#include <rlayout.h>
#include <rgrid.h>
#include <rfreepolygons.h>
#include <rproblem2d.h>
#include <rpromcriterion.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPlacementHeuristic class provides an abstract class for placement
* heuristics.
* @author Pascal Francq
* @short Generic Placement Heuristic class.
*/
class RPlacementHeuristic
{
	class ObjectPos;

protected:

	/**
	* The random number generator to use
	*/
	RRandom* Random;

	/**
	* The grid used for the placement.
	*/
	RGrid* Grid;

	/**
	* The layout.
	*/
	RLayout* Layout;

	/**
	* The limits for the placement.
	*/
	RSize Limits;

	/**
	* Total number of objects to place.
	*/
	size_t NbObjs;

	/**
	* Number of objects already placed.
	*/
	size_t NbObjsOk;

	/**
	* The bounding rectangle of the resulting placement. It must be updated
	* by the inheriting classes.
	*/
	RRect Result;

	/**
	* The sum of the connections distances.
	*/
	double Distances;

	/**
	* Order in which the objects are to be treated.
	*/
	RGeoInfo** Order;

	/**
	* Geometric information of the current object to place.
	*/
	RGeoInfo* CurInfo;

	/**
	* Free Polygons resulting of the placement.
	*/
	RFreePolygons Free;

	/**
	* Calculations of free polygons.
	*/
	bool CalcFree;

	/**
	* Use of free polygons.
	*/
	bool UseFree;

	/**
	* All possible orientation must be tested.
	*/
	bool AllOri;

	/**
	* Valid positions.
	*/
	RContainer<ObjectPos,true,false> ValidPos;

	/**
	 * PROMERHEE kernel.
	 */
	RPromKernel Prom;

	/**
	 * PROMETHEE distance criterion.
	 */
	RPromCriterion* Dist;

	/**
	* PROMETHEE Parameters for the distance.
	*/
	const RParam* DistParams;

	/**
	 * PROMETHEE area criterion.
	 */
	RPromCriterion* Area;

	/**
	* PROMETHEE Parameters for the area.
	*/
	const RParam* AreaParams;

public:

	/**
	* Construct the placement heuristic.
	* @param maxobjs        The maximum objects to placed fore this object.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random generator to use.
	* @param dist           Distance criteria parameters.
	* @param area           Area criteria parameters.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementHeuristic(size_t maxobjs,bool calc,bool use,RRandom* r,RParamStruct* dist,RParamStruct* area,bool ori=false);

	/**
	* Initialize the heuristic. The method chooses all the objects to place. In
	* particular, if an object is associated to an aggregator, it is not placed
	* and the method supposes that the aggregator itself is to placed.
	* @param prob            The problem.
	* @param layout          Layout.
	* @param grid            Pointer to the grid.
	*/
	virtual void Init(RProblem2D* prob,RLayout* layout,RGrid* grid);

protected:

	/**
	* Select the next object to place. By default, the method chooses the first
	* object randomly. Then, the next objects are chosen according to their
	* connections (the most connection) with those already placed.
	* @return the geometric information of the object to place.
	*/
	virtual RGeoInfo* SelectNextObject(void);

	/**
	* Calculate all the possible positions to place a given object. The method
	* must register these valid positions with the 'AddValidPosition' method.
	* @param info           Geometric information representing the object placed.
	*/
	virtual void SearchValidPositions(RGeoInfo* info)=0;

	/**
	* This method is called each time a given object is placed at a given
	* position. It can be used	to make some specific computational updates. By
	* default, the method does nothing.
	*
	* This method must update Result, the rectangle bounding all placed
	* objects.
	*
	* @param info           Geometric information representing the object placed.
	* @param pos            The position where it is placed.
	*/
	virtual void PostPlace(RGeoInfo* info,const RPoint& pos)=0;

public:

	/**
	 * Select and place an unassigned object. The steps followed by this method
	 * are:
	 * -# Select an object with the 'SelectNextObject' method.
	 * -# Look if a free polygon can hold the object (if managed).
	 * -# If not, for every possible orientation of the object, the method
	 *    'SearchValidPositions' is called to find all valid positions.
	 * -# If multiple positions are possible, PROMETHEE is used to select the
	 *    best one : the position minimizing the total area occupied and the
	 *    total distance of the connections.
	 * -# The object is placed and 'PostPlace' is called.
	 * -# If free polygons are managed, update them.
	 * @return the geometric information of the object placed.
	 */
	RGeoInfo* NextObject(void);

	/**
	* Run the heuristic.
	* @param prob            The problem.
	* @param layout          Layout.
	* @param grid            Grid.
	*/
	void Run(RProblem2D* prob,RLayout* layout,RGrid* grid);

	/**
	* Add a valid position fot the current geometric information to place.
	* @param pos            Position to be a valid.
	*/
	void AddValidPosition(RPoint& pos);

	/**
	* Do some operations after the run.
	*/
	virtual void PostRun(void);

	/**
	* Return the bound rectangle containing all the objects.
	*/
	RRect GetResult(void);

	/**
	* Return the sum of the distances of all the connections.
	*/
	double GetDistances(void) {return(Distances);}

	/**
	* Return true if all the objects are placed.
	*/
	inline bool IsEnd(void) {return(NbObjsOk==NbObjs);}

	/**
	* Return the number of objects placed.
	*/
	inline size_t GetNbObjsOk(void) {return(NbObjsOk);}

	/**
	* Return a pointer to all the free polygons.
	*/
	inline RFreePolygons* GetFreePolygons(void) {return(&Free);}

	/**
	* Destruct the placement heuristic.
	*/
	virtual ~RPlacementHeuristic(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
