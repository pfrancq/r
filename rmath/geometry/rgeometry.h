/*

  Geometry.h

  Geometry base classes - Basic definition - Header.

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
#ifndef GeometryH
#define GeometryH


//---------------------------------------------------------------------------
#include <limits.h>


//---------------------------------------------------------------------------
namespace RGeometry2D{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Forward class declaration
class RPoint;
class RPoints;
class RLine;
class RRect;
class RRects;
class RPolygon;
class RPolygons;


//---------------------------------------------------------------------------
/** The RCoord type represents a coordinate.*/
typedef int RCoord;


//---------------------------------------------------------------------------
/** The MaxCoord constante represent the maximal positive value that a
	* coordinate can have. It is often used to indicate an invalid coordinate.*/
const RCoord MaxCoord=INT_MAX;


//---------------------------------------------------------------------------
/** Represent the different orientation of a given polygon.*/
enum ROrientation{Normal=0,NormalX=1,NormalY=2,NormalYX=3,Rota90=4,Rota90X=5,Rota90Y=6,Rota90YX=7};


//---------------------------------------------------------------------------
/** This variable represents a tolerance for calculations.*/
extern double Epsi;


}  //-------- End of namespace RGeometry ------------------------------------
#endif
