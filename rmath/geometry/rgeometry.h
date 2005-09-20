/*

	R Project Library

	Geometry.h

	Geometry base classes - Basic definition - Header.

	Copyright 1999-2004 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef Geometry_H
#define Geometry_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rcursor.h>
#include <rtextfile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RPoint;
class RPoints;
class RLine;
class RRect;
class RRects;
class RPolygon;
class RPolygons;


//------------------------------------------------------------------------------
/**
* The RCoord type represents a coordinate.
*/
typedef int RCoord;


//------------------------------------------------------------------------------
/**
* The MaxCoord constante represent the maximal positive value that a
* coordinate can have. It is often used to indicate an invalid coordinate.
*/
const RCoord MaxCoord=2147483647;


//------------------------------------------------------------------------------
/**
* Represent the different orientation of a given polygon.
*/
enum ROrientation
{
	Normal=0,
	NormalX=1,
	NormalY=2,
	NormalYX=3,
	Rota90=4,
	Rota90X=5,
	Rota90Y=6,
	Rota90YX=7
};


//------------------------------------------------------------------------------
/**
* Represent different directions.
*/
enum RDirection
{
	NoDirection,
	Left,
	Right,
	Down,
	Up,
	Behind,
	Beyond,
	Origin,
	Destination,
	Between
};


//------------------------------------------------------------------------------
/**
* This variable represents a tolerance for calculations.
*/
extern double Epsi;


//------------------------------------------------------------------------------
/**
* Affect the coordinates in function of a given direction.
* @param x                  X Coordinate.
* @param y                  Y Coordinate.
* @param from               The direction from where "it" is coming.
*/
void AdaptXY(RCoord& x,RCoord& y,const RDirection from);


//------------------------------------------------------------------------------
/**
* Calculate the coordinates to test in function of a given direction.
* @param x                  X Coordinate.
* @param y                  Y Coordinate.
* @param from               The direction from where "it" is coming.
*/
void AdaptTestXY(RCoord& x,RCoord& y,const RDirection from);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
