/*

	R Project Library

	REdge.cpp

	Edge of a graph  - Implementation.

	(c) 2001 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgraph/redge.h>
#include <rgraph/rvertex.h>
using namespace RGraphs;



//-----------------------------------------------------------------------------
//
// class REdge
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGraphs::REdge::REdge(void)
	: v1(0), v2(0)
{
}


//-----------------------------------------------------------------------------
RGraphs::REdge::REdge(RVertex* _v1,RVertex* _v2,const double w)
	: v1(_v1), v2(_v2), Weight(w)
{
}


//-----------------------------------------------------------------------------
RGraphs::REdge::~REdge(void)
{
}
