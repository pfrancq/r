/*

  RRect.cpp

  Rectangle - Implemtation

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
#include "rrect.h"
using namespace RGeometry;



//---------------------------------------------------------------------------
//
// Class "RRect"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RRect::RRect(void) : Pt1(),Pt2()
{
}


//---------------------------------------------------------------------------
RRect::RRect(RPoint *pt1,RPoint *pt2) : Pt1(pt1),Pt2(pt2)
{
}


//---------------------------------------------------------------------------
RRect::RRect(RCoord MinX,RCoord MinY,RCoord MaxX,RCoord MaxY) : Pt1(MinX,MinY),Pt2(MaxX,MaxY)
{
}


//---------------------------------------------------------------------------
RCoord RRect::Area(void)
{
  RCoord X=labs(Pt2.X-Pt1.X),Y=labs(Pt2.Y-Pt1.Y);
  return(X*Y);
}




//---------------------------------------------------------------------------

