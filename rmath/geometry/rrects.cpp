/*

	R Project Library

	RRect.cpp

	Rectangle - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rrects.h>
#include <rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RRects
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RRects::RRects(void)
	: RContainer<RRect,true,false>(20,10)
{
}


//------------------------------------------------------------------------------
RRects::RRects(size_t max)
	: RContainer<RRect,true,false>(max,10)
{
}


//------------------------------------------------------------------------------
RRects::RRects(const RRects& rects)
	: RContainer<RRect,true,false>(rects)
{
}


//------------------------------------------------------------------------------
tCoord RRects::Area(void) const
{
	tCoord Area;

	RCursor<RRect> rect(*this);
	for(rect.Start(),Area=0;!rect.End();rect.Next())
		Area+=rect()->GetArea();
	return(Area);
}
