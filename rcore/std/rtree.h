/*

	R Project Library

	RTree.h

	Generic Tree - Header.

	Copyright 1999-2003 by the Universit� Libre de Bruxelles.

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
#ifndef RTree_H
#define RTree_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implement a generic tree of node of class N. The class N must
* derived from the RNode<N,bool> class. The paremeter bAlloc specify if the
* tree is responsible for the desallocation of the nodes. The parameter bOrder
* specify if the nodes are ordered. Here is a example:
* <pre>
* class MyOrderResponsibleTree : public RTree<MyNode,true,true>
*	{
*	public:
*		MyOrderResponsibleTree(unsigned int max,unsigned int inc) throw(bad_alloc);
*	};
* </pre>
* @author Pascal Francq
* @short Generic Tree.
*/
template<class N,bool bAlloc,bool bOrder>
	class RTree : public RContainer<N,unsigned int,bAlloc,bOrder>
{
public:
	/**
	* This container hold the top nodes.
	*/
	RContainer<N,unsigned int,false,bOrder>* Top;

	/**
	* Construct the tree.
	* @param max            Initial size of the array of top nodes.
	* @param inc            Increment size of the array.
	*/
	RTree(unsigned int max,unsigned int inc) throw(bad_alloc);

	/**
	* Add the node to the parent.
	* @param parent         The parent node.
	* @param node           The node to add to the tree.
	*/
	void AddNode(N* parent,N* node) throw(bad_alloc);

	/**
	* Delete the node from the tree.
	* @param node           The node to be delete from the tree.
	*/
	void DeleteNode(N* node);

	/**
	* Get the number of nodes in the tree.
	* @return unsigned int
	*/
	unsigned int GetNbNodes(void) const;

	/**
	* Destruct the tree.
	*/
	~RTree(void);
};


//------------------------------------------------------------------------------
// Template implementation
#include <rstd/rtree.hh>


}//---- End namespace R --------------------------------------------------------


//------------------------------------------------------------------------------
#endif
