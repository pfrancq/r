/*

	R Project Library

	RTree.hh

	Generic Tree - Inline Implementation.

	Copyright 1999-2006 by the Université Libre de Bruxelles.

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
	RCursor<N> curs(child->GetNodes());
	N *new_node;
	new_node = new N(*child);
	parent->InsertNode(parent);
	for(curs.Start(); !curs.End(); curs.Next())
		DeepCopy(curs(), new_node);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::RTree(unsigned int max,unsigned int inc)
		: RContainer<N,bAlloc,bOrder>(max,inc), Top(0)
{
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::InsertNode(N* to,N* node)
{
	unsigned int tmp;

	if(!to)
	{
		// Must be the top node
		if(!Top)
		{
			Top=node;
			InsertPtrAt(Top,0);
			Top->Index=0;
			return;
		}
		to=Top;
	}
	if(to->NbSubNodes)
	{
		tmp=to->SubNodes+to->NbSubNodes;
		InsertPtrAt(node,tmp,false);
		node->Index=tmp;
		RCursor<N> Nodes(*this);
		for(Nodes.Start();!Nodes.End();Nodes.Next())
			if((Nodes()->SubNodes>to->SubNodes)&&(Nodes()->SubNodes!=NullId))
				Nodes()->SubNodes++;
		if((Top->SubNodes>to->SubNodes)&&(Top->SubNodes!=NullId))
			Top->SubNodes++;
	}
	else
	{
		to->SubNodes=RContainer<N,bAlloc,bOrder>::GetNb();
		InsertPtrAt(node,to->SubNodes,false);
		node->Index=to->SubNodes;
	}
	node->Parent = to;
	to->NbSubNodes++;
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::DeleteNode(N* node)
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
	if(!from) return;
	DeletePtrAt(node->Index);
	size_t j=from->SubNodes;
	if(!(--from->NbSubNodes))
		from->SubNodes=NullId;
	RCursor<N> Nodes(*this);
	for(Nodes.Start();!Nodes.End();Nodes.Next())
		if((Nodes()->SubNodes>j)&&(Nodes()->SubNodes!=NullId))
			Nodes()->SubNodes--;
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::DeleteEmptyNodes(void)
{
//	if(Top)
//		Top->DeleteEmptySubNodes();
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder> template<bool a, bool o>
	void RTree<N,bAlloc,bOrder>::Copy(const RTree<N,a,o>& src)
{
	RCursor<N> curs(src.Top->GetNodes());
	for (curs.Start(); !curs.End(); curs.Next())
		DeepCopy(curs(), 0);
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	template<class TUse>
		N* RTree<N,bAlloc,bOrder>::GetNode(const TUse& tag,bool sortkey) const
{
	return(RContainer<N,bAlloc,bOrder>::GetPtr(tag,sortkey));
}


//------------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::~RTree(void)
{
}
