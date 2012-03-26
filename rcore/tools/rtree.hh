/*

	R Project Library

	RTree.hh

	Generic Tree - Inline Implementation.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
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
	RTree<T,N,bAlloc>::RTree(void)
		: First(0), Last(0), NbTopNodes(0), NbNodes(0), MaxDepth(0)
{
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::ClearNode(N* node)
{
	N* Node(node->First);
	while(Node)
	{
		N* Next(Node->Next);
		ClearNode(Node);
		Node=Next;
	}
	node->Clear();
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::Clear(void)
{
	N* Node(First);
	while(Node)
	{
		N* Next(Node->Next);
		if(bAlloc)
			DeleteNode(Node,true);
		else
			ClearNode(Node);
		Node=Next;
	}
	NbNodes=NbTopNodes=0;
	First=Last=0;
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
	if(NbTopNodes>1)
		ThrowRException("More than one top node");
	return(First);
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	const N* RTree<T,N,bAlloc>::GetTop(void) const
{
	if(NbTopNodes>1)
		ThrowRException("More than one top node");
	return(First);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	size_t RTree<T,N,bAlloc>::GetTab(N** nodes,N* node,bool children)
{
	size_t Pos(0);
	N** ptr(nodes);
	N* n;
	if(node)
		n=node->First;
	else
		n=First;
	while(n)
	{
		(*(ptr++))=n;
		Pos++;
		if(children)
		{
			Pos+=GetTab(ptr,n,true);
			ptr=&nodes[Pos]; // The pointer must be repositionned.
		}
		n=n->Next;
	}
	return(Pos);
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::InsertNode(N* to,N* node)
{
	if((node->Tree)&&(node->Tree!=static_cast<T*>(this)))
		ThrowRException("Node is already inserted in another tree");
	if(to&&(to->Tree!=static_cast<T*>(this)))
		ThrowRException("Node where to attach is not inserted in the same tree");

	// Insert the node in its parent and siblings
	node->Tree=static_cast<T*>(this);
	if(to)
	{
		to->NbSubNodes++;
		if(!to->First)
			to->First=node;
		node->Prev=to->Last;
		if(to->Last)
			to->Last->Next=node;
		to->Last=node;
	}
	else
	{
		// Top node
		NbTopNodes++;
		if(!First)
			First=node;
		node->Prev=Last;
		if(Last)
			Last->Next=node;
		Last=node;
	}
	node->Parent=to;
	NbNodes++;

	if(to)
		node->Depth=to->Depth+1;
	else
		node->Depth=0;
	if(node->Depth>MaxDepth)
		MaxDepth=node->Depth;
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::DeleteNodes(N* from,bool del)
{
	N* ptr(from->First);
	while(ptr)
	{
		N* Next(ptr->Next);
		DeleteNodes(ptr,del);
		if(del)
			delete ptr;
		else
		{
			ptr->Tree=0;
			ptr->Parent=ptr->Next=ptr->Prev=0;
		}
		NbNodes--;
		ptr=Next;
	}
	from->NbSubNodes=0;
	from->First=from->Last=0;
}


//---------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::DeleteNode(N* node,bool del)
{
	// If the depth of the node to delete correspond to the maximal depth -> it must be recomputed.
	if(node->Depth==MaxDepth)
	{
		if((!node->Prev)&&(!node->Next))
			MaxDepth=cNoRef;
	}

	// Delete child nodes
	DeleteNodes(node,del);

	// Delete the node from its parents and siblings
	if(node->Parent)
	{
		if(node->Prev)
			node->Prev->Next=node->Next;
		if(node->Next)
			node->Next->Prev=node->Prev;
		if(node->Parent->First==node)
			node->Parent->First=node->Next;
		if(node->Parent->Last==node)
			node->Parent->Last=node->Prev;
		node->Parent->NbSubNodes--;
	}
	else
	{
		if(node->Prev)
			node->Prev->Next=node->Next;
		if(node->Next)
			node->Next->Prev=node->Prev;
		if(First==node)
			First=node->Next;
		if(Last==node)
			Last=node->Prev;
		NbTopNodes--;
	}

	// Delete the node
	if(del)
		delete node;
	else
	{
		node->Tree=0;
		node->Parent=node->Next=node->Prev=0;
	}
	NbNodes--;
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::MoveNode(N* to,N* node)
{
	if(to->Tree!=node->Tree)
		ThrowRException("Cannot move between different trees");
	if(to==node->Parent)
		return;

	// Delete the node from its parents and siblings
	if(node->Parent)
	{
		if(node->Prev)
			node->Prev->Next=node->Next;
		if(node->Next)
			node->Next->Prev=node->Prev;
		if(node->Parent->First==node)
			node->Parent->First=node->Next;
		if(node->Parent->Last==node)
			node->Parent->Last=node->Prev;
		node->Parent->NbSubNodes--;
	}
	else
	{
		if(node->Prev)
			node->Prev->Next=node->Next;
		if(node->Next)
			node->Next->Prev=node->Prev;
		if(First==node)
			First=node->Next;
		if(Last==node)
			Last=node->Prev;
		NbTopNodes--;
	}

	// Insert it in the new parent and siblings
	if(to)
	{
		to->NbSubNodes++;
		if(!to->First)
			to->First=node;
		node->Prev=to->Last;
		to->Last->Next=node;
		to->Last=node;
	}
	else
	{
		// Top node
		NbTopNodes++;
		if(!First)
			First=node;
		node->Prev=Last;
		Last->Next=node;
		Last=node;
	}
	node->Parent=to;
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc> template<bool a>
	void RTree<T,N,bAlloc>::Copy(const RTree<T,N,a>& src)
{
	N* ptr(src.First);
	while(ptr)
	{
		DeepCopy(ptr,0);
		ptr=ptr->Next;
	}
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RTree<T,N,bAlloc>::DeepCopy(N* src,N* parent)
{
	N* NewNode=new N(*src);
	InsertNode(parent,NewNode);
	N* ptr(src->First);
	while(ptr)
	{
		DeepCopy(ptr,NewNode);
		ptr=ptr->Next;
	}
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	template<class TUse>
		N* RTree<T,N,bAlloc>::GetNode(const TUse& tag) const
{
	N* ptr(First);
	while(ptr)
	{
		if(!ptr->Compare(tag))
			return(ptr);
		N* Find(ptr->GetNode(tag));
		if(Find)
			return(Find);
		ptr=ptr->Next;
	}
	return(0);
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
	if(bAlloc)
	{
		N* Node(First);
		while(Node)
		{
			N* Next(Node->Next);
			DeleteNode(Node,true);
			Node=Next;
		}
	}
}
