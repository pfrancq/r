/*

  Geometry

  Geometry base classes - Basic definition

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

/*
		The followind defines are used;
		RGEOMETRY_TYPE : by default it is long
		RGEOMETRY_DIMS : by default it is 2 (max=3)
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
// Defines
#ifndef RGEOMETRY_TYPE
	#define RGEOMETRY_TYPE long
#endif
#ifndef RGEOMETRY_DIMS
	#define RGEOMETRY_DIMS 2
#endif
#if RGEOMETRY_DIMS<2 || RGEOMETRY_DIMS>3
	#error RGEOMETRY_DIMS must be either 2 or 3.
#endif


//---------------------------------------------------------------------------
// Typedefs
typedef RGEOMETRY_TYPE RCoord;                 // A coordinate type


//---------------------------------------------------------------------------
// Const
const RCoord MaxCoord=ULONG_MAX;
const char orNormal=0;
const char orNormalMirrorX=1;
const char orNormalMirrorY=2;
const char orNormalMirrorYX=3;
const char orRota90=4;
const char orRota90MirrorX=5;
const char orRota90MirrorY=6;
const char orRota90MirrorYX=7;



}  //-------- End of namespace RStd ---------------------------------------
#endif
