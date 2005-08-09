/*

	R Project Library

	RTree.hh

	Generic Tree - Inline Implementation.

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

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
// class RTree
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::DeepCopy(N* child, N* parent)
{
	RCursor<N> curs(*child);
	N *new_node;
	new_node = new N(*child);
	new_node->Clear();
	AddNode(parent, new_node);
	for(curs.Start(); !curs.End(); curs.Next())
		DeepCopy(curs(), new_node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::RTree(unsigned int max,unsigned int inc) throw(std::bad_alloc)
		: RContainer<N,bAlloc,bOrder>(max,inc), Top(0)
{
	Top = new RContainer<N,false,bOrder>(10,5);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::AddNode(N* parent,N* node) throw(std::bad_alloc)
{
	RReturnIfFail(node);
	if(!node) return;
	RContainer<N,bAlloc,bOrder>::InsertPtr(node);
	if(parent)
	{
		node->Parent=parent;
		parent->RContainer<N,false,bOrder>::InsertPtr(node);
	}
	else
		Top->RContainer<N,false,bOrder>::InsertPtr(node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::DeleteNode(N* node)
{
	RReturnIfFail(node);
	if(!node) return;
	if(node->Parent)
	{
		node->Parent->RContainer<N,false,bOrder>::DeletePtr(*node);
		node->Parent=0;
	}
	else
		Top->RContainer<N,false,bOrder>::DeletePtr(*node);
	RContainer<N,bAlloc,bOrder>::DeletePtr(*node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	unsigned int RTree<N,bAlloc,bOrder>::GetNbNodes(void) const
{
	return(this->GetNb());
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder> template<bool a, bool o>
	void RTree<N,bAlloc,bOrder>::Copy(const RTree<N,a,o>& src)
{
	RCursor<N> curs(*src.Top);
	for (curs.Start(); !curs.End(); curs.Next())
		DeepCopy(curs(), 0);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::~RTree(void)
{
	if(Top) delete Top;
}
