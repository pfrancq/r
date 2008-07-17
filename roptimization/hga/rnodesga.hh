/*

	R Project Library

	RNodesGA.hh

	Class representing a collection of nodes - Inline implementation

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



//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>::RNodesGA(RCursor<cObj> objs,size_t maxnodes,size_t maxattr)
		: RContainer<cNode,true,false>(maxnodes), Used(maxnodes), Top(0),
		   Objs(objs), NodesAss(maxnodes), ObjsAss(objs.GetNb()), ObjsNoAss(objs.GetNb()), MaxAttr(maxattr)
{
	ObjectsAss = new size_t[Objs.GetNb()];
	memset(ObjectsAss,0xFF,Objs.GetNb()*sizeof(size_t));

	// Suppose no object is assigned
	for(Objs.Start();!Objs.End();Objs.Next())
		ObjsNoAss.InsertPtr(Objs());
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::Init(void)
{
	// Init Nodes
	Top=new cNode(static_cast<cNodes*>(this),NoNode,MaxAttr);
	for(size_t i=0;i<this->MaxPtr;i++)
		InsertPtr(new cNode(static_cast<cNodes*>(this),i,MaxAttr));
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::ClearNodes(void)
{
	RCursor<cNode> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Top->Clear();
	Used.Clear();
	NodesAss.Clear();
	ObjsAss.Clear();
	ObjsNoAss.Clear();
	for(Objs.Start();!Objs.End();Objs.Next())
		ObjsNoAss.InsertPtr(Objs());
	memset(ObjectsAss,0xFF,Objs.GetNb()*sizeof(size_t));
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RNodesGA<cNode,cObj,cNodes>::ReserveNode(void)
{
	size_t i,NewSize;

	if(Used.GetNb()+1>this->GetMaxNb())
	{
		NewSize=this->GetMaxNb()+this->GetIncNb();

		// Create New nodes
		for(i=this->GetMaxNb();i<NewSize;i++)
			InsertPtr(new cNode(static_cast<cNodes*>(this),i,MaxAttr));
	}
	RCursor<cNode> ptr(*this);
	ptr.Start();
	while(ptr()->Reserved)
		ptr.Next();
	ptr()->Reserved=true;
	Used.InsertPtr(ptr());
	return(ptr());
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::ReleaseNode(cNode* node)
{
	if(!node->Reserved)
		return;		
	DeleteNodes(node);
	DeleteObjs(node);
	if(node->Parent)
		DeleteNode(node);
	if(node!=Top)
		Used.DeletePtr(node);
	node->Clear();
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::InsertNode(cNode* to,cNode* node)
{
	if(node==Top) return;
	if(!to)
		to=Top;
	if(to->NbSubNodes)
	{
		size_t tmp=to->SubNodes+to->NbSubNodes;
		NodesAss.InsertPtrAt(node,tmp,false);
		RCursor<cNode> Cur(*this);
		for(Cur.Start();!Cur.End();Cur.Next())
			if((Cur()->SubNodes>to->SubNodes)&&(Cur()->SubNodes!=NoNode))
				Cur()->SubNodes++;
		if((Top->SubNodes>to->SubNodes)&&(Top->SubNodes!=NoNode))
			Top->SubNodes++;
	}
	else
	{
		to->SubNodes=NodesAss.GetNb();
		NodesAss.InsertPtrAt(node,to->SubNodes,false);
	}
	node->Parent = to;
	to->NbSubNodes++;
	to->PostInsert(node);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteNode(cNode* node)
{
	cNode* from;

	if(node==Top) return;
	NodesAss.DeletePtr(node);
	from=node->Parent;
	if(!(--from->NbSubNodes))
		from->SubNodes=NoNode;
	node->Parent=0;
	from->PostDelete(node);
	if((Top->SubNodes>from->SubNodes)&&(Top->SubNodes!=NoNode))
		Top->SubNodes--;
	RCursor<cNode> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubNodes>from->SubNodes)&&(Cur()->SubNodes!=NoNode))
			Cur()->SubNodes--;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteNodes(cNode* from)
{
	size_t i,j;
	cNode* Node;

	if(!(from->NbSubNodes)) return;
	for(i=0;i<from->NbSubNodes;i++)
	{
		Node=NodesAss[from->SubNodes];
		NodesAss.DeletePtr(Node);
		from->PostDelete(Node);
		ReleaseNode(Node);
	}
	j=from->SubNodes;
	RCursor<cNode> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubNodes>j)&&(Cur()->SubNodes!=NoNode))
			Cur()->SubNodes-=from->NbSubNodes;
	if((Top->SubNodes>j)&&(Top->SubNodes!=NoNode))
		Top->SubNodes-=from->NbSubNodes;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::MoveNode(cNode* to,cNode* node)
{
	if(to==node->Parent)
		return;
	DeleteNode(node);
	InsertNode(to,node);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::MoveNodes(cNode* to,cNode* from)
{
	size_t j,i,*ptr;
	cNode **N,Node;

	if(!from->NbSubNodes)
		return;
		for(i=0;i<from->NbSubNodes;i++)
			MoveNode(to,&Used.Tab[from->SubNodes+i]);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::InsertObj(cNode* to,const cObj* obj)
{
	size_t j;

	if(to==Top) return;
	if(to->NbSubObjects)
	{
		j=to->SubObjects;
		RCursor<cNode> Cur(Used);
		for(Cur.Start();!Cur.End();Cur.Next())
			if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=NoObject))
				Cur()->SubObjects++;
		ObjsAss.InsertPtrAt(obj,to->SubObjects+to->NbSubObjects,false);
	}
	else
	{
		to->SubObjects=ObjsAss.GetNb();
		ObjsAss.InsertPtrAt(obj,to->SubObjects,false);
	}
	to->NbSubObjects++;
	ObjectsAss[obj->GetId()]=to->Id;
	ObjsNoAss.DeletePtr(obj);
	to->PostInsert(obj);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteObj(cNode* from,const cObj* obj)
{
	size_t j;

	if(from==Top) return;
	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=NoNode;
	ObjsAss.DeletePtr(obj);
	ObjsNoAss.InsertPtr(obj);
	if(!(--(from->NbSubObjects)))
		from->SubObjects=NoObject;
	from->PostDelete(obj);
	RCursor<cNode> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=NoObject))
			Cur()->SubObjects--;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteObj(const cObj* obj)
{
	size_t i,j;
	cNode* from;

	i=ObjectsAss[obj->GetId()];
	if(i==NoNode) return;
	from=(*this)[i];
	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=NoNode;
	ObjsAss.DeletePtr(obj);
	ObjsNoAss.InsertPtr(obj);
	if(!(--(from->NbSubObjects)))
		from->SubObjects=NoObject;
	from->PostDelete(obj);
	RCursor<cNode> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=NoObject))
			Cur()->SubObjects--;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteObjs(cNode* from)
{
	size_t j;

	if(from==Top) return;
	if(!(from->NbSubObjects)) return;
	RCursor<cObj> o(GetObjs(*from));
	for(o.Start();!o.End();o.Next())
	{
		// No need to increment o, because delete moves the everything
		ObjectsAss[o()->GetId()]=NoNode;
		ObjsNoAss.InsertPtr(o());
		ObjsAss.DeletePtr(o());
		if(!(--from->NbSubObjects))
			from->SubObjects=NoObject;
		from->PostDelete(o());
	}
	j=from->SubObjects;
	RCursor<cNode> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=NoObject))
			Cur()->SubObjects-=from->NbSubObjects;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::MoveObjs(cNode* to,cNode* from)
{
	for(size_t i=0;i<from->NbSubObjects;i++)
	{
		// Because DeleteNode, always take the first object.
		cObj* obj=ObjsAss[from->SubObjects];
		DeleteObj(from,obj);
		InsertObj(to,obj);
	}
}

	
//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RCursor<cNode> RNodesGA<cNode,cObj,cNodes>::GetNodes(const cNode& node) const
{
	if(!node.NbSubNodes)
		return(RCursor<cNode>(NodesAss,0,0));	
	return(RCursor<cNode>(NodesAss,node.SubNodes,node.SubNodes+node.NbSubNodes));
}

	
//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	size_t RNodesGA<cNode,cObj,cNodes>::GetNodes(cNode** nodes,const cNode& node)
{
	NodesAss.GetTab(nodes,node.SubNodes,node.SubNodes+node.NbSubNodes);
	return(node.NbSubNodes);
}
	
	
	
//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RCursor<cObj> RNodesGA<cNode,cObj,cNodes>::GetObjs(const cNode& node) const
{
	if(!node.NbSubObjects)
		return(RCursor<cObj>(ObjsAss,0,0));			
	return(RCursor<cObj>(ObjsAss,node.SubObjects,node.SubObjects+node.NbSubObjects));
}

		
//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodesGA<cNode,cObj,cNodes>::VerifyNodes(void)
{
	size_t nbobjs=0;
	RCursor<cNode> Cur(this->GetNodes(Top));
	for(Cur.Start();!Cur.End();Cur.Next())
		if(!Cur()->Verify(nbobjs))
			return(false);
	if(nbobjs!=Objs.GetNb())
	{
		std::cerr<<"All objects are not attached"<<std::endl;
		return(false);
	}
 	if(ObjsAss.GetNb()+ObjsNoAss.GetNb()!=Objs.GetNb())
 	{
 		std::cerr<<"There is a problem in the assignment information of objects"<<std::endl;
 		return(false);
 	}
 	return(true);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>& RNodesGA<cNode,cObj,cNodes>::operator=(const RNodesGA& nodes)
{
	ClearNodes();
	Top->Copy(nodes.Top);
	return(*this);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>::~RNodesGA(void)
{
	if(Top)
		delete Top;
	delete[] ObjectsAss;
}
