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
#ifndef RPolygonH
#define RPolygonH


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
* This class represent a polygon as a set of points regroup in a container.
* Most of the functions assume that the order of the points in the container
* must be anti-clockwise. The ReOrder() method does this job, so don't be
* afraid to call it each time it's necessary.
* @author Pascal Francq
* @short Polygon class.
*/
class RPolygon : public RStd::RContainer<RPoint,unsigned int,true,false>
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
	RPolygon(int Max);

	/**
	* Construct a polygon from another.
	* @param poly           Polygon used as reference.
	*/
	RPolygon(const RPolygon *poly);

	/**
	* Construct a polygon from another.
	* @param poly           Polygon used as reference.
	*/
	RPolygon(const RPolygon &poly);

	/**
	* The assign operator.
	*/
	RPolygon& operator=(const RPolygon &poly);

	/**
	* The equal operator.
	*/
	bool operator==(const RPolygon &poly);

	/**
	* The non-equal operator.
	*/
	bool operator!=(const RPolygon &poly);

	/**
	* Make a translation of the polygon.
	* @param pt             The point representing the vector used.
	*/
	RPolygon& operator+=(const RPoint &pt) throw(bad_alloc);

	/**
	* Make a translation of the polygon.
	* @param pt             The point representing the vector used.
	*/
	RPolygon& operator-=(const RPoint &pt) throw(bad_alloc);

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly           Polygon used for the comparaison.
	*/
	int Compare(RPolygon *poly) {return((*this)!=(*poly));}

	/**
	* This function returns a pointer to the point on the same horizontal vertex.
	* @param pt             Point used as reference.
	*/
	RPoint* GetConX(RPoint *pt) const;

	/** This function returns a pointer to the point on the same vertital vertex.
	* @param pt             Point used as reference.
	*/
	RPoint* GetConY(RPoint *pt) const;

	/**
	* Return a pointer to the most bottom-left point of the polygon.
	*/
	RPoint* GetBottomLeft(void) const;

	/**
	* Return a pointer to the most bottom-left point of the polygon responding
	* to the criteria.
	* @param MinX           Minimal X position of the point to search.
	* @param MinY           Minimal Y position of the point to search.
	* @param MaxX           Maximal X position of the point to search.
	*/
	RPoint* GetBottomLeft(RCoord MinX,RCoord MinY,RCoord MaxX) const;

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
	RPoint* GetLeftBottom(RCoord MinX,RCoord MinY,RCoord MaxY) const;

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
	bool Edge(RPoint *pt1,RPoint *pt2) const;

	/**
	* Return true if the point is a vertice.
	*/
	bool IsVertice(const RPoint &pt) const;

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
	void Boundary(RRect &rect) const;

	/**
	* Modify the polygon to a certain orientation.
	* @param o              The orientation.
	* @param min            The vector that has been substracted to have the
	*                       bottom-left point at (0,0).
	*/
	void ChangeOrientation(ROrientation o,RPoint& min);

	/**
	* Decompose the polygon in a container of rectangles.
	* @param rects          A pointer to the container of rectangles.
	*/
	void RectDecomposition(RRects *rects) const;

	/**
	* Add the points of the polygon to a container of points.
	* @param points         A pointer to the container of points.
	*/
	void AddPoints(RPoints *points);

	/**
	* Shift the points of the polygon to make the bottom-left point be the first
	* one, and the rest are in the anti-clockwise order.
	*/
	void ReOrder(void);

	/**
	* This function returns when there are duplicate points.
	*/
	bool DuplicatePoints(void);

	/**
	* Return a pointer to a temporary object of class rpolygon.
	*/
	static RPolygon* GetPolygon(void);
	
	/**
	* Compute a point in the polygon which represents a sort of "Mass Point".
	*/
	RPoint& GetCentralPoint(void);

	/**
	* Save the polygon in a file.
	*/
	void Save(RTextFile& f);

	/**
	* Calibrate the polygon, make the bottom-left point at (0,0).
	* @return The translation that has be done.
	*/
	RPoint& Calibrate(void);
};


}  //-------- End of namespace RGeometry2D ------------------------------------


//-----------------------------------------------------------------------------
#endif
