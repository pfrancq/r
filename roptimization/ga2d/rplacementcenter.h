/*

	Rainbow Library Project

	RPlacementCenter.h

	Center Heuristic for Placement - Header

	(C) 1998-2000 by P. Francq.

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
#ifndef RPlacementCenterH
#define RPlacementCenterH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rplacementheuristic.h"


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPlacementCenter class provides the center heuristic.
*
* The heuristic places the first object in the center of the rectangle
* representing the limits. After that, the objects are placed so that the
* resulting rectangle is proportional to the limits.
*
* The heuristic doubles the limits of the result during the placement. The
* PostRun method make then a translation of the bottom-left coin of the
* resulting rectangle to (0,0).
* @author Pascal Francq
* @short Generic Placement Heuristic class.
*/
class RPlacementCenter : public RPlacementHeuristic
{
	/**
	* The ICalcPos structure represents a valid position for an object.
	*/
	struct ICalcPos
	{
		RPoint Pos;
		RCoord Area;
		RCoord Dist;
	};

	/**
	* An array of calculate position for an element.
	*/
   ICalcPos CalcPos[100];
	
   /**
	* Number of calculated position.
	*/
	unsigned int NbCalcPos;

	/**
	* The list of all the polygons representing objects already placed.
	*/
	RPolygons Sol;

	/**
	* The union of the placed objects.
	*/
	RPolygon Union;

	/**
	* Size of the resulting rectangle.
	*/
	RPoint Max;

	/**
	* The attraction point, the middle of the limits rectangle.
	*/
	RPoint Attraction;

	/**
	* Holds the limits of the problem, because the heuristic double them for the
	* placement.
	*/
	RPoint HoldLimits;

	/**
	* Minimal distance between current position and best position if total area
	* is smaller.
	*/
	RCoord MinDist;

public:

	/**
	* Construct the center heuristic.
	* @param maxobjs		The maximal number of objects to place.
	* @param calcfree		Must free polygons be calculated.
	*/
	RPlacementCenter(unsigned int maxobjs,bool calcfree,RCoord mindist=15);

	/**
	* Initialize the heuristic.
	* @param limits		Limits for the placement.
	* @param grid			Pointer to the grid.
	* @param objs			Pointer to the objects.
	* @param infos			Pointer to the geometric information.
	* @param nbobjs		Number of objects to place.
	*/
	virtual void Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs);

	/**
	* Calculate all the possible position for the current object.
	*/
	void CalcPositions(void);

	/**
	* Place the next object for a specific information.
	* @return		The function returns true if the object can be placed with the
	*					current orientation.
	*/
	virtual bool NextObjectOri(void);

	/**
	*	Do some operations after the run.
	*/
	virtual void PostRun(RPoint &limits);
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif