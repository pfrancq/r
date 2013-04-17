	/*

	R Project Library

	RPolygon.h

	Polygon - Header.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>
#include <rpoint.h>
#include <rrect.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a polygon as a set of points regroup in a container.
* Most of the functions assume that the order of the points in the container
* must be anti-clockwise. The ReOrder() method does this job, it is
* automatically called by these functions.
* @short Polygon class.
* @attention Currently, some methods work only with rectangular polygons.
*/
class RPolygon : RContainer<RPoint,true,false>
{
	/**
	 *  Remember if the polygon is reordered.
	 */
	bool Order;

	/**
	 * Remember if the polygon is a rectangular one.
	 */
	bool Rect;

public:

	/**
	* Construct a polygon.
	*/
	RPolygon(void);

	/**
	* Construct a polygon with an initial maximum size.
	* @param Max            Initial maximal size.
	*/
	RPolygon(const int Max);

	/**
	* Construct a polygon from another.
	* @param poly           Polygon used as reference.
	*/
	RPolygon(const RPolygon& poly);

	/**
	* This function compares two polygons. This function is used for the class
	* RContainer.
 	* @param poly            Polygon used for the comparison.
	* @return
	* - -1 The left-bottom point of the current polygon is more at the left (at
	*   the bottom) of the other one.
	* - 0 if there have the same number of points and at the same positions.
	* - +1 The left-bottom point of the current polygon is more at the right (
	*   upper) of the other one.
	* @attention The polygons are reordered if necessary.
	*/
	int Compare(const RPolygon& poly) const;

	/**
	 * Get the number of vertices
    * @return the number of vertices.
	 */
	size_t GetNbVertices(void) const {return(GetNb());}

	/**
	* Get a cursor on the vertices of the polygon.
	* @return RPointCursor.
	*/
	R::RCursor<RPoint> GetVertices(void) const;

	/**
	* Get the ith vertex of the polygon (Only read). The operator generates an
	* exception is the index is out of range.
	* @param idx             Index of the vertex to get.
	* @return Return a point.
	*/
	const RPoint* operator[](size_t idx) const {return(RContainer<RPoint,true,false>::operator[](idx));}

	/**
	* Get the ith vertex of the polygon (Read/Write). The operator generates an
	* exception is the index is out of range.
	* @param idx             Index of the vertex to get.
	* @return the pointer.
	*/
	RPoint* operator[](size_t idx) {return(RContainer<RPoint,true,false>::operator[](idx));}

	/**
	 * Clear the polygon.
    */
	void Clear(void);

	/**
	* Insert an vertex into the polygon.
	* @param x	            X Position.
	* @param y              Y Position.
   */
	void InsertVertex(tCoord x,tCoord y);

	/**
	 * Insert an vertex into the polygon. A new RPoint is created and added to
	 * the polygon.
    * @param pt             Vertex to insert.
    */
	void InsertVertex(const RPoint& pt);

	/**
	 * Insert an vertex into the polygon.
    * @param pt             Vertex to insert.
    */
	void InsertVertex(RPoint* pt);

	/**
	 * Delete an vertex from the polygon.
    * @param pt             Vertex to insert.
    */
	void DeleteVertex(RPoint* pt);

	/**
	* The assign operator.
	*/
	RPolygon& operator=(const RPolygon& poly);

	/**
	* The equal operator.
	* @attention The polygons are reordered if necessary.
	*/
	bool operator==(const RPolygon& poly) const;

	/**
	* The non-equal operator.
	* @attention The polygons are reordered if necessary.
	*/
	bool operator!=(const RPolygon& poly) const;

	/**
	* Make a translation of the polygon.
	* @param pt             The point representing the vector used.
	*/
	RPolygon& operator+=(const RPoint& pt);

	/**
	* Make a translation of the polygon.
	* @param pt             The point representing the vector used.
	*/
	RPolygon& operator-=(const RPoint& pt);

	/**
	* This function returns a pointer to the vertex on the same horizontal line.
	* If the polygon is not a rectangular one, such a vertex may not be found.
	* @param pt             Point used as reference.
	* @return the vertex or an infinitive point (no vertex found).
	*/
	RPoint GetConX(const RPoint& pt) const;

