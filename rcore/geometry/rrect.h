/*

	R Project Library

	RRect.h

	Rectangle - Header

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
#ifndef RRect_H
#define RRect_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpoint.h>
#include <rsize.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a rectangle. In practice, Pt1 is always the bottom/left
* point while Pt2 is always the upper/right point.
* @short Rectangle.
*/
class RRect
{
	/**
	* The First point of the line.
	*/
	RPoint Pt1;

	/**
	* The second point of the line.
	*/
	RPoint Pt2;

public:

	/**
	* Construct a rectangle with two points at (0,0).
	*/
	RRect(void);

	/**
	* Construct a rectangle from another one.
	* @param rect           The rectangle used as reference.
	*/
	RRect(const RRect& rect);

	/**
	* Construct a rectangle with two points.
	* @param pt1            First point used.
	* @param pt2            Second point used.
	*/
	RRect(const RPoint& pt1,const RPoint& pt2);

	/**
	* Construct a rectangle from four coordinates.
	* @param x1              First X-coordinate.
	* @param y1              First Y-coordinate.
	* @param x2              Second X-coordinate.
	* @param y2              Second Y-coordinate.
	*/
	RRect(tCoord x1,tCoord y1,tCoord x2,tCoord y2);

private:

	/**
	 * ReOrder de points if necessary to follow the convention.
    */
	void ReOrder(void);

public:

	/**
	* Compare two rectangles. This function is used with the class RContainer.
	* @param rect           Rectangle used for the comparison.
	* @return
	* - -1 The left-bottom point of the current polygon is more at the left (at
	*   the bottom) of the other one. If identical, the second points are
	*   compared.
	* - 0 if there have the points.
	* - +1 The left-bottom point of the current line is more at the right (
	*   upper) of the other one. If identical, the second points are compared.
	*/
	int Compare(const RRect& rect) const;

	/**
	* Set the coordinates of the rectangle.
	* @param pt1            First point used.
	* @param pt2            Second point used.
	*/
	void Set(const RPoint& pt1,const RPoint& pt2);

	/**
	* Set the coordinates of the rectangle.
	* @param x1              The most left position.
	* @param y1              The most bottom position
	* @param x2              The most right position.
	* @param y2              The most up position.
	*/
	void Set(tCoord x1,tCoord y1,tCoord x2,tCoord y2);

	/**
	 * Get the (bottom,left) point.
	 * @return RPoint
	 */
	RPoint inline GetPt1(void) const {return(Pt1);}

	/**
	* @return the left limit.
	*/
	tCoord GetX1(void) const {return(Pt1.X);}

	/**
	* @return the bottom limit.
	*/
	tCoord GetY1(void) const {return(Pt1.Y);}

	/**
	 * Get the (upper,right) point.
	 * @return RPoint
	 */
	RPoint inline GetPt2(void) const {return(Pt2);}

	/**
	* @return the right limit.
	*/
	tCoord GetX2(void) const {return(Pt2.X);}

	/**
	* @return the upper limit.
	*/
	tCoord GetY2(void) const {return(Pt2.Y);}

	/**
	* Return the width of the rectangle. The width includes the border, i.e. a
	* rectangle where X1=X2 has a width of 1.
	*/
	tCoord GetWidth(void) const {return(Abs(Pt2.X-Pt1.X+1));}

	/**
	* Return the height of the rectangle. The height includes the border, i.e.
	* a rectangle where Y1=Y2 has a height of 1.
	*/
	tCoord GetHeight(void) const {return(Abs(Pt2.Y-Pt1.Y+1));}

	/**
	 * @return the size of the rectangle.
	 */
	RSize GetSize(void) const {return(RSize(Abs(Pt2.X-Pt1.X+1),Abs(Pt2.Y-Pt1.Y+1)));}

	/**
	* Set the width of the rectangle. The method adapts the X-coordinate of
	* Pt2. The width includes the border, i.e. a width of 1 means that X2=X1.
	* @param width           New width.
	*/
	inline void SetWidth(tCoord width) {Pt2.X=width+Pt1.X-1;}

	/**
	* Set the height of the rectangle. The method adapts the Y-coordinate of
	* Pt2. The height includes the border, i.e. a width of 1 means that Y2=Y1.
	* @param height          New height.
	*/
	inline void SetHeight(tCoord height) {Pt2.Y=height+Pt1.Y-1;}

