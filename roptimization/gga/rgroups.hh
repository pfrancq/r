/*

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	Copyright 2001-2005 by the Université Libre de Bruxelles.

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
// class RGroups<cGroup,cObj,cGroupData,cGroups>
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>::RGroups(RCursor<cObj>* objs,const unsigned max)
		: RContainer<cGroup,true,false>(max,max<20?20:max/2), Used(max,max<20?20:max/2),
		  GroupData(0), Objs(objs), ObjsAss(objs->GetNb()), ObjsNoAss(objs->GetNb()),
		  OrdObjectsAss(0), NewUsedId(0)
{
	ObjectsAss = new unsigned int[Objs->GetNb()];
	memset(ObjectsAss,0xFF,Objs->GetNb()*sizeof(unsigned int));

	// Init of the arrays needed.
	OrdObjectsAss=new unsigned int[Objs->GetNb()];
	NewUsedId=new unsigned int[this->GetMaxNb()];

	// Suppose no object is assigned
	for(Objs->Start();!Objs->End();Objs->Next())
		ObjsNoAss.InsertPtr((*Objs)());
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::Init(cGroupData* data)
{
	unsigned int i;

	// Create the groups
	GroupData=data;
	for(i=0;i<this->GetMaxNb();i++)
		InsertPtr(new cGroup(static_cast<cGroups*>(this),i,GroupData));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ClearGroups(void)
{
	RCursor<cGroup> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Used.Clear();
	ObjsAss.Clear();
	ObjsNoAss.Clear();
	for(Objs->Start();!Objs->End();Objs->Next())
		ObjsNoAss.InsertPtr((*Objs)());
	memset(ObjectsAss,0xFF,Objs->GetNb()*sizeof(unsigned int));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroupData,cGroups>::ReserveGroup(void)
{
	unsigned int i,NewSize;
	unsigned int* n;

	if(Used.GetNb()+1>this->GetMaxNb())
	{
		NewSize=this->GetMaxNb()+this->GetIncNb();

		// Recreate a new NewUsedId array with the new size
		n=new unsigned int[NewSize];
		memcpy(n,NewUsedId,sizeof(unsigned int)*this->GetMaxNb());
		delete[] NewUsedId;
		NewUsedId=n;

		// Create New groups
		for(i=this->GetMaxNb();i<NewSize;i++)
			InsertPtr(new cGroup(static_cast<cGroups*>(this),i,GroupData));
	}
	RCursor<cGroup> ptr(*this);
	ptr.Start();
	while(ptr()->Reserved)
		ptr.Next();
	ptr()->Reserved=true;
	Used.InsertPtr(ptr());
	return(ptr());
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ReleaseGroup(cGroup* grp)
{
	if(!grp->Reserved)
		return;
	DeleteObjs(grp);
	Used.DeletePtr(grp);
	grp->Clear();
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::InsertObj(cGroup* to,const cObj* obj)
{
	unsigned int tmp,j;

	if(to->NbSubObjects)
	{
		tmp=to->SubObjects+to->NbSubObjects;
		j=to->SubObjects;
		RCursor<cGroup> G(Used);
		for(G.Start();!G.End();G.Next())
			if((G()->SubObjects>j)&&(G()->SubObjects!=NoObject))
				G()->SubObjects++;
		ObjsAss.InsertPtrAt(obj,tmp,false);
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


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::DeleteObj(cGroup* from,const cObj* obj)
{
	unsigned int j;

	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=NoGroup;
	ObjsNoAss.InsertPtr(obj);
	ObjsAss.DeletePtr(obj);
	if(!(--from->NbSubObjects))
		from->SubObjects=NoObject;
	from->PostDelete(obj);
	RCursor<cGroup> G(Used);
	for(G.Start();!G.End();G.Next())
		if((G()->SubObjects>j)&&(G()->SubObjects!=NoObject))
			G()->SubObjects--;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::DeleteObjs(cGroup* from)
{
	unsigned int j,i,tmp;
	cObj* tmpObj;

	if(!(from->NbSubObjects)) return;
	tmp=from->NbSubObjects;
	j=from->SubObjects;
	RCursor<cObj> obj(ObjsAss);
	for(i=from->NbSubObjects+1,obj.GoTo(from->SubObjects);--i;)
	{
		 // No need to increment obj, because delete moves everything
		tmpObj=obj();
		ObjectsAss[tmpObj->GetId()]=NoGroup;
		ObjsNoAss.InsertPtr(tmpObj);
		ObjsAss.DeletePtr(tmpObj);
		if(!(--from->NbSubObjects))
			from->SubObjects=NoObject;
		from->PostDelete(tmpObj);
	}
	RCursor<cGroup> G(Used);
	for(G.Start();!G.End();G.Next())
		if((G()->SubObjects>j)&&(G()->SubObjects!=NoObject))
			G()->SubObjects-=tmp;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::Verify(void)
{
	unsigned int i,*list,nbobjs;
	char tmp[200];

	RCursor<cGroup> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Verify();
	for(i=Objs->GetNb()+1,list=ObjectsAss,nbobjs=0;--i;list++)
		if((*list)!=NoGroup)
			nbobjs++;
	if(ObjsAss.GetNb()+ObjsNoAss.GetNb()!=Objs->GetNb())
	{
		sprintf(tmp,"Problem with the number of objects: ObjsAss=%u and ObjsNoAss=%u",ObjsAss.GetNb(),ObjsNoAss.GetNb());
		throw RGAException(tmp,RGAException::eGAVerify);
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroupData,cGroups>::GetGroup(unsigned int id) const
{
	unsigned int idx=ObjectsAss[id];

	if(idx==NoObject) return(0);
	return((*const_cast<RGroups<cGroup,cObj,cGroupData,cGroups>*>(this))[idx]);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroupData,cGroups>::GetGroup(const cObj* obj) const
{
	unsigned int idx=ObjectsAss[obj->GetId()];

	if(idx==NoObject) return(0);
	return((*const_cast<RGroups<cGroup,cObj,cGroupData,cGroups>*>(this))[idx]);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RCursor<cObj> RGroups<cGroup,cObj,cGroupData,cGroups>::GetObjs(const cGroup& grp) const
{
	return(RCursor<cObj>(ObjsAss,grp.SubObjects,grp.SubObjects+grp.NbSubObjects));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>& RGroups<cGroup,cObj,cGroupData,cGroups>::operator=(const RGroups& grps)
{
	cGroup* ptr;

	ClearGroups();
	RCursor<cGroup> G(grps.Used);
	for(G.Start();!G.End();G.Next())
	{
		ptr=ReserveGroup();           // Reserve a new group
		ptr->Copy(G());               // Copy the necessary objects
		(*ptr)=(*G());                // Update internal information.
	}
	return(*this);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::ComputeOrd(void)
{
	unsigned int *oldo,*newo;
	unsigned int i,id,nbgrp;

	memset(NewUsedId,0xFF,sizeof(unsigned int)*this->GetMaxNb());
	for(i=ObjsAss.GetNb()+1,oldo=ObjectsAss,newo=OrdObjectsAss,nbgrp=0;--i;oldo++,newo++)
	{
		id=NewUsedId[*oldo];
		if(id==NoGroup)
			id=NewUsedId[*oldo]=(nbgrp++);
		(*newo)=id;
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	bool RGroups<cGroup,cObj,cGroupData,cGroups>::SameGroupment(const RGroups* grps) const
{
	unsigned int i,*ass,*cass;

	for(i=ObjsAss.GetNb()+1,ass=OrdObjectsAss,cass=grps->OrdObjectsAss;--i;ass++,cass++)
		if((*ass)!=(*cass))
			return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RCursor<cObj> RGroups<cGroup,cObj,cGroupData,cGroups>::GetObjsCursor(void) const
{
	return(RCursor<cObj>(ObjsAss));
}


//------------------------------------------------------------------------------
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
