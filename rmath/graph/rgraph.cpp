/*

	R Project Library

	RGraph.h

	A Graph - Implementation.

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
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgraph/rgraph.h>
using namespace RGraphs;



//-----------------------------------------------------------------------------
//
// clas RGraph
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGraphs::RGraph::RGraph(unsigned int nb)
	: Vertices(nb,nb/2), Edges(nb*(nb-1)/2,nb*(nb-1)/4)
{
}


//-----------------------------------------------------------------------------
void RGraphs::RGraph::Clear(void)
{
	Vertices.Clear();
	Edges.Clear();
}


//-----------------------------------------------------------------------------
RVertex* RGraphs::RGraph::CreateVertex(void)
{
	RVertex *ptr;

	Vertices.InsertPtr(ptr=new RVertex(Vertices.NbPtr,Vertices.NbPtr,Vertices.MaxPtr));
	return(ptr);
}


//-----------------------------------------------------------------------------
RVertex* RGraphs::RGraph::CreateVertex(const unsigned int id)
{
	RVertex *ptr;

	Vertices.InsertPtr(ptr=new RVertex(id,Vertices.NbPtr,Vertices.MaxPtr));
	return(ptr);
}


//-----------------------------------------------------------------------------
RVertex* RGraphs::RGraph::GetVertex(const unsigned int id)
{
	RVertex *ptr;
	RVertex::VertexStruct s;

	s.id=id;
	s.idx=Vertices.NbPtr;
	s.nb=Vertices.NbPtr;
	ptr=Vertices.GetInsertPtr<RVertex::VertexStruct>(s);
	return(ptr);
}


//-----------------------------------------------------------------------------
REdge* RGraphs::RGraph::CreateEdge(RVertex* v1,RVertex* v2,double w)
{
	REdge *ptr;

	Edges.InsertPtr(ptr=new REdge(v1,v2,w));
	v1->Edges.InsertPtr(ptr);
	v2->Edges.InsertPtr(ptr);	
	return(ptr);
}


//-----------------------------------------------------------------------------
void RGraphs::RGraph::MinSpanningTree(RGraph* g)
{
	RVertex *v1, *v2;
	REdge **e,*best=0;
	double bestw;
	unsigned nb=Vertices.NbPtr,i;
	bool b1,b2,b;

	g->Clear();
	if(!nb) return;
	g->CreateVertex(Vertices.Tab[0]->Id);
	while(--nb)
	{
		bestw=HUGE_VAL;

		// Go through the edges and find the minimal one with a connected.
		for(i=Edges.NbPtr+1,e=Edges.Tab;--i;e++)
		{
			// Test if possible good vertex
			if((*e)->Weight<bestw)
			{
				// Test if only one is in g
				b1=g->Vertices.IsIn<const unsigned int>((*e)->v1->Id);
				b2=g->Vertices.IsIn<const unsigned int>((*e)->v2->Id);
				if((b1&&!b2)||(!b1&&b2))
				{
					best=(*e);
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


//-----------------------------------------------------------------------------
RGraphs::RGraph::~RGraph(void)
{
}
