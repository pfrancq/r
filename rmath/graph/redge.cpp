/*

	R Project Library

	REdge.cpp

	Edge of a graph  - Implementation.

	Copyright 2001-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/redge.h>
#include <rmath/rvertex.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class REdge
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
REdge::REdge(void)
	: v1(0), v2(0)
{
}


//------------------------------------------------------------------------------
REdge::REdge(RVertex* _v1,RVertex* _v2,const double w)
	: v1(_v1), v2(_v2), Weight(w)
{
}


//------------------------------------------------------------------------------
REdge::~REdge(void)
{
}
