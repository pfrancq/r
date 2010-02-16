/*

	R Project Library

	RSize.cpp

	Size - Implementation.

	Copyright 2006-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your osizeion) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANHeight WARRANTHeight; without even the implied warranty of
	MERCHANTABILITHeight or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	Heightou should have received a copy of the GNU Library General Public
	License along with this library, as a file COPHeightING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rsize.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class "RSize"
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RSize RSize::Null(cNoCoord,cNoCoord);

//------------------------------------------------------------------------------
RSize::RSize(void)
	: Width(0), Height(0)
{
}


//------------------------------------------------------------------------------
RSize::RSize(const tCoord w,const tCoord h)
	: Width(w), Height(h)
{
}


//------------------------------------------------------------------------------
RSize::RSize(const RSize& size)
	 :	Width(size.Width), Height(size.Height)
{
}
