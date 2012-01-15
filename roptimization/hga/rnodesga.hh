/*

	R Project Library

	RNodesGA.hh

	Class representing a collection of nodes - Inline implementation

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



//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>::RNodesGA(RCursor<cObj> objs,size_t maxnodes,size_t maxattr)
		: RTree<cNodes,cNode,false>(maxnodes,maxnodes/2), Reserved(maxnodes),
		   Objs(objs), ObjsAss(objs.GetNb()), ObjsNoAss(objs.GetNb()), MaxAttr(maxattr)
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
	// Initialize Nodes
	for(size_t i=0;i<Reserved.GetMaxNb();i++)
		Reserved.InsertPtr(new cNode(static_cast<cNodes*>(this),i,MaxAttr));
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::ClearNodes(void)
{
	RTree<cNodes,cNode,false>::Clear();
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

	if(GetNbNodes()+1>Reserved.GetMaxNb())
	{
		NewSize=Reserved.GetMaxNb()+Reserved.GetIncNb();

		// Create New nodes
		for(i=Reserved.GetMaxNb();i<NewSize;i++)
			Reserved.InsertPtr(new cNode(static_cast<cNodes*>(this),i,MaxAttr));
	}
	RCursor<cNode> ptr(Reserved);
	ptr.Start();
	while(ptr()->Reserved)
		ptr.Next();
	ptr()->Reserved=true;
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
	node->Clear();
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::InsertNode(cNode* to,cNode* node)
{
	RTree<cNodes,cNode,false>::InsertNode(to,node);
	if(to)
		to->PostInsert(node);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteNode(cNode* node)
{
	cNode* from(node->Parent);
	RTree<cNodes,cNode,false>::DeleteNode(node,true);
	if(from)
		from->PostDelete(node);
	ReleaseNode(node);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteNodes(cNode* from)
{
	if(!from->NbSubNodes)
		return;

	RContainer<cNode,false,false> Del(from->NbSubNodes);
	RCursor<cNode> Cur(from->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Del.InsertPtr(Cur());
	Cur.Set(Del);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		cNode* from(Cur()->GetParent());
		DeleteNode(Cur());
		if(from)
			from->PostDelete(Cur());
		ReleaseNode(Cur());
	}
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::MoveNode(cNode* to,cNode* node)
{
	if(to==node->Parent)
		return;
	cNode* from(node->Parent);
	RTree<cNodes,cNode,false>::MoveNode(to,node);
	if(to)
		to->PostInsert(node);
	if(from)
		from->PostDelete(node);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RNodesGA<cNode,cObj,cNodes>::CopyExceptBranch(const cNodes* from,const cNode* excluded,RNumContainer<size_t,true>* objs,bool copyobjs)
{
	cNode* Ret(0);
	cNode* CurNode;

	// Copy first the top nodes
	RCursor<cNode> Cur(from->GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		CurNode=Cur();
		if(CurNode==excluded)
			Ret=static_cast<cNode*>(0);
		else
		{
			// Insert a new top node
			cNode* New=ReserveNode();
			InsertNode(0,New);

			// Copy the attributes, the child nodes and the objects from CurNode
			cNode* Find=New->CopyExceptBranch(CurNode,excluded,objs,copyobjs);
			if(Find)
				Ret=Find;

			// If the node 'Node' has no child nodes and no objects attached -> Remove it.
			if((!New->GetNbObjs())&&(!New->GetNbNodes()))
			{
				// If the node to remove is the crossing node -> The current node becomes the crossing node
				if(Ret==New)
					Ret=static_cast<cNode*>(0);
				DeleteNode(New);   // Remove it from the tree
				ReleaseNode(New);  // Release it.
			}
		}
	}
	return(Ret);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::InsertObj(cNode* to,cObj* obj)
{
	size_t j;

	if(to->NbSubObjects)
	{
		j=to->SubObjects;
		RCursor<cNode> Cur(GetNodes());
		for(Cur.Start();!Cur.End();Cur.Next())
			if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=cNoRef))
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

	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=cNoRef;
	ObjsAss.DeletePtr(obj);
	ObjsNoAss.InsertPtr(obj);
	if(!(--(from->NbSubObjects)))
		from->SubObjects=cNoRef;
	from->PostDelete(obj);
	RCursor<cNode> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=cNoRef))
			Cur()->SubObjects--;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteObj(const cObj* obj)
{
	size_t i,j;
	cNode* from;

	i=ObjectsAss[obj->GetId()];
	if(i==cNoRef) return;
	from=(*this)[i];
	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=cNoRef;
	ObjsAss.DeletePtr(obj);
	ObjsNoAss.InsertPtr(obj);
	if(!(--(from->NbSubObjects)))
		from->SubObjects=cNoRef;
	from->PostDelete(obj);
	RCursor<cNode> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=cNoRef))
			Cur()->SubObjects--;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteObjs(cNode* from)
{
	size_t j;

	if(!(from->NbSubObjects)) return;
	RCursor<cObj> o(GetObjs(*from));
	for(o.Start();!o.End();o.Next())
	{
		// No need to increment o, because delete moves the everything
		ObjectsAss[o()->GetId()]=cNoRef;
		ObjsNoAss.InsertPtr(o());
		ObjsAss.DeletePtr(o());
		if(!(--from->NbSubObjects))
			from->SubObjects=cNoRef;
		from->PostDelete(o());
	}
	j=from->SubObjects;
	RCursor<cNode> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=cNoRef))
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
	RCursor<cObj> RNodesGA<cNode,cObj,cNodes>::GetObjs(const cNode& node) const
{
	if(!node.NbSubObjects)
		return(RCursor<cObj>(ObjsAss,0,0));
	return(RCursor<cObj>(ObjsAss,node.SubObjects,node.SubObjects+node.NbSubObjects-1));
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodesGA<cNode,cObj,cNodes>::VerifyNodes(bool complete)
{
	size_t nbobjs=0;
	RCursor<cNode> Cur(GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Parent)
		{
			std::cerr<<"Top nodes cannot have a parent"<<std::endl;
			return(false);
		}
		if(!Cur()->Verify(nbobjs))
			return(false);
	}
	if(complete&&(nbobjs!=Objs.GetNb()))
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
	void RNodesGA<cNode,cObj,cNodes>::CopyTree(const cNodes& nodes)
{
	ClearNodes();
	RCursor<cNode> Cur(nodes.GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Create a top node
		cNode* New(ReserveNode());
		InsertNode(0,New);

		// Copy the attributes, all the child nodes and objects of Cur() in Top.
		New->CopyExceptBranch(Cur());
	}
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::BuildFile(const RString& name)
{
	RTextFile File(name);
	File.Open(RIO::Create);
	RCursor<cNode> Cur(GetTopNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->PrintNode(File,0);
}

//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>::~RNodesGA(void)
{
	delete[] ObjectsAss;
}
