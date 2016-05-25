/*

	R Project Library

	RLine.h

	Line - Header.

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
* This class represent a line. Be default, Pt1 is always the most bottom/left
* point and Pt2 is always the most upper/right point.
*
* A line can also be considered as an oriented segment.
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

	/**
	 * Is the segment oriented to the top.
	 */
	bool OrientedTop;

	/**
	 * Is the segment oriented to the right.
	 */
	bool OrientedRight;

public:

	/**
	* Construct a imaginary line.
	*/
	RLine(void);

	/**
	 * Copy constructor.
    * @param line           Line used for the initialisation.
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
	* @param reorder        Define if the line must be reorder.
	*/
	RLine(tCoord x1,tCoord y1,tCoord x2,tCoord y2,bool seg=true,bool reorder=true);

	/**
	* Construct a line. The points are eventually exchanged to ensure that the
	* first one is the most left-upper point.
	* @param pt1            The first point used.
	* @param pt2            The second point used.
	* @param seg            Is the line a segment.
	* @param reorder        Define if the line must be reorder.
	*/
	RLine(const RPoint& pt1,const RPoint& pt2,bool seg=true,bool reorder=true);

private:

	/**
	 * ReOrder de points if necessary to follow the convention.
    */
	void ReOrder(void);

public:

	/**
	* Compare two lines. This function is used with the class RContainer.
	* @param line            Line used for the comparison.
	* @return
	* - -1 The left-bottom point of the current polygon is more at the left (at
	*   the bottom) of the other one. If identical, the second points are
	*   compared.
	* - 0 if there have the points.
	* - +1 The left-bottom point of the current line is more at the right (
	*   upper) of the other one. If identical, the second points are compared.
	*/
	int Compare(const RLine& line) const;

	/**
	* Compute the length of the segment formed by both points.
	* @return the length.
	*/
	tCoord GetLength(void) const;

	/**
	* Compute the angle of the line compared to an horizontal one.
	* @param ccw             Define if the angles are measured counter-clockwise
	*                        or not.
	* @return the angle.
	*/
	tCoord GetAngle(bool ccw=true) const;

	/**
	* Compute the angle of the line compared to another one.
	* @param line            Line to compared with.
	* @param ccw             Define if the angles are measured counter-clockwise
	*                        or not.
	* @return the angle.
	*/
	tCoord GetAngle(const RLine& line,bool ccw=true) const;

	/**
	* Set new points to the line.
	* @param x1             X-coordinate of the first point used.
	* @param y1             Y-coordinate of the first point used.
	* @param x2             X-coordinate of the second point used.
	* @param y2             Y-coordinate of the second point used.
	* @param seg            Is the line a segment.
	* @param reorder        Define if the line must be reorder.
	*/
	void SetPoints(tCoord x1,tCoord y1,tCoord x2,tCoord y2,bool seg=true,bool reorder=true);

	/**
	* Set new points to the line.
	* @param pt1            The first point used.
	* @param pt2            The second point used.
	* @param seg            Is the line a segment.
	* @param reorder        Define if the line must be reorder.
	*/
	void SetPoints(const RPoint& pt1,const RPoint& pt2,bool seg=true,bool reorder=true)
	{
		SetPoints(pt1.X,pt1.Y,pt2.X,pt2.Y,seg,reorder);
	}

	/**
	 * Look a segment is oriented to the top.
	 * @return true if it is the case.
	 * @exception An exception is generated if the line is not a segment.
	 */
	bool IsOrientedTop(void) const;

	/**
	 * Look a segment is oriented to the right.
	 * @return true if it is the case.
	 * @exception An exception is generated if the line is not a segment.
	 */
	bool IsOrientedRight(void) const;

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
	bool IsHorizontal(void) const {return(Abs(Pt1.Y-Pt2.Y)<cEpsi);}

	/**
	 * Verify if a line is a vertical one.
    * @return true if the line is vertical.
    */
	bool IsVertical(void) const {return(Abs(Pt1.X-Pt2.X)<cEpsi);}

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
extern std::ostream& operator<<(std::ostream& os,const R::RLine& line);

#ifndef __APPLE__
}
#endif


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif
