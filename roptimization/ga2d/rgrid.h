/*

	Rainbow Library Project

	RGrid.h

	Grid for the 2D placement - Header

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



//---------------------------------------------------------------------------
#ifndef RGridH
#define RGridH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
using namespace RGeometry2D;
#include "robj2d.h"


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
/**
* The RGrid class provides some functions to manipulate a grid of geometric
* points.
*	@author Pascal Francq
* @short Grid class.
*/
class RGrid
{
	/**
	* Limits for the construction.
	*/
	RPoint Limits;

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
	* @param limits		Representing the maximal limits for the grid. In fact,
	* 									internally, the grid is allocate 2x the size to be used
	*									by the "center" heuristic.
	*/	
	RGrid(RPoint &limits) throw(bad_alloc);

	/**
	* This function returns true if the position (x,y) is free.
	*/
	inline bool IsFree(RCoord x,RCoord y)
	{RReturnValIfFail(OccupiedX,false); return(OccupiedX[x][y]==NoObject);}

	/**
	* This function returns true if the position (x,y) is occupied.
	*/
	inline bool IsOcc(RCoord x,RCoord y)
	{RReturnValIfFail(OccupiedX,false); return(OccupiedX[x][y]!=NoObject);}
	
	/**
	* This function returns the identificator of the object placed at position
	* (x,y) or NoObject if no object is placed there.
	*/
   unsigned int GetObjId(RCoord x,RCoord y)
	{RReturnValIfFail(OccupiedX,false); return(OccupiedX[x][y]);}

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
	* Add all the polygons representing free spaces that are closed and
	* resulting of the placement of an object.
	* @param ins		The geometric information representing the last inserted
	*								object.
	* @param polys	The container of polygons where all the free spaces
	*								calculated must be add.
	* @param bound	The bounding rectangle of the objects already placed.
	*/
	void AddFreePolygons(RGeoInfo *ins,RPolygons *polys,RRect &bound);

	/**
	* Destruct the Grid.
	*/
	~RGrid(void);
};


}//------- End of namespace RGA ---------------------------------------------


//---------------------------------------------------------------------------
#endif