/*

  RRect.h

  Rectangle - Header

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
#ifndef RRectH
#define RRectH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rstd/rcontainer.h"
using namespace RStd;


//---------------------------------------------------------------------------
// include files for RGeometry
#include "rpoint.h"
using namespace RGeometry;


//---------------------------------------------------------------------------
namespace RGeometry{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Rectangle
class RRect
{
public:
  RPoint Pt1,Pt2;

  RRect(void);
	RRect(RRect *rect);
  RRect(RPoint *pt1,RPoint *pt1);
  RRect(RCoord MinX,RCoord MinY,RCoord MaxX,RCoord MaxY);
  double Area(void);
  inline RCoord Length(void);
  inline RCoord Width(void);
	int Compare(RRect *) { return(0); }
	~RRect(void) {}
};


//---------------------------------------------------------------------------
// Rectangles
class RRects : public RContainer<RRect,unsigned int,true,false>
{
public:
	RRects(void);
	RRects(unsigned int max);
	RRects(RRects *retcs);
  inline void AddRect(RRect* rect);
  inline void AddRect(RCoord MinX,RCoord MinY,RCoord MaxX,RCoord MaxY);
  RRects& operator=(const RRects &rects);
};


//---------------------------------------------------------------------------
// include definition of templates
#include "rrect.hh"


}  //-------- End of namespace RStd ---------------------------------------
#endif
