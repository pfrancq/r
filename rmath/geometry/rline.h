/*

  RLine.h

  Line - Header.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


//---------------------------------------------------------------------------
#ifndef RLineH
#define RLineH


//---------------------------------------------------------------------------
#include "rpoint.h"
using namespace RGeometry2D;


//---------------------------------------------------------------------------
namespace RGeometry2D{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This class represent a line.
	* @author Pascal Francq
	* @short Line.
	*/
class RLine
{
	/** Internal function to determine if two lines intersect.*/
  int CCW(const RPoint &) const;
public:
	/** The First point of the line.*/
  RPoint Pt1;
	/** The second point of the line.*/
	RPoint Pt2;
	/** Is the line a segment delimited by the two points.*/
  bool Segment;

	/** Construct a imaginary line. */
  RLine(void);

	/** Construct a line.
		* @param pt1		The first point used.
		* @param pt2		The second point used.
		* @param seg		Is the line a segment.
		*/
  RLine(RPoint *pt1,RPoint *pt2,bool seg=true);

	/** Return the length of the line.*/
  float Length(void);

	/** Determine if there is an intersection between two lines.*/
  bool Inter(RLine *);

	/** Determine if there is an intersection between two lines.*/
  bool Inter(const RLine &);

	/** Determine if a point is on the line/segment.*/
	bool IsIn(const RPoint &);
};


}  //-------- End of namespace RStd ---------------------------------------

//---------------------------------------------------------------------------
#endif
