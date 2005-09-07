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
//		class RNode
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class N,bool bOrder>
	RNode<N,bOrder>::RNode(unsigned int max,unsigned int inc)
		: RContainer<N,false,bOrder>(max,inc), Parent(0)
{
}


//------------------------------------------------------------------------------
template<class N,bool bOrder>
	int RNode<N,bOrder>::Compare(const RNode*) const
{
	return(-1);
}


//------------------------------------------------------------------------------
template<class N,bool bOrder>
	N* RNode<N,bOrder>::GetParent(void) const
{
	return(Parent);
}


//-----------------------------------------------------------------------------
template<class N,bool bOrder>
	size_t RNode<N,bOrder>::GetNbNodes(void) const
{
	return(RContainer<N,false,bOrder>::GetNb());
}


//-----------------------------------------------------------------------------
template<class N,bool bOrder>
	RCursor<N> RNode<N,bOrder>::GetNodes(void) const
{
	return(R::RCursor<N>(*this));
}


//------------------------------------------------------------------------------
template<class N,bool bOrder>
	RNode<N,bOrder>::~RNode(void)
{
}
