/*

	R Project Library

	RFirstNodeHeuristic.hh

	First Fit Heuristic adapted for Trees - Inline Implemenation

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
//
// RGroupingHeuristic<cGroup,cObj,cGroupData>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RFirstNodeHeuristic<cNode,cObj,cNodes>::RFirstNodeHeuristic(RRandom& r,RCursor<cObj> objs,RDebug* debug)
	: RTreeHeuristic<cNode,cObj,cNodes>("FistNode",r,objs,debug), tmpNodes(0), tmpAttrs(100)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RFirstNodeHeuristic<cNode,cObj,cNodes>::Init(cNodes* nodes)
{
	RTreeHeuristic<cNode,cObj,cNodes>::Init(nodes);
	if(!tmpNodes)
		tmpNodes=new cNode*[nodes->Reserved.GetMaxNb()];
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RFirstNodeHeuristic<cNode,cObj,cNodes>::NewNode(const RAttrList& attr,cNode* parent)
{
	cNode* Node=Nodes->ReserveNode();
	Node->SetAttr(attr);
	Nodes->InsertNode(parent,Node);
	return(Node);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RFirstNodeHeuristic<cNode,cObj,cNodes>::FindNode(void)
{
	// Initialization Part
	cNode* Node;                                       // Pointer to the node sharing the maximal attributes with the object
	const RAttrList& CurAttr(CurObj->GetAttr());	   // CurAttr are the attributes to search for
	cNode* ParentNode(0);                              // Start a the top node
	bool GoDeeper;                                     // Must go deeper in the hierarchy ?
	size_t NbMax;                                      // Maximal number of attributes found at the current level
	cNode** CurNode;                                   // Nodes to treat at the current branch

	// Each loop manages one level of one branch in the hierarchy
	while(true)
	{
		// Parse randomly all the nodes of that level
		size_t NbNodes(Nodes->GetTab(tmpNodes,ParentNode,false));
		RandOrder(tmpNodes,NbNodes);
		for(CurNode=tmpNodes,Node=0,GoDeeper=false,NbMax=0,NbNodes++;--NbNodes;CurNode++)
		{
			// Look the number of common attributes between the current child node and the object to attach
			size_t NbCommon=(*CurNode)->GetNbCommon(CurAttr);

			// If no common attributes -> go no the next node
			if(!NbCommon)
				continue;

			// If all the attributes of the node are in list of attributes of the object to attach -> Found a good branch
			if(NbCommon==(*CurNode)->GetAttr().GetNb())
			{
				// If exactly the same number of attributes -> right node
				if((*CurNode)->GetAttr().GetNb()==CurAttr.GetNb())
					return((*CurNode));

				// Else we must go further in the hierarchy
				ParentNode=(*CurNode);
				GoDeeper=true;
				break;
			}

			// Remember the node (in Node) having the maximum number of common attributes with the object to attach
			if(NbCommon>NbMax)
			{
				NbMax=NbCommon;
				Node=(*CurNode);
			}
		}

		// Verify if we must go to next level (CurNode contains the branch to follow)
		if(GoDeeper)
			continue;

		// If no node was found -> Create new one
		if(!Node)
			return(NewNode(CurAttr,ParentNode));

		// A node was found but contains some attributes not representing the objects.
		tmpAttrs.Inter(CurAttr,Node->GetAttr());     // Find the common attributes in tmpAttrs.

		// Verify if the common attributes are not those of the parent
		// -> If yes, take ParentNode as parent of a new node
		if(ParentNode&&(ParentNode->GetAttr().GetNb()==tmpAttrs.GetNb()))
			return(NewNode(CurAttr,ParentNode));

		// A new node containing tmpAttrs must be created and attached to ParentNode
		// It has two child nodes : Node and a new node containing the object
		ParentNode=NewNode(tmpAttrs,ParentNode);
		Nodes->MoveNode(ParentNode,Node);
		return(NewNode(CurAttr,ParentNode));    // The object will be attached to a new node.
	}
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RFirstNodeHeuristic<cNode,cObj,cNodes>::~RFirstNodeHeuristic(void)
{
	if(tmpNodes)
		delete[] tmpNodes;
}
