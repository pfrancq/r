/*

	R Project Library

	RTree.h

	Generic Tree - Header.

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



//------------------------------------------------------------------------------
#ifndef RTree_H
#define RTree_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rcursor.h>
#include <rnode.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

// forward declaration
template<class N,bool bAlloc,bool bOrder> class RNode;


//------------------------------------------------------------------------------
/**
* @param N                   The class of the elements of the tree.
* @param bAlloc              Specify if the elements are desallocated by the
*                            tree.
* @param bOrder              Specify if the elements are ordered in the tree.
* This class implement a generic tree of node of a given class N. This class N
* must derived from the RNode class.
*
* The tree is supposed to have only one root node which has to be created
* manually.
*
* Here is a example:
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
*    MyNode(const RString& name) : RNode<MyNode,true,true>(), Name(name) {}
*    int Compare(const MyNode& node) const {return(Name.Compare(node.Name));}
*    void DoSomething(void) {cout<<Name<<endl;}
* };
*
* class MyTree : public RTree<MyNode,true,true>
* {
* public:
*    MyTree(size_t max,size_t inc) : RTree<MyNode,true,true>(max,inc) {}
* };
*
* int main()
* {
*    MyTree Tree(20,10);
*
*    // Insert the top node
*    Tree.InsertNode(0,new MyNode("Root")); // First node inserted in the tree with no parent, is always the top
*
*    // Insert two sub nodes
*    Tree.InsertNode(0,new MyNode("First Level (1)"));  // First Level since there is already a top node
*    Tree.InsertNode(0,new MyNode("First Level (2)"));  // First Level since there is already a top node
*
*    // Parse the nodes
*    R::RCursor<MyNode> Cur(Tree.GetNodes());
*    for(Cur.Start();!Cur.End();Cur.Next())
*       Cur()->DoSomething();
* }
* @endcode
* @author Pascal Francq
* @short Generic Tree.
*/
template<class N,bool bAlloc,bool bOrder>
	class RTree : private RContainer<N,bAlloc,bOrder>
{
protected:

	/**
	* The top node.
	*/
	N* Top;

public:

	/**
	* Construct the tree.
	* @param max            Initial size of the array of top nodes.
	* @param inc            Increment size of the array.
	*/
	RTree(size_t max,size_t inc);

	/**
	* Return the top node.
	*/
	N* GetTop(void) const {return(Top);}

	/**
	* Get the total number of nodes in the tree.
	* @return size_t
	*/
	inline size_t GetNbNodes(void) const {return(RContainer<N,bAlloc,bOrder>::GetNb());}

	/**
	* Get a cursor over all the nodes of the tree.
	*/
	inline RCursor<N> GetNodes(void) const {return(R::RCursor<N>(*this));}

	/**
	* Copy the array of nodes into a temporary array. This array must have
	* the right size (Read only). This method can be limited to a part of the
	* tree.
	* @param tab             Array of nodes.
	* @param min             Starting index.
	* @param max             Ending index.
	* @return number of elements of the tree.
	*/
	inline size_t GetTab(const N** tab,size_t min=0, size_t max=0) const {return(RContainer<N,bAlloc,bOrder>::GetTab(tab,min,max));}

	/**
	* Copy the array of nodes into a temporary array. This array must have
	* the right size (Read/Write). This method can be limited to a part of the
	* tree.
	* @param tab             Array of nodes.
	* @param min             Starting index.
	* @param max             Ending index.
	* @return number of elements in the tree.
	*/
	inline size_t GetTab(N** tab,size_t min=0, size_t max=0) {return(RContainer<N,bAlloc,bOrder>::GetTab(tab,min,max));}

	/**
	* Clear the tree and destruct the nodes if it is responsible for
	* the desallocation.
	* @param m              New maximal size of the array. If null, the old
	*                       size remains.
	* @param i              New increasing value. If null, the old value
	*                       remains.
	*/
	inline void Clear(size_t m=0,size_t i=0) {RContainer<N,bAlloc,bOrder>::Clear(m,i); Top=0;}

	/**
	* Insert a node and attached it to a parent. If the parent is null,
	* the node is considered as the top node of the tree.
	* @param to             Parent where the node is to attached.
	* @param node           Node to insert.
	*/
	void InsertNode(N* to,N* node);

	/**
	* Delete a node from the tree. All the child nodes are also deleted.
	* @param node           Node to delete.
	*/
	void DeleteNode(N* node);

	/**
	* Delete all empty nodes of the tree (except the top one).
	*/
	void DeleteEmptyNodes(void);

	/**
	* Deep copy of a the tree.
	* @param src             Source tree.
	*/
	template<bool a, bool o> void Copy(const RTree<N,a,o>& src);

	/**
	* Do a deep copy of a node of another tree.
	* @param src             Source node in the another tree.
	* @param newparent       Parent node.
	*/
	void DeepCopy(N* src,N* newparent);

	/**
	* Get a pointer to a certain node of the tree.
	* @param TUse            The type of the tag used for the search.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	template<class TUse> N* GetNode(const TUse& tag,bool sortkey=bOrder) const;

	/**
	* Destruct the tree.
	*/
	virtual ~RTree(void);

	// The RNode is a friend class
	friend class RNode<N,bAlloc,bOrder>;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rtree.hh>


}//---- End namespace R --------------------------------------------------------


//------------------------------------------------------------------------------
#endif
