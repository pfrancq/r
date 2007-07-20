/*

	R Project Library

	RRect.h

	Rectangle - Header

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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
#ifndef RRect_H
#define RRect_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpoint.h>


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
	* @param rect           The rectangle used as reference.
	*/
	RRect(const RRect* rect);

	/**
	* Construct a rectangle with two points.
	* @param pt1            Point used as (left,bottom) edge.
	* @param pt2            Point used as (right,up) edge.
	*/
	RRect(const RPoint* pt1,const RPoint* pt2);

	/** Construct a rectangle from four coordinates.
	* @param MinX           The most left position.
	* @param MinY           The most bottom position
	* @param MaxX           The most right position.
	* @param MaxY           The most up position.
	*/
	RRect(const tCoord MinX,const tCoord MinY,const tCoord MaxX,const tCoord MaxY);

	/**
	* Return the width of the rectangle.
	*/
	tCoord Width(void) const;

	/**
	* Return the height of the rectangle.
	*/
	tCoord Height(void) const;

	/**
	* Return the width of the rectangle.
	*/
	tCoord GetWidth(void) const;

	/**
	* Return the height of the rectangle.
	*/
	tCoord GetHeight(void) const;

	/**
	* Return the area of the rectangle.
	*/
	inline tCoord Area(void) const {return(Height()*Width());}

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
	RRect& operator+=(const RPoint& pt);

	/**
	* Make a minus translation of the rectangle.
	* @param pt             The point representing the vector used.
	*/
	RRect& operator-=(const RPoint& pt);

	/**
	* Compare two rectangles and return 0 if there are at the same. This function
	* is used with the class RContainer.
	* @param rect           Rectangle used for the comparaison.
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
	* Make a translation of the rectangle.
	* @param x              The x to add.
	* @param y              The y to add.
	*/
	void Translation(const tCoord x,const tCoord y);

	/**
	* This function returns true if the two rectangles overlapped.
	*/
	bool Overlap(const RRect* rect) const;

	/**
	* This function returns true if a given point is in the rectangle.
	* @param X              X position of the point.
	* @param Y              Y position of the point.
	*/
	bool IsIn(const tCoord X,const tCoord Y) const;

	/**
	* This function returns true if a given point is in the rectangle.
	*/
	inline bool IsIn(const RPoint& pos) const {return(IsIn(pos.X,pos.Y));}

	/**
	* The assign Operator.
	*/
	inline RRect& operator=(const RRect& rect) {Pt1=rect.Pt1;Pt2=rect.Pt2;return(*this);}

	/**
	* Get the X position of the first point.
	*/
	tCoord GetX1(void) const {return(Pt1.GetX());}

	/**
	* Get the Y position of the first point.
	*/
	tCoord GetY1(void) const {return(Pt1.GetY());}

	/**
	* Get the X position of the second point.
	*/
	tCoord GetX2(void) const {return(Pt2.GetX());}

	/**
	* Get the Y position of the second point.
	*/
	tCoord GetY2(void) const {return(Pt2.GetY());}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
