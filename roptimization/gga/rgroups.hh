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



//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroups<cGroup,cObj,cGroupData>::RGroups(const unsigned int nbobjs,const unsigned max) throw(bad_alloc)
		: RStd::RContainer<cGroup,unsigned int,true,false>(max+1,max/2),
		  Used(max+1,max/2), GroupData(0), NbObjs(nbobjs), Objs(0)
{
	ObjectsAss = new unsigned int[NbObjs];
	memset(ObjectsAss,0xFF,NbObjs*sizeof(unsigned int));
	ObjectsList = new unsigned int[NbObjs];
	memset(ObjectsList,0xFF,NbObjs*sizeof(unsigned int));
	NbObjectsList=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::Init(cObj** objs,cGroupData* data)
{
	unsigned int i;

	Objs=objs;
	GroupData=data;
	for(i=0;i<MaxPtr;i++)
		InsertPtr(new cGroup(static_cast<RGroups<cGroup,cObj,cGroupData>*>(this),i,GroupData));
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::ClearGroups(void)
{
	cGroup **G;
	unsigned int i;

	for(i=NbPtr+1,G=Tab;--i;G++)
		(*G)->Clear();
	Used.Clear();
	memset(ObjectsAss,0xFF,NbObjs*sizeof(unsigned int));
	memset(ObjectsList,0xFF,NbObjs*sizeof(unsigned int));
	NbObjectsList=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	cGroup* RGroups<cGroup,cObj,cGroupData>::ReserveGroup(void)
{
	cGroup **ptr;

	if(Used.NbPtr+1>MaxPtr)
	{
		InsertPtr(new cGroup(static_cast<RGroups<cGroup,cObj,cGroupData>*>(this),Used.NbPtr,GroupData));
	}
	ptr=Tab;
	while((*ptr)->Reserved)
		ptr++;
	(*ptr)->Reserved=true;
	Used.InsertPtr(*ptr);
	return((*ptr));
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::ReleaseGroup(const unsigned int grp)
{
	cGroup *G=Tab[grp];

	DeleteObjs(G);
	Used.DeletePtr(G);
	G->Clear();
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::InsertObj(cGroup* to,const cObj* obj)
{
	unsigned int *ptr,tmp,i,j;
	cGroup** G;

	if(to->NbSubObjects)
	{
		for(i=0,ptr=&ObjectsList[to->SubObjects];(i<to->NbSubObjects)&&((*ptr)<obj->GetId());ptr++,i++);
		tmp=to->SubObjects+i;
		if(to->SubObjects+i<NbObjs-1)
			memmove(ptr+1,ptr,(NbObjectsList-tmp)*sizeof(unsigned int));
		(*ptr)=obj->GetId();
		j=to->SubObjects;
		NbObjectsList++;
		for(i=NbPtr+1,G=Tab;--i;G++)
			if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
				(*G)->SubObjects++;
	}
	else
		ObjectsList[to->SubObjects=NbObjectsList++]=obj->GetId();
	to->NbSubObjects++;
	ObjectsAss[obj->GetId()]=to->Id;
	to->PostInsert(obj);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::DeleteObj(cGroup* from,const cObj* obj)
{
	unsigned int *ptr,j,i;
	cGroup** G;

	for(i=0,ptr=&ObjectsList[from->SubObjects];(*ptr)!=obj->GetId();ptr++,i++);
	memcpy(ptr,ptr+1,(NbObjectsList-from->SubObjects-i)*sizeof(unsigned int));
	j=from->SubObjects;
	for(i=NbPtr+1,G=Tab;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects--;
	if(!(--(from->NbSubObjects)))
		from->SubObjects=NoObject;
	NbObjectsList--;
	ObjectsAss[obj->GetId()]=NoGroup;
	from->PostDelete(obj);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::DeleteObjs(cGroup* from)
{
	unsigned int *ptr,j,i;
	cGroup** G;

	if(!(from->NbSubObjects)) return;
	for(i=from->NbSubObjects+1,ptr=&ObjectsList[from->SubObjects];--i;ptr++)
		ObjectsAss[*ptr]=NoGroup;
	j=from->SubObjects;
	ptr=&ObjectsList[j];
	memcpy(ptr,ptr+from->NbSubObjects,(NbObjectsList-j)*sizeof(unsigned int));
	for(i=NbPtr+1,G=Tab;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects-=from->NbSubObjects;
	NbObjectsList-=from->NbSubObjects;
	from->NbSubObjects=0;
	from->SubObjects=NoObject;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroups<cGroup,cObj,cGroupData>::MoveObjs(cGroup* to,cGroup* from)
{
	unsigned int nbt=to->NbSubObjects,nbf=from->NbSubObjects,nbtemp,nbtemp2;
	unsigned int *ptrf=&ObjectsList[from->SubObjects],*ptrt=&ObjectsList[to->SubObjects];
	unsigned int *ptr;
	cGroup** G;
	unsigned int i;

	RReturnIfFail(to->Owner==from->Owner);
	if(!nbf) return;
	for(nbtemp=from->NbSubObjects+1,ptr=&ObjectsList[from->SubObjects];--nbtemp;ptr++)
		ObjectsAss[*ptr]=to->Id;

	// if To has no objects, use the same the subobjects
	if(!nbt)
	{
		to->SubObjects=from->SubObjects;
		to->NbSubObjects=from->NbSubObjects;
		from->SubObjects=NoObject;
		from->NbSubObjects=0;
		return;
	}
	ptr=tmpObjs;
	nbtemp2=nbf;
	nbtemp=nbt+nbf;
	while(nbtemp)
	{
		while(nbf&&((!nbt)||((*ptrf)<(*ptrt))))
		{
			nbf--;
			nbtemp--;
			(*(ptr++))=(*(ptrf++));
		}
		while(nbt&&((!nbf)||((*ptrt)<(*ptrf))))
		{
			nbt--;
			nbtemp--;
			(*(ptr++))=(*(ptrt++));
		}
	}
	nbtemp=from->SubObjects;
	ptr=&ObjectsList[nbtemp];
	memcpy(ptr,ptr+from->NbSubObjects,(NbObjectsList-nbtemp)*sizeof(unsigned int));
	for(i=MaxGroups+1,N=Groups;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>nbtemp)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects-=from->NbSubObjects;
	from->NbSubObjects=0;
	from->SubObjects=NoObject;
	nbtemp=to->SubObjects+to->NbSubObjects;
	to->NbSubObjects+=nbtemp2;
	if(nbtemp+nbtemp2<Instance->NbObjects-1)
	{
		ptr=&ObjectsList[nbtemp];
		memmove(ptr+nbtemp2,ptr,(NbObjectsList-to->NbSubObjects-to->SubObjects)*sizeof(unsigned int));
		for(i=MaxGroups+1,N=Groups;--i;N++)
			if(((*G)->Reserved)&&((*G)->SubObjects>to->SubObjects)&&((*G)->SubObjects!=NoObject))
				(*G)->SubObjects+=nbtemp2;
	}
	memcpy(&ObjectsList[to->SubObjects],tmpObjs,to->NbSubObjects*sizeof(unsigned int));
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	bool RGroups<cGroup,cObj,cGroupData>::Verify(void)
{
	unsigned int i,*list;
	cGroup** G;

	for(i=Used.NbPtr+1,G=Used.Tab;--i;G++)
		if(!(*G)->Verify())
			return(false);
	if(NbObjectsList!=NbObjs)
		return(false);
	for(i=NbObjectsList+1,list=ObjectsAss;--i;list++)
		if((*list)==NoGroup)
			return(false);
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroups<cGroup,cObj,cGroupData>& RGroups<cGroup,cObj,cGroupData>::operator=(const RGroups& grps)
{
	unsigned int i;
	cGroup** G;

	ClearGroups();
	for(i=grps.Used.NbPtr+1,G=grps.Used.Tab;--i;G++)
		ReserveGroup()->Insert(Objs,*G);
	return(*this);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroups<cGroup,cObj,cGroupData>::~RGroups(void)
{
	if(ObjectsAss)
		delete[] ObjectsAss;
	if(ObjectsList)
		delete[] ObjectsList;
}
