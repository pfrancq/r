/*

	R Project Library

	RPoint.h

	Points - Header

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
#ifndef RPoints_H
#define RPoints_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/geometry.h>
#include <rmath/rpoint.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a set of points regrouped in a container.
* @author Pascal Francq
* @short Container of points.
*/
class RPoints : public RContainer<RPoint,unsigned int,true,false>
{
public:

	/**
	* Construct a container of points.
	*/
	RPoints(void);

	/**
	* Construct a container of points with an initial maximal size.
	* @param max            Initial maximal size of the container.
	*/
	RPoints(const unsigned int max);

	/**
	* Construct a container of points from another one.
	* @param points         The container used as reeference.
	*/
	RPoints(const RPoints* points);

	/**
	* Find the next point to the left and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt             Point used as reference.
	* @param polys          The polygons used as reference.
	*/
	RPoint* FindLeft(const RPoint* pt,const RPolygons* polys) const;

	/**
	* Find the next point to the right and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt             Point used as reference.
	* @param polys          The polygons used as reference.
	*/
	RPoint* FindRight(const RPoint* pt,const RPolygons* polys) const;

	/**
	* Find the next point to the bottom and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt             Point used as reference.
	* @param polys          The polygons used as reference.
	*/
	RPoint* FindBottom(const RPoint* pt,const RPolygons* polys) const;

	/**
	* Find the next point to the up and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt             Point used as reference.
	* @param polys          The polygons used as reference.
	*/
	RPoint* FindUp(const RPoint* pt,const RPolygons* polys) const;

	/**
	* Find the most bottom-left point of the container.
	*/
	RPoint* FindBottomLeft(void) const;

	/**
	* This function returns true when there are duplicate points.
	*/
	bool DuplicatePoints(void) const;

	/**
	* Assignment operator.
	*/
	RPoints& operator=(const RPoints& points);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
