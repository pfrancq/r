/*

  RTree.hh

  Generic Tree - Template Implementation.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::RTree(unsigned int max,unsigned int inc) throw(bad_alloc)
		: RContainer<N,unsigned int,bAlloc,bOrder>(max,inc), Top(0)
{
  Top = new RContainer<N,unsigned int,false,bOrder>(10,5);
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::AddNode(N *parent,N *node) throw(bad_alloc)
{
	RReturnIfFail(node);
	if(!node) return;
  InsertPtr(node);
  if(parent)
  {
    node->Parent=parent;
    parent->InsertPtr(node);
  }
  else
    Top->InsertPtr(node);
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	void RTree<N,bAlloc,bOrder>::DeleteNode(N *node)
{
	RReturnIfFail(node);
	if(!node) return;
  if(node->Top)
  {
    node->Top->DeletePtr(node);
    node->Top=0;
  }
  else
    Top->DeletePtr(node);
  DeletePtr(node);
}


//---------------------------------------------------------------------------
template<class N,bool bAlloc,bool bOrder>
	RTree<N,bAlloc,bOrder>::~RTree(void)
{
  if(Top) delete Top;
}