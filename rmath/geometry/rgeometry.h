/*

	Rainbow Library Project

	Geometry.h

	Geometry base classes - Basic definition - Header.

	(C) 1999-2000 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
#ifndef GeometryH
#define GeometryH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <limits.h>
#include <math.h>


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstd.h>
#include <rstd/rcontainer.h>
#include <rstd/rtextfile.h>
using namespace RStd;


/**
* \namespace RGeometry2D
* \brief 2D-Geometry Classes.
*
* This namespace declares classes to manipulate geometric entities in 2D.
*/


//-----------------------------------------------------------------------------
namespace RGeometry2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RPoint;
class RPoints;
class RLine;
class RRect;
class RRects;
class RPolygon;
class RPolygons;


//-----------------------------------------------------------------------------
/**
* The RCoord type represents a coordinate.
*/
typedef int RCoord;


//-----------------------------------------------------------------------------
/**
* The MaxCoord constante represent the maximal positive value that a
* coordinate can have. It is often used to indicate an invalid coordinate.
*/
const RCoord MaxCoord=INT_MAX;


//-----------------------------------------------------------------------------
/**
* Represent the different orientation of a given polygon.
*/
enum ROrientation {Normal=0,NormalX=1,NormalY=2,NormalYX=3,Rota90=4,Rota90X=5,Rota90Y=6,Rota90YX=7};


//-----------------------------------------------------------------------------
/**
* Represent different directions.
*/
enum RDirection {NoDirection,Left,Right,Down,Up};


//-----------------------------------------------------------------------------
/**
* This variable represents a tolerance for calculations.
*/
extern double Epsi;


//-----------------------------------------------------------------------------
/**
* Affect the coordinates in function of a given direction.
*	@param x				X Coordinate.
*	@param y				Y Coordinate.
* @param from			The direction from where "it" is coming.
*/
void AdaptXY(RCoord &x,RCoord &y,RDirection from);

//-----------------------------------------------------------------------------
/**
* Calculate the coordinates to test in function of a given direction.
* @param x				X Coordinate.
* @param y				Y Coordinate.
* @param from			The direction from where "it" is coming.
*/
void AdaptTestXY(RCoord &x,RCoord &y,RDirection from);


}  //-------- End of namespace RGeometry2D ------------------------------------


//-----------------------------------------------------------------------------
#endif
