/*

	R Project Library

	RGrid.h

	Grid for the 2D placement - Header

	Copyright 1998-2011 by Pascal Francq (pascal@francq.info).
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
#ifndef RGrid_H
#define RGrid_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rpoint.h>
#include <rsize.h>
#include <robj2d.h>
#include <rfreepolygon.h>
#include <rfreepolygons.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RGrid class provides some functions to manipulate a grid of geometric
* points.
* @author Pascal Francq
* @short Grid.
*/
class RGrid
{
	/**
	* Limits for the construction.
	*/
	RSize Limits;

	/**
	* Internal Limits.
	*/
	RSize InternalLimits;

	/**
	* Grid with X as entry.
	*/
	size_t** OccupiedX;

	/**
	* Grid with Y as entry.
	*/
	size_t** OccupiedY;

public:

	/**
	* Construct the grid.
	* @param limits         Representing the maximal limits for the grid. In fact,
	*                       internally, the grid is allocate 2x the size to be used
	*                       by the "center" heuristic.
	*/
	RGrid(const RSize& limits);

	/**
	* Clear the Grid.
	*/
	void Clear(void);

	/**
	* Assign a rectangle to the grid.
	* @param rect           The rectangle to assign.
	* @param pos            The position.
	* @param id             The identifier to put.
	*/
	void Assign(RRect& rect,RPoint& pos,size_t id);

	/**
	* This function returns true if the position (x,y) is free or if it is out
	* of the limits.
	* @param x              X position.
	* @param y              Y position.
	* @returns The function return true if the point is out of the limits
	* or if the position is free.
	*/
	bool IsFree(tCoord x,tCoord y) const;

	/**
	* This function returns true if the position (x,y) is occupied.
	* @param x              X position.
	* @param y              Y position.
	* @returns The function returns true if the position is occupied, false
	* else (also if the point is out of the limits).
	*/
	bool IsOcc(tCoord x,tCoord y) const;

	/**
	* This function returns true if at least one position corresponding to a
	* given rectangle is occupied.
	* @param rect            Rectangle.
	* @returns The function returns true if the position is occupied, false
	* else (also if the point is out of the limits).
	*/
	bool IsOcc(const RRect& rect) const;

	/**
	 * Get the maximal X size allocated.
	 * @return size_t
	 */
	inline size_t GetMaxX(void) const {return(static_cast<size_t>(InternalLimits.GetWidth())+1);}

	/**
	 * Get the maximal Y size allocated.
	 * @return size_t
	 */
	inline size_t GetMaxY(void) const {return(static_cast<size_t>(InternalLimits.GetHeight())+1);}

	/**
	* This function returns the identifier of the object placed at position
	* (x,y) or cNoRef if no object is placed there.
	* @param x              X position.
	* @param y              Y position.
	*/
	inline size_t GetObjId(tCoord x,tCoord y) const
	{
		RReturnValIfFail(OccupiedX,false);
		return(OccupiedX[static_cast<size_t>(x)][static_cast<size_t>(y)]);
	}

	/**
	* Find the left most x coordinates that is free of a reference.
	* @param pt             Point used as reference.
	*/
	tCoord LookLeft(const RPoint& pt) const;

	/**
	* Find the right most x coordinates that is free of a reference.
	* @param pt             Point used as reference.
	*/
	tCoord LookRight(const RPoint& pt) const;

	/**
	* Find the up most x coordinates that is free of a reference.
	* @param pt             Point used as reference.
	*/
	tCoord LookUp(const RPoint& pt) const;

	/**
	* Find the down most x coordinates that is free of a reference.
	* @param pt             Point used as reference.
	*/
	tCoord LookDown(const RPoint& pt) const;

	/**
	* Find the left most x coordinates that is free and that skirt an edge.
	* @param pt             Point used as reference.
	* @param bound          The rectangle which limits the research.
	*/
	tCoord SkirtLeft(const RPoint& pt,const RRect& bound) const;

	/**
	* Find the right most x coordinates that is free and that skirt an edge.
	* @param pt             Point used as reference.
	* @param bound          The rectangle which limits the research.
	*/
	tCoord SkirtRight(const RPoint& pt,const RRect& bound) const;

	/**
	* Find the up most x coordinates that is free and that skirt an edge.
	* @param pt             Point used as reference.
	* @param bound          The rectangle which limits the research.
	*/
	tCoord SkirtUp(const RPoint& pt,const RRect& bound) const;

	/**
	* Find the down most x coordinates that is free and that skirt an edge.
	* @param pt             Point used as reference.
	* @param bound          The rectangle which limits the research.
	*/
	tCoord SkirtDown(const RPoint& pt,const RRect& bound) const;

	/**
	* Calculate a free polygon from a given position and a given direction.
	* @param X              X-Coordinate of the position.
	* @param Y              Y-Coordinate of the position.
	* @param from           The direction from where "it" is coming.
	* @param bound          The bound rectangle.
	* @param poly           The polygon to calculated (modified.
	* @returns The function returns true if a close polygon has been calculated
	* or false if not.
	*/
	bool CalculateFreePolygon(tCoord X,tCoord Y,tDirection from,const RRect& bound,RPolygon& poly) const;

	/**
	* Add all the polygons representing free spaces that are closed and resulting
	* of the placement of an object.
	* @param ins            The geometric information representing the last inserted
	*                       object.
	* @param free           The container of polygons where all the free spaces
	*                       calculated must be add.
	* @param bound          The bounding rectangle of the objects already placed.
	*/
	void AddFreePolygons(RGeoInfo* ins,RFreePolygons* free,const RRect& bound);

	/**
	* Destruct the Grid.
	*/
	~RGrid(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
