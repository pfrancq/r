/*

	R Project Library

	RPoint.h

	Point - Header

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
#ifndef RPointsH
#define RPointsH


//-----------------------------------------------------------------------------
// include files for RGeometry
#include <rgeometry/geometry.h>
#include <rgeometry/rpoint.h>
using namespace RGeometry2D;


//-----------------------------------------------------------------------------
namespace RGeometry2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represents a set of points regrouped in a container.
* @author Pascal Francq
* @short Container of points.
*/
class RPoints : public RStd::RContainer<RPoint,unsigned int,true,false>
{
public:

	/**
	* Construct a container of points.
	*/
	RPoints(void);

	/**
	* Construct a container of points with an initial maximal size.
	* @param max		Initial maximal size of the container.
	*/
	RPoints(unsigned int max);

	/**
	* Construct a container of points from another one.
	* @param points		The container used as reeference.
	*/
	RPoints(RPoints *points);

	/**
	* Find the next point to the left and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    The polygons used as reference.
	*/
	RPoint* FindLeft(RPoint *pt,RPolygons *polys);

	/**
	* Find the next point to the right and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    The polygons used as reference.
	*/
	RPoint* FindRight(RPoint *pt,RPolygons *polys);

	/**
	* Find the next point to the bottom and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    	The polygons used as reference.
	*/
	RPoint* FindBottom(RPoint *pt,RPolygons *polys); // in rpoint.cpp

	/**
	* Find the next point to the up and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    The polygons used as reference.
	*/
	RPoint* FindUp(RPoint *pt,RPolygons *polys);

	/**
	* Find the most bottom-left point of the container.
	*/
	RPoint* FindBottomLeft(void);

	/**
	* This function returns true when there are duplicate points.
	*/
	bool DuplicatePoints(void);

	/**
	* Assignment operator.
	*/
	RPoints& operator=(const RPoints &points);		
};


}  //-------- End of namespace RGeometry2D ------------------------------------


//-----------------------------------------------------------------------------
#endif
