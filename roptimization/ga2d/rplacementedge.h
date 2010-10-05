/*

	R Project Library

	RPlacementEdge.h

	Edge Heuristic for Placement - Header

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef RPlacementEdge_H
#define RPlacementEdge_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rplacementheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPlacementEdge class provides an edge placement heuristic.
* The RPlacementEdge class provides an edge placement heuristic, it is a
* modified version of the bottom-left heuristic where the resulting rectangle
* have a width and a height in proportiion to the limits.<BR>
* This heuristic hanbles objects in the edge. The first object is place
* in bottom-left part of the rectangle representing the limits. After that, the
* heuristic try to place the next object either at the right or on the top of
* the first one depending of the width and height of the first object. The next
* objects are then place to try to fill the resulting rectangle while his width
* and height are proportional to the limits.
* @author Pascal Francq
* @short Edge Placement Heuristic.
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
	size_t NbLevels;

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
	size_t CurLevel;

public:

	/**
	* Construct the edge heuristic.
	* @param maxobjs        The maximum objects to placed fore this object.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random generator to use.
	* @param dist           Distance criteria parameters.
	* @param area           Area criteria parameters.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementEdge(size_t maxobjs,bool calc,bool use,RRandom& r,RParamStruct* dist,RParamStruct* area,bool ori=false);

	/**
	* Initialize the heuristic.
	* @param prob           The problem.
	* @param layout         Layout.
	* @param grid           Grid.
	*/
	virtual void Init(RProblem2D* prob,RLayout* layout,RGrid* grid);

	/**
	* Calculate all the possible positions to place a given object. The method
	* must register these valid positions with the 'AddValidPosition' method.
	* @param info           Geometric information representing the object placed.
	*/
	virtual void SearchValidPositions(RGeoInfo* info);

	/**
	* This method is called each time a given object is placed at a given
	* position. It can be used	to make some specific computational updates.
	*
	* This method must update Result, the rectangle bounding all placed
	* objects.
	*
	* @param info           Geometric information representing the object placed.
	* @param pos            The position where it is placed.
	*/
	virtual void PostPlace(RGeoInfo* info,const RPoint& pos);

};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
