/*

	Rainbow Library Project

	RPlacementEdge.h

	Edge Heuristic for Placement - Header

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
#ifndef RPlacementEdgeH
#define RPlacementEdgeH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rplacementheuristic.h"


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPlacementEdge class provides an edge placement heuristic, it is a
* modified version of the bottom-left heuristic where the resulting rectangle
* have a width and a height in proportiion to the limits.
*
* This heuristic hanbles objects in the edge. The first object is place
* in bottom-left part of the rectangle representing the limits. After that, the
* heuristic try to place the next object either at the right or on the top of
* the first one depending of the width and height of the first object. The next
* objects are then place to try to fill the resulting rectangle while his width
* and height are proportional to the limits.
* @author Pascal Francq
* @short Edge Heuristic class.
*/
class RPlacementEdge : public RPlacementHeuristic
{
	/**
	* Array to hold the different lines actually handled by the heuristic.
	*/
	RPoint Levels[40];

	/**
	* The number of total lines handled.
	*/
	unsigned int NbLevels;

	/**
	* Position where the next object has to be placed.
	*/
	RPoint Actual;

	/**
	* The dimension of the resulting rectangle.
	*/
	RPoint Max;

	/**
	* Indicate where a new line has to be added.
	*/
	RPoint Last;

	/**
	* Current line handled by the heuristic.
	*/
	unsigned int CurLevel;

public:

	/**
	* Construct the edge heuristic.
	*/
	RPlacementEdge(unsigned int maxobjs);

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