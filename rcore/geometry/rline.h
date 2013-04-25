/*

	R Project Library

	RLine.h

	Line - Header.

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
#ifndef RLine_H
#define RLine_H


//------------------------------------------------------------------------------
//#include files for R Project
#include <rpoint.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a line. In practice, Pt1 is always the most bottom/left
* point and Pt2 is always the most upper/right point.
* @short Line.
*/
class RLine
{
	/**
	* The First point of the line.
	*/
	RPoint Pt1;

	/**
	* The second point of the line.
	*/
	RPoint Pt2;

	/**
	* Is the line a segment delimited by the two points.
	*/
	bool Segment;

public:

	/**
	* Construct a imaginary line.
	*/
	RLine(void);

	/**
	 * Copy constructor.
    * @param line           Line used for the initialization.
    */
	RLine(const RLine& line);

	/**
	* Construct a line. The points are eventually exchanged to ensure that the
	* first one is the most left-upper point.
	* @param x1             X-coordinate of the first point used.
	* @param y1             Y-coordinate of the first point used.
	* @param x2             X-coordinate of the second point used.
	* @param y2             Y-coordinate of the second point used.
	* @param seg            Is the line a segment.
	*/
	RLine(tCoord x1,tCoord y1,tCoord x2,tCoord y2,const bool seg=true);

	/**
	* Construct a line. The points are eventually exchanged to ensure that the
	* first one is the most left-upper point.
	* @param pt1            The first point used.
	* @param pt2            The second point used.
	* @param seg            Is the line a segment.
	*/
	RLine(const RPoint& pt1,const RPoint& pt2,const bool seg=true);

private:

	/**
	 * ReOrder de points if necessary to follow the convention.
    */
	void ReOrder(void);

public:

	/**
	* Compute the length of the segment formed by both points.
	* @return the length.
	*/
	tCoord GetLength(void) const;

	/**
	* Compute the angle of the line compared to an horizontal one.
	* @return the angle.
	*/
	tCoord GetAngle(void) const;

	/**
	* Compute the angle of the line compared to another one.
	* @param line            Line to compared with.
	* @return the angle.
	*/
	tCoord GetAngle(const RLine& line) const;

private:

	/**
	* Look at the needed clock direction to go between the two extremities by
	* passing by an intermediate point.
	* @param pt             Intermediate point.
	* @return
	* -1 if the direction is anticlockwise.
	* 0 if the point is one the two extremities.
	* +1 if the direction is clockwise.
	*/
	int CounterClockwise(const RPoint& pt) const;

public:

	/**
	* Determine if there is an intersection between two lines. If the two lines
	* are similar, the intersection point is left undefined.
	* @param line            Line used for the comparison.
	* @param pt              Point that will contain the intersection point (if
	*                        any).
   * @return true if there is at least one intersection.
	*/
	bool Inter(const RLine& line,RPoint& pt) const;

	/**
	* Determine if there is an intersection between two lines (To verify!!!).
	* @param line           Line used for the comparison.
	* @return true if there is an intersection.
	*/
	bool Inter(const RLine& line) const;

	/**
	* Determine if a point is on the line/segment.
	* @param pt             Point used for the comparison.
	* @return true if there the point is contained.
	*/
	bool IsIn(const RPoint& pt) const;

	/**
	* Get the X position of the first point.
	* @return a coordinate.
	*/
	tCoord GetX1(void) const {return(Pt1.X);}

	/**
	* Get the Y position of the first point.
	* @return a coordinate.
	*/
	tCoord GetY1(void) const {return(Pt1.Y);}

	/**
	* Get the X position of the second point.
	* @return a coordinate.
	*/
	tCoord GetX2(void) const {return(Pt2.X);}

	/**
	* Get the Y position of the second point.
	* @return a coordinate.
	*/
	tCoord GetY2(void) const {return(Pt2.Y);}

	/**
	 * Get the most left-bottom point.
    * @return a point.
    */
	RPoint GetPt1(void) const {return(RPoint(Pt1));}

	/**
	 * Get the most right-up point.
    * @return a point.
    */
	RPoint GetPt2(void) const {return(RPoint(Pt2));}

	/**
	 * Verify that if the line is a segment or not.
    * @return true if it is a segment.
    */
	bool IsSegment(void) const {return(Segment);}

	/**
	 * Verify if a line is a horizontal one.
    * @return true if the line is horizontal.
    */
	bool IsHorizontal(void) const {return(Pt1.Y==Pt2.Y);}

	/**
	 * Verify if a line is a vertical one.
    * @return true if the line is vertical.
    */
	bool IsVertical(void) const {return(Pt1.X==Pt2.X);}

};


//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
 * Print the coordinate of the line in the form (x1,y1);(x2,y2).
 * @param os                Output stream.
 * @param line              Line to print.
 * @return the stream.
 */
std::ostream& operator<<(std::ostream& os,const RLine& line);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
