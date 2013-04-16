/*

	R Project Library

	RGenericEdge.cpp

	Generic edge of a graph  - Inline Implementation.

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
// class RGenericEdge
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class V,class E>
	RGenericEdge<V,E>::RGenericEdge(V* f,V* t,const double w)
		: From(f), To(t), Weight(w)
{
	if((!From)||(!To))
		ThrowRException("Null vertices");

	// Insert the edge in the corresponding vertices
	From->Edges.InsertPtr(static_cast<E*>(this));
	To->Edges.InsertPtr(static_cast<E*>(this));
}


//------------------------------------------------------------------------------
template<class V,class E>
	bool RGenericEdge<V,E>::IsConnect(size_t id) const
{
	if((!From)||(!To))
		return(false);
	if((From->GetId()==id)||(From->GetId()==id))
		return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class V,class E>
	RGenericEdge<V,E>::~RGenericEdge(void)
{
	// Delete the edge in the corresponding vertices if necessary
	if(From)
		From->Edges.DeletePtr(*static_cast<E*>(this));
	if(To)
		To->Edges.DeletePtr(*static_cast<E*>(this));
}
