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
		: RStd::RContainer<cGroup,unsigned int,true,false>(max), Used(max),
		  GroupData(0), Objs(objs), ObjsAss(objs->GetNb()), ObjsNoAss(objs->GetNb())
{
	ObjectsAss = new unsigned int[Objs->GetNb()];
	memset(ObjectsAss,0xFF,Objs->GetNb()*sizeof(unsigned int));
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::Init(cGroupData* data)
{
	unsigned int i;

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
	unsigned int i,j;

	if(Used.NbPtr+1>MaxPtr)
	{
		j=MaxPtr+IncPtr;
		for(i=MaxPtr;i<j;i++)
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
	void RGroups<cGroup,cObj,cGroupData,cGroups>::InsertObj(cGroup* to,const cObj* obj)
{
	unsigned int tmp,i,j;
	cGroup** G;
//	cObj** o;

	if(to->NbSubObjects)
	{
//		for(i=0,o=&ObjsAss.Tab[to->SubObjects];(i<to->NbSubObjects)&&((*o)<obj);o++,i++);
//		tmp=to->SubObjects+i;
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
	cObj** o;

	for(i=0,o=&ObjsAss.Tab[from->SubObjects];(*o)!=obj;o++,i++);
	j=from->SubObjects;
	for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
		if(((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects--;
	if(!(--(from->NbSubObjects)))
		from->SubObjects=NoObject;
	ObjectsAss[obj->GetId()]=NoGroup;
	ObjsNoAss.InsertPtr(obj);
	ObjsAss.DeletePtr(obj);
	from->PostDelete(obj);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	void RGroups<cGroup,cObj,cGroupData,cGroups>::DeleteObjs(cGroup* from)
{
	unsigned int j,i,tmp;
	cGroup** G;
	cObj** obj;

	if(!(from->NbSubObjects)) return;
	tmp=from->NbSubObjects;
	for(i=from->NbSubObjects+1,obj=&ObjsAss.Tab[from->SubObjects];--i;)
	{
		 // No need to increment obj, because delete moves eveything
		ObjectsAss[(*obj)->GetId()]=NoGroup;
		ObjsNoAss.InsertPtr(*obj);
		ObjsAss.DeletePtr(*obj);
		from->NbSubObjects--;
		from->PostDelete(*obj);
	}
	j=from->SubObjects;
	for(i=NbPtr+1,G=Tab;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects-=tmp;
	from->SubObjects=NoObject;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	bool RGroups<cGroup,cObj,cGroupData,cGroups>::Verify(void)
{
	unsigned int i,*list,nbobjs;
	cGroup** G;

	for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
		if(!(*G)->Verify())
			return(false);
	for(i=Objs->GetNb()+1,list=ObjectsAss,nbobjs=0;--i;list++)
		if((*list)!=NoGroup)
			nbobjs++;
	if(ObjsAss.NbPtr+ObjsNoAss.NbPtr!=Objs->GetNb())
	{
		cout<<"Problem with the number of objects: ObjsAss="<<ObjsAss.NbPtr<<" and ObjsNoAss="<<ObjsNoAss.NbPtr<<endl;
		return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>& RGroups<cGroup,cObj,cGroupData,cGroups>::operator=(const RGroups& grps)
{
	unsigned int i;
	cGroup** G;

	ClearGroups();
	for(i=grps.Used.NbPtr+1,G=grps.Used.Tab;--i;G++)
		ReserveGroup()->Insert(Objs,*G);
	return(*this);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData,class cGroups>
	RGroups<cGroup,cObj,cGroupData,cGroups>::~RGroups(void)
{
	if(ObjectsAss)
		delete[] ObjectsAss;
}
