/*

	R Project Library

	RRect.h

	Rectangle - Header

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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


//------------------------------------------------------------------------------
#ifndef RRect_H
#define RRect_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/rpoint.h>


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
	RRect(const RCoord MinX,const RCoord MinY,const RCoord MaxX,const RCoord MaxY);

	/**
	* Return the width of the rectangle.
	*/
	RCoord Width(void) const;

	/**
	* Return the height of the rectangle.
	*/
	RCoord Height(void) const;

	/**
	* Return the area of the rectangle.
	*/
	inline RCoord Area(void) const {return(Height()*Width());}

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
	RRect& operator+=(const RPoint& pt) throw(bad_alloc);

	/**
	* Make a minus translation of the rectangle.
	* @param pt             The point representing the vector used.
	*/
	RRect& operator-=(const RPoint& pt) throw(bad_alloc);

	/**
	* Compare two rectangles and return 0 if there are at the same. This function
	* is used with the class RContainer.
	* @param rect           Rectangle used for the comparaison.
	*/
	int Compare(const RRect* rect) const { return((*this)!=(*rect)); }

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
	void Translation(const RCoord x,const RCoord y);

	/**
	* This function returns true if the two rectangles overlapped.
	*/
	bool Overlap(const RRect* rect) const;

	/**
	* This function returns true if a given point is in the rectangle.
	* @param X              X position of the point.
	* @param Y              Y position of the point.
	*/
	bool IsIn(const RCoord X,const RCoord Y) const;

	/**
	* This function returns true if a given point is in the rectangle.
	*/
	inline bool IsIn(const RPoint& pos) const {return(IsIn(pos.X,pos.Y));}

	/**
	* Return true if the polygon poly is inside the rectangle.
	* @param poly           The polygon to known if is in.
	*/
	bool IsIn(const RPolygon& poly) const
		{return(IsIn(&poly));}

	/**
	* Return true if the polygon poly is inside the rectangle.
	* @param poly           The polygon to known if is in.
	*/
	bool IsIn(const RPolygon* poly) const;

	/**
	* The assign Operator.
	*/
	inline RRect& operator=(const RRect& rect) {Pt1=rect.Pt1;Pt2=rect.Pt2;return(*this);}

	/**
	* Return a pointer to a temporary object of class rectangle.
	*/
	static RRect* GetRect(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
