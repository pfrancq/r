/*

	R Project Library

	RNodesGA.hh

	Class representing a collection of nodes - Inline implementation

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



//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>::RNodesGA(RCursor<cObj> objs,size_t maxnodes,size_t maxattr)
		: RTree<cNodes,cNode,false>(), Reserved(maxnodes), Nodes(maxnodes),
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
		Reserved.InsertPtr(new cNode(i,MaxAttr));
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::Clear(void)
{
	RTree<cNodes,cNode,false>::Clear();
	Nodes.Clear();
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

	if(Nodes.GetNb()+10>Reserved.GetMaxNb())
	{
		NewSize=Reserved.GetMaxNb()+Reserved.GetIncNb();
		if(NewSize<Nodes.GetNb()+10)
			NewSize=Nodes.GetNb()+10;

		// Create New nodes
		for(i=Reserved.GetMaxNb();i<NewSize;i++)
			Reserved.InsertPtr(new cNode(i,MaxAttr));
	}
	RCursor<cNode> ptr(Reserved);
	ptr.Start();
	while((ptr()->Reserved)&&(!ptr.End()))
		ptr.Next();
	if(ptr.End())
		mThrowRException("Too many unused reserved nodes");
	ptr()->Reserved=true;
	return(ptr());
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::ReleaseNode(cNode* node)
{
	if(!node->Reserved)
		return;
	if(node->GetNbNodes())
		mThrowRException("Cannot release a node with child nodes");
	DeleteObjs(node);
	if(node->Parent)
		DeleteNode(node);
	node->Clear();
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::InsertNode(cNode* to,cNode* node)
{
	Nodes.InsertPtr(node);
	node->Owner=static_cast<cNodes*>(this);
	RTree<cNodes,cNode,false>::InsertNode(to,node);
	if(to)
		to->PostInsert(node);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteNode(cNode* node)
{
	mAssert(node);

	// Delete the child nodes
	RNodesGA<cNode,cObj,cNodes>::DeleteNodes(node);

	// Delete the node from RNodesGA and RTree
	Nodes.DeletePtr(node);
	cNode* from(node->Parent);
	RTree<cNodes,cNode,false>::DeleteNode(node,false);

	// Launch a Post-Delete
	if(from)
		from->PostDelete(node);

	// Release the nodes and its objects
	ReleaseNode(node);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteNodes(cNode* from)
{
	if(!from->NbSubNodes)
		return;

	cNode* Node(from->GetFirst());
	while(Node)
	{
		cNode* Next(Node->Next);
		RNodesGA<cNode,cObj,cNodes>::DeleteNode(Node);
		Node=Next;
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
	if(from)
		from->PostDelete(node);
	if(to)
		to->PostInsert(node);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RNodesGA<cNode,cObj,cNodes>::CopyExceptBranch(const cNodes* from,const cNode* excluded,RNumContainer<size_t,true>* objs,bool copyobjs)
{
	cNode* Ret(0);

	// Copy first the top nodes
	RNodeCursor<cNodes,cNode> Cur(*from);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()==excluded)
			Ret=static_cast<cNode*>(0);  // The node to exclude is a top node
		else
		{
			// Insert a new top node
			cNode* New=ReserveNode();
			RNodesGA<cNode,cObj,cNodes>::InsertNode(0,New);

			// Copy the attributes, the child nodes and the objects from CurNode
			cNode* Find=CopyExceptBranch(New,Cur(),excluded,objs,copyobjs);
			if(Find)
				Ret=Find;

			// If the node 'New' has no child nodes and no objects attached -> Remove it.
			if((!New->GetNbObjs())&&(!New->GetNbNodes()))
			{
				// If the node to remove is the crossing node -> The node to exclude is a top node
				if(Ret==New)
					Ret=static_cast<cNode*>(0);
				DeleteNode(New);   // Remove it from the tree
			}
		}
	}
	return(Ret);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RNodesGA<cNode,cObj,cNodes>::CopyExceptBranch(cNode* to,const cNode* from,const cNode* excluded,RNumContainer<size_t,true>* objs,bool copyobjs)
{
	// Verify that the nodes are from different owners
	mReturnValIfFail(to->Owner!=from->Owner,0);

	cNode* Ret;

	// Copy information
	to->CopyInfos(*from);

	// Copy first the child nodes of from
	RNodeCursor<cNodes,cNode> Cur(from);
	for(Cur.Start(),Ret=0;!Cur.End();Cur.Next())
	{
		// Verify if the node to copy is not the one to exclude
		// -> Remember it and pass to the next node
		if(Cur()==excluded)
			Ret=to;
		else
		{
			// If the current node has at least one object attached and no child nodes
			// -> do not copied it
			/*bool CopyObjs=(!Cur()->HasSomeObjects(objs));
			if((!CopyObjs)&&(!Cur()->NbSubNodes))
				continue;*/

			// Insert a new child node
			cNode* New(ReserveNode());
			InsertNode(to,New);

			// Copy the attributes, the child nodes and the objects from Cur())
			cNode* Find=CopyExceptBranch(New,Cur(),excluded,objs,copyobjs);
			if(Find)
				Ret=Find;

			// If the node 'Node' has no child nodes and no objects attached -> Remove it.
			if((!New->GetNbObjs())&&(!New->GetNbNodes()))
			{
				// If the node to remove is the crossing node -> The current node becomes the crossing node
				if(Ret==New)
					Ret=to;
				DeleteNode(New);   // Remove it from the tree
			}
		}
	}

	// Copy the objects of from that are not in objs
	if(copyobjs)
	{
		RCursor<cObj> Objs(from->GetObjs());
		for(Objs.Start();!Objs.End();Objs.Next())
		{
			if((!objs)||(!objs->IsIn(Objs()->GetId())))
				InsertObj(to,Objs());
		}
	}

	return(Ret);
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::InsertObj(cNode* to,cObj* obj)
{
	size_t j;
	size_t* Ass(&ObjectsAss[obj->GetId()]);

	if((*Ass)!=cNoRef)
		mThrowRException("Cannot insert an object twice ina tree");

	if(to->NbSubObjects)
	{
		j=to->SubObjects;
		RCursor<cNode> Cur(Nodes);
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
	(*Ass)=to->Id;
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
	RCursor<cNode> Cur(Nodes);
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
	RCursor<cNode> Cur(Nodes);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>j)&&(Cur()->SubObjects!=cNoRef))
			Cur()->SubObjects--;
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::DeleteObjs(cNode* from)
{
	if(!(from->NbSubObjects)) return;
	RCursor<cObj> o(from->GetObjs());
	for(o.Start();!o.End();o.Next())
	{
		// No need to increment o, because delete moves the everything
		ObjectsAss[o()->GetId()]=cNoRef;
		ObjsNoAss.InsertPtr(o());
		ObjsAss.DeletePtr(o());
		from->PostDelete(o());
	}
	RCursor<cNode> Cur(Nodes);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((Cur()->SubObjects>from->SubObjects)&&(Cur()->SubObjects!=cNoRef))
			Cur()->SubObjects-=from->NbSubObjects;
	from->SubObjects=cNoRef;
	from->NbSubObjects=0;
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
	RCursor<cObj> RNodesGA<cNode,cObj,cNodes>::GetObjs(const cNode* node) const
{
	if((!node)||(!node->NbSubObjects))
		return(RCursor<cObj>());
	return(RCursor<cObj>(ObjsAss,node->SubObjects,node->SubObjects+node->NbSubObjects-1));
}


//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodesGA<cNode,cObj,cNodes>::VerifyNodes(bool complete)
{
	size_t nbobjs=0;
	RCursor<cNode> Cur(Nodes);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(!Cur()->Verify(nbobjs))
			return(false);
	}
	if(nbobjs>Objs.GetNb())
	{
		size_t Prob(nbobjs-Objs.GetNb());
		std::cerr<<Prob<<" object";
		if(Prob==1)
			std::cerr<<" is assigned twice"<<std::endl;
		else
			std::cerr<<"s are assigned twice"<<std::endl;
		return(false);
	}
	if(complete&&(nbobjs!=Objs.GetNb()))
	{
		bool First(true);
		std::cerr<<"All objects are not attached (";
		for(size_t i=0;i<Objs.GetNb();i++)
		{
			size_t Id(ObjectsAss[i]);
			if(Id!=cNoRef)
				continue;
			if(First)
				First=false;
			else
				std::cerr<<",";
			std::cerr<<i;
		}
		std::cerr<<")"<<std::endl;
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
	Clear();
	RNodeCursor<cNodes,cNode> Cur(nodes);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Create a top node
		cNode* New(ReserveNode());
		InsertNode(0,New);

		// Copy the attributes, all the child nodes and objects of Cur() in New.
		CopyExceptBranch(New,Cur());
	}
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodesGA<cNode,cObj,cNodes>::BuildFile(const RString& name)
{
	RTextFile File(name);
	File.Open(RIO::Create);
	RNodeCursor<cNodes,cNode> Cur(*static_cast<cNodes*>(this));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->PrintNode(File,0);
}

//---------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodesGA<cNode,cObj,cNodes>::~RNodesGA(void)
{
	delete[] ObjectsAss;
}
