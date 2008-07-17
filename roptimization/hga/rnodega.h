/*

	R Project Library

	RNodeGA.h

	GA Node - Header.

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
#ifndef RNodeGA_H
#define RNodeGA_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcursor.h>
#include <rhga.h>
#include <rattrlist.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
template<class cNode,class cObj,class cNodes> class RNodedGA;


//------------------------------------------------------------------------------
// Constance
const size_t NoNode=0xFFFFFFFF;


//------------------------------------------------------------------------------
/**
 * The RNodeGA class provides a representation of a node in a HGA.
 * @see R::RNodesGA
 * @author Pascal Francq
 * @short HGA Node.
 */
template<class cNode,class cObj,class cNodes>
	class RNodeGA
{
protected:

	/**
	* Identificator of the Node
	*/
	size_t Id;

	/**
	* Owner of the node.
	*/
	cNodes* Owner;

	/**
	* The list of attributes for the node.
	*/
	RAttrList Attr;

	/**
	* Index of the first subnode in Nodes.
	*/
	size_t SubNodes;

	/**
	* Number of subnodes
	*/
	size_t NbSubNodes;

	/**
	* Pointer to the parent node.
	*/
	cNode* Parent;

	/**
	* Index of the first object attached to the node.
	*/
	size_t SubObjects;

	/**
	* Number of objects attached to the node.
	*/
	size_t NbSubObjects;

	/**
	* Is the node reserved?
	*/
	bool Reserved;

public:

	/**
	* Copy constructor.
	* @param node           Node to copy.
	*/
	RNodeGA(const cNode* node);

	/**
	* Construct the node.
	* @param owner          Owner of the node.
	* @param id             Identificator of the node.
	* @param max            Maximum number of attributes.
	*/
	RNodeGA(cNodes* owner,size_t id,size_t max);

	/**
	* Verify if the node is not violating the integrity of the system.
	* @param nbobjs          Increase the number of objects attached.
	* @return true if the node is correct, false else.
	*/
	bool Verify(size_t &nbobjs);

	/**
	 * Get the parent of the node.
	 */
	cNode* GetParent(void) const {return(Parent);}
	
	/**
	* Return a reference of the attributes.
	*/
	const RAttrList& GetAttr(void) const {return(Attr);}
	
	/**
	 * Assign a given list of attributes to the current node.
	 * @param attr           List of attributes.
	 */
	void SetAttr(const RAttrList& attr);
	
	/**
	* Verify if a given set of attributes can be attached.
	* @param attr           List of attributes.
	*/
	virtual bool CanAttach(const RAttrList& attr) const;

	/**
	* Verify if an given set of attributes has the exactly the same attributes.
	* @param attr           List of attributes.
	*/
	virtual bool IsSame(const RAttrList& attr) const;

	/**
	* Count the number of attributes in common in the node and a list of
	* attributes.
	* @param attr           List of attributes.
	*/
	size_t GetNbCommon(const RAttrList& attr) const;

	/**
	* test if two nodes have the same objects attached.
	* @param node            Node to compare with.
	* @returns always true.
	*/
	bool IsSameObjs(const cNode* node) const;

	/**
	* Method call to insert a node.
	* @param node           Node to insert.
	*/
	void Insert(cNode* node) {Owner->InsertNode(static_cast<cNode*>(this),node);}
	
	/**
	* Method call to delete a node.
	* @param node            Node to delete.
	*/
	void Delete(cNode* node) {Owner->DeleteNode(node);}
	
	/**
	* Copy all the objects and the nodes of a node except one node if it exists.
	* The two nodes have to be of two different owners.
	* @param from           Node to copy from.
	* @param copyobjs       Must the objects of from be copied?
	* @param objs           Objects which nodes must not be copied.
	* @param excluded       Node that must (eventually) not be copied.
	* 
	* This method can be used to copy a whole tree "from" into another "to":
	* @code
	* to->GetTop()->Copy(from->GetTop);
	* @encode
	* @return Pointer to the node that was supposed to have the node excluded
	* (or 0 if not found).
	*/
	cNode* Copy(const cNode* from,const cNode* excluded=0,RVectorInt<size_t,true>* objs=0,bool copyobjs=true);

	/**
	* Method call to insert an object.
	* @param obj            Object to insert.
	*/
	void Insert(const cObj* obj) {Owner->InsertObj(static_cast<cNode*>(this),obj);}

	/**
	* Method call to delete an object.
	* @param obj             Object to delete.
	*/
	void Delete(cObj* obj) {Owner->DeleteObj(obj);}

	/**
	* Method called after a node is attach.
	* @param node           Node attached.
	*/
	virtual void PostInsert(const cNode* node);

	/**
	* Method called after a node is detach.
	* @param node           Node detached.
	*/
	virtual void PostDelete(const cNode* node);

	/**
	* Method called after an object is attach.
	* @param node           Object attached.
	*/
	virtual void PostInsert(const cObj* obj);

	/**
	* Method called after an object is detach.
	* @param node           Object detached.
	*/
	virtual void PostDelete(const cObj* obj);

	/**
	* Method that clears a node.
	*/
	virtual void Clear(void);

	/**
	* Construct a list of all objects contained in the node or its subnodes.
	* This list is ordered by object id.
	* @param objs           The list that will hold the objects. Must be created
	*                       by the caller.
	* @param nbobjs         The number of objects contained in the list.
	*/
	void ConstructAllObjects(cObj** objs,size_t& nbobjs);

	/**
	 * Put the identifiers of all objects contained in the node or its
	 * subnodes).
	 * @param objs           Vector that will hold the objects' identifiers.
	 */
	void GetAllObjects(RVectorInt<size_t,true>& objs) const;
	
	/**
	* Assignment operator.
	* @param node           The node used as source.
	*/
	RNodeGA<cNode,cObj,cNodes>& operator=(const RNodeGA<cNode,cObj,cNodes>& node);

	/**
	* Return the identificator of the node.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Return the number of the subnodes.
	*/
	size_t GetNbNodes(void) const {return(NbSubNodes);}

	/**
	* Return the list of the nodes. The list is created by RNode, but must
	* be deleted by the caller. The list is ended by a NoNode value.
	*/
	size_t* GetNodesId(void) const;

	/**
	* Return the number of the subobjects.
	*/
	size_t GetNbObjs(void) const {return(NbSubObjects);}

	/**
	* Return the list of the objects. The list is created by RNode, but must
	* be deleted by the caller. The list is ended by a NoObject value.
	*/
	size_t* GetObjectsId(void) const;

	/**
	 * Look if the current node has some objects atatched to it.
	 * @param objs           Objects to verify.
	 */
	bool HasSomeObjects(RVectorInt<size_t,true>* objs) const;
	
	/**
	* Destruct the node.
	*/
	virtual ~RNodeGA(void);

	// friend classes
	friend class RNodesGA<cNode,cObj,cNodes>;
	friend class RTreeHeuristic<cNode,cObj,cNodes>;
	friend class RFirstNodeHeuristic<cNode,cObj,cNodes>;
};


//------------------------------------------------------------------------------
// template implementation
#include <rnodega.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
