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


//------------------------------------------------------------------------------
// forward declaration
template<class T,class N,bool bAlloc> class RNode;


//------------------------------------------------------------------------------
/**
* @param T                   The class of the tree.
* @param N                   The class of the elements of the tree.
* @param bAlloc              Specify if the elements are deallocated by the
*                            tree.
* This class implement a generic tree of node of a given class N. The class N
* must derived from the RNode class.
*
* Here is a example:
* @code
* #include <rtree.h>
* #include <rnode.h>
* #include <rcursor.h>
* using namespace R;
*
* class MyTree;         // Forward declaration
*
* class MyNode : public RNode<MyTree,MyNode,true>
* {
*    RString Name;
* public:
*    MyNode(const RString& name) : RNode<MyTree,MyNode,true>(), Name(name) {}
*    int Compare(const MyNode& node) const {return(Name.Compare(node.Name));}
*    void DoSomething(void) {cout<<Name<<endl;}
* };
*
* class MyTree : public RTree<MyTree,MyNode,true>
* {
* public:
*    MyTree(size_t max,size_t inc) : RTree<MyTree,MyNode,true>(max,inc) {}
* };
*
* int main()
* {
*    MyTree Tree(20,10);
*
*    // Insert a top node
*    cNode* Top(new MyNode("First Level"));
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
* @short Generic Tree.
*/
template<class T,class N,bool bAlloc>
	class RTree : private RContainer<N,bAlloc>
{
	using RContainer<N,bAlloc>::DeletePtrAt;

protected:

	/**
	* Number of top nodes
	*/
	size_t NbTopNodes;

public:

	/**
	* Construct the tree.
	* @param max             Initial size of the array of top nodes.
	* @param inc             Increment size of the array.
	*/
	RTree(size_t max,size_t inc);

	/**
	 * Clear the nodes of the tree.
	 */
	void ClearNodes(void);

	/**
	 * This method should be used with caution, since it works only if there is
	 * one top node. If several top nodes were inserted, an exception is
	 * generated.
	 * @return the top node or 0 if no top node.
	 */
	N* GetTop(void);

	/**
	 * This method should be used with caution, since it works only if there is
	 * one top node. If several top nodes were inserted, an exception is
	 * generated.
	 */
	const N* GetTop(void) const;

	/**
	* Get a cursor over the top nodes.
	* @return RCursor<N>.
	*/
	RCursor<N> GetTopNodes(void) const;

	/**
	* Get the total number of nodes in the tree.
	* @return size_t
	*/
	inline size_t GetNbNodes(void) const {return(RContainer<N,bAlloc,false>::GetNb());}

	/**
	* Get a cursor over all the nodes of the tree.
	*/
	inline RCursor<N> GetNodes(void) const {return(R::RCursor<N>(*this));}

	/**
	* Get a cursor over the child nodes of a given node.
	* @param node            Parent node. If null, the top nodes are returned.
	*/
	inline RCursor<N> GetNodes(N* node) const;

	/**
	* Copy the array of nodes into a temporary array. This array must have
	* the right size (Read only). This method can be limited to a part of the
	* tree.
	* @param tab             Array of nodes.
	* @param min             Starting index.
	* @param max             Ending index.
	* @return number of elements of the tree.
	*/
	inline size_t GetTab(const N** tab,size_t min=0, size_t max=0) const {return(RContainer<N,bAlloc,false>::GetTab(tab,min,max));}

	/**
	* Copy the array of nodes into a temporary array. This array must have
	* the right size (Read/Write). This method can be limited to a part of the
	* tree.
	* @param tab             Array of nodes.
	* @param min             Starting index.
	* @param max             Ending index.
	* @return number of elements in the tree.
	*/
	inline size_t GetTab(N** tab,size_t min=0, size_t max=0) {return(RContainer<N,bAlloc,false>::GetTab(tab,min,max));}

	/**
	 * Copy the nodes of a node into a temporary array. This array must have
	 * the right size.
	 * @param nodes           Temporary array.
	 * @param node            Parent node. If null, the array contains the top
	 *                        nodes.
	 * @return Number of the nodes copied in the array
	 */
	size_t GetTab(N** tab,N* node);

	/**
	* Clear the tree and destruct the nodes if it is responsible for
	* the deallocation.
	* @param m               New maximal size of the array. If null, the old
	*                        size remains.
	* @param i               New increasing value. If null, the old value
	*                        remains.
	*/
	inline void Clear(size_t m=0,size_t i=0) {RContainer<N,bAlloc,false>::Clear(m,i); NbTopNodes=0;}

	/**
	* Insert a node and attached it to a parent. If the parent is null,
	* the node is considered as the top node of the tree.
	* @param to              Parent where the node is to attached.
	* @param node            Node to insert.
	*/
	void InsertNode(N* to,N* node);

	/**
	* Delete a node from the tree.
	* @param node            Node to delete.
	* @param childs          Child nodes are also deleted ?
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	*/
	void DeleteNode(N* node,bool childs,bool del=bAlloc);

	/**
	 * Move a node from one parent to another inside the same tree.
	 * @param to             Parent where the node is to attached.
	 * @param node           Node to insert.
	 */
	void MoveNode(N* to,N* node);

	/**
	* Deep copy of a the tree.
	* @param src             Source tree.
	*/
	template<bool a> void Copy(const RTree<T,N,a>& src);

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
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	template<class TUse> N* GetNode(const TUse& tag) const;

	/**
	 * Get a node at a given index (const version). This function must be used
	 * carefully.
	 * @param idx            Index.
	 * @return Pointer to the node.
	 */
	const N* GetNodeAt(size_t idx) const {return((*this)[idx]);}

	/**
	 * Get a node at a given index. This function must be used carefully.
	 * @param idx            Index.
	 * @return Pointer to the node.
	 */
	N* GetNodeAt(size_t idx) {return((*this)[idx]);}

	/**
	* Destruct the tree.
	*/
	virtual ~RTree(void);

	// The RNode is a friend class
	friend class RNode<T,N,bAlloc>;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rtree.hh>


}//---- End namespace R --------------------------------------------------------


//------------------------------------------------------------------------------
#endif
