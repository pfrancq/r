/*

	R Project Library

	RTree.h

	Generic Tree - Header.

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
template<class T,class N> class RNodeCursor;


//------------------------------------------------------------------------------
/**
* @tparam T                  The class of the tree. It must inherit from RTree.
* @tparam N                  The class of the nodes in the tree. It must
*                            inherit from RNode.
* @tparam bAlloc             Specify if the elements are deallocated by the
*                            tree.
* This class implement a generic tree of node of a given class N. The class N
* must derived from the RNode class.
*
* Here is a example:
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
*       RNodeCursor<MyTree,MyNode> Cur(*this);
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
*
* The tree itself acts a root node, i.e. there may be several top nodes. Since
* the root node is not managed as an instance of the node class, the developer
* must always create at least one top node.
*
* When you have to do multiple steps of insertions and deletes in the tree, it
* is a better idea to ensure the allocations and deallocations through a
* specific class inheriting from RTree rather than let the latest be the
* responsible. The RNodesGA class illustrates this approach.
* @short Generic Tree.
*/
template<class T,class N,bool bAlloc>
	class RTree
{
protected:

	/**
	 * First top node.
	 */
	N* First;

	/**
	 * Last top node.
	 */
	N* Last;

	/**
	* Number of top nodes.
	*/
	size_t NbTopNodes;

	/**
	* Number of nodes.
	*/
	size_t NbNodes;

	/**
	 * Depth of the tree. The depth can be undefined, the value is then set to
	 * 'cNoRef'.
	 */
	size_t MaxDepth;

public:

	/**
	* Construct the tree.
	*/
	RTree(void);

	/**
	 * Call the RNode::Clear() method for a node and all its children.
    * @param node           Node to clear.
    */
	void ClearNode(N* node);

	/**
	 * Clear the nodes of the tree. If the tree is not responsible for the
	 * deallocation, the method calls RNode::Clear() for each node.
	 */
	virtual void Clear(void);

	/**
    * @return the first root node. If null, the tree is empty.
    */
	inline N* GetFirst(void) const {return(First);}

	/**
    * @return the last root node. If null, the tree is empty.
    */
	inline N* GetLast(void) const {return(Last);}

	/**
	 * @return the depth of the tree. If necessary, the depth is recomputed.
	 */
	inline size_t GetMaxDepth(void) const;

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
	* @return the total number of nodes in the tree.
	*/
	inline size_t GetNbNodes(void) const {return(NbNodes);}

	/**
	* @return the total number of top nodes in the tree.
	*/
	inline size_t GetNbTopNodes(void) const {return(NbTopNodes);}

	/**
	 * Copy the nodes of a node into a temporary array. This array must have
	 * the right size.
	 * @param tab             Temporary array.
	 * @param node            Parent node. If null, the array contains the top
	 *                        nodes.
	 * @param children        Must the children be copied too.
	 * @return Number of the nodes copied in the array
	 */
	size_t GetTab(N** tab,N* node,bool children=false);

	/**
	* Insert a node and attached it to a parent. If the parent is null,
	* the node is considered as one of the top node of the tree.
	* @param to              Parent where the node is to attached. This node
	*                        must already been attached to the tree.
	* @param node            Node to insert.
	*/
	void InsertNode(N* to,N* node);

	/**
	* Delete all the child nodes of a given node.
	* @param node           Pointer to the node.
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	*/
	void DeleteNodes(N* node,bool del=bAlloc);

	/**
	* Delete a node from the tree. Its child nodes are also deleted.
	* @param node            Node to delete.
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	*/
	void DeleteNode(N* node,bool del=bAlloc);

	/**
	 * Update the depth of all child nodes of a given node. In practice, at each
	 * new step, the depth is incremented of 1.
	 *
	 * This method is used by MoveNode.
    * @param node           Parent node.
    */
	void UpdateDepth(N* node);

	/**
	 * Move a node from one parent to another inside the same tree.
	 * @param to             Parent where the node is to attached.
	 * @param node           Node to insert.
	 */
	void MoveNode(N* to,N* node);

	/**
	* Deep copy of a the tree.
	* @tparam a              Determine if the source container is responsible for
	*                        the deallocation.
	* @param src             Source tree.
	* @warning The tree is not emptied before the source tree is copied.
	*/
	template<bool a> void Copy(const RTree<T,N,a>& src);

	/**
	* Do a deep copy of a node of another tree.
	* @param src             Source node in the another tree.
	* @param newparent       Parent node.
	*/
	void DeepCopy(N* src,N* newparent);

	/**
	* Get a pointer to a certain node of the tree. In practice, the search is
	* done by parsing the tree, starting from the first top node inserted.
	* @tparam TUse           The type of the tag used for the search.
	* @param tag             The tag used.
	* @return Return the pointer or 0 if the element is not in the tree.
	*/
	template<class TUse> N* GetNode(const TUse& tag) const;

	/**
	 * Get the cost of the Up operations to move a token from a node to another
	 * one.
	 *
	 * In their paper <em>TreeRank: A Similarity Measure for Nearest Neighbor
	 * Searching in Phylogenetic Databases</em>, Wang, Shan, Shasha and Piel
	 * define a similarity measure between two trees based on the different
	 * costs of up operations of the nodes of each tree.
	 * @param u              First node.
	 * @param v              Second node.
	 * @return Cost of the up operations.
	 */
	double GetUpOperationsCost(const N* u,const N* v) const;

	/**
	* Destruct the tree.
	*/
	virtual ~RTree(void);

	// The RNode is a friend class
	friend class RNode<T,N,bAlloc>;
	friend class RNodeCursor<T,N>;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rtree.hh>


}//---- End namespace R --------------------------------------------------------


//------------------------------------------------------------------------------
#endif
