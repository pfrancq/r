/*

	Rainbow Library Project

	RPlacementBottomLeft.h

	Bottom Left Heuristic for Placement - Header

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
#ifndef RPlacementBottomLeftH
#define RPlacementBottomLeftH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rplacementheuristic.h"


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPlacementHeuristic class provides an abstract class for placement
* heuristics.
*
* This heuristic hanbdles objects "line after line". The first object is place
* in bottom-left part of the rectangle representing the limits. After that, the
* heuristic try to place so much objects as possible to the right of the first
* object. When it is not more possible, the next object is place top of the
* first one. The heuristic try then to put the rest of the objects to the right
* and so on.
* @author Pascal Francq
* @short Generic Placement Heuristic class.
*/
class RPlacementBottomLeft : public RPlacementHeuristic
{
	/**
	* Position where to place the next object.
	*/
	RPoint Actual;

	/**
	* Where the next line begin.
	*/
	RPoint Last;

public:

	/**
	* Construct the bottom-left heuristic.
	* @param maxobjs		Maximal number of objects to place.
	* @param calcfree		Must free polygons be calculated.
	*/
	RPlacementBottomLeft(unsigned int maxobjs,bool calcfree);

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
	* Place the next object for a specific information.
	* @return		The function returns true if the object can be placed with the
	*					current orientation.
	*/
	virtual bool NextObjectOri(void);
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif