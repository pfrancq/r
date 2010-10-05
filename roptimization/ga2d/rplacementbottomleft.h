/*

	R Project Library

	RPlacementBottomLeft.h

	Bottom Left Heuristic for Placement - Header

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
#ifndef RPlacementBottomLeft_H
#define RPlacementBottomLeft_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rplacementheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a bottom-left heuristic.
* This heuristic handles objects "line after line". The first object is place
* in bottom-left part of the rectangle representing the limits. After that, the
* heuristic try to place so much objects as possible to the right of the first
* object. When it is not more possible, the next object is place top of the
* first one. The heuristic try then to put the rest of the objects to the right
* and so on.
* @author Pascal Francq
* @short Bottom-Left Placement Heuristic.
*/
class RPlacementBottomLeft : public RPlacementHeuristic
{
	/**
	* Position where to place the next object.
	*/
	RPoint NextObj;

	/**
	* Where the next line begin.
	*/
	RPoint NextLine;

public:

	/**
	* Construct the bottom-left heuristic.
	* @param maxobjs        Maximal number of objects to place.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random generator to use.
	* @param dist           Distance criteria parameters.
	* @param area           Area criteria parameters.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementBottomLeft(size_t maxobjs,bool calc,bool use,RRandom& r,RParamStruct* dist,RParamStruct* area,bool ori=false);

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
