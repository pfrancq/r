/*

	R Project Library

	RTree.hh

	Generic Tree - Inline Implementation.

	Copyright 1999-2010 by Pascal Francq (pascal@francq.info).
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
template<class T,class N,bool bAlloc>
	RTree<T,N,bAlloc>::RTree(size_t max,size_t inc)
		: RContainer<N,bAlloc,false>(max,inc), NbTopNodes(0), MaxDepth(0)
{
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::Clear(size_t max,size_t inc)
{
	if(!bAlloc)
	{
		RCursor<N> Cur(*this);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Clear();
	}
	NbTopNodes=0;
	RContainer<N,bAlloc,false>::Clear(max,inc);
	MaxDepth=0;
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	size_t RTree<T,N,bAlloc>::GetMaxDepth(void) const
{
	if(MaxDepth==cNoRef)
	{
		// Depth must be recomputed
		const_cast<RTree<T,N,bAlloc>*>(this)->MaxDepth=0;
		RCursor<N> Nodes(*this);
		for(Nodes.Start();!Nodes.End();Nodes.Next())
			if(Nodes()->Depth>MaxDepth)
				const_cast<RTree<T,N,bAlloc>*>(this)->MaxDepth=Nodes()->Depth;
	}
	return(MaxDepth);
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	N* RTree<T,N,bAlloc>::GetTop(void)
{
	if(!NbTopNodes)
		return(0);
	if(NbTopNodes>1)
		ThrowRException("More than one top node");
	return((*this)[0]);
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	const N* RTree<T,N,bAlloc>::GetTop(void) const
{
	if(!NbTopNodes)
		return(0);
	if(NbTopNodes>1)
		ThrowRException("More than one top node");
	return((*this)[0]);
}


//-----------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	RCursor<N> RTree<T,N,bAlloc>::GetTopNodes(void) const
{
	if(!NbTopNodes)
		return(R::RCursor<N>());
	return(R::RCursor<N>(*this,0,NbTopNodes-1));
}


//-----------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	RCursor<N> RTree<T,N,bAlloc>::GetNodes(const N* node) const
{
	if(node)
	{
		if(!node->NbSubNodes)
			return(R::RCursor<N>());
		return(R::RCursor<N>(*this,node->SubNodes,node->SubNodes+node->NbSubNodes-1));
	}
	if(!NbTopNodes)
		return(R::RCursor<N>());
	return(R::RCursor<N>(*this,0,NbTopNodes-1));
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	size_t RTree<T,N,bAlloc>::GetTab(N** nodes,N* node)
{
	if(node)
	{
		GetTab(nodes,node->SubNodes,node->SubNodes+node->NbSubNodes-1);
		return(node->NbSubNodes);
	}
	GetTab(nodes,0,NbTopNodes-1);
	return(NbTopNodes);
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::InsertNode(N* to,N* node)
{
	size_t Index;        // Index where the node will be inserted (All references after must be increased by one).

	if((node->Tree)&&(node->Tree!=static_cast<T*>(this)))
		ThrowRException("Node is already inserted in another tree");
	if(to&&((to->Tree!=static_cast<T*>(this))||(to->Index==cNoRef)))
		ThrowRException("Node where to attach is not inserted in the same tree");
	node->Tree=static_cast<T*>(this);
	if(to)
	{
		// Parent node
		if(!to->NbSubNodes)
			to->SubNodes=RContainer<N,bAlloc,false>::GetNb();
		Index=to->SubNodes+to->NbSubNodes++;
	}
	else
	{
		// Top node
		Index=NbTopNodes++;
	}
	InsertPtrAt(node,Index,false);
	node->Parent=to;
	node->Index=Index;
	RCursor<N> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		if(Nodes()==node)
			continue;

		// Update the index
		if(Nodes()->Index>=Index)
			Nodes()->Index++;

		// Update the references to the child nodes
		if((Nodes()!=to)&&(Nodes()->SubNodes!=cNoRef)&&(Nodes()->SubNodes>=Index))
			Nodes()->SubNodes++;
	}
	if(to)
		node->Depth=to->Depth+1;
	else
		node->Depth=1;
	if(node->Depth>MaxDepth)
		MaxDepth=node->Depth;
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::DeleteNode(N* node,bool childs,bool del)
{
	// If the depth of the node to delete correspond to the maximal depth -> it must be recomputed.
	if(node->Depth==MaxDepth)
		MaxDepth=cNoRef;

	// Delete child nodes
	if(childs&&node->NbSubNodes)
	{
		RContainer<N,false,false> Del(node->NbSubNodes);
		RCursor<N> Cur(node->GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			Del.InsertPtr(Cur());
		Cur.Set(Del);
		for(Cur.Start();!Cur.End();Cur.Next())
			DeleteNode(Cur(),true);
		node->NbSubNodes=0;
		node->SubNodes=cNoRef;
	}

	// Delete the node and update the index
	N* From(node->Parent);
	node->Parent=0;
	node->Tree=0;
	size_t Index(node->Index);      // Index where the node will be inserted (All references after must be decreased by one).
	node->Index=cNoRef;
	DeletePtrAt(Index,true,del);
	if(From)
	{
		// Parent node
		if(!(--From->NbSubNodes))
			From->SubNodes=cNoRef;
	}
	else
	{
		// Top node
		NbTopNodes--;
	}
	RCursor<N> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
	{
		// Update the index
		if(Nodes()->Index>Index)
			Nodes()->Index--;

		// Update the references to the child nodes
		if((Nodes()->SubNodes!=cNoRef)&&(Nodes()->SubNodes>Index))
			Nodes()->SubNodes--;
	}

	// Since node is not deleted -> update its references to child nodes
	if(!del)
	{
		if((node->SubNodes!=cNoRef)&&(node->SubNodes>Index))
			node->SubNodes--;
	}
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::DeleteNodes(N* from)
{
	if(!from->NbSubNodes)
		return;

	RContainer<N,false,false> Del(from->NbSubNodes);
	RCursor<N> Cur(from->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Del.InsertPtr(Cur());
	Cur.Set(Del);
	for(Cur.Start();!Cur.End();Cur.Next())
		DeleteNode(Cur(),true);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::MoveNode(N* to,N* node)
{
	if(to->Tree!=node->Tree)
		ThrowRException("Cannot move between different trees");
	if(to==node->Parent)
		return;

	DeleteNode(node,false,false);  // Do not delete the nodes and do not deallocate the node
	InsertNode(to,node);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc> template<bool a>
	void RTree<T,N,bAlloc>::Copy(const RTree<T,N,a>& src)
{
	RCursor<N> Nodes(src.Top->GetNodes());
	for(Nodes.Start();!Nodes.End();Nodes.Next())
		DeepCopy(Nodes(),0);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::DeepCopy(N* src,N* parent)
{
	N* NewNode=new N(*src);
	InsertNode(parent,NewNode);
	RCursor<N> curs(src->GetNodes());
	for(curs.Start(); !curs.End(); curs.Next())
		DeepCopy(curs(),NewNode);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	template<class TUse>
		N* RTree<T,N,bAlloc>::GetNode(const TUse& tag) const
{
	return(RContainer<N,bAlloc,false>::GetPtr(tag,false));
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	double RTree<T,N,bAlloc>::GetUpOperationsCost(const N* u,const N* v) const
{
	if(!u)
		ThrowRException("Node u cannot be null");
	if(!v)
		ThrowRException("Node v cannot be null");

	double Cost(0.0);

	// Verify if u must be moved up to have the same depth as v
	while(u->Depth>v->Depth)
	{
		Cost+=u->GetUpOperationCost();
		u=u->Parent;
	}

	// Verify if v must be moved up to have the same depth as u
	while(v->Depth>u->Depth)
		v=v->Parent;

	// Now that u and v are as the same depth -> count the number of up operations to find the same parent
	while(v!=u)
	{
		Cost+=u->GetUpOperationCost();
		u=u->Parent;
		v=v->Parent;
	}

	// Return the cost
	return(Cost);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	RTree<T,N,bAlloc>::~RTree(void)
{
}
