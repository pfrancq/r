/*

	R Project Library

	RPolygons.h

	Polygons - Header.

	Copyright 1999-2004 by the Universit�Libre de Bruxelles.

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
#ifndef RPolygons_H
#define RPolygons_H


//------------------------------------------------------------------------------
// include files for RGeometry
#include <rmath/geometry.h>
#include <rmath/rpolygon.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a set of polygons regrouped in a container.
*	@author Pascal Francq
*	@short Container of polygons.
 */
class RPolygons : public RContainer<RPolygon,true,false>
{
public:

	/**
	* Construct a container of polygons.
	*/
	RPolygons(void);

	/**
	* Return true if the point is on an edge of one of the polygon contained.
	* @param pt             The point used.
	*/
	bool Edge(const RPoint* pt) const;

	/**
	* Return true if the point is on an edge of a certain polygon.
	* @param pt             The point used.
	* @param poly           The polygon to look in.
	*/
	bool Edge(const RPoint* pt,const RPolygon* poly) const;

	/**
	* Return true if two points are on the same edge of one of the polygon contained.
	* @param pt1            The first point used.
	* @param pt2            The second point used.
	*/
	bool Edge(const RPoint* pt1,const RPoint* pt2) const;

	/**
	* Add the points of the polygons contained to a container of points.
	* @param points         A pointer to the container of points.
	*/
	void PutPoints(RPoints* points) const;

	/**
	* This function calculate the union of the polygons contained.
	* @param upoly          A pointer to the polygon representing the union.
	*/
	void Union(RPolygon* upoly) const;

	/**
	* This function returns true when there are duplicate points.
	*/
	bool DuplicatePoints(void) const;

	/**
	* Return true if the point is inside one of the polygons.
	*/
	bool IsIn(const RCoord X,const RCoord Y) const;

	/**
	* Return true if the point is inside one of the polygons.
	*/
	bool IsIn(const RPoint& pt) const;

	/**
	* Assign operator.
	*/
	RPolygons& operator=(const RPolygons& poly);

	/**
	* Save the polygons in a file.
	* param f               Text file where the information must be written.
	*/
	void Save(RTextFile& f);

	/**
	* Get a cursor on the polygons.
	* @return RPolygonCursor.
	*/
	R::RCursor<RPolygon> GetPolygonsCursor(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
