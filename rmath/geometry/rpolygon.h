/*

  Polygons.h

  Polygons - Header.

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
#ifndef PolygonsH
#define PolygonsH


//---------------------------------------------------------------------------
#include "rpoint.h"
#include "rrect.h"
using namespace RGeometry;
#include "rcontainer.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RGeometry{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Polygon
class RPolygon : public RContainer<RPoint,unsigned,true,false>
{
public:
  RPolygon(void);
  RPolygon(int Max);
  RPolygon(RPolygon *);
  RPolygon& operator=(const RPolygon &poly);
  int Compare(RPolygon*);
  inline void AddPoint(RPoint* pt);
  inline void AddPoint(RCoord x,RCoord y,RCoord z=0);
  bool IsIn(RPoint *pt);
  RRect* Boundary(void);
  void Boundary(RRect&);
  void Fill(bool **Occupied,RPoint Pt);
  void Orientation(char Ori);  
};


//---------------------------------------------------------------------------
// A collection of polygons
class RPolygons : public RContainer<RPolygon,unsigned,true,false>
{
public:
  RPolygons(void);
  void Union(RPolygon *poly);
};


#include "polygons.hh"

}  //-------- End of namespace RStd ---------------------------------------
#endif
