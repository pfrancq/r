/*

	R Project Library

	RLines.cpp

	Lines - Implementation.

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
#include <rstd.h>
#include <rlines.h>
//#include <rpolygon.h>
//#include <rpolygons.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RLines
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLines::RLines(void)
	: RContainer<RLine,true,false>(10,5)
{
}


//------------------------------------------------------------------------------
RLines::RLines(const size_t max)
	: RContainer<RLine,true,false>(max,5)
{
}


//------------------------------------------------------------------------------
RLines::RLines(const RLines& lines)
	: RContainer<RLine,true,false>(lines.GetMaxNb(),lines.GetIncNb())
{
	RCursor<RLine> Line(*this);
	for(Line.Start();!Line.End();Line.Next())
		InsertPtr(new RLine(*Line()));
}


//------------------------------------------------------------------------------
RLines& RLines::operator=(const RLines& lines)
{
	RContainer<RLine,true,false>::operator=(lines);
	return(*this);
}
