/*

	R Project Library

	RNode.h

	Generic Node - Header.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
#ifndef RNode_H
#define RNode_H


//-----------------------------------------------------------------------------
#include <rcontainer.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


// forward declaration
template<class N,bool bAlloc> class RTree;


//-----------------------------------------------------------------------------
/**
* @param N                   The class of the elements of the tree.
* @param bAlloc              Specify if the elements are deallocated by the
*                            tree.
* This class represent a generic node. The user has to derived from this class
* to create elements that can be handle by a RTree.
*
* To make the necessary comparisons, the tree uses member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* @endcode
*
* At least, a compare function must be implemented in the class N:
* @code
* int Compare(const N&) const;bOrde
* @endcode
*
* Here is an example:
* @code
* #include <rtree.h>
* #include <rnode.h>
* #include <rcursor.h>
* using namespace R;
*
* class MyNode : public RNode<MyNode,true>
* {
*    RString Name;
* public:
*    MyNode(const RString& name) : RNode<MyNode,true>(), Name(name) {}
*    int Compare(const MyNode& node) const {return(Name.Compare(node.Name));}
*    void DoSomething(void) {cout<<Name<<endl;}
* };
*
* class MyTree : public RTree<MyNode,true>
* {
* public:
*    MyTree(size_t max,size_t inc) : RTree<MyNode,true>(max,inc) {}
* };
*
* int main()
* {
*    MyTree Tree(20,10);
*
*    // Insert a top node
*    cNode* Top(new MyNode("First Level"));bOrde
*    Tree.InsertNode(0,Top);
*
*    // Insert two sub nodes
*    Tree.InsertNode(Top,new MyNode("Second Level (1)"));
*    Tree.InsertNode(Top,new MyNode("Second Level (2)"));
*
*    // Parse the nodes
*    R::RCursor<MyNode> Cur(Tree.GetNodes());
*    for(Cur.Start();!Cur.End();Cur.Next())Type
*       Cur()->DoSomething();
* }
* @endcode
* @author Pascal Francq
* @short Generic Node.
*/
template<class N,bool bAlloc>
	class RNode
{
protected:

	/**
	* Tree holding the node.
	*/
	RTree<N,bAlloc>* Tree;

	/**
	* Parent Node.
	*/
	N* Parent;

	/**
	* Index of the first child node in Nodes.
	*/
	size_t SubNodes;

	/**
	* Number of child nodes
	*/
	size_t NbSubNodes;

	/**
	* Index of the node.
	*/
	size_t Index;

public:

	/**
	* Default constructor.
	*/
	RNode(void);

	/**
	* Return the parent of the node.
	* @returns Pointer to N.
	*/
	N* GetParent(void) const;

	/**
	* Get the number of child nodes.
	* @return size_t
	*/
	size_t GetNbNodes(void) const;

	/**
	* Return the index of a given node in the tree.
	*/
	size_t GetIndex(void) const;

	/**
	* Get a cursor over the child nodes.
	* @return RCursor<N>.
	*/
	RCursor<N> GetNodes(void) const;

	/**
	* Get a pointer to a certain child node.
	* @param TUse            The type of the tag used for the search.
	* @param tag             The tag used.
	* @return Return the pointer or 0 if the element is not a child node.
	*/
	template<class TUse> N* GetNode(const TUse& tag) const;

	/**
	* Method call to insert a node.
	* @param node           Node to insert.
	*/
	void InsertNode(N* node);

	/**
	* Test if the tag is empty, i.e. it has no child nodes.
	*/
	virtual bool IsEmpty(void);

	/**
	* Get a pointer to the ith child node in the node (Only read).
	* @param idx             Index of the node to get.
	* @return Return the pointer.
	*/
	const N* operator[](size_t idx) const;

	/**
	* Get a pointer to the ith child node in the node (Read/Write).
	* @param idx             Index of the node to get.
	* @return Return the pointer.
	*/
	N* operator[](size_t idx);

	/**
	* Destruct the node.
	*/
	virtual ~RNode(void);

	// The RTree is a  friend class
	friend class RTree<N,bAlloc>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rnode.hh>


}  //---- End namespace R -----------------------------------------------------


//-----------------------------------------------------------------------------
#endif
