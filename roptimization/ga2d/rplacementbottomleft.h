/*

	R Project Library

	RPlacementBottomLeft.h

	Bottom Left Heuristic for Placement - Header

	Copyright 1998-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
	RPoint Actual;

	/**
	* Where the next line begin.
	*/
	RPoint Last;

public:

	/**
	* Construct the bottom-left heuristic.
	* @param maxobjs        Maximal number of objects to place.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random genrator to use.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementBottomLeft(size_t maxobjs,bool calc,bool use,RRandom* r,bool ori=false);

	/**
	* Initialize the heuristic.
	* @param prob           The problem.
	* @param infos          Pointer to the geometric information.
	* @param grid           Pointer to the grid.
	*/
	virtual void Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid);

	/**
	* Calculate the position to place the next object for a specific geometric
	* information. The function have to register the valid positions with the
	* 'AddValidPosition' method.
	*/
	virtual void NextObjectOri(void);

	/**
	* Place the current object to a specific position. This function is called
	* by the NextObject method.<BR>
	* This function is responsible to update Result.
	* @param pos            The position where to place it.
	*/
	virtual void Place(RPoint& pos);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
