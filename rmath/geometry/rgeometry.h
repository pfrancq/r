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

/*
		The followind defines are used;
		RGEOMETRY_TYPE : by default it is long
*/


//---------------------------------------------------------------------------
#ifndef GeometryH
#define GeometryH


//---------------------------------------------------------------------------
#include <limits.h>


//---------------------------------------------------------------------------
namespace RGeometry{
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
// Defines
#ifndef RGEOMETRY_TYPE
	#define RGEOMETRY_TYPE int
#endif


//---------------------------------------------------------------------------
// Typedefs
typedef RGEOMETRY_TYPE RCoord;                 // A coordinate type


//---------------------------------------------------------------------------
// Const
const RCoord MaxCoord=INT_MAX;
const char orNormal=0;
const char orNormalMirrorX=1;
const char orNormalMirrorY=2;
const char orNormalMirrorYX=3;
const char orRota90=4;
const char orRota90MirrorX=5;
const char orRota90MirrorY=6;
const char orRota90MirrorYX=7;


//---------------------------------------------------------------------------
// General Varaibles
extern double Epsi;														// tolerance for calculations


}  //-------- End of namespace RGeometry ------------------------------------
#endif
