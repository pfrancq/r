/*

	R Project Library

	RNode.hh

	Generic Node - Inline implementation.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
//
//		class RNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RNode<N,bAlloc,bOrder>::RNode(RTree<N,bAlloc,bOrder>* tree)
		: Tree(tree), Parent(0), SubNodes(NullId), NbSubNodes(0)
{
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	N* RNode<N,bAlloc,bOrder>::GetParent(void) const
{
	return(Parent);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	size_t RNode<N,bAlloc,bOrder>::GetNbNodes(void) const
{
	return(NbSubNodes);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RCursor<N> RNode<N,bAlloc,bOrder>::GetNodes(void) const
{
	if(!NbSubNodes)
		return(R::RCursor<N>());
	return(R::RCursor<N>(*Tree,SubNodes,SubNodes+NbSubNodes));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	template<class TUse> N* RNode<N,bAlloc,bOrder>::GetNode(const TUse& tag,bool sortkey) const
{
	return(Tree->GetPtr(tag,sortkey,SubNodes,SubNodes+NbSubNodes));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RNode<N,bAlloc,bOrder>::InsertNode(N* node)
{
	Tree->InsertNode(static_cast<N*>(this),node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RNode<N,bAlloc,bOrder>::~RNode(void)
{
}
