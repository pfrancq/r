/*

	R Project Library

	RNode.h

	Generic Node - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
#ifndef RNode_H
#define RNode_H


//------------------------------------------------------------------------------
#include <rstd/rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent a generic node. The user has to derived from this class
* and implement the Compare function. The paramter N specify the real class and
* the parameter bOrder specify if the nodes has to be ordered. Here is an example:
* @code
* class MyNoOrderNode : public RNode<MyNoOrderNode,false>
*	{
*	public:
*		MyNoOrderNode(unsigned int max,unsigned int inc);
*		int Compare(MyNoOrderNode *node);
*	};
* @endcode
* @author Pascal Francq
* @short Generic Node.
*/
template<class N,bool bOrder>
	class RNode : public RContainer<N,unsigned int,false,bOrder>
{
public:
	/**
	* Parent Node.
	*/
	N* Parent;

	/**
	* Construct the node.
	* @param max            The size of initial array of pointer to child.
	* @param inc            The increment size for the array.
	*/
	RNode(unsigned int max,unsigned int inc) throw(std::bad_alloc);

	/**
	* Is used to compare two nodes. The function returns the same type of
	* information than the strcmp function from the standard C library.
	*/
	virtual int Compare(RNode*);

	/**
	* Return the parent of the node.
	* @returns Pointer to N.
	*/
	N* GetParent(void);

	/**
	* Destruct the node.
	*/
	virtual ~RNode(void);
};


//------------------------------------------------------------------------------
// Template implementation
#include <rstd/rnode.hh>


}	//---- End namespace R -----------------------------------------------------


//------------------------------------------------------------------------------
#endif
