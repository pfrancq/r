/*

	R Project Library

	Geometry.cpp

	Geometry base classes - Basic definition - Implementation.

	Copyright 2000-2003 by the Universit�Libre de Bruxelles.

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
// include files for R Project
#include <rgeometry.h>
using namespace R;



//------------------------------------------------------------------------------
void R::AdaptXY(tCoord& x,tCoord& y,const RDirection from)
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

		case NoDirection:
			RAssertMsg("Direction can't be undefined");
			break;

		default:
			RAssertMsg("Not a valid Direction in this context");
			break;
	}
}


//------------------------------------------------------------------------------
void R::AdaptTestXY(tCoord& x,tCoord& y,const RDirection from)
{
	switch(from)
	{
		case Left:
			y--;
			break;

		case Right:
			y++;
			break;

		case Down:
			x++;
			break;

		case Up:
			x--;
			break;

		case NoDirection:
			RAssertMsg("Direction can't be undefined");
			break;

		default:
			RAssertMsg("Not a valid Direction in this context");
			break;
	}
}
