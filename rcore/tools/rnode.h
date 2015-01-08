/*

	R Project Library

	RNode.h

	Generic Node - Header.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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
// include files for R
#include <rcontainer.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


// forward declaration
template<class T,class N,bool bAlloc> class RTree;
template<class T,class N> class RNodeCursor;


//-----------------------------------------------------------------------------
/**
* @tparam T                  The class of the tree. It must inherit from RTree.
* @tparam N                  The class of the nodes of the tree. It must
*                            inherit from RNode.
* @tparam bAlloc             Specify if the elements are deallocated by the
*                            tree.
* This class represent a generic node. The user has to derived from this class
* to create elements that can be handle by a RTree.
*
* To make the necessary comparisons, the tree uses member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
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
* #include <rnodecursor.h>
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
*    void DoSomething(void)
*    {
*       for(size_t i=0;i<GetDepth();i++)
*          cout<<" ";
*       cout<<Name<<endl;
*       RNodeCursor<MyTree,MyNode> Cur(this);
*       for(Cur.Start();!Cur.End();Cur.Next())
*          Cur()->DoSomething();
*    }
*    friend class R::RNodeCursor<MyTree,MyNode>;
* };
*
* class MyTree : public RTree<MyTree,MyNode,true>
* {
* public:
*    MyTree(void) : RTree<MyTree,MyNode,true>() {}
*    friend class R::RNodeCursor<MyTree,MyNode>;
* };
*
* int main()
* {
*    MyTree Tree;
*    MyNode *a,*b,*c,*d,*m;
*    Tree.InsertNode(0,a=new MyNode("mammal"));
*    Tree.InsertNode(a,m=new MyNode("rabbit"));
*    Tree.InsertNode(a,d=new MyNode("carnivore"));
*    Tree.InsertNode(m,b=new MyNode("fox"));
*    Tree.InsertNode(m,c=new MyNode("dog"));
*
*    RNodeCursor<MyTree,MyNode> Cur(Tree);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       Cur()->DoSomething();
* }
* @endcode
* @remark Note that RNodeCursor class is defined as friend class for both, the
* node class and the tree class. This is necessary to use the RNodeCursor class.
*
* In practice, all child nodes of a given node (or the root node) are stored as
* a doubly linked list.
* @author Pascal Francq
* @short Generic Node.
*/
template<class T,class N,bool bAlloc>
	class RNode
{
protected:

	/**
	* Parent Node.
	*/
	N* Parent;

	/**
	 * Previous node.
	 */
	N* Prev;

	/**
	 * Next node.
	 */
	N* Next;

	/**
	* First child node.
	*/
	N* First;

	/**
	* Last child node.
	*/
	N* Last;

	/**
	* Number of child nodes
	*/
	size_t NbSubNodes;

	/**
	 * Depth of the node.
	 */
	size_t Depth;

public:

	/**
	* Default constructor.
	*/
	RNode(void);

	/**
	* Default constructor.
	* @param tree            The tree.
	*/
	RNode(T* tree);

	/**
	 * Clear the node.
	 */
	virtual void Clear(void);

	/**
    * @return a pointer to the concept tree.
    */
//	const T* GetTree(void) const {return(Tree);}

	/**
    * @return a pointer to the concept tree.
    */
//	T* GetTree(void) {return(Tree);}

	/**
	* Return the parent of the node.
	* @returns Pointer to N.
	*/
	inline N* GetParent(void) const {return(Parent);}

	/**
    * @return the first child node. If null, the node has no children.
    */
	inline N* GetFirst(void) const {return(First);}

	/**
    * @return the last child node. If null, the node has no children.
    */
	inline N* GetLast(void) const {return(Last);}

	/**
    * @return the previous node of the same parent. If null, the node is the
	 * first child.
    */
	inline N* GetPrev(void) const {return(Prev);}

	/**
    * @return the next node of the same parent. If null, the node is the last
	 * child.
    */
	inline N* GetNext(void) const {return(Next);}

	/**
	* @return the number of child nodes.
	*/
	inline size_t GetNbNodes(void) const {return(NbSubNodes);}

	/**
    * @return the total number of child nodes (including all possible children).
    */
	size_t GetNbTotalNodes(void) const;

	/**
	 * Get the depth of the node.
	 */
	inline size_t GetDepth(void) const {return(Depth);}

	/**
	* Get a pointer to a certain child node. In practice, In practice, the search
	* is done by parsing the tree starting from the first child node inserted.
	* @tparam TUse           The type of the tag used for the search.
	* @param tag             The tag used.
	* @return Return the pointer or 0 if the element is not a child node.
	*/
	template<class TUse> N* GetNode(const TUse& tag) const;

	/**
	* Method call to insert a node.
	* @param node           Node to insert.
	*/
//	void InsertNode(N* node);

	/**
	* Test if the tag is empty, i.e. it has no child nodes.
	*/
	virtual bool IsEmpty(void);

	/**
	 * Verify the integrity of the node.
	 * @param id             Identifier identifying the node for the caller.
	 * @return true if the node seems coherent.
	 */
	bool VerifyNode(size_t id);

	/**
	 * Get the cost of an Up operation of the current node. By default, the
	 * cost equals to 1.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define the up operation as the operation that moves a token from one
	 * node to its parent.
	 * @return Cost of the up operation.
	 */
	virtual double GetUpOperationCost(void) const;

	/**
	* Destruct the node.
	*/
	virtual ~RNode(void);

	// The RTree is a  friend class
	friend class RTree<T,N,bAlloc>;
	friend class RNodeCursor<T,N>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rnode.hh>


}  //---- End namespace R -----------------------------------------------------


//-----------------------------------------------------------------------------
#endif