	/**
	* This function returns a pointer to the vertex on the same vertical line.
	* If the polygon is not a rectangular one, such a vertex may not be found.
	* @param pt             Point used as reference.
	* @return the vertex or an infinitive point (no vertex found).
	*/
	RPoint GetConY(const RPoint& pt) const;

	/**
	* Return a pointer to the most bottom-left vertex of the polygon.
	*/
	RPoint GetBottomLeft(void) const;

	/**
	* Return a pointer to the most bottom-left vertex of the polygon responding
	* to the criteria.
	* @param minx           Minimal X position of the vertex to search.
	* @param miny           Minimal Y position of the vertex to search.
	* @param maxx           Maximal X position of the vertex to search.
	*/
	RPoint* GetBottomLeft(const tCoord minx,const tCoord miny,const tCoord maxx) const;

	/**
	* Return a pointer to the most left-bottom vertex of the polygon.
	*/
	RPoint* GetLeftBottom(void) const;

	/**
	* Return a pointer to the most left-bottom vertex of the polygon responding
	* to the criteria.
	* @param minx           Minimal X position of the vertex  to search.
	* @param miny           Minimal Y position of the vertex to search.
	* @param maxy           Maximal Y position of the vertex to search.
	*/
	RPoint* GetLeftBottom(const tCoord minx,const tCoord miny,const tCoord maxy) const;

	/**
	* Return true if the point is on an edge.
	* @param X              The X Coordinate used.
	* @param Y              The Y Coordinate used.
	*/
	bool Edge(const tCoord X,const tCoord Y) const;

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
	bool Edge(const RPoint& pt1,const RPoint& pt2) const;

	/**
	* Return true if the point is a vertex.
	* @param x               X-coordinate of the point.
	* @param y               Y-coordinate of the point.
	* @param pt              Point to verify.
	*/
	bool IsVertex(const tCoord x,const tCoord y) const;

	/**
	* Return true if the point is a vertex.
	* @param pt              Point to verify.
	*/
	bool IsVertex(const RPoint& pt) const
		{return(IsVertex(pt.X,pt.Y));}

	/**
	* Return true if the point is inside the polygon.
	* @param x               X-coordinate of the point.
	* @param y               Y-coordinate of the point.
	*/
	bool IsIn(const tCoord x,const tCoord y) const;

	/**
	* Return true if a point is inside the polygon.
	* @param pt              Point to verify.
	*/
	bool IsIn(const RPoint& pt) const
		{return(IsIn(pt.X,pt.Y));}

	/**
	* Return true if the polygon poly is inside the polygon. The two polygons are
	* supposed to be "rectangular". This function determines if all the vertices
	* of poly are inside the polygon.
	* @param poly           The polygon to known if is in.
	*/
	bool IsIn(const RPolygon& poly) const;

	/**
	* Return true if the polygon is inside the rectangle.
	* @param rect            Rectangle to known if contains.
	*/
	bool Contained(const RRect& rect) const;

	/**
	* Return the area of the polygon.
	*/
	tCoord Area(void) const;

	/**
	* Return the boundary rectangle of the polygon.
	* @param rect           The rectangle.
	*/
	void Boundary(RRect& rect) const;

	/**
	* Modify the polygon to a certain orientation.
	* @param o              The orientation.
	* @param min            The vector that has been subtracted to have the
	*                       bottom-left point at (0,0).
	*/
	void ChangeOrientation(const tOrientation o,RPoint& min);

	/**
	* Decompose the polygon in a container of rectangles.
	* @param rects          A pointer to the container of rectangles.
	*/
	void RectDecomposition(RContainer<RRect,true,false>& rects) const;

	/**
	* Add the vertices of the polygon to a container of points.
	* @param points         A pointer to the container of points.
	*/
	void AddVertices(RContainer<RPoint,true,false>& points) const;

	/**
	* Shift the vertices of the polygon to make the bottom-left point be the first
	* one, and the rest are in the anti-clockwise order.
	*/
	void ReOrder(void);

	/**
	* Delete all necessary vertices to not have two adjacent vertexes.
	*/
	void ReValid(void);

	/**
	* This function returns when there are duplicate vertices.
	*/
	bool DuplicateVertices(void) const;

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
	 * @return true if the polygon is a rectangle.
	 */
	bool IsRect(void) const;

	/**
	 * @return true if the polygon is a square.
	 */
	inline bool IsSquare(void) const {return(IsRect()&&(((*this)[2])->X==((*this)[2])->Y));}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
