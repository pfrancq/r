/*

	Rainbow Library Project

	RRect.h

	Rectangle - Header

	(C) 1999-2000 by P. Francq.

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


//---------------------------------------------------------------------------
#ifndef RRectH
#define RRectH


//---------------------------------------------------------------------------
// include files for RGeometry
#include "rpoint.h"
using namespace RGeometry2D;


//---------------------------------------------------------------------------
namespace RGeometry2D{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* This class implements a rectangle.
* @author Pascal Francq
* @short Rectangle.
*/
class RRect
{
public:

	/**
	* (left,bottom) edge.
	*/
  RPoint Pt1;
	/**
	* (right,up) edge.
	*/
	RPoint Pt2;

	/**
	* Construct a rectangle with two points at (0,0).
	*/
	RRect(void);

	/**
	* Construct a rectangle from another one.
	* @param rect		The rectangle used as reference.
	*/
	RRect(RRect *rect);

	/**
	* Construct a rectangle with two points.
	* @param pt1		Point used as (left,bottom) edge.
	* @param pt2		Point used as (right,up) edge.
	*/
	RRect(RPoint *pt1,RPoint *pt2);

	/** Construct a rectangle from four coordinates.
	* @param MinX		The most left position.
	* @param MinY			The most bottom position
	* @param MaxX		The most right position.
	* @param MaxY		The most up position.
	*/
	RRect(RCoord MinX,RCoord MinY,RCoord MaxX,RCoord MaxY);

	/**
	* Return the width of the rectangle.
	*/
	inline RCoord Width(void) {return(labs(Pt2.X-Pt1.X+1));}
	
	/**
	* Return the height of the rectangle.
	*/
	inline RCoord Height(void) {return(labs(Pt2.Y-Pt1.Y+1));}

	/**
	* Return the area of the rectangle.
	*/
	inline RCoord Area(void) {return(Height()*Width());}

	/**
	* The equal operator.
	*/
	inline bool operator==(const RRect &rect) {return((Pt1==rect.Pt1)&&(Pt2==rect.Pt2));}

	/**
	* The non-equal operator.
	*/
	inline bool operator!=(const RRect &rect) {return((Pt1!=rect.Pt1)||(Pt2!=rect.Pt2));}

	/**
	* Make a translation of the rectangle.
	* @param pt 	The point representing the vector used.
	*/
	RRect& operator+=(const RPoint &pt) throw(bad_alloc);

	/**
	* Compare two rectangles and return 0 if there are at the same. This function
	* is used with the class RContainer.
	*	@param rect		Rectangle used for the comparaison.
	*/
	int Compare(RRect *rect) { return((*this)!=(*rect)); }

	/**
	* Adapt the rectangle to be contained in a given region.
	* @param clip		The region used as reference.
	* @returns 			The function returns true if the rectangle was clipped.
	*/
	bool Clip(const RRect &clip);

	/**
	* Adapt the rectangle to be contained in a given region.
	* @param limits		The point representing the limits of the region.
	* @returns 	The function returns true if the rectangle was clipped.
	*/
	bool Clip(const RPoint &limits);

	/**
	* Make a translation of the rectangle.
	* @param x		The x to add.
	* @param y		The y to add.
	*/
	void Translation(RCoord x,RCoord y);

	/**
	* This function returns true if the two rectangles overlapped.
	*/
	bool Overlap(RRect *rect);

	/**
	* This function returns true if a given point is in the rectangle.
	* @param X		X position of the point.
	* @param Y 		Y position of the point.
	*/
	bool IsIn(RCoord X,RCoord Y);

	/**
	* This function returns true if a given point is in the rectangle.
	*/
	inline bool IsIn(RPoint& pos) {return(IsIn(pos.X,pos.Y));}

	/**
	* The assign Operator.
	*/
	inline RRect& operator=(const RRect &rect) {Pt1=rect.Pt1;Pt2=rect.Pt2;return(*this);}

	/**
	* Return a pointer to a temporary object of class rectangle.
	*/
	static RRect* GetRect(void);
};


//---------------------------------------------------------------------------
/**
* This class implements a container of rectangles.
* @author Pascal Francq
* @short Rectangles container.
*/
class RRects : public RStd::RContainer<RRect,unsigned int,true,false>
{
public:

	/**
	* Construct a container of rectangles.
	*/
	RRects(void);

	/**
	* Construct a container of rectangles with a intial maximal size.
    * @param max	Initial maxiaml size.
	*/
	RRects(unsigned int max);

	/**
	* Construct a container of rectangles from another.
	* @param rects	The container used as reference.
	*/
	RRects(RRects *rects);

	/**
	* Return the area of all the rectangles.
	*/
	RCoord Area(void);
};


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif