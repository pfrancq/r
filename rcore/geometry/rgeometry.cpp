/*

	R Project Library

	Geometry.cpp

	Geometry base classes - Basic definition - Implementation.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
void R::AdaptXY(tCoord& x,tCoord& y,const tDirection from)
{
	switch(from)
	{
		case dLeft:
			x++;
			break;

		case dRight:
			x--;
			break;

		case dDown:
			y++;
			break;

		case dUp:
			y--;
			break;

		case dNoDirection:
			mAssertMsg("Direction can't be undefined");
			break;

		default:
			mAssertMsg("Not a valid Direction in this context");
			break;
	}
}


//------------------------------------------------------------------------------
void R::AdaptTestXY(tCoord& x,tCoord& y,const tDirection from)
{
	switch(from)
	{
		case dLeft:
			y--;
			break;

		case dRight:
			y++;
			break;

		case dDown:
			x++;
			break;

		case dUp:
			x--;
			break;

		case dNoDirection:
			mAssertMsg("Direction can't be undefined");
			break;

		default:
			mAssertMsg("Not a valid Direction in this context");
			break;
	}
}
