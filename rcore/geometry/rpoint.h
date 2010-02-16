/*

	R Project Library

	RPoint.h

	Point - Header

	Copyright 1999-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef RPoint_H
#define RPoint_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// forward class declaration
class RTextFile;


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
	tCoord X;

	/**
	* This is the Y coordinate.
	*/
	tCoord Y;

	/**
	* Construct a point at (0,0).
	*/
	RPoint(void);

	/**
	* Construct a point at (x,y).
	* @param x	            X Position.
	* @param y              Y Position.
	*/
	RPoint(tCoord x,tCoord y);

	/**
	* Construct a point from pt.
	* @param pt             The point used as reference.
	*/
	RPoint(const RPoint& pt);

	/**
	* The equal operator.
	* @param pt             Point used for the comparison.
	*/
	inline bool operator==(const RPoint& pt) const {return((X==pt.X)&&(Y==pt.Y));}

	/**
	* The non-equal operator.
	 @param pt             Point used for the comparison.
	*/
	inline bool operator!=(const RPoint& pt) const {return((X!=pt.X)||(Y!=pt.Y));}

	/**
	* Make a translation of the point.
	* @param pt             Point used for the comparison.
	*/
	RPoint& operator+=(const RPoint& pt) {X+=pt.X;Y+=pt.Y;return(*this);}

	/**
	* Make a translation of the point.
	* @param pt             Point used for the comparison.
	*/
	RPoint& operator-=(const RPoint& pt) {X-=pt.X;Y-=pt.Y;return(*this);}

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param pt             Point used for the comparison.
	*/
	inline int Compare(const RPoint* pt) const { return((*this)!=(*pt)); }

	/**
	* Compare two points and return 0 if there are at the same position. This function
	* is used with the class RContainer.
	* @param pt             Point used for the comparison.
	*/
	inline int Compare(const RPoint& pt) const { return((*this)!=pt); }

	/**
	* Assignment operator.
	* @param pt              Original point.
	*/
	inline RPoint& operator=(const RPoint& pt) {X=pt.X;Y=pt.Y;return(*this);}

	/**
	* This function returns true if the two points are side by side.
	* @param pt              Reference point.
	*/
	inline bool Near(const RPoint& pt) const;

	/**
	* Set the point to the given position.
	* @param x	            X Position.
	* @param y              Y Position.
	*/
	inline void Set(tCoord x,tCoord y) {X=x; Y=y;}

	/**
	* Return the Manhattan distance from a given point.
	* @param pt              Reference point.
	*/
	tCoord ManhattanDist(const RPoint& pt) const;

	/**
	* Return the euclidean distance from a given point.
	* @param pt              Reference point.
	*/
	double EuclideanDist(const RPoint& pt) const;

	/**
	* Return the length of the vector represented by the current point.
	*/
	inline double Length(void) const {return(sqrt(static_cast<double>((X*X)+(Y*Y))));}

	/**
	* Test if the point is a valid one.
	*/
	bool IsValid(void) const { return((X!=cNoCoord)&&(Y!=cNoCoord));}

	/**
	* @return the classification of the current point relative to the segment
	* formed by two points.
	* @param p0              First point.
	* @param p1              Second point.
	*/
	tDirection Classify(const RPoint& p0,const RPoint& p1) const;

	/**
	* @return the classification of the current point relative to the segment
	* formed by two points.
	* @param p0              First point.
	* @param p1              Second point.
	*/
	inline tDirection Classify(const RPoint* p0,const RPoint* p1) const
		{ return(Classify(*p0,*p1)); }

	/**
	* Change the "orientation" of the point.
	* @param o               Orientation.
	*/
	void ChangeOrientation(const tOrientation o);

	/**
	* Save the point in a file.
	* @param f               File where to save.
	*/
	void Save(RTextFile& f) const;

	/**
	 * Represent an invalid point.
	 */
	static RPoint Null;
};


//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* Add two points.
*/
inline RPoint operator+(const RPoint& arg1,const RPoint& arg2)  {return(RPoint(arg1)+=arg2);}

//------------------------------------------------------------------------------
/**
* Subtract two points.
*/
inline RPoint operator-(const RPoint& arg1,const RPoint& arg2) {return(RPoint(arg1)-=arg2);}


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
