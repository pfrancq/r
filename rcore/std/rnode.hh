/*

  RNode.hh

  Generic Node - Template implementation.

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
//	class RNode
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class N,bool bOrder>
	RNode<N,bOrder>::RNode(unsigned int id,unsigned int max,unsigned int inc) throw(bad_alloc)
		: RContainer<N,unsigned int,false,bOrder>(max,inc), Top(NULL), Id(id)
{
}


//---------------------------------------------------------------------------
template<class N,bool bOrder>
	int RNode<N,bOrder>::Compare(RNode*)
{
	return(0);
}


//---------------------------------------------------------------------------
template<class N,bool bOrder>
	RNode<N,bOrder>::~RNode(void)
{
	delete Sub;
}

