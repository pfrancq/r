/*

	R Project Library

	RPoint.h

	Point - Header

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
#ifndef RPoint_H
#define RPoint_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/geometry.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
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
	* @param pt             The point used as reference.
	*/
	RPoint(const RPoint& pt);

	/**
	* Construct a point from pt.
	* @param pt             The point used as reference.
	*/
	RPoint(const RPoint* pt);

	/**
	* The equal operator.
	*/
	inline bool operator==(const RPoint& pt) const {return((X==pt.X)&&(Y==pt.Y));}

	/**
	* The non-equal operator.
	*/
	inline bool operator!=(const RPoint& pt) const {return((X!=pt.X)||(Y!=pt.Y));}

	/**
	* Make a translation of the point.
	* @param pt             The point representing the vector used.
	*/
	RPoint& operator+=(const RPoint& pt) {X+=pt.X;Y+=pt.Y;return(*this);}

	/**
	* Make a translation of the point.
	* @param pt             The point representing the vector used.
	*/
	RPoint& operator-=(const RPoint& pt) {X-=pt.X;Y-=pt.Y;return(*this);}

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param pt             Point used for the comparaison.
	*/
	inline int Compare(const RPoint* pt) const { return((*this)!=(*pt)); }

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param pt             Point used for the comparaison.
	*/
	inline int Compare(const RPoint& pt) const { return((*this)!=pt); }

	/**
	* Assignment operator.
	*/
	inline RPoint& operator=(const RPoint& pt) {X=pt.X;Y=pt.Y;return(*this);}

	/**
	* This function returns true if the two points are side by side.
	*/
	inline bool Near(const RPoint* pt) const;

	/**
	* Set the point to the given position.
	* @param x	            X Position.
	* @param y              Y Position.
	*/
	void Set(const RCoord x,const RCoord y) {X=x; Y=y;}

	/**
	* Return the manhatan distance from a given point.
	*/
	RCoord ManhattanDist(const RPoint& pt) const;

	/**
	* Return the euclidean distance from a given point.
	*/
	double EuclideanDist(const RPoint& pt) const;

	/**
	* Return the length of the vector represented by the current point.
	*/
	inline double Length(void) const {return(sqrt(static_cast<double>((X*X)+(Y*Y))));}

	/**
	* Test if the point is a valid one.
	*/
	bool IsValid(void) const { return((X!=MaxCoord)&&(Y!=MaxCoord));}

	/**
	* Return the classification of the current point relative to the segment
	* formed by two points.
	*/
	RDirection Classify(const RPoint& p0,const RPoint& p1) const
		{ return(Classify(&p0,&p1)); }

	/**
	* Return the classification of the current point relative to the segment
	* formed by two points.
	*/
	RDirection Classify(const RPoint* p0,const RPoint* p1) const;

	/**
	* Change the "orientation" of the point.
	*/
	void ChangeOrientation(const ROrientation o);

	/**
	* Save the point in a file.
	*/
	void Save(RTextFile& f) const;
};


//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* Add two points.
*/
RPoint operator+(const RPoint& arg1,const RPoint& arg2);

//------------------------------------------------------------------------------
/**
* Substract two points.
*/
RPoint operator-(const RPoint& arg1,const RPoint& arg2);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
