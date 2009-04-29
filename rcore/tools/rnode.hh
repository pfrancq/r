/*

	R Project Library

	RNode.hh

	Generic Node - Inline implementation.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
// Class RNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	RNode<N,bAlloc>::RNode(void)
		: Tree(0), Parent(0), SubNodes(cNoRef), NbSubNodes(0), Index(cNoRef)
{
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	N* RNode<N,bAlloc>::GetParent(void) const
{
	return(Parent);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc>
	size_t RNode<N,bAlloc>::GetNbNodes(void) const
{
	return(NbSubNodes);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc>
	size_t RNode<N,bAlloc>::GetIndex(void) const
{
	return(Index);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc>
	RCursor<N> RNode<N,bAlloc>::GetNodes(void) const
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	if(!NbSubNodes)
		return(R::RCursor<N>());
	return(R::RCursor<N>(*Tree,SubNodes,SubNodes+NbSubNodes));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	template<class TUse> N* RNode<N,bAlloc>::GetNode(const TUse& tag) const
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	return(Tree->GetPtr(tag,false,SubNodes,SubNodes+NbSubNodes));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	void RNode<N,bAlloc>::InsertNode(N* node)
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	Tree->InsertNode(static_cast<N*>(this),node);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc>
	bool RNode<N,bAlloc>::IsEmpty(void)
{
	return(!NbSubNodes);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	const N* RNode<N,bAlloc>::operator[](size_t idx) const
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	if(idx>=NbSubNodes)
	{
		char tmp[80];
		sprintf(tmp,"RNode::operator[] const : index %u outside range [0,%u]",idx,NbSubNodes-1);
		throw std::range_error(tmp);
	}
	return((*Tree)[SubNodes+idx]);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	N* RNode<N,bAlloc>::operator[](size_t idx)
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	if(idx>=NbSubNodes)
	{
		char tmp[80];
		sprintf(tmp,"RNode::operator[] const : index %u outside range [0,%u]",idx,NbSubNodes-1);
		throw std::range_error(tmp);
	}
	return((*Tree)[SubNodes+idx]);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	RNode<N,bAlloc>::~RNode(void)
{
}
