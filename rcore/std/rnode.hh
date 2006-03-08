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
// Class RNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RNode<N,bAlloc,bOrder>::RNode(void)
		: Tree(0), Parent(0), SubNodes(NullId), NbSubNodes(0), Index(SIZE_MAX)
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
	size_t RNode<N,bAlloc,bOrder>::GetIndex(void) const
{
	return(Index);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RCursor<N> RNode<N,bAlloc,bOrder>::GetNodes(void) const
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	if(!NbSubNodes)
		return(R::RCursor<N>());
	return(R::RCursor<N>(*Tree,SubNodes,SubNodes+NbSubNodes));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	template<class TUse> N* RNode<N,bAlloc,bOrder>::GetNode(const TUse& tag,bool sortkey) const
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	return(Tree->GetPtr(tag,sortkey,SubNodes,SubNodes+NbSubNodes));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RNode<N,bAlloc,bOrder>::InsertNode(N* node)
{
	if(!Tree)
		throw RException("Node not assign to a tree");
	Tree->InsertNode(static_cast<N*>(this),node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RNode<N,bAlloc,bOrder>::DeleteEmptySubNodes(void)
{
/*	if(!NbSubNodes) return;
	RContainer<N,false,false> ToTreat(20,10);
	RContainer<N,false,false> ToDel(20,10);

	// Go through the subnodes.
	RCursor<N> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		ToTreat.InsertPtr(Cur());

	// Treat all subnodes
	Cur.Set(ToTreat);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->DeleteEmptySubNodes();
		if(Cur()->IsEmpty())
			ToDel.InsertPtr(Cur());
	}

	// Delete those needed
	Cur.Set(ToDel);
 	for(Cur.Start();!Cur.End();Cur.Next())
		Tree->DeleteNode(Cur());*/
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	bool RNode<N,bAlloc,bOrder>::IsEmpty(void)
{
	return(!NbSubNodes);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	const N* RNode<N,bAlloc,bOrder>::operator[](size_t idx) const
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
template<class N,bool bAlloc,bool bOrder>
	N* RNode<N,bAlloc,bOrder>::operator[](size_t idx)
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
template<class N,bool bAlloc,bool bOrder>
	RNode<N,bAlloc,bOrder>::~RNode(void)
{
}
