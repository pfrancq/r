/*

	R Project Library

	RVertex.cpp

	Vertex of a graph - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
using namespace std;



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
RCursor<REdge> RVertex::GetEdges(void) const
{
	return(RCursor<REdge>(Edges));
}


//------------------------------------------------------------------------------
REdge* RVertex::GetEdge(size_t id) const
{
	RCursor<REdge> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->Connects(id))
			return(Cur());
	return(0);
}


//------------------------------------------------------------------------------
REdge* RVertex::GetEdgeTo(size_t id) const
{
	RCursor<REdge> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		cout<<"  Search "<<id<<"=?="<<Cur()->GetTo()->GetId()<<endl;
		if(Cur()->GetTo()->GetId()==id)
			return(Cur());
	}
	cout<<"   Not Found"<<endl;
	return(0);
}


//------------------------------------------------------------------------------
REdge* RVertex::GetEdgeFrom(size_t id) const
{
	RCursor<REdge> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetFrom()->GetId()==id)
			return(Cur());
	return(0);
}


//------------------------------------------------------------------------------
RVertex::~RVertex(void)
{
}
