/*

	R Project Library

	RVertex.cpp

	Vertex of a graph - Implementation.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
#include <rvertex.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RVertex
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RVertex::RVertex(size_t id,size_t nb)
	: Id(id), Edges(nb,nb/2)
{
}


//------------------------------------------------------------------------------
RVertex::RVertex(const RVertex& v)
	: Edges(v.Edges.GetMaxNb(),v.Edges.GetIncNb())
{
}


//------------------------------------------------------------------------------
RVertex::RVertex(const VertexStruct& s)
	: Id(s.id), Edges(s.nb,s.nb/2)
{
}


//------------------------------------------------------------------------------
RCursor<REdge> RVertex::GetEdges(void) const
{
	return(RCursor<REdge>(Edges));
}


//------------------------------------------------------------------------------
RVertex::~RVertex(void)
{
}
