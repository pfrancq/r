/*

  RPoint.h

  Point - Header

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
#ifndef RPointH
#define RPointH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rstd/rcontainer.h"
using namespace RStd;


//---------------------------------------------------------------------------
// Forward class declaration
namespace RGeometry
{
	class RPolygon;
}


//---------------------------------------------------------------------------
// include files for RGeometry
#include "geometry.h"
using namespace RGeometry;


//---------------------------------------------------------------------------
namespace RGeometry{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// RPoint
class RPoint
{
public:
  RCoord X,Y;
	#if RGEOMETRY_DIMS==3
		RCoord Z;
	#endif
	
  RPoint(void);
  RPoint(RCoord x,RCoord y,RCoord z=0);
  RPoint(const RPoint& pt);
  RPoint(RPoint *pt);
  inline int Compare(RPoint *pt) { return(this==pt); }
  inline RPoint& operator=(const RPoint &pt);
  inline bool operator==(const RPoint &pt);
  inline bool operator!=(const RPoint &pt);
	~RPoint(void) {}
};


//---------------------------------------------------------------------------
// RPoints
class RPoints : public RContainer<RPoint,unsigned int,true,false>
{
public:
	RPoints(void);
	RPoints(unsigned int max);
	RPoints(RPoints *points);
  inline void AddPoint(RPoint* pt);
  inline void AddPoint(RCoord x,RCoord y,RCoord z=0);
  bool IsIn(const RPoint &pt);
	void GetPolygon(RPolygon *poly);
  RPoints& operator=(const RPoints &points);		
};

#include "rpoint.hh"

}  //-------- End of namespace RStd ---------------------------------------
#endif
