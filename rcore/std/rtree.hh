/*

	R Project Library

	RTree.hh

	Generic Tree - Inline Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
	RTree<N,bAlloc,bOrder>::RTree(unsigned int max,unsigned int inc) throw(std::bad_alloc)
		: RContainer<N,unsigned int,bAlloc,bOrder>(max,inc), Top(0)
{
	Top = new RContainer<N,unsigned int,false,bOrder>(10,5);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::AddNode(N* parent,N* node) throw(std::bad_alloc)
{
	RReturnIfFail(node);
	if(!node) return;
	RContainer<N,unsigned int,bAlloc,bOrder>::InsertPtr(node);
	if(parent)
	{
		node->Parent=parent;
		parent->RContainer<N,unsigned int,false,bOrder>::InsertPtr(node);
	}
	else
		Top->RContainer<N,unsigned int,false,bOrder>::InsertPtr(node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::DeleteNode(N* node)
{
	RReturnIfFail(node);
	if(!node) return;
	if(node->Parent)
	{
		node->Parent->RContainer<N,unsigned int,false,bOrder>::DeletePtr(node);
		node->Parent=0;
	}
	else
		Top->RContainer<N,unsigned int,false,bOrder>::DeletePtr(node);
	RContainer<N,unsigned int,bAlloc,bOrder>::DeletePtr(node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	unsigned int RTree<N,bAlloc,bOrder>::GetNbNodes(void) const
{
	return(NbPtr);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::~RTree(void)
{
	if(Top) delete Top;
}
