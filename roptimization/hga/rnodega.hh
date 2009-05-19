/*

	R Project Library

	RNodeGA.hh

	GA Node - Inline implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
// class RNodeGA<cNode,cObj,cNodes>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodeGA<cNode,cObj,cNodes>::RNodeGA(cNodes* owner,size_t id,size_t max)
		: RNode<cNodes,cNode,false>(owner), Id(id), Attr(max)
{
	NbSubObjects= 0;
	SubObjects = cNoRef;
	Reserved=false;
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::SetAttr(const RAttrList& attr)
{
	Attr=attr;
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::Verify(size_t &nbobjs)
{
	if(!RNodeGA<cNode,cObj,cNodes>::VerifyNode(Id))
		return(false);

	// Increase the number of attached objects.
	nbobjs+=NbSubObjects;

	// Each node must have at least one object or one child node
	if((!GetNbNodes())&&(!NbSubObjects))
	{
		cerr<<"Node "<<Id<<": No child node and no objects attached."<<endl;
		return(false);
	}

	// Verify objects attached.
// 	if(SubObjects!=cNoRef)
// 	{
// 		// The number of objects attached can't be null.
// 		if(!NbSubObjects)
// 			Ok=false;
//
// 		// The index of the first objects attached can not exceed the total number of objects.
// 		if(SubObjects>NbObjects)
// 			Ok=false;
//
// 		// The index of the last objects attached can npt exceed the total number of objects.
// 		if(SubObjects+NbSubObjects>NbObjects+1)
// 			Ok=false;
// 	}
// 	else
// 	{
// 		// The number of objects attached must be null.
// 		if(NbSubObjects)
// 			Ok=false;
// 	}
//
// 	// Verify coherence with the subnodes.
// 	for(i=NbSubNodes,Sub=&(Owner->NodesList[SubNodes]);i--;Sub++)
// 	{
// 		ptr = Nodes[*Sub];
//
// 		// The identificator of the parent must be the one of the current node.
// 		if(Id!=ptr->ParentNode)
// 			Ok=false;
//
// 		// The number of attributes of the subnodes must be greather than the current node.
// 		if(Attr.NbAttr>=ptr->Attr.NbAttr)
// 			Ok=false;
//
// 		// The subnode must also be verified.
// 		if(!ptr->Verify(countobj))
// 			Ok=false;
//
// 		// The attributes of the different subnodes can't be the same.
// 		for(k=i,Sub2=&(Owner->NodesList[SubNodes+NbSubNodes-i]);k--;Sub2++)
// 			if(Nodes[*Sub2]->Attr.IsSame(ptr->Attr))
// 				Ok=false;
// 	}
//
// 	// Verify coherence with the objects.
// 	for(j=NbSubObjects+1,obj=&(Owner->ObjectsList[SubObjects]);--j;obj++)
// 		if(Owner->ObjectsAss[*obj]!=Id)
// 			Ok=false;
//
// 	// If a parent node, the identifier of node must be in the list.
// 	if(ParentNode!=NoNode)
// 	{
// 		ptr = Nodes[ParentNode];
// 		i=ptr->NbSubNodes;
// 		Sub=&(Owner->NodesList[ptr->SubNodes]);
// 		while(i&&((*(Sub++))!=Id)) i--;
// 		if(!i)
// 			Ok=false;
// 	}

	// Verify child nodes
	RCursor<cNode> Sub(GetNodes());
	for(Sub.Start();!Sub.End();Sub.Next())
		if(!Sub()->Verify(nbobjs))
			return(false);

	// return the value of the verification.
	return(true);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::CanAttach(const RAttrList& attr) const
{
	if(Id==cNoRef)
		return(false);
	return(Attr.IsParent(attr));
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::IsSame(const RAttrList& attr) const
{
	if(Id==cNoRef)
		return(false);
	return(Attr.IsSame(attr));
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	size_t RNodeGA<cNode,cObj,cNodes>::GetNbCommon(const RAttrList& attr) const
{
	return(Attr.GetNbCommon(attr));
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::IsSameObjs(const cNode* Node) const
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	cNode* RNodeGA<cNode,cObj,cNodes>::CopyExceptBranch(const cNode* from,const cNode* excluded,RNumContainer<size_t,true>* objs,bool copyobjs)
{
	cNode* Ret;
	cNode* CurNode;

	// Verify that the nodes are from different owners
	RReturnValIfFail(Tree!=from->Tree,0);

	// Update other information
	CopyInfos(*from);

	// Copy first the child nodes of from
	RCursor<cNode> Cur(from->GetNodes());
	for(Cur.Start(),Ret=0;!Cur.End();Cur.Next())
	{
		CurNode=Cur();

		// Verify if the node to copy is not the one to exclude
		// -> Remember it and pass to the next node
		if(CurNode==excluded)
			Ret=static_cast<cNode*>(this);
		else
		{
			// If the current node has at least one object attached and no child nodes
			// -> do not copied it
			bool CopyObjs=(!CurNode->HasSomeObjects(objs));
			if((!CopyObjs)&&(!CurNode->NbSubNodes))
				continue;

			// Insert a new child node
			cNode* New=Tree->ReserveNode();
			InsertNode(New);

			// Copy the attributes, the child nodes and the objects from CurNode
			cNode* Find=New->CopyExceptBranch(CurNode,excluded,objs,CopyObjs);
			if(Find)
				Ret=Find;

			// If the node 'Node' has no child nodes and no objects attached -> Remove it.
			if((!New->GetNbObjs())&&(!New->GetNbNodes()))
			{
				// If the node to remove is the crossing node -> The current node becomes the crossing node
				if(Ret==New)
					Ret=static_cast<cNode*>(this);
				Tree->DeleteNode(New);   // Remove it from the tree
				Tree->ReleaseNode(New);  // Release it.
			}
		}
	}

	// Copy the objects of from
	if(copyobjs)
	{
		RCursor<cObj> Objs(from->GetObjs());
		for(Objs.Start();!Objs.End();Objs.Next())
			Insert(Objs());
	}

	return(Ret);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::PostInsert(const cNode*)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::PostDelete(const cNode*)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::PostInsert(const cObj*)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::PostDelete(const cObj*)
{
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::Clear(void)
{
	RNode<cNodes,cNode,false>::Clear();
	Reserved=false;
	SubObjects=cNoRef;
	NbSubObjects=0;
	Attr.Clear();
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::ConstructAllObjects(cObj** objs,size_t& nbobjs)
{
	size_t i;
	cObj** tmpObjs;
	size_t id;

	// Goes in each child nodes to find their objects
	RCursor<cNode> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ConstructAllObjects(objs,nbobjs);

	// Add the objects of the current node
	RCursor<cObj> Objs(GetObjs());
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		// Id of the current object
		id=Objs()->GetId();

		// Find where this object must goes -> eventually create a empty place
		// in objs
		for(i=nbobjs+1,tmpObjs=objs;(--i)&&((*tmpObjs)->GetId()<id);tmpObjs++) ;
		if(i) memmove(tmpObjs+1,tmpObjs,sizeof(size_t)*i);

		// Put the object in objs
		nbobjs++;
		i++;
		(*tmpObjs) = Objs();
	}
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::GetAllObjects(RNumContainer<size_t,true>& objs) const
{
	// Goes in each child nodes to find their objects
	RCursor<cNode> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->GetAllObjects(objs);

	// Add the objects of the current node
	RCursor<cObj> Objs(GetObjs());
	for(Objs.Start();!Objs.End();Objs.Next())
		objs.Insert(Objs()->GetId());
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::CopyInfos(const cNode& node)
{
	Attr=node.Attr;
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	size_t* RNodeGA<cNode,cObj,cNodes>::GetNodesId(void) const
{
	size_t* tmp;
	size_t* tmp2;

	tmp2=tmp=new size_t[GetNbNodes()+1];
	RCursor<cNode> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next(),tmp2++)
		(*tmp2)=Cur()->GetId();
	(*tmp2)=cNoRef;
	return(tmp);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	size_t* RNodeGA<cNode,cObj,cNodes>::GetObjectsId(void) const
{
	size_t* tmp;
	size_t* tmp2;

	tmp2=tmp=new size_t[NbSubObjects+1];
	RCursor<cObj> Objs(GetObjs());
	for(Objs.Start();!Objs.End();Objs.Next(),tmp2++)
		(*tmp2)=Objs()->GetId();
	(*tmp2)=cNoRef;
	return(tmp);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::HasSomeObjects(RNumContainer<size_t,true>* objs) const
{
	if(!objs)
		return(false);
	RCursor<cObj> Objs(GetObjs());
	for(Objs.Start();!Objs.End();Objs.Next())
		if(objs->IsIn(Objs()->GetId()))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::PrintNode(RTextFile& file,int depth)
{
	for(int i=0;i<depth;i++)
		file<<"\t";
	file<<"Node"<<GetId()<<"[";
	RAttrList List(GetAttr());
	for(List.Start();!List.End();List.Next())
	{
		file<<List();
	}
	file<<"]"<<endl;
	RCursor<cNode> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->PrintNode(file,depth+1);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodeGA<cNode,cObj,cNodes>::~RNodeGA(void)
{
}