	/**
	* Set the coordinates of the rectangle based on a shape.
	* @param x               The most left position.
	* @param y               The most bottom position
	* @param w               Width.
	* @param h               Height.
	*/
	inline void SetShape(tCoord x,tCoord y,tCoord w,tCoord h) {Pt1.Set(x,y); Pt2.Set(x+w-1,y+h-1);}

	/**
	* Return the area of the rectangle.
	*/
	inline tCoord GetArea(void) const {return(GetHeight()*GetWidth());}

	/**
	* The equal operator.
	*/
	inline bool operator==(const RRect& rect) const {return((Pt1==rect.Pt1)&&(Pt2==rect.Pt2));}

	/**
	* The non-equal operator.
	*/
	inline bool operator!=(const RRect& rect) const {return((Pt1!=rect.Pt1)||(Pt2!=rect.Pt2));}

	/**
	* Make a translation of the rectangle.
	* @param pt             The point representing the vector used.
	*/
	void Translation(const RPoint& pt);

	/**
	* Make a translation of the rectangle.
	* @param x              The x to add.
	* @param y              The y to add.
	*/
	inline void Translation(tCoord x,tCoord y)
	{
		RPoint pt(x,y);
		Translation(pt);
	}

	/**
	* Put a rectangle at a given position.
	* @param pt             New position.
	*/
	void SetPos(const RPoint& pt);

	/**
	* Put a rectangle at a given position.
	* @param x              X position.
	* @param y              Y position.
	*/
	inline void SetPos(tCoord x,tCoord y)
	{
		RPoint pt(x,y);
		SetPos(pt);
	}

	/**
	* Make a translation of the rectangle.
	* @param pt             The point representing the vector used.
	*/
	inline RRect& operator+=(const RPoint& pt) {Translation(pt.X,pt.Y); return(*this);}

	/**
	* Make a minus translation of the rectangle.
	* @param pt             The point representing the vector used.
	*/
	inline RRect& operator-=(const RPoint& pt) {Translation(-pt.X,-pt.Y); return(*this);}

	/**
	* Adapt the rectangle to be contained in a given region.
	* @param clip           The region used as reference.
	* @returns The function returns true if the rectangle was clipped.
	*/
	bool Clip(const RRect& clip);

	/**
	* Adapt the rectangle to be contained in a given region.
	* @param limits         The point representing the right/up limits of the
	 *                      region.
	* @returns The function returns true if the rectangle was clipped.
	*/
	bool Clip(const RPoint& limits);

	/**
	* This function returns true if the two rectangles overlapped.
	* @param rect            Reference rectangle.
	*/
	bool Overlap(const RRect& rect) const;

	/**
	* This function returns true if a given point is in the rectangle.
	* @param pt              Point to analyze.
	* @param overlap         Specify if the point can be on one of the edges. By
	*                        default, it is true.
	*/
	bool IsIn(const RPoint& pt,bool overlap=true) const;

	/**
	* This function returns true if a given point is in the rectangle.
	* @param x               X position of the point.
	* @param y               Y position of the point.
	* @param overlap         Specify if the point can be on one of the edges. By
	*                        default, it is true.
	*/
	inline bool IsIn(tCoord x,tCoord y,bool overlap=true) const
	{
		RPoint pt(x,y);
		return(IsIn(pt,overlap));
	}

	/**
	* This function returns true if a given rect is in the rectangle.
	* @param rect            Rectangle.
	* @param overlap         Specify if the rectangle may overlap, i.e. some of
	*                        its points are on an edge of the other one. By
	*                       default, it is true.
	*/
	inline bool IsIn(const RRect& rect,bool overlap=true) const {return(IsIn(rect.Pt1,overlap)&&IsIn(rect.Pt2,overlap));}

	/**
	* The assign Operator.
	*/
	inline RRect& operator=(const RRect& rect) {Pt1=rect.Pt1;Pt2=rect.Pt2;return(*this);}

	/**
	* Modify the rectangle to a certain orientation.
	* @param o              The orientation.
	* @param min            The vector that has been subtracted to have the
	*                       bottom-left point at (0,0).
	*/
	void ChangeOrientation(const tOrientation o,RPoint& min);
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
 * Print the coordinate of the rectangle in the form (x1,y1);(x2,y2).
 * @param os                Output stream.
 * @param rect              Rectangle to print.
 * @return the stream.
 */
extern std::ostream& operator<<(std::ostream& os,const R::RRect& rect);

#ifndef __APPLE__
}
#endif


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif
