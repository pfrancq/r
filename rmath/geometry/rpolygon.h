/*

	R Project Library

	RPolygon.h

	Polygon - Header.

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
#ifndef RPolygon_H
#define RPolygon_H


//------------------------------------------------------------------------------
// include files for RGeometry
#include <rmath/geometry.h>
#include <rmath/rpoint.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a polygon as a set of points regroup in a container.
* Most of the functions assume that the order of the points in the container
* must be anti-clockwise. The ReOrder() method does this job, so don't be
* afraid to call it each time it's necessary.
* @author Pascal Francq
* @short Polygon class.
*/
class RPolygon : public RContainer<RPoint,true,false>
{
public:

	/**
	* Construct a polygon.
	*/
	RPolygon(void);

	/**
	* Construct a polygon with an initial maxiam size.
	* @param Max            Initial maximal size.
	*/
	RPolygon(const int Max);

	/**
	* Construct a polygon from another.
	* @param poly           Polygon used as reference.
	*/
	RPolygon(const RPolygon& poly);

	/**
	* The assign operator.
	*/
	RPolygon& operator=(const RPolygon& poly);

	/**
	* The equal operator.
	*/
	bool operator==(const RPolygon& poly) const;

	/**
	* The non-equal operator.
	*/
	bool operator!=(const RPolygon& poly) const;

	/**
	* Make a translation of the polygon.
	* @param pt             The point representing the vector used.
	*/
	RPolygon& operator+=(const RPoint& pt) throw(std::bad_alloc);

	/**
	* Make a translation of the polygon.
	* @param pt             The point representing the vector used.
	*/
	RPolygon& operator-=(const RPoint& pt) throw(std::bad_alloc);

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly           Polygon used for the comparaison.
	*/
	int Compare(const RPolygon* poly) const {return((*this)!=(*poly));}

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly           Polygon used for the comparaison.
	*/
	int Compare(const RPolygon& poly) const {return((*this)!=poly);}

	/**
	* This function returns a pointer to the point on the same horizontal vertex.
	* @param pt             Point used as reference.
	*/
	RPoint GetConX(const RPoint* pt) const;

	/** This function returns a pointer to the point on the same vertital vertex.
	* @param pt             Point used as reference.
	*/
	RPoint GetConY(const RPoint* pt) const;

	/**
	* Return a pointer to the most bottom-left point of the polygon.
	*/
	RPoint GetBottomLeft(void) const;

	/**
	* Return a pointer to the most bottom-left point of the polygon responding
	* to the criteria.
	* @param MinX           Minimal X position of the point to search.
	* @param MinY           Minimal Y position of the point to search.
	* @param MaxX           Maximal X position of the point to search.
	*/
	RPoint* GetBottomLeft(const RCoord MinX,const RCoord MinY,const RCoord MaxX) const;

	/**
	* Return a pointer to the most left-bottom point of the polygon.
	*/
	RPoint* GetLeftBottom(void) const;

	/**
	* Return a pointer to the most left-bottom point of the polygon responding
	* to the criteria.
	* @param MinX           Minimal X position of the point to search.
	* @param MinY           Minimal Y position of the point to search.
	* @param MaxY           Maximal Y position of the point to search.
	*/
	RPoint* GetLeftBottom(const RCoord MinX,const RCoord MinY,const RCoord MaxY) const;

	/**
	* Return true if the point is on an edge.
	* @param X              The X Coordinate used.
	* @param Y              The Y Coordinate used.
	*/
	bool Edge(const RCoord X,const RCoord Y) const;

	/**
	* Return true if the point is on an edge.
	* @param pt             The point used.
	*/
	bool Edge(const RPoint* pt) const {return(Edge(pt->X,pt->Y));}

	/**
	* Return true if the point is on an edge.
	* @param pt             The point used.
	*/
	bool Edge(const RPoint& pt) const {return(Edge(pt.X,pt.Y));}

	/**
	* Return true if two points are on the same edge.
	* @param pt1            The first point used.
	* @param pt2            The second point used.
	*/
	bool Edge(const RPoint* pt1,const RPoint* pt2) const;

	/**
	* Return true if the point is a vertice.
	*/
	bool IsVertice(const RPoint& pt) const;

	/**
	* Return true if the point is inside the polygon.
	*/
	bool IsIn(const RCoord X,const RCoord Y) const;

	/**
	* Return true if the point is inside the polygon.
	*/
	bool IsIn(const RPoint& pt) const
		{return(IsIn(pt.X,pt.Y));}

	/**
	* Return true if the point is inside the polygon.
	*/
	bool IsIn(const RPoint* pt) const
		{return(IsIn(pt->X,pt->Y));}

	/**
	* Return true if the polygon poly is inside the polygon. The two polygons are
	* supposed to be "rectangular". This function determines if all the points
	* of poly are inside the polygon.
	* @param poly           The polygon to known if is in.
	*/
	bool IsIn(const RPolygon& poly) const
		{return(IsIn(&poly));}

	/**
	* Return true if the polygon poly is inside the polygon. The two polygons
	* are supposed to be "rectangular". This function determines if all the
	* points of the bound of the polygon poly are inside the current one.
	* @param poly           The polygon to known if is in.
	*/
	bool IsIn(const RPolygon* poly) const;

	/**
	* Return the area of the polygon.
	*/
	RCoord Area(void) const;

	/**
	* Return the boundary rectangle of the polygon.
	* @param rect           The rectangle.
	*/
	void Boundary(RRect& rect) const;

	/**
	* Modify the polygon to a certain orientation.
	* @param o              The orientation.
	* @param min            The vector that has been substracted to have the
	*                       bottom-left point at (0,0).
	*/
	void ChangeOrientation(const ROrientation o,RPoint& min);

	/**
	* Decompose the polygon in a container of rectangles.
	* @param rects          A pointer to the container of rectangles.
	*/
	void RectDecomposition(RRects* rects) const;

	/**
	* Add the points of the polygon to a container of points.
	* @param points         A pointer to the container of points.
	*/
	void AddPoints(RPoints* points) const;

	/**
	* Shift the points of the polygon to make the bottom-left point be the first
	* one, and the rest are in the anti-clockwise order.
	*/
	void ReOrder(void);

	/**
	* Delete all necessary points to not have two adjance vertexes.
	*/
	void ReValid(void);

	/**
	* This function returns when there are duplicate points.
	*/
	bool DuplicatePoints(void) const;

	/**
	* Compute a point in the polygon which represents a sort of "Mass Point".
	*/
	RPoint GetCentralPoint(void);

	/**
	* Save the polygon in a file.
	* param f               Text file where the information must be written.
	*/
	void Save(RTextFile& f);

	/**
	* Calibrate the polygon, make the bottom-left point at (0,0).
	* @return The translation that has be done.
	*/
	RPoint Calibrate(void);

	/**
	* Get a cursor on the points of the polygon.
	* @return RPointCursor.
	*/
	R::RCursor<RPoint> GetPointsCursor(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
