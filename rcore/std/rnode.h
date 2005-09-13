/*

	R Project Library

	RNode.h

	Generic Node - Header.

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
template<class N,bool bAlloc,bool bOrder> class RTree;


//-----------------------------------------------------------------------------
/**
* @param N                   The class of the elements of the tree.
* @param bAlloc              Specify if the elements are desallocated by the
*                            tree.
* @param bOrder              Specify if the elements are ordered in the tree.
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
* int Compare(const N&) const;
* @endcode
*
* Here is an example:
* @code
* #include <rtree.h>
* #include <rnode.h>
* #include <rcursor.h>
* using namespace R;
*
* class MyTree;   // Forward declaration
*
* class MyNode : public RNode<MyNode,true,true>
* {
*    RString Name;
* public:
*    MyNode(MyTree* tree,const RString& name) : RNode<MyNode,true,true>(tree), Name(name) {}
*    int Compare(const MyNode& node) const {return(Name.Compare(node.Name));}
*    void DoSomething(void) {cout<<Name<<endl;}
* };
*
* class MyTree : public RTree<MyNode,true,true>
* {
* public:
*    MyTree(unsigned int max,unsigned int inc) : RTree<MyNode,true,true>(max,inc) {}
* };
*
* int main()
* {
*    MyTree Tree(20,10);
*
*    // Insert the top node
*    Tree.InsertNode(0,new MyNode(&Tree,"Root"));
*
*    // Insert two sub nodes
*    Tree.InsertNode(Tree.GetTop(),new MyNode(&Tree,"First Level (1)"));
*    Tree.InsertNode(Tree.GetTop(),new MyNode(&Tree,"First Level (2)"));
*
*    // Parse the nodes
*    R::RCursor<MyNode> Cur(Tree.GetNodes());
*    for(Cur.Start();!Cur.End();Cur.Next())
*       Cur()->DoSomething();
* }
* @endcode
* @author Pascal Francq
* @short Generic Node.
*/
template<class N,bool bAlloc,bool bOrder>
	class RNode
{
protected:

	/**
	* Tree holding the node.
	*/
	RTree<N,bAlloc,bOrder>* Tree;

	/**
	* Parent Node.
	*/
	N* Parent;

	/**
	* Index of the first subnode in Nodes.
	*/
	size_t SubNodes;

	/**
	* Number of subnodes
	*/
	size_t NbSubNodes;

public:

	/**
	* Construct the node.
	* @param tree           The tree.
	*/
	RNode(RTree<N,bAlloc,bOrder>* tree);

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
	* Get a cursor over the child nodes.
	* @return RCursor<N>.
	*/
	RCursor<N> GetNodes(void) const;

	/**
	* Get a pointer to a certain child node.
	* @param TUse            The type of the tag used for the search.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return Return the pointer or 0 if the element is not a child node.
	*/
	template<class TUse> N* GetNode(const TUse& tag,bool sortkey=bOrder) const;

	/**
	* Method call to insert a node.
	* @param node           Node to insert.
	*/
	void InsertNode(N* node);

	/**
	* Destruct the node.
	*/
	virtual ~RNode(void);

	// The RTree is a  friend class
	friend class RTree<N,bAlloc,bOrder>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rnode.hh>


}  //---- End namespace R -----------------------------------------------------


//-----------------------------------------------------------------------------
#endif
