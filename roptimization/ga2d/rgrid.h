/*

	R Project Library

	RGrid.h

	Grid for the 2D placement - Header

	(C) 1998-2001 by P. Francq.

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
#ifndef RGridH
#define RGridH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
using namespace RGeometry2D;
#include <rga2d/robj2d.h>
#include <rga2d/rfreepolygon.h>
#include <rga2d/rfreepolygons.h>
using namespace RGA2D;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RGrid class provides some functions to manipulate a grid of geometric
* points.
* @author Pascal Francq
* @short Grid class.
*/
class RGrid
{
	/**
	* Limits for the construction.
	*/
	RPoint Limits;

	/**
	* Internal Limits.
	*/
	RPoint InternalLimits;

	/**
	* Grid with X as entry.
	*/
	unsigned int **OccupiedX;

	/**
	* Grid with Y as entry.
	*/
	unsigned int **OccupiedY;

public:

	/**
	* Construct the grid.
	* @param limits	Representing the maximal limits for the grid. In fact,
	* 						internally, the grid is allocate 2x the size to be used
	*						by the "center" heuristic.
	*/	
	RGrid(RPoint &limits) throw(bad_alloc);

	/**
	* This function returns true if the position (x,y) is free or if it is out
	* of the limits.
	* @param x		X position.
	* @param y		Y position.
	* @returns		The function return true if the point is out of the limits
	*				or if the position is free.
	*/
	bool IsFree(RCoord x,RCoord y);

	/**
	* This function returns true if the position (x,y) is occupied.
	* @param x		X position.
	* @param y		Y position.
	* @returns	The function returns true if the position is occupied, false
	*			else (also if the point is out of the limits).
	*/
	bool IsOcc(RCoord x,RCoord y);
	
	/**
	* This function returns the identificator of the object placed at position
	* (x,y) or NoObject if no object is placed there.
	* @param x		X position.
	* @param y		Y position.
	*/
   	unsigned int GetObjId(RCoord x,RCoord y) {RReturnValIfFail(OccupiedX,false); return(OccupiedX[x][y]);}

	/**
	* Clear the Grid.
	*/
	void Clear(void);

	/**
	* Assign a rectangle to the grid.
	* @param rect		The rectangle to assign.
	* @param pos		The position.
	* @param id			The identificator to put.
	*/
	void Assign(RRect &rect,RPoint &pos,unsigned int id);
	
	/**
	* Find the left most x coordinates that is free of a reference.
	* @param pt		Point used as reference.
	*/
	RCoord LookLeft(RPoint& pt);

	/**
	* Find the right most x coordinates that is free of a reference.
	* @param pt		Point used as reference.
	*/
	RCoord LookRight(RPoint& pt);

	/**
	* Find the up most x coordinates that is free of a reference.
	* @param pt		Point used as reference.
	*/
	RCoord LookUp(RPoint& pt);

	/**
	* Find the down most x coordinates that is free of a reference.
	* @param pt		Point used as reference.
	*/
	RCoord LookDown(RPoint& pt);

	/**
	* Find the left most x coordinates that is free and that skirt an edge.
	* @param pt			Point used as reference.
	* @param bound	   The rectangle which limits the research.
	*/
	RCoord SkirtLeft(RPoint& pt,RRect &bound);

	/**
	* Find the right most x coordinates that is free and that skirt an edge.
	* @param pt			Point used as reference.
	* @param bound	   The rectangle which limits the research.
	*/
	RCoord SkirtRight(RPoint& pt,RRect &bound);

	/**
	* Find the up most x coordinates that is free and that skirt an edge.
	* @param pt			Point used as reference.
	* @param bound	   The rectangle which limits the research.
	*/
	RCoord SkirtUp(RPoint& pt,RRect &bound);

	/**
	* Find the down most x coordinates that is free and that skirt an edge.
	* @param pt		Point used as reference.
	* @param bound	   The rectangle which limits the research.
	*/
	RCoord SkirtDown(RPoint& pt,RRect &bound);

	/**
	* Calculate a free polygon from a given position and a given direction.
	* @param X			X-Coordinate of the position.
	* @param Y			Y-Coordinate of the position.
	* @param from		The direction from where "it" is coming.
	* @param bound		The bound rectangle.
	* @param poly		The polygon to calculated.
	* @returns	The function returns true if a close polygon has been calculated
	*				, or false if not.
	*
	*/
	bool CalculateFreePolygon(RCoord X,RCoord Y,RDirection from,RRect &boud,RPolygon& poly);

	/**
	* Add all the polygons representing free spaces that are closed and resulting
	* of the placement of an object.
	* @param ins		The geometric information representing the last inserted
	*								object.
	* @param polys	The container of polygons where all the free spaces
	*								calculated must be add.
	* @param bound	The bounding rectangle of the objects already placed.
	*/
	void AddFreePolygons(RGeoInfo *ins,RFreePolygons *free,RRect &bound);

	/**
	* Destruct the Grid.
	*/
	~RGrid(void);
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
