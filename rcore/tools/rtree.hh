/*

	R Project Library

	RTree.hh

	Generic Tree - Inline Implementation.

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
// class RTree
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	RTree<N,bAlloc>::RTree(size_t max,size_t inc)
		: RContainer<N,bAlloc,false>(max,inc), NbTopNodes(0)
{
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc>
	N* RTree<N,bAlloc>::GetTop(void)
{
	if(!NbTopNodes)
		return(0);
	if(NbTopNodes>1)
		throw RException("RTree<N,bAlloc>::GetTop(void): More than one node");
	return((*this)[0]);
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc>
	const N* RTree<N,bAlloc>::GetTop(void) const
{
	if(!NbTopNodes)
		return(0);
	if(NbTopNodes>1)
		throw RException("RTree<N,bAlloc>::GetTop(void) const: More than one node");
	return((*this)[0]);
}


//-----------------------------------------------------------------------------
template<class N,bool bAlloc>
	RCursor<N> RTree<N,bAlloc>::GetTopNodes(void) const
{
	if(!NbTopNodes)
		return(R::RCursor<N>());
	return(R::RCursor<N>(*this,0,NbTopNodes));
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc>
	void RTree<N,bAlloc>::InsertNode(N* to,N* node)
{
	size_t tmp;

	if(node->Tree)
		throw RException("Node is already inserted in a tree");
	node->Tree=this;
	if(!to)
	{
		tmp=NbTopNodes++;
		InsertPtrAt(node,tmp,false);
		node->Index=tmp;
		RCursor<N> Nodes(*this);
		for(Nodes.Start();!Nodes.End();Nodes.Next())
			if(Nodes()->SubNodes!=cNoRef)
				Nodes()->SubNodes++;
	}
	else if(to->NbSubNodes)
	{
		tmp=to->SubNodes+to->NbSubNodes;
		InsertPtrAt(node,tmp,false);
		node->Index=tmp;
		RCursor<N> Nodes(*this);
		for(Nodes.Start();!Nodes.End();Nodes.Next())
			if((Nodes()->SubNodes>to->SubNodes)&&(Nodes()->SubNodes!=cNoRef))
				Nodes()->SubNodes++;
		node->Parent = to;
		to->NbSubNodes++;
	}
	else
	{
		to->SubNodes=RContainer<N,bAlloc,false>::GetNb();
		InsertPtrAt(node,to->SubNodes,false);
		node->Index=to->SubNodes;
		node->Parent = to;
		to->NbSubNodes++;
	}
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc>
	void RTree<N,bAlloc>::DeleteNode(N* node)
{
	N* from;

	// Delete sub-nodes
	if(node->NbSubNodes)
	{
		RContainer<N,false,false> Del(node->NbSubNodes);
		RCursor<N> Cur(node->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			Del.InsertPtr(Cur());
		Cur.Set(Del);
		for(Cur.Start();!Cur.End();Cur.Next())
			DeleteNode(Cur());
	}

	// Delete the node and update the index
	from=node->Parent;
	node->Tree=0;
	if(!from) return;
	DeletePtrAt(node->Index);
	size_t j=from->SubNodes;
	if(!(--from->NbSubNodes))
		from->SubNodes=cNoRef;
	RCursor<N> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
		if((Nodes()->SubNodes>j)&&(Nodes()->SubNodes!=cNoRef))
			Nodes()->SubNodes--;
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc> template<bool a>
	void RTree<N,bAlloc>::Copy(const RTree<N,a>& src)
{
	RCursor<N> Nodes(src.Top->GetNodes());
	for(Nodes.Start();!Nodes.End();Nodes.Next())
		DeepCopy(Nodes(),0);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	void RTree<N,bAlloc>::DeepCopy(N* src,N* parent)
{
	N* NewNode=new N(*src);
	InsertNode(parent,NewNode);
	RCursor<N> curs(src->GetNodes());
	for(curs.Start(); !curs.End(); curs.Next())
		DeepCopy(curs(),NewNode);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	template<class TUse>
		N* RTree<N,bAlloc>::GetNode(const TUse& tag) const
{
	return(RContainer<N,bAlloc,false>::GetPtr(tag,false));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc>
	RTree<N,bAlloc>::~RTree(void)
{
}
