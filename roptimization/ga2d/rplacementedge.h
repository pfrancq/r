/*

	R Project Library

	RPlacementEdge.h

	Edge Heuristic for Placement - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

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
#ifndef RPlacementEdge_H
#define RPlacementEdge_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga/rplacementheuristic.h>


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
	* @param maxobjs        The maximum objects to placed fore this object.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random genrator to use.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementEdge(unsigned int maxobjs,bool calc,bool use,RRandom* r,bool ori=false);

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
	virtual void NextObjectOri(void) throw(RPlacementHeuristicException);
	
	/**
	* Place the current object to a specific position. This function is called
	* by the NextObject method.<BR>
	* This function is responsible to update Result.
	* @param pos            The position where to place it.
	*/
	virtual void Place(RPoint& pos) throw(RPlacementHeuristicException);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
