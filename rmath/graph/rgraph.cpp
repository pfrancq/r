/*

	R Project Library

	RGraph.h

	A Graph - Implementation.

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/rgraph.h>
#include <rstd/rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// clas RGraph
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGraph::RGraph(unsigned int nb)
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
RVertex* RGraph::CreateVertex(void)
{
	RVertex *ptr;

	Vertices.InsertPtr(ptr=new RVertex(Vertices.GetNb(),Vertices.GetNb(),Vertices.GetMaxNb()));
	return(ptr);
}


//------------------------------------------------------------------------------
RVertex* RGraph::CreateVertex(const unsigned int id)
{
	RVertex *ptr;

	Vertices.InsertPtr(ptr=new RVertex(id,Vertices.GetNb(),Vertices.GetMaxNb()));
	return(ptr);
}


//------------------------------------------------------------------------------
RVertex* RGraph::GetVertex(const unsigned int id)
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
REdge* RGraph::CreateEdge(RVertex* v1,RVertex* v2,double w)
{
	REdge *ptr;

	Edges.InsertPtr(ptr=new REdge(v1,v2,w));
	v1->Edges.InsertPtr(ptr);
	v2->Edges.InsertPtr(ptr);	
	return(ptr);
}


//------------------------------------------------------------------------------
void RGraph::MinSpanningTree(RGraph* g)
{
	RVertex *v1, *v2;
	REdge *best=0;
	double bestw;
	unsigned nb=Vertices.GetNb();
	bool b1,b2,b;

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
				b1=g->Vertices.IsIn<const unsigned int>(e()->v1->Id);
				b2=g->Vertices.IsIn<const unsigned int>(e()->v2->Id);
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
			v1=g->Vertices.GetPtr<const unsigned int>(best->v1->Id);
			v2=g->CreateVertex(best->v2->Id);
		}
		else
		{
			v1=g->Vertices.GetPtr<const unsigned int>(best->v2->Id);
			v2=g->CreateVertex(best->v1->Id);
		}

		// Insert the Edge
		g->Edges.InsertPtr(new REdge(v1,v2,best->Weight));
	}
}


//------------------------------------------------------------------------------
RGraph::~RGraph(void)
{
}
