/*

	R Project Library

	RPlacementCenter.h

	Center Heuristic for Placement - Header

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
#ifndef RPlacementCenter_H
#define RPlacementCenter_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpromethee.h>
#include <rplacementheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPlacementCenter class provides the center heuristic.
* The heuristic places the first object in the center of the rectangle
* representing the limits. After that, the objects are placed so that the
* resulting rectangle is proportional to the limits.<BR>
* The heuristic doubles the limits of the result during the placement. The
* PostRun method make then a translation of the bottom-left coin of the
* resulting rectangle to (0,0).
* @author Pascal Francq
* @short Center Placement Heuristic.
*/
class RPlacementCenter : public RPlacementHeuristic
{
	/**
	* The list of all the polygons representing objects already placed.
	*/
	RPolygons Sol;

	/**
	* The union of the placed objects.
	*/
	RPolygon Union;

	/**
	* The attraction point, the middle of the limits rectangle.
	*/
	RPoint Attraction;

public:

	/**
	* Construct the center heuristic.
	* @param maxobjs        The maximal number of objects to place.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random generator to use.
	* @param dist           Distance criteria parameters.
	* @param area           Area criteria parameters.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementCenter(size_t maxobjs,bool calc,bool use,RRandom* r,RParamStruct* dist,RParamStruct* area,bool ori=false);

	/**
	* Initialize the heuristic.
	* @param prob            The problem.
	* @param layout          Layout.
	* @param grid            Grid.
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

	/**
	* Do some operations after the run.
	* @param limits         Limits of the area.
	*/
	virtual void PostRun(void);

	/**
	* Destruct the heuristic.
	*/
	virtual ~RPlacementCenter(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
