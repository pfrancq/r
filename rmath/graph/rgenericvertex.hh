/*

	R Project Library

	RGenericVertex.cpp

	Generic vertex of a graph - Inline Implementation.

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
// class RGenericVertex
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class V,class E>
	RGenericVertex<V,E>::RGenericVertex(size_t id,size_t nb)
	: Id(id), Edges(nb,nb/2)
{
}


//------------------------------------------------------------------------------
template<class V,class E>
	RGenericVertex<V,E>::RGenericVertex(const RGenericVertex& v)
	: Edges(v.Edges.GetMaxNb(),v.Edges.GetIncNb())
{
}


//------------------------------------------------------------------------------
template<class V,class E>
	RCursor<E> RGenericVertex<V,E>::GetEdges(void) const
{
	return(RCursor<E>(Edges));
}


//------------------------------------------------------------------------------
template<class V,class E>
	E* RGenericVertex<V,E>::GetEdge(size_t id) const
{
	RCursor<E> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->Connects(id))
			return(Cur());
	return(0);
}


//------------------------------------------------------------------------------
template<class V,class E>
	E* RGenericVertex<V,E>::GetEdgeTo(size_t id) const
{
	RCursor<E*> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetTo()->GetId()==id)
			return(Cur());
	}
	return(0);
}


//------------------------------------------------------------------------------
template<class V,class E>
	E* RGenericVertex<V,E>::GetEdgeFrom(size_t id) const
{
	RCursor<E> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetFrom()->GetId()==id)
			return(Cur());
	return(0);
}


//------------------------------------------------------------------------------
template<class V,class E>
	template<bool bOrder>
		void RGenericVertex<V,E>::GetAdjacentVertices(RContainer<V,false,bOrder>& vertices,bool verify) const
{
	RCursor<E> Cur(Edges);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		V* Other;
		if(Cur()->GetFrom() == static_cast<const V*>(this))
			Other=Cur()->GetTo();
		else
			Other=Cur()->GetFrom();
		if(!Other)
			continue;
		if((!verify) || (verify&&(!vertices.IsIn(*Other))))
			vertices.InsertPtr(Other);
	}
}


//------------------------------------------------------------------------------
template<class V,class E>
	RGenericVertex<V,E>::~RGenericVertex(void)
{
}
