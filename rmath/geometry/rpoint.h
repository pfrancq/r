/*

	Rainbow Library Project

	RPoint.h

	Point - Header

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
#ifndef RPointH
#define RPointH


//---------------------------------------------------------------------------
// include files for RGeometry
#include <rgeometry/geometry.h>
using namespace RGeometry2D;


//---------------------------------------------------------------------------
namespace RGeometry2D{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* This class represent a point.
* @author Pascal Francq
* @short Point class.
*/
class RPoint
{
public:

	/**
	* This is the X coordinate.
	*/
	RCoord X;

	/**
	* This is the Y coordinate.
	*/
	RCoord Y;

	/**
	* Construct a point at (0,0).
	*/	
	RPoint(void);

	/**
	* Construct a point at (x,y).
	*/
	RPoint(const RCoord x,const RCoord y);

	/**
	* Construct a point from pt.
	* @param pt		The point used as reference.
	*/
	RPoint(const RPoint& pt);

	/**
	* Construct a point from pt.
	* @param pt		The point used as reference.
	*/
	RPoint(const RPoint *pt);

	/**
	* The equal operator.
	*/
	inline bool operator==(const RPoint &pt) {return((X==pt.X)&&(Y==pt.Y));}

	/**
	* The non-equal operator.
	*/
	inline bool operator!=(const RPoint &pt) {return((X!=pt.X)||(Y!=pt.Y));}

	/**
	* Make a translation of the point.
	* @param pt 	The point representing the vector used.
	*/
	RPoint& operator+=(const RPoint &pt) {X+=pt.X;Y+=pt.Y;return(*this);}

	/**
	* Make a translation of the point.
	* @param pt 	The point representing the vector used.
	*/
	RPoint& operator-=(const RPoint &pt) {X-=pt.X;Y-=pt.Y;return(*this);}

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param pt		Point used for the comparaison.
	*/
	inline int Compare(const RPoint *pt) { return((*this)!=(*pt)); }

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param pt		Point used for the comparaison.
	*/
	inline int Compare(const RPoint &pt) { return((*this)!=pt); }

	/**
	* Assignment operator.
	*/
	inline RPoint& operator=(const RPoint &pt) {X=pt.X;Y=pt.Y;return(*this);}

	/**
	* This function returns true if the two points are side by side.
	*/
	inline bool Near(const RPoint *pt) {return((labs(X-pt->X)<=1)&&(labs(Y-pt->Y)<=1));}

	/**
	* Set the point to the given position.
	* @param x	X Position;
	* @param y	Y Position;
	*/
	void Set(const RCoord x,const RCoord y) {X=x; Y=y;}

	/**
	* Return the manhatan distance from a given point.
	*/
	inline RCoord ManhattanDist(const RPoint &pt) {return(labs(X-pt.X)+labs(Y-pt.Y));}

	/**
	* Return a pointer to a temporary object of class point.
	*/
	static RPoint* GetPoint(void);
};


//---------------------------------------------------------------------------
/**
* This class represents a set of points regrouped in a container.
* @author Pascal Francq
* @short Container of points.
*/
class RPoints : public RStd::RContainer<RPoint,unsigned int,true,false>
{
public:

	/**
	* Construct a container of points.
	*/
	RPoints(void);

	/**
	* Construct a container of points with an initial maximal size.
	* @param max		Initial maximal size of the container.
	*/
	RPoints(unsigned int max);

	/**
	* Construct a container of points from another one.
	* @param points		The container used as reeference.
	*/
	RPoints(RPoints *points);

	/**
	* Find the next point to the left and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    The polygons used as reference.
	*/
	RPoint* FindLeft(RPoint *pt,RPolygons *polys);

	/**
	* Find the next point to the right and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    The polygons used as reference.
	*/
	RPoint* FindRight(RPoint *pt,RPolygons *polys);

	/**
	* Find the next point to the bottom and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    	The polygons used as reference.
	*/
	RPoint* FindBottom(RPoint *pt,RPolygons *polys);

	/**
	* Find the next point to the up and that is on a vertex of a polygon from
	* a given set. This function is used to calculated the union of polygons.
	* @param pt				Point used as reference.
	* @param polys    The polygons used as reference.
	*/
	RPoint* FindUp(RPoint *pt,RPolygons *polys);

	/**
	* Find the most bottom-left point of the container.
	*/
	RPoint* FindBottomLeft(void);

	/**
	* This function returns true when there are duplicate points.
	*/
	bool DuplicatePoints(void);

	/**
	* Assignment operator.
	*/
	RPoints& operator=(const RPoints &points);		
};


//---------------------------------------------------------------------------
/**
* Add two points.
*/
RPoint& operator+(const RPoint &arg1,const RPoint &arg2);

/**
* Substract two points.
*/
RPoint& operator-(const RPoint &arg1,const RPoint &arg2);


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
