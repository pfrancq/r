/*

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	(C) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
//
// class RGroups<cGroup,cObj,cGroupData,cGroups>
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>::RGroups(RStd::RCursor<cObj,unsigned int>* objs,const unsigned max) throw(bad_alloc)
		: RStd::RContainer<cGroup,unsigned int,true,false>(max,max<20?20:max/2), Used(max,max<20?20:max/2),
		  GroupData(0), Objs(objs), ObjsAss(objs->GetNb()), ObjsNoAss(objs->GetNb()),
		  OrdObjectsAss(0), NewUsedId(0)
{
	ObjectsAss = new unsigned int[Objs->GetNb()];
	memset(ObjectsAss,0xFF,Objs->GetNb()*sizeof(unsigned int));

	// Init of the arrays needed.
	OrdObjectsAss=new unsigned int[Objs->GetNb()];
	NewUsedId=new unsigned int[MaxPtr];

	// Suppose no object is assigned
	for(Objs->Start();!Objs->End();Objs->Next())
		ObjsNoAss.InsertPtr((*Objs)());
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::Init(cGroupData* data)
{
	unsigned int i;

	// Create the groups
	GroupData=data;
	for(i=0;i<MaxPtr;i++)
		InsertPtr(new cGroup(static_cast<cGroups*>(this),i,GroupData));
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ClearGroups(void)
{
	cGroup **G;
	unsigned int i;

	for(i=NbPtr+1,G=Tab;--i;G++)
		(*G)->Clear();
	Used.Clear();
	ObjsAss.Clear();
	ObjsNoAss.Clear();
	for(Objs->Start();!Objs->End();Objs->Next())
		ObjsNoAss.InsertPtr((*Objs)());
	memset(ObjectsAss,0xFF,Objs->GetNb()*sizeof(unsigned int));
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroupData,cGroups>::ReserveGroup(void)
{
	cGroup **ptr;
	unsigned int i,NewSize;
	unsigned int* n;

	if(Used.NbPtr+1>MaxPtr)
	{
		NewSize=MaxPtr+IncPtr;

		// Recreate a new NewUsedId array with the new size
		n=new unsigned int[NewSize];
		memcpy(n,NewUsedId,sizeof(unsigned int)*MaxPtr);
		delete[] NewUsedId;
		NewUsedId=n;

		// Create New groups
		for(i=MaxPtr;i<NewSize;i++)
			InsertPtr(new cGroup(static_cast<cGroups*>(this),i,GroupData));
	}
	ptr=Tab;
	while((*ptr)->Reserved)
		ptr++;
	(*ptr)->Reserved=true;
	Used.InsertPtr(*ptr);
	return((*ptr));
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ReleaseGroup(const unsigned int grp)
{
	cGroup *G=Tab[grp];

	DeleteObjs(G);
	Used.DeletePtr(G);
	G->Clear();
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ReleaseGroup(cGroup* grp)
{
	DeleteObjs(grp);
	Used.DeletePtr(grp);
	grp->Clear();
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::InsertObj(cGroup* to,const cObj* obj)
{
	unsigned int tmp,i,j;
	cGroup** G;

	if(to->NbSubObjects)
	{
		tmp=to->SubObjects+to->NbSubObjects;
		j=to->SubObjects;
		for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
			if(((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
				(*G)->SubObjects++;
		ObjsAss.InsertPtrAt(obj,tmp,false);
	}
	else
	{
		to->SubObjects=ObjsAss.NbPtr;
		ObjsAss.InsertPtrAt(obj,to->SubObjects,false);
	}
	to->NbSubObjects++;
	ObjectsAss[obj->GetId()]=to->Id;
	ObjsNoAss.DeletePtr(obj);
	to->PostInsert(obj);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::DeleteObj(cGroup* from,const cObj* obj)
{
	unsigned int j,i;
	cGroup** G;

	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=NoGroup;
	ObjsNoAss.InsertPtr(obj);
	ObjsAss.DeletePtr(obj);
	if(!(--from->NbSubObjects))
		from->SubObjects=NoObject;
	from->PostDelete(obj);
	for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects--;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::DeleteObjs(cGroup* from)
{
	unsigned int j,i,tmp;
	cGroup** G;
	cObj** obj;
	cObj* tmpObj;

	if(!(from->NbSubObjects)) return;
	tmp=from->NbSubObjects;
	j=from->SubObjects;
	for(i=from->NbSubObjects+1,obj=&ObjsAss.Tab[from->SubObjects];--i;)
	{
		 // No need to increment obj, because delete moves everything
		tmpObj=(*obj);
		ObjectsAss[tmpObj->GetId()]=NoGroup;
		ObjsNoAss.InsertPtr(tmpObj);
		ObjsAss.DeletePtr(tmpObj);
		if(!(--from->NbSubObjects))
			from->SubObjects=NoObject;
		from->PostDelete(tmpObj);
	}
	for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects-=tmp;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::Verify(void) throw(RGA::eGA)
{
	unsigned int i,*list,nbobjs;
	cGroup** G;
	char tmp[200];

	for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
		(*G)->Verify();
	for(i=Objs->GetNb()+1,list=ObjectsAss,nbobjs=0;--i;list++)
		if((*list)!=NoGroup)
			nbobjs++;
	if(ObjsAss.NbPtr+ObjsNoAss.NbPtr!=Objs->GetNb())
	{
		sprintf(tmp,"Problem with the number of objects: ObjsAss=%u and ObjsNoAss=%u",ObjsAss.NbPtr,ObjsNoAss.NbPtr);
		throw RGA::eGAVerify(tmp);
	}
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>& RGroups<cGroup,cObj,cGroupData,cGroups>::operator=(const RGroups& grps)
{
	unsigned int i;
	cGroup** G;
	cGroup* ptr;

	ClearGroups();
	for(i=grps.Used.NbPtr+1,G=grps.Used.Tab;--i;G++)
	{
		ptr=ReserveGroup();           // Reserve a new group
		ptr->Insert(Objs,*G);         // Insert the necessary objects
		(*ptr)=(**G);                 // Update internal information.
	}
	return(*this);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ComputeOrd(void)
{
	unsigned int *oldo,*newo;
	unsigned int i,id,nbgrp;

	memset(NewUsedId,0xFF,sizeof(unsigned int)*MaxPtr);
	for(i=ObjsAss.NbPtr+1,oldo=ObjectsAss,newo=OrdObjectsAss,nbgrp=0;--i;oldo++,newo++)
	{
		id=NewUsedId[*oldo];
		if(id==NoGroup)
			id=NewUsedId[*oldo]=(nbgrp++);
		(*newo)=id;
	}
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	bool RGroups<cGroup,cObj,cGroupData,cGroups>::SameGroupment(const RGroups* grps) const
{
	unsigned int i,*ass,*cass;

	for(i=ObjsAss.NbPtr+1,ass=OrdObjectsAss,cass=grps->OrdObjectsAss;--i;ass++,cass++)
		if((*ass)!=(*cass))
			return(false);
	return(true);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>::~RGroups(void)
{
	if(ObjectsAss)
		delete[] ObjectsAss;
	if(OrdObjectsAss)
		delete[] OrdObjectsAss;
	if(NewUsedId)
		delete[] NewUsedId;
}
