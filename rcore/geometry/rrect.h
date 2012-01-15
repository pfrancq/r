/*

	R Project Library

	RRect.h

	Rectangle - Header

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
* This class implements a rectangle.
* @author Pascal Francq
* @short Rectangle.
*/
class RRect
{
	/**
	* Left limit.
	*/
	tCoord X1;

	/**
	* Bottom limit.
	*/
	tCoord Y1;

	/**
	* Right limit.
	*/
	tCoord X2;

	/**
	* Upper limit.
	*/
	tCoord Y2;

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
	* @param pt1            Point used as (left,bottom) edge.
	* @param pt2            Point used as (right,up) edge.
	*/
	RRect(const RPoint& pt1,const RPoint& pt2);

	/**
	* Construct a rectangle from four coordinates.
	* @param x1              The most left position.
	* @param y1              The most bottom position
	* @param x2              The most right position.
	* @param y2              The most up position.
	*/
	RRect(tCoord x1,tCoord y1,tCoord x2,tCoord y2);

	/**
	* Set the coordinates of the rectangle.
	* @param x1              The most left position.
	* @param y1              The most bottom position
	* @param x2              The most right position.
	* @param y2              The most up position.
	*/
	void Set(tCoord x1,tCoord y1,tCoord x2,tCoord y2);

	/**
	* @return the left limit.
	*/
	tCoord GetX1(void) const {return(X1);}

	/**
	* @return the bottom limit.
	*/
	tCoord GetY1(void) const {return(Y1);}

	/**
	* @return the right limit.
	*/
	tCoord GetX2(void) const {return(X2);}

	/**
	* @return the upper limit.
	*/
	tCoord GetY2(void) const {return(Y2);}

	/**
	* Return the width of the rectangle. The width includes the border, i.e. a
	* rectangle where X1=X2 has a width of 1.
	*/
	tCoord GetWidth(void) const {return(Abs(X2-X1+1));}

	/**
	* Return the height of the rectangle. The height includes the border, i.e.
	* a rectangle where Y1=Y2 has a height of 1.
	*/
	tCoord GetHeight(void) const {return(Abs(Y2-Y1+1));}

	/**
	 * @return the size of the rectangle.
	 */
	RSize GetSize(void) const {return(RSize(Abs(X2-X1+1),Abs(Y2-Y1+1)));}

	/**
	* Set the width of the rectangle. The method adapts the X-coordinate of
	* Pt2. The width includes the border, i.e. a width of 1 means that X2=X1.
	* @param width           New width.
	*/
	inline void SetWidth(tCoord width) {X2=width+X1-1;}

	/**
	* Set the height of the rectangle. The method adapts the Y-coordinate of
	* Pt2. The height includes the border, i.e. a width of 1 means that Y2=Y1.
	* @param height          New height.
	*/
	inline void SetHeight(tCoord height) {Y2=height+Y1-1;}

	/**
	* Set the coordinates of the rectangle based on a shape.
	* @param x               The most left position.
	* @param y               The most bottom position
	* @param w               Width.
	* @param h               Height.
	*/
	inline void SetShape(tCoord x,tCoord y,tCoord w,tCoord h) {X1=x; Y1=y; X2=x+w-1; Y2=y+h-1;}

	/**
	* Return the area of the rectangle.
	*/
	inline tCoord GetArea(void) const {return(GetHeight()*GetWidth());}

	/**
	* The equal operator.
	*/
	inline bool operator==(const RRect& rect) const {return((X1==rect.X1)&&(Y1==rect.Y1)&&(X2==rect.X2)&&(Y2==rect.Y2));}

	/**
	* The non-equal operator.
	*/
	inline bool operator!=(const RRect& rect) const {return((X1!=rect.X1)||(Y1!=rect.Y1)||(X2!=rect.X2)||(Y2!=rect.Y2));}

	/**
	* Make a translation of the rectangle.
	* @param x              The x to add.
	* @param y              The y to add.
	*/
	void Translation(tCoord x,tCoord y);

	/**
	* Put a rectangle at a given position.
	* @param x              X position.
	* @param y              Y position.
	*/
	void SetPos(tCoord x,tCoord y);

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
	* Compare two rectangles and return 0 if there are at the same. This function
	* is used with the class RContainer.
	* @param rect           Rectangle used for the comparison.
	*/
	int Compare(const RRect& rect) const;

	/**
	* Adapt the rectangle to be contained in a given region.
	* @param clip           The region used as reference.
	* @returns The function returns true if the rectangle was clipped.
	*/
	bool Clip(const RRect& clip);

	/**
	* Adapt the rectangle to be contained in a given region.
	* @param limits         The point representing the limits of the region.
	* @returns The function returns true if the rectangle was clipped.
	*/
	bool Clip(const RPoint& limits);

	/**
	* This function returns true if the two rectangles overlapped.
	* @param rect            Reference rectangle.
	*/
	bool Overlap(const RRect& rect) const;

	/**
	* This function returns true if the two rectangles overlapped.
	* @param rect            Reference rectangle.
	*/
	inline bool Overlap(const RRect* rect) const {return(Overlap(*rect));}

	/**
	* This function returns true if a given point is in the rectangle.
	* @param X              X position of the point.
	* @param Y              Y position of the point.
	*/
	bool IsIn(tCoord X,tCoord Y) const;

	/**
	* This function returns true if a given point is in the rectangle.
	* @param pos             Position.
	*/
	inline bool IsIn(const RPoint& pos) const {return(IsIn(pos.X,pos.Y));}

	/**
	* This function returns true if a given rect is in the rectangle.
	* @param rect            Rectangle.
	*/
	inline bool IsIn(const RRect& rect) const {return(IsIn(rect.X1,rect.Y1)||IsIn(rect.X2,rect.Y2));}

	/**
	* The assign Operator.
	*/
	inline RRect& operator=(const RRect& rect) {X1=rect.X1;Y1=rect.Y1;X2=rect.X2;Y2=rect.Y2;return(*this);}

	/**
	 * Get the (bottom,left) point.
	 * @return RPoint
	 */
	RPoint inline GetPt1(void) const {return(RPoint(X1,Y1));}

	/**
	 * Get the (upper,right) point.
	 * @return RPoint
	 */
	RPoint inline GetPt2(void) const {return(RPoint(X2,Y2));}

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
#endif
