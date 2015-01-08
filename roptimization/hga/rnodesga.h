/*

	R Project Library

	RNodesGA.h

	Class representing a collection of nodes - Header

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RNodesGA_H
#define RNodesGA_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rnumcontainer.h>
#include <rcursor.h>
#include <rtree.h>
#include <rnodega.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RNodesGA class provides a representation of a tree in a HGA.
 * @see R::RNodeGA
 * @author Pascal Francq
 * @short HGA Tree.
 */
template<class cNode,class cObj,class cNodes>
	class RNodesGA : public RTree<cNodes,cNode,false>
{
	using RTree<cNodes,cNode,false>::InsertNode;
	using RTree<cNodes,cNode,false>::GetNbNodes;

protected:

	/**
	 * Nodes reserved. It contains a number of allocated nodes, some of them
	 * being used in the tree.
	 */
	RContainer<cNode,true,false> Reserved;

	/**
	 * All the nodes used in the tree, ordered by identifier.
	 */
	RContainer<cNode,false,true> Nodes;

	/**
	* The Objects to put in a tree.
	*/
	RCursor<cObj> Objs;

	/**
	* Assignment of the objects.
	*/
	size_t* ObjectsAss;

	/**
	* Array of objects attached.
	*/
	RContainer<cObj,false,false> ObjsAss;

	/**
	* Array of objects not yet attached.
	*/
	RContainer<cObj,false,false> ObjsNoAss;

	/**
	 * Maximum number of attributes for an object.
	 */
	size_t MaxAttr;

public:

	/**RVector
	* Construct the nodes.
	* @param objs           Objects to attach.
	* @param maxnodes       Maximum number of nodes.
	* @param maxattr        Maximum number of attributes per node.
	*/
	RNodesGA(RCursor<cObj> objs,size_t maxnodes,size_t maxattr);

	/**
	* Init the nodes.
	*/
	void Init(void);

	/**
	* Clear all the information.
	*/
	virtual void Clear(void);

	/**
	* @return the total number of nodes in the tree.
	*/
	inline size_t GetNbNodes(void) const {return(Nodes.GetNb());}

	/**
	* @return the total number of top nodes in the tree.
	*/
	inline size_t GetNbTopNodes(void) const {return(RTree<cNodes,cNode,false>::GetNbTopNodes());}

	/**
	* @return the maximum depth in the tree.
	*/
	inline size_t GetMaxDepth(void) const {return(RTree<cNodes,cNode,false>::GetMaxDepth());}

	/**
	* Reserve a Node.
	* @returns A pointer to the node reserved.
	*/
	cNode* ReserveNode(void);

private:

	/**
	* Release a node.
	* @param node           Pointer to the node.
	*/
	void ReleaseNode(cNode* node);

public:

	/**
	* Insert a node in the list attached to a parent. If the parent is null,
	* the node is to insert as a top node of the tree.
	* @param parent         Parent where the node is to attached.
	* @param node           Node to insert.
	*/
	void InsertNode(cNode* parent,cNode* node);

	/**
	* Delete and release a node in the list attached from a parent. If the parent
	*  is null, the node to delete is considered a top node of the tree.
	*
	*  The child nodes are also deleted and released.
	* @param node           Node to delete.
	*/
	void DeleteNode(cNode* node);

	/**
	* Delete and Releases all the child nodes of a given node.
	* @param node           Pointer to the node.
	*/
	virtual void DeleteNodes(cNode* node);

	/**
	* Move a node to the list of child nodes of a given node.
	* @param to             Pointer to the destination node.
	* @param node           Pointer to the node to move.
	*/
	void MoveNode(cNode* to,cNode* node);

	/**
	* Copy a tree except one branch.
	* @param from           Tree to copy from.
	* @param excluded       Node that must (eventually) not be copied.
	* @param objs           Objects that must not be copied.
	* @param copyobjs       Must the objects be copied?
	* @return Pointer to the node that was supposed to have the node excluded
	* (or 0 if not found).
	*/
	cNode* CopyExceptBranch(const cNodes* from,const cNode* excluded=0,RNumContainer<size_t,true>* objs=0,bool copyobjs=true);

	/**
	* Copy all the objects and the nodes to a node except one node if it exists.
	* The two nodes have to be of two different owners.
	* @param to             Node where to copy.
	* @param from           Node to copy from.
	* @param copyobjs       Must the objects of from be copied?
	* @param objs           Objects that must not be copied.
	* @param excluded       Node that must (eventually) not be copied.
	*
	* This method can be used to copy a whole tree "from" into another "to":
	* @code
	* to->Copy(to->GetTop(),from->GetTop);
	* @endcode
	* @return Pointer to the node that was supposed to have the node excluded
	* (or 0 if not found).
	*/
	cNode* CopyExceptBranch(cNode* to,const cNode* from,const cNode* excluded=0,RNumContainer<size_t,true>* objs=0,bool copyobjs=true);

	/**
	* Insert an object to a node.
	* @param to             Pointer of the destination node.
	* @param obj            Pointer to the object to insert.
	*/
	void InsertObj(cNode* to,cObj* obj);

	/**
	* Delete an object from a node.
	* @param from           Pointer to the node.
	* @param obj            Pointer to the object to delete.
	*/
	void DeleteObj(cNode* from,const cObj* obj);

	/**
	* Delete an object from the tree.
	* @param obj            Pointer to the object to delete.
	*/
	void DeleteObj(const cObj* obj);

	/**
	* Delete all the objects attached to a given node.
	* @param node           Pointer to the node.
	*/
	void DeleteObjs(cNode* node);

	/**
	* Move all the objects of a node to another.
	* @param to             Pointer of the destination node.
	* @param from           Pointer of the origin node.
	*/
	void MoveObjs(cNode* to,cNode* from);

	/**
	* Return a cursor over the objects of a node. The cursor cannot iterate
	* after the last object.
	* @param node            Node.
	*/
	RCursor<cObj> GetObjs(const cNode* node) const;

	/**
	* Verify the validity of the tree.
	* @param complete        Is the tree supposed to be complete.
	* @return True if the chromosome is a valid one, false else.
	*/
	bool VerifyNodes(bool complete);

	/**
	* Copy a whole tree in the current one.
	* @param nodes           Nodes used as source.
	*/
	void CopyTree(const cNodes& nodes);

	/**
	 * Build a file with a tree.
	 * @param name           Name of the file.
	 */
	void BuildFile(const RString& name);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RNodesGA(void);

	// Friend classes
	friend class RNodeGA<cNode,cObj,cNodes>;
	friend class RTreeHeuristic<cNode,cObj,cNodes>;
	friend class RFirstNodeHeuristic<cNode,cObj,cNodes>;
	friend class RNodeCursor<cNodes,cNode>;
};


//------------------------------------------------------------------------------
// Inline implementation
#include <rnodesga.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
