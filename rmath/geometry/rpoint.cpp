/*

  RPoint.cpp

  Point implementation

  By (c) P. Francq, 1999.

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
#include "rpoint.h"
using namespace RGeometry;


//---------------------------------------------------------------------------
//
// Class "RPoint"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPoint::RPoint(void)
{
  X=Y=0;
}


//---------------------------------------------------------------------------
RPoint::RPoint(RCoord x,RCoord y,RCoord z)
{
  X=x;
  Y=y;
  #if RGEOMETRY_DIMS==3
  	Z=z;
  #endif
}


//---------------------------------------------------------------------------
RPoint::RPoint(const RPoint& pt)
{
  X=pt.X;
  Y=pt.Y;
  #if RGEOMETRY_DIMS==3
  	Z=pt.Z;
  #endif
}


//---------------------------------------------------------------------------
RPoint::RPoint(RPoint *pt)
{
  X=pt->X;
  Y=pt->Y;
  #if RGEOMETRY_DIMS==3
  	Z=pt->Z;
  #endif
}


//---------------------------------------------------------------------------

