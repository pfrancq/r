/*

	R Project Library

	RTriangle.h

	Triangle - Header.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RTriangle_H
#define RTriangle_H


//------------------------------------------------------------------------------
//#include files for R Project
#include <rpoint.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * This class represent a triangle. Be default, Pt1 is always the most
 * bottom/left edge.
 * @short Triangle.
 */
class RTriangle
{
	/**
	* The First edge of a triangle.
	*/
	RPoint Pt1;

	/**
	* The second edge of the triangle.
	*/
	RPoint Pt2;

	/**
	* The third edge of the triangle.
	*/
	RPoint Pt3;

public:

	/**
	* Construct a triangle limited to the point (0,0).
	*/
	RTriangle(void);

	/**
	 * Copy constructor.
    * @param triangle       Triangle used for the initialisation.
    */
	RTriangle(const RTriangle& triangle);

	/**
	* Construct a line. The points are eventually exchanged to ensure that the
	* first one is the most left-upper point.
	* @param x1              X-coordinate of the first edge.
	* @param y1              Y-coordinate of the first edge.
	* @param x2              X-coordinate of the second edge.
	* @param y2              Y-coordinate of the second edge.
	* @param x3              X-coordinate of the third edge.
	* @param y3              Y-coordinate of the third edge.
	* @param reorder         Define if the line must be reorder.
	*/
	RTriangle(tCoord x1,tCoord y1,tCoord x2,tCoord y2,tCoord x3,tCoord y3,bool reorder=true);

	/**
	* Construct a line. The points are eventually exchanged to ensure that the
	* first one is the most left-upper point.
	* @param pt1             The first edge.
	* @param pt2             The second edge.
	* @param pt2             The third edge.
	* @param reorder         Define if the line must be reorder.
	*/
	RTriangle(const RPoint& pt1,const RPoint& pt2,const RPoint& pt3,bool reorder=true);

private:

	/**
	 * ReOrder de points if necessary to follow the convention.
    */
	void ReOrder(void);

public:

	/**
	* Compare two triangles. This function is used with the class RContainer.
	* @param triangle        Triangle used for the comparison.
	* @return
	* - -1 The first edge of the current triangle is more at the left (at
	*   the bottom) of the other one. If identical, the same comparison is done
	 *  for the second and third edges.
	* - +1 The left-bottom edge of the current triangle is more at the right (
	*   upper) of the other one. If identical, the same comparison is done
	*  for the second and third edges.
	*/
	int Compare(const RTriangle& triangle) const;

	/**
	* Compute the area of the triangle.
	* @return the area.
	*/
	tCoord GetArea(void) const;

	/**
	* Set new points to the triangle.
	* @param x1              X-coordinate of the first edge.
	* @param y1              Y-coordinate of the first edge.
	* @param x2              X-coordinate of the second edge.
	* @param y2              Y-coordinate of the second edge.
	* @param x3              X-coordinate of the third edge.
	* @param y3              Y-coordinate of the third edge.
	* @param reorder         Define if the line must be reorder.
	*/
	void SetPoints(tCoord x1,tCoord y1,tCoord x2,tCoord y2,tCoord x3,tCoord y3,bool reorder=true);

	/**
	* Set new points to the triangle.
	* @param pt1             The first edge.
	* @param pt2             The second edge.
	* @param pt2             The third edge.
	* @param reorder         Define if the line must be reorder.
	*/
	void SetPoints(const RPoint& pt1,const RPoint& pt2,const RPoint& pt3,bool reorder=true)
	{
		SetPoints(pt1.X,pt1.Y,pt2.X,pt2.Y,pt3.X,pt3.Y,reorder);
	}

	/**
	* Get the X position of the first edge.
	* @return a coordinate.
	*/
	tCoord GetX1(void) const {return(Pt1.X);}

	/**
	* Get the Y position of the first edge.
	* @return a coordinate.
	*/
	tCoord GetY1(void) const {return(Pt1.Y);}

	/**
	* Get the X position of the second edge.
	* @return a coordinate.
	*/
	tCoord GetX2(void) const {return(Pt2.X);}

	/**
	* Get the Y position of the second edge.
	* @return a coordinate.
	*/
	tCoord GetY2(void) const {return(Pt2.Y);}

	/**
	* Get the X position of the third edge.
	* @return a coordinate.
	*/
	tCoord GetX3(void) const {return(Pt3.X);}

	/**
	* Get the Y position of the third edge.
	* @return a coordinate.
	*/
	tCoord GetY3(void) const {return(Pt3.Y);}

	/**
	 * Get the first edge.
    * @return a point.
    */
	RPoint GetPt1(void) const {return(RPoint(Pt1));}

	/**
	 * Get the second edge.
    * @return a point.
    */
	RPoint GetPt2(void) const {return(RPoint(Pt2));}

	/**
	 * Get the third edge.
    * @return a point.
    */
	RPoint GetPt3(void) const {return(RPoint(Pt3));}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
namespace std{
//------------------------------------------------------------------------------


#ifndef __APPLE__
extern "C++"
{
#endif

//------------------------------------------------------------------------------
/**
 * Print the coordinate of the line in the form (x1,y1);(x2,y2).
 * @param os                Output stream.
 * @param line              Line to print.
 * @return the stream.
 */
extern std::ostream& operator<<(std::ostream& os,const R::RTriangle& triangle);

#ifndef __APPLE__
}
#endif


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif
