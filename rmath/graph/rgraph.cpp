/*

	R Project Library

	RGraph.h

	A Graph - Implementation.

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rgraph.h>
#include <rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// clas RGraph
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGraph::RGraph(size_t nb)
	: Vertices(nb,nb/2), Edges(nb*(nb-1)/2,nb*(nb-1)/4)
{
}


//------------------------------------------------------------------------------
void RGraph::Clear(void)
{
	Vertices.Clear();
	Edges.Clear();
}


//------------------------------------------------------------------------------
RCursor<RVertex> RGraph::GetVertices(void) const
{
	return(RCursor<RVertex>(Vertices));
}


//------------------------------------------------------------------------------
RCursor<REdge> RGraph::GetEdges(void) const
{
	return(RCursor<REdge>(Edges));
}


//------------------------------------------------------------------------------
void RGraph::Insert(RVertex* v)
{
	if(v->Edges.GetNb())
		throw RException("Graph::InsertVertex(RVertex*) : Cannot insert a vertex with edges");
	Vertices.InsertPtr(v);
}


//------------------------------------------------------------------------------
RVertex* RGraph::CreateVertex(void)
{
	RVertex* ptr(new RVertex(Vertices.GetNb(),Vertices.GetMaxNb()));
	Insert(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
RVertex* RGraph::CreateVertex(const size_t id)
{
	RVertex* ptr(new RVertex(id,Vertices.GetMaxNb()));
	Insert(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
RVertex* RGraph::GetVertex(const size_t id)
{
	RVertex *ptr;
	RVertex::VertexStruct s;

	s.id=id;
	s.idx=Vertices.GetNb();
	s.nb=Vertices.GetMaxNb();
	ptr=Vertices.GetInsertPtr<RVertex::VertexStruct>(s);
	return(ptr);
}


//------------------------------------------------------------------------------
void RGraph::Insert(REdge* e)
{
	Edges.InsertPtr(e);
}


//------------------------------------------------------------------------------
REdge* RGraph::CreateEdge(RVertex* v1,RVertex* v2,double w)
{
	REdge* ptr(new REdge(v1,v2,w));
	Insert(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
void RGraph::MinSpanningTree(RGraph* g)
{
	RVertex *v1, *v2;
	REdge *best=0;
	double bestw;
	size_t nb=Vertices.GetNb();
	bool b1,b2,b(false);

	g->Clear();
	if(!nb) return;
	g->CreateVertex(Vertices[0]->Id);
	while(--nb)
	{
		bestw=HUGE_VAL;

		// Go through the edges and find the minimal one with a connected.
		RCursor<REdge> e(Edges);
		for(e.Start();!e.End();e.Next())
		{
			// Test if possible good vertex
			if(e()->Weight<bestw)
			{
				// Test if only one is in g
				b1=g->Vertices.IsIn<const size_t>(e()->GetVertex1()->GetId());
				b2=g->Vertices.IsIn<const size_t>(e()->GetVertex2()->GetId());
				if((b1&&!b2)||(!b1&&b2))
				{
					best=e();
					bestw=best->Weight;
					b=b1;
				}
			}
		}

		// Insert the vertex
		if(b)
		{
			v1=g->Vertices.GetPtr<const size_t>(best->GetVertex1()->GetId());
			v2=g->CreateVertex(best->GetVertex2()->GetId());
		}
		else
		{
			v1=g->Vertices.GetPtr<const size_t>(best->GetVertex2()->GetId());
			v2=g->CreateVertex(best->GetVertex1()->GetId());
		}

		// Insert the Edge
		g->Edges.InsertPtr(new REdge(v1,v2,best->GetWeight()));
	}
}


//------------------------------------------------------------------------------
RGraph::~RGraph(void)
{
}
