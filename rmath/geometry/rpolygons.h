/*

	R Project Library

	Polygons.h

	Polygons - Header.

	(C) 1999-2001 by P. Francq.

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
#ifndef RPolygonsH
#define RPolygonsH


//-----------------------------------------------------------------------------
// include files for RGeometry
#include <rgeometry/geometry.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;


//-----------------------------------------------------------------------------
namespace RGeometry2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represents a set of polygons regrouped in a container.
*	@author Pascal Francq
*	@short Container of polygons.
 */
class RPolygons : public RStd::RContainer<RPolygon,unsigned int,true,false>
{
public:

	/**
	* Construct a container of polygons.
	*/
	RPolygons(void);

	/**
	* Return true if the point is on an edge of one of the polygon contained.
	* @param pt				The point used.
	*/
	bool Edge(RPoint *pt);

	/**
	* Return true if the point is on an edge of a certain polygon.
	* @param pt				The point used.
	* @param poly			The polygon to look in.
	*/
	bool Edge(RPoint *pt,RPolygon *poly);

	/**
	* Return true if two points are on the same edge of one of the polygon contained.
	* @param pt1		The first point used.
	* @param pt2		The second point used.
	*/
	bool Edge(RPoint *pt1,RPoint *pt2);

	/**
	* Add the points of the polygons contained to a container of points.
	* @param points		A pointer to the container of points.
	*/
	void PutPoints(RPoints *points);

	/**
	* This function calculate the union of the polygons contained.
	* @param upoly		A pointer to the polygon representing the union.
	*/
	void Union(RPolygon *upoly);

	/**
	* This function returns true when there are duplicate points.	
	*/
	bool DuplicatePoints(void);

	/**
	* Return true if the point is inside one of the polygons.
	*/
	bool IsIn(const RCoord X,const RCoord Y);

	/**
	* Return true if the point is inside one of the polygons.
	*/
	bool IsIn(const RPoint &pt);

	/**
	* Assign operator.
	*/
	RPolygons& operator=(const RPolygons &poly);

	/**
	* Save the polygons in a file.
	*/
	void Save(RTextFile& f);
};


}  //-------- End of namespace RGeometry2D ------------------------------------


//-----------------------------------------------------------------------------
#endif
