/*

	R Project Library

	RFirstNodeHeuristic.hh

	First Fit Heuristic adapted for Trees - Inline Implemenation

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
//
// RGroupingHeuristic<cGroup,cObj,cGroupData>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RFirstNodeHeuristic<cNode,cObj,cNodes>::RFirstNodeHeuristic(RRandom* r,RCursor<cObj> objs,RDebug* debug)
	: RTreeHeuristic<cNode,cObj,cNodes>("FistNode",r,objs,debug), tmpNodes(0), CurNode(0), CurAttr(0)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RFirstNodeHeuristic<cNode,cObj,cNodes>::Init(cNodes* nodes)
{
	RTreeHeuristic<cNode,cObj,cNodes>::Init(nodes);
	if(!tmpNodes)
		tmpNodes=new cNode*[nodes->MaxPtr];
}

	
//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RFirstNodeHeuristic<cNode,cObj,cNodes>::NewNode(void)
{
	cNode* Node=this->Nodes->ReserveNode();
	Node->Attr=(*CurAttr);
	this->Nodes->InsertNode(CurNode,Node);
	return(Node);
}

	
//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RFirstNodeHeuristic<cNode,cObj,cNodes>::FindNode(void)
{
	size_t i;
	size_t NbMax;    // Maximum number of common attributes find so far
	cNode* Node;     // Pointer to the node than can hold an object
	cNode **N;       // Pointer to go through the existing nodes
	
	// Init Part
	CurNode=this->Nodes->Top;              // Start a the top node
	CurAttr=&this->CurObj->GetAttr();      // Attributes of the object to attach
	
	while(true)
	{
		// Has the current node subnodes where to attach the object?
		if(!CurNode->GetNbNodes())
		{
			// No -> If the current node has the same attributes as the object to attach, it is the searched node
			if(CurNode->IsSame(*CurAttr))
				return(CurNode);
			
			// Else a new subnode must be created
			return(NewNode());
		}
		
		// Get the subnodes of the current node and randomize them
		i=this->Nodes->GetNodes(tmpNodes,*CurNode);
		RandOrder<cNode*>(tmpNodes,i);
		
		// Go trough the subnodes
		for(N=tmpNodes,NbMax=0,Node=0,i++;--i;N++)
		{
			// Look the number of common attributes between the current subnode and the object to attach
			size_t NbCommon=(*N)->GetNbCommon(*CurAttr);

			// If no common attributes -> go no the next node
			if(!NbCommon)
				continue;
			
			// If all the attributes of the node are in list of attributes of the object to attach -> Found a good branch
			if(NbCommon==(*N)->GetAttr().GetNb())
			{
				// If exactly the same number of attributes -> right node
				if((*N)->GetAttr().GetNb()==CurAttr->GetNb())
					return(*N);
				
				// Else we must go further in the hierarchy
				Node=CurNode=(*N);
				break;
			}
			
			// Remember the node having the maximum number of common attributes with the object to attach
			if(NbCommon>NbMax)
			{
				NbCommon=NbMax;
				Node=(*N);
			}
		}
		
		// Verify if we must go to next level
		if(CurNode==Node)
			continue;
		
		// If no node was found -> Create new one
		if(!Node)
			return(NewNode());
		
		// A new node must be created with the common attributes (new level in the hierarchy):
		// The current subnode must be move to it.
		// Another node must be created to hold the object to attach		
		CurNode=NewNode();
		CurNode->Attr.Inter(*CurAttr,Node->GetAttr());
		if(CurNode->Parent->IsSame(CurNode->GetAttr()))
		{			
			cNode* parent=CurNode->Parent;
			this->Nodes->ReleaseNode(CurNode);
			this->Nodes->MoveNode(parent,Node);
			CurNode=parent;
		}
		else
		{
			// Verify if the new node is not identical to another node of the parent
			RCursor<cNode> ParentSub(this->Nodes->GetNodes(CurNode->Parent));
			for(ParentSub.Start();!ParentSub.End();ParentSub.Next())
				if((ParentSub()!=CurNode)&&(ParentSub()->IsSame(CurNode->GetAttr())))
					break;
			if(!ParentSub.End())
			{
				// A compatible node exist at the upper level
				cNode* parent=ParentSub();
				this->Nodes->ReleaseNode(CurNode);
				CurNode=parent;				
			}
			else
				this->Nodes->MoveNode(CurNode,Node);
		}
		return(NewNode());
	};	
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RFirstNodeHeuristic<cNode,cObj,cNodes>::~RFirstNodeHeuristic(void)
{
	if(tmpNodes)
		delete[] tmpNodes;
}
