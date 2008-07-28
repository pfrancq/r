/*

	R Project Library

	RNodeGA.hh

	GA Node - Inline implementation.

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
// class RNodeGA<cNode,cObj,cNodes>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodeGA<cNode,cObj,cNodes>::RNodeGA(const cNode* node)
		: Id(node->Id), Owner(node->Owner), Attr(node->Attr)
{
	NbSubNodes=node->NbSubNodes;
	NbSubObjects=node->NbSubObjects;
	Parent=node->Parent;
	SubNodes = node->SubNodes;
	SubObjects = node->SubObjects;
	Reserved=node->Reserved;
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodeGA<cNode,cObj,cNodes>::RNodeGA(cNodes* owner,size_t id,size_t max)
		: Id(id), Owner(owner), Attr(max)
{
	NbSubNodes=NbSubObjects= 0;
	Parent=0;
	SubNodes = NoNode;
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
// 	cNode *ptr;
// 	size_t i,k,*Sub,*Sub2;
// 	size_t j,*obj;
// 	bool Ok;

	// Increase the number of attached objects.
	nbobjs+=NbSubObjects;

	// Each node must have a parent.
	if(!Owner)
	{
		cerr<<"Node n"<<Id<<": No Owner for this node."<<endl;
		return(false);
	}

	// Each node must have at least one object or one subnode
	if((!NbSubNodes)&&(!NbSubObjects))
	{
		cerr<<"Node n"<<Id<<": No subnode and no objects attched."<<endl;
		return(false);
	}

	// Verify information about subnodes
	if(NbSubNodes)
	{
		// There number of subnodes can't be null.
		if(SubNodes==NoNode)
		{
			cerr<<"Node n"<<Id<<": SubNodes==NoNode"<<endl;
			return(false);
		}

		// The index of the first subnode can not exceed the total number of nodes allocated in the parent.
		if(SubNodes>Owner->Used.GetNb())
		{
			cerr<<"Node n"<<Id<<": SubNodes>Owner->Used.NbPtr"<<endl;
			return(false);
		}

		// The index of the lasst subnode can not exceed the total number of nodes allocated in the parent.
		if(SubNodes+NbSubNodes>Owner->Used.GetNb())
		{
			cerr<<"Node n"<<Id<<": SubNodes+NbSubNodes>Owner->Used.NbPtr"<<endl;
			return(false);
		}
	}
	else
	{
		// The number of subnodes must be null.
		if(SubNodes!=NoNode)
		{
			cerr<<"Node n"<<Id<<": SubNodes!=NoNode"<<endl;
			return(false);
		}
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
// 	// If a parent node, the identificator of node myust be in the list.
// 	if(ParentNode!=NoNode)
// 	{
// 		ptr = Nodes[ParentNode];
// 		i=ptr->NbSubNodes;
// 		Sub=&(Owner->NodesList[ptr->SubNodes]);
// 		while(i&&((*(Sub++))!=Id)) i--;
// 		if(!i)
// 			Ok=false;
// 	}

	// Verify subnodes
	RCursor<cNode> Sub(Owner->GetNodes(static_cast<const cNode*>(this)));
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
	if(Id==NoNode)
		return(false);
	return(Attr.IsParent(attr));
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::IsSame(const RAttrList& attr) const
{
	if(Id==NoNode)
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
	cNode* RNodeGA<cNode,cObj,cNodes>::Copy(const cNode* from,const cNode* excluded,RVectorInt<size_t,true>* objs,bool copyobjs)
{
	cNode* Ret;

	// Verify that the nodes are from different owners
	RReturnValIfFail(Owner!=from->Owner,0);

	// Copy first the subnodes of from
	RCursor<cNode> Cur(from->Owner->GetNodes(*from));
	for(Cur.Start(),Ret=0;!Cur.End();Cur.Next())
	{
		// Verify if the node to copy is not the one to exclude
		// -> Remember it and pass to the next node
		if(Cur()==excluded)
			Ret=static_cast<cNode*>(this);
		else
		{
			// If the current node has at least one object attached and no subnodes
			// -> do not copied it
			bool CopyObjs=(!Cur()->HasSomeObjects(objs));
			if((!CopyObjs)&&(!Cur()->NbSubNodes))
				continue;

			// Reserve a node
			cNode* New=Owner->ReserveNode();
			cNode* Find=New->Copy(Cur(),excluded,objs,CopyObjs);
			if(Find)
				Ret=Find;
			if(New->GetNbObjs()||New->GetNbNodes())
				Insert(New);
			else
			{
				if(Ret==New)
					Ret=static_cast<cNode*>(this);
				Owner->ReleaseNode(New);
			}
		}
	}

	// Copy the objects of from
	if(copyobjs)
	{
		RCursor<cObj> Objs(from->Owner->GetObjs(*from));
		for(Objs.Start();!Objs.End();Objs.Next())
			Insert(Objs());
	}

	// Update other information
	(*this)=(*from);
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
	Reserved=false;
	Parent=0;
	SubObjects=cNoRef;
	NbSubObjects=0;
	SubNodes=NoNode;
	NbSubNodes=0;
	Attr.Clear();
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::ConstructAllObjects(cObj** objs,size_t& nbobjs)
{
	size_t i;
	cObj** tmpObjs;
	size_t id;

	// Goes in each subnodes to find their subobjects
	RCursor<cNode> Cur(Owner->GetNodes(*static_cast<cNode*>(this)));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ConstructAllObjects(objs,nbobjs);

	// Add the subobjects of the current node
	RCursor<cObj> Objs(Owner->GetObjs(*static_cast<cNode*>(this)));
	for(Objs.Start();!Objs.End();Objs.Next())
	{
		// Id of the current object
		id=Objs()->GetId();

		// Find where this object must goes -> eventually create a empty place
		// in objs
		for(i=nbobjs+1,tmpObjs=objs;(--i)&&((*tmpObjs)->GetId()<id);tmpObjs++);
		if(i) memmove(tmpObjs+1,tmpObjs,sizeof(size_t)*i);

		// Put the object in objs
		nbobjs++;
		i++;
		(*tmpObjs) = Objs();
	}
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	void RNodeGA<cNode,cObj,cNodes>::GetAllObjects(RVectorInt<size_t,true>& objs) const
{
	// Goes in each subnodes to find their objects
	RCursor<cNode> Cur(Owner->GetNodes(*static_cast<const cNode*>(this)));
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->GetAllObjects(objs);

	// Add the objects of the current node
	RCursor<cObj> Objs(Owner->GetObjs(*static_cast<const cNode*>(this)));
	for(Objs.Start();!Objs.End();Objs.Next())
		objs.Insert(Objs()->GetId());
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodeGA<cNode,cObj,cNodes>& RNodeGA<cNode,cObj,cNodes>::operator=(const RNodeGA<cNode,cObj,cNodes>& node)
{
	Attr=node.Attr;
	return(*this);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	size_t* RNodeGA<cNode,cObj,cNodes>::GetNodesId(void) const
{
	size_t* tmp;
	size_t* tmp2;

	tmp2=tmp=new size_t[NbSubNodes+1];
	RCursor<cNode> Cur(Owner->GetNodes(this));
	for(Cur.Start();!Cur.End();Cur.Next(),tmp2++)
		(*tmp2)=Cur()->GetId();
	(*tmp2)=NoNode;
	return(tmp);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	size_t* RNodeGA<cNode,cObj,cNodes>::GetObjectsId(void) const
{
	size_t* tmp;
	size_t* tmp2;

	tmp2=tmp=new size_t[NbSubObjects+1];
	RCursor<cObj> Objs(Owner->GetObjs(this));
	for(Objs.Start();!Objs.End();Objs.Next(),tmp2++)
		(*tmp2)=Objs()->GetId();
	(*tmp2)=cNoRef;
	return(tmp);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	bool RNodeGA<cNode,cObj,cNodes>::HasSomeObjects(RVectorInt<size_t,true>* objs) const
{
	if(!objs)
		return(false);
	RCursor<cObj> Objs(Owner->GetObjs(static_cast<const cNode*>(this)));
	for(Objs.Start();!Objs.End();Objs.Next())
		if(objs->IsIn(Objs()->GetId()))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class cNode,class cObj,class cNodes>
	RNodeGA<cNode,cObj,cNodes>::~RNodeGA(void)
{
}
