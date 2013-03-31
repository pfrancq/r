/*

	R Project Library

	RGraph.h

	A Graph - Inline Implementation.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
//
// clas RGraph
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	RGraph<V,E,bAllocVertices,bAllocEdges>::RGraph(size_t nb)
	: Vertices(nb,nb/2), Edges(nb*(nb-1)/2,nb*(nb-1)/4)
{
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	void RGraph<V,E,bAllocVertices,bAllocEdges>::Clear(void)
{
	Vertices.Clear();
	Edges.Clear();
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	RCursor<V> RGraph<V,E,bAllocVertices,bAllocEdges>::GetVertices(void) const
{
	return(RCursor<V>(Vertices));
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	RCursor<E> RGraph<V,E,bAllocVertices,bAllocEdges>::GetEdges(void) const
{
	return(RCursor<E>(Edges));
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	void RGraph<V,E,bAllocVertices,bAllocEdges>::Insert(V* v)
{
	if(v->GetNbEdges())
		throw RException("Graph::InsertVertex(RVertex*) : Cannot insert a vertex with edges");
	Vertices.InsertPtr(v);
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	V* RGraph<V,E,bAllocVertices,bAllocEdges>::CreateVertex(void)
{
	V* ptr(new V(Vertices.GetNb(),Vertices.GetMaxNb()));
	Insert(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	V* RGraph<V,E,bAllocVertices,bAllocEdges>::CreateVertex(const size_t id)
{
	V* ptr(new V(id,Vertices.GetMaxNb()));
	Insert(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	V* RGraph<V,E,bAllocVertices,bAllocEdges>::GetVertex(const size_t id)
{
	V* ptr(Vertices.GetPtr(id));
	if(!ptr)
		Vertices.InsertPtr(ptr=new V(id,Vertices.GetMaxNb()));
	return(ptr);
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	void RGraph<V,E,bAllocVertices,bAllocEdges>::Insert(E* e)
{
	Edges.InsertPtr(e);
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	E* RGraph<V,E,bAllocVertices,bAllocEdges>::CreateEdge(V* v1,V* v2,double w)
{
	E* ptr(new REdge(v1,v2,w));
	Insert(ptr);
	return(ptr);
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	void RGraph<V,E,bAllocVertices,bAllocEdges>::MinSpanningTree(RGraph* g)
{
	V *v1, *v2;
	E* best(0);
	double bestw;
	size_t nb=Vertices.GetNb();
	bool b1,b2,b(false);

	g->Clear();
	if(!nb) return;
	g->CreateVertex(Vertices[0]->GetId());
	while(--nb)
	{
		bestw=std::numeric_limits<double>::max();

		// Go through the edges and find the minimal one with a connected.
		RCursor<E> e(Edges);
		for(e.Start();!e.End();e.Next())
		{
			// Test if possible good vertex
			if(e()->GetWeight()<bestw)
			{
				// Test if only one is in g
				b1=g->Vertices.IsIn(e()->GetVertex1()->GetId());
				b2=g->Vertices.IsIn(e()->GetVertex2()->GetId());
				if((b1&&!b2)||(!b1&&b2))
				{
					best=e();
					bestw=best->GetWeight();
					b=b1;
				}
			}
		}

		// Insert the vertex
		if(b)
		{
			v1=g->Vertices.GetPtr(best->GetVertex1()->GetId());
			v2=g->CreateVertex(best->GetVertex2()->GetId());
		}
		else
		{
			v1=g->Vertices.GetPtr(best->GetVertex2()->GetId());
			v2=g->CreateVertex(best->GetVertex1()->GetId());
		}

		// Insert the Edge
		g->Edges.InsertPtr(new E(v1,v2,best->GetWeight()));
	}
}


//------------------------------------------------------------------------------
template<class V,class E,bool bAllocVertices,bool bAllocEdges>
	RGraph<V,E,bAllocVertices,bAllocEdges>::~RGraph(void)
{
}
