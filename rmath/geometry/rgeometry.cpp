/*

	Rainbow Library Project

	Geometry.cpp

	Geometry base classes - Basic definition - Implementation.

	(c) 2000 by P. Francq.

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



//---------------------------------------------------------------------------
// include files for Rainbow
#include "geometry.h"
using namespace RGeometry2D;


//---------------------------------------------------------------------------
// Global variables
double RGeometry2D::Epsi=0.0001;


//---------------------------------------------------------------------------
void RGeometry2D::AdaptXY(RCoord &x,RCoord &y,RDirection from)
{
	switch(from)
	{
		case Left:
			x++;
			break;

		case Right:
			x--;
			break;

		case Down:
			y++;
			break;

		case Up:
			y--;
			break;
	}
}