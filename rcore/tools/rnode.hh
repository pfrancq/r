/*

	R Project Library

	RNode.hh

	Generic Node - Inline implementation.

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
// Class RNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	RNode<T,N,bAlloc>::RNode(void)
		: Parent(0), Prev(0), Next(0), First(0), Last(0), NbSubNodes(0),
	     Depth(cNoRef)
{
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	RNode<T,N,bAlloc>::RNode(T* tree)
		: Parent(0), Prev(0), Next(0), First(0), Last(0), NbSubNodes(0),
	     Depth(cNoRef)
{
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	void RNode<T,N,bAlloc>::Clear(void)
{
	Parent=Last=First=Prev=Next=0;
	NbSubNodes=0;
	Depth=cNoRef;
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	size_t RNode<T,N,bAlloc>::GetNbTotalNodes(void) const
{
	size_t Nb(0);
	N* Node(First);
	while(Node)
	{
		Nb+=1+Node->GetNbTotalNodes();
		Node=Node->Next;
	}
	return(Nb);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	template<class TUse> N* RNode<T,N,bAlloc>::GetNode(const TUse& tag) const
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
/*template<class T,class N,bool bAlloc>
	void RNode<T,N,bAlloc>::InsertNode(N* node)
{
	if(!Tree)
		mThrowRException("Node not assign to a tree");
	Tree->InsertNode(static_cast<N*>(this),node);
}*/



//-----------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	bool RNode<T,N,bAlloc>::IsEmpty(void)
{
	return(!NbSubNodes);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	bool RNode<T,N,bAlloc>::VerifyNode(size_t id)
{
	// Each node must have a parent.
/*	if(!Tree)
	{
		std::cerr<<"Node "<<id<<": No Owner for this node."<<std::endl;
		return(false);
	}*/

	// Verify information about child nodes
	if(NbSubNodes)
	{
		// There must exist at least one child.
		if(!First)
		{
			std::cerr<<"Node "<<id<<": SubNodes==NoNode"<<std::endl;
			return(false);
		}
	}
	else
	{
		// No children.
		if(First)
		{
			std::cerr<<"Node "<<id<<": SubNodes!=NoNode"<<std::endl;
			return(false);
		}
	}

	// Everything looks OK.
	return(true);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	double RNode<T,N,bAlloc>::GetUpOperationCost(void) const
{
	return(1.0);
}


//------------------------------------------------------------------------------
template<class T,class N,bool bAlloc>
	RNode<T,N,bAlloc>::~RNode(void)
{
}
