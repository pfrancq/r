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
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RChromoG(cInst *inst,unsigned id) throw(bad_alloc)
		: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id)
{
	NbGroups=0;
	NbGroupsList=0;
	NbObjectsList=0;
	Groups=0;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Init(cThreadData *thData) throw(bad_alloc)
{
	cGroup** G;
	unsigned int i;

	// Initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// General Initalisation
	MaxGroups=Instance->MaxGroups;
	Objects=Instance->Objects;
	Groups=new cGroup*[MaxGroups];
	memset(Groups,0,MaxGroups*sizeof(cGroup*));
	for(i=0,G=Groups;i<MaxGroups;G++,i++)
		(*G) = new cGroup(static_cast<cChromo*>(this),i);
	ObjectsAss = new unsigned int[Instance->NbObjects];
	memset(ObjectsAss,0xFF,Instance->NbObjects*sizeof(unsigned int));
	GroupsList = new unsigned int[MaxGroups];
	memset(GroupsList,0,MaxGroups*sizeof(unsigned int));
	ObjectsList = new unsigned int[Instance->NbObjects];
	memset(ObjectsList,0,Instance->NbObjects*sizeof(unsigned int));

	// Init "thread-dependent" data
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Clear(void)
{
	cGroup **G;
	unsigned int i;

	for(i=NbGroups+1,G=Groups;--i;G++)
	{
		(*G)->Reserved=false;
		(*G)->NbSubObjects = 0;
		(*G)->SubObjects=NoObject;
	}
	NbGroups = 0;
	NbNodesList=0;
	NbObjectsList=0;
	memset(ObjectsAss,0xFF,Instance->NbObjects*sizeof(unsigned int));
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	cGroup* RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::ReserveGroup(void)
{
	cGroup **ptr=Groups;

	if(NbGroups+1>MaxGroups)
		return(0);
	while((*ptr)->Reserved)
		ptr++;
	(*ptr)->Reserved=true;
	NbGroups++;
	return((*ptr));
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::ReleaseGroup(const unsigned int grp)
{
	cGroup *G=Groups[grp];

	N->Reserved=false;
	NbGroups--;
	N->NbSubObjects = 0;
	N->SubObjects=NoObject;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::InsertObj(cGroup* to,const unsigned int obj)
{
	unsigned int *ptr,tmp,i,j;
	cGroup** G;

	if(to->NbSubObjects)
	{
		for(i=0,ptr=&ObjectsList[to->SubObjects];(i<to->NbSubObjects)&&((*ptr)<obj);ptr++,i++);
		tmp=to->SubObjects+i;
		if(to->SubObjects+i<Instance->NbObjects-1)
			memmove(ptr+1,ptr,(NbObjectsList-tmp)*sizeof(unsigned int));
		(*ptr)=obj;
		j=to->SubObjects;
		NbObjectsList++;
		for(i=MaxGroups+1,G=Groups;--i;G++)
			if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
				(*G)->SubObjects++;
	}
	else
		ObjectsList[to->SubObjects=NbObjectsList++]=obj;
	to->NbSubObjects++;
	ObjectsAss[obj]=to->Id;
//	Verify();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::DeleteObj(cGroup* from,const unsigned int obj)
{
	unsigned int *ptr,j,i;
	cGroup** G;

	for(i=0,ptr=&ObjectsList[from->SubObjects];(*ptr)!=obj;ptr++,i++);
	memcpy(ptr,ptr+1,(NbObjectsList-from->SubObjects-i)*sizeof(unsigned int));
	j=from->SubObjects;
	for(i=MaxGroups+1,G=Groups;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects--;
	if(!(--(from->NbSubObjects)))
		from->SubObjects=NoObject;
	NbObjectsList--;
	ObjectsAss[obj]=NoGroup;
//	Verify();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::DeleteObjs(cGroup* from)
{
	unsigned int *ptr,j,i;
	cGroup** G;

	if(!(from->NbSubObjects)) return;
	for(i=from->NbSubObjects+1,ptr=&ObjectsList[from->SubObjects];--i;ptr++)
		ObjectsAss[*ptr]=NoGroup;
	j=from->SubObjects;
	ptr=&ObjectsList[j];
	memcpy(ptr,ptr+from->NbSubObjects,(NbObjectsList-j)*sizeof(unsigned int));
	for(i=MaxGroups+1,G=Groups;--i;G++)
		if(((*G)->Reserved)&&((*G)->SubObjects>j)&&((*G)->SubObjects!=NoObject))
			(*G)->SubObjects-=from->NbSubObjects;
	NbObjectsList-=from->NbSubObjects;
	from->NbSubObjects=0;
	from->SubObjects=NoObject;
//	Verify();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::MoveObjs(cGroup* to,cGroup* from)
{
	unsigned int nbt=to->NbSubObjects,nbf=from->NbSubObjects,nbtemp,nbtemp2;
	unsigned int *ptrf=&ObjectsList[from->SubObjects],*ptrt=&ObjectsList[to->SubObjects];
	unsigned int *ptr;
	cGroup** G;
	unsigned int i;

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
//	Verify();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	bool RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RandomConstruct(void)
{
	ToEval=true;
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	bool RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Crossover(cChromo* parent1,cChromo* parent2)
{
	ToEval=true;
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	bool RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Mutation(void)
{
	ToEval=true;
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	bool RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Verify(void)
{
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>& RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::operator=(const RChromoG& chromo)
{
//	cNode **N1,**N2;
//	unsigned int i;
//
//	if(this==&chromo) return(*this);
//	Clear();
//	for(i=chromo.MaxNodes+1,N1=Nodes,N2=chromo.Nodes;--i;N1++,N2++)
//		(**N1)=(**N2);
//	TopNodes=chromo.TopNodes;
//	NbTopNodes=chromo.NbTopNodes;
//	NbNodes=chromo.NbNodes;
//	NbNodesList=chromo.NbNodesList;
//	memcpy(NodesList,chromo.NodesList,sizeof(unsigned int)*NbNodesList);
//	NbObjectsList=chromo.NbObjectsList;
//	memcpy(ObjectsList,chromo.ObjectsList,sizeof(unsigned int)*NbObjectsList);
//	memcpy(ObjectsAss,chromo.ObjectsAss,sizeof(unsigned int)*NbObjectsList);
//	(*Fitness)=(*(chromo.Fitness));
//  Verify();
  return(*this);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RChromoG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RChromoG(void)
{
	unsigned int i;
	cGroup **G;

	if(Groups)
	{
		for(i=MaxGroups+1,G=Groups;--i;G++)
			if(*G) delete (*G);
		delete[] Nodes;
	}
	delete[] NodesList;
	delete[] ObjectsAss;
	delete[] ObjectsList;
}
