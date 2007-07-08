/*

	RChromoG.hh

	Class representing a chromosome for a GGA - Inline implementation

	Copyright 2001-2007 by the Université Libre de Bruxelles.

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
// class RGroups<cGroup,cObj,cGroups>
//
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroups<cGroup,cObj,cGroups>::RGroups(RCursor<cObj>* objs,const unsigned max)
		: RContainer<cGroup,true,false>(max,max<20?20:max/2), Used(max,max<20?20:max/2),
		  Objs(objs), ObjsAss(objs->GetNb()), ObjsNoAss(objs->GetNb()),
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
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::Init(void)
{
	unsigned int i;

	// Create the groups
	for(i=0;i<this->GetMaxNb();i++)
		InsertPtr(new cGroup(static_cast<cGroups*>(this),i));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::ClearGroups(void)
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
template<class cGroup,class cObj,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroups>::ReserveGroup(void)
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
			InsertPtr(new cGroup(static_cast<cGroups*>(this),i));
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
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::ReleaseGroup(cGroup* grp)
{
	if(!grp->Reserved)
		return;
	DeleteObjs(grp);
	Used.DeletePtr(grp);
	grp->Clear();
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::InsertObj(cGroup* to,const cObj* obj)
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
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::DeleteObj(cGroup* from,const cObj* obj)
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
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::DeleteObjs(cGroup* from)
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
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::Verify(void)
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
template<class cGroup,class cObj,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroups>::GetGroup(unsigned int id) const
{
	unsigned int idx=ObjectsAss[id];

	if(idx==NoObject) return(0);
	return((*const_cast<RGroups<cGroup,cObj,cGroups>*>(this))[idx]);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* RGroups<cGroup,cObj,cGroups>::GetGroup(const cObj* obj) const
{
	unsigned int idx=ObjectsAss[obj->GetId()];

	if(idx==NoObject) return(0);
	return((*const_cast<RGroups<cGroup,cObj,cGroups>*>(this))[idx]);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RCursor<cObj> RGroups<cGroup,cObj,cGroups>::GetObjs(const cGroup& grp) const
{
	return(RCursor<cObj>(ObjsAss,grp.SubObjects,grp.SubObjects+grp.NbSubObjects));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroups<cGroup,cObj,cGroups>& RGroups<cGroup,cObj,cGroups>::operator=(const RGroups& grps)
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
template<class cGroup,class cObj,class cGroups>
	void RGroups<cGroup,cObj,cGroups>::ComputeOrd(void)
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
template<class cGroup,class cObj,class cGroups>
	bool RGroups<cGroup,cObj,cGroups>::SameGroupment(const RGroups* grps) const
{
	unsigned int i,*ass,*cass;

	for(i=ObjsAss.GetNb()+1,ass=OrdObjectsAss,cass=grps->OrdObjectsAss;--i;ass++,cass++)
		if((*ass)!=(*cass))
			return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RCursor<cObj> RGroups<cGroup,cObj,cGroups>::GetObjsCursor(void) const
{
	return(RCursor<cObj>(ObjsAss));
}

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double RGroups<cGroup,cObj,cGroups>::ComputeAdjustedRandIndex(const cGroups& groups) const
{
	unsigned int NbRows,NbCols;                   // Rows and Cols for matrix
	unsigned int NbObjs;                          // Total Number of objects
	unsigned int col;
	cGroup* Group2;
	double a,b,c,d,num,den;
	double* VectorRows=0;                         // Sum of the rows of the matrix
	double* VectorCols=0;                         // Sum of the columns of the matrix
	double* VectorColsTemp=0;                     // temp sum of the columns of the matrix
	double* ptr;
	double Total;

	// Init part
	Total=0.0;
	NbObjs=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	NbRows=this->Used.GetNb();
	NbCols=groups.Used.GetNb();
	if((!NbRows)||(!NbCols))
		return(-1.0);
	VectorRows=new double[NbRows];
	VectorCols=new double[NbCols];
	VectorColsTemp=new double[NbCols];

	// Construction of the container for relation between id and column in the matrix.
	RContainer<GroupId,true,true> GroupsId(NbCols,NbCols/2);
	RCursor<cGroup> Cur2(groups.Used);
	for(Cur2.Start(),col=0;!Cur2.End();Cur2.Next())
		GroupsId.InsertPtr(new GroupId(Cur2()->GetId(),col++));

	// Initialisation of the variable used for computing the subtotal
	a=b=c=d=0.0;

	// Initalisation of the vectors
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));

	// For each group of the current chromosome and for each object in this group
	// -> Compute the differents terms of the total
	int row,position;
	row=0;

	RCursor<cGroup> Cur1(this->Used);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		memset(VectorColsTemp,0,NbCols*sizeof(double));
		RCursor<cObj> Objs(GetObjs(*Cur1()));
		for(Objs.Start();!Objs.End();Objs.Next())
		{
			if(!GetGroup(Objs()))
				continue;
			VectorRows[row]++;
			NbObjs++;
			Group2=groups.GetGroup(Objs());
			if(!Group2)
				continue;
			position=GroupsId.GetPtr(Group2->GetId())->position;
			VectorCols[position]++;
			VectorColsTemp[position]++;
		}
		row++;
		for(col=NbCols+1,ptr=VectorColsTemp;--col;ptr++)
			a+=(((*ptr)*((*ptr)-1))/2);
	}

	// Finish the computation
	for(col=NbCols+1,ptr=VectorCols;--col;ptr++)
		b+=(((*ptr)*((*ptr)-1))/2);
	for(row=NbRows+1,ptr=VectorRows;--row;ptr++)
		c+=(((*ptr)*((*ptr)-1))/2);
	d=(NbObjs*(NbObjs-1))/2;
	num=a-((b*c)/d);
	den=(0.5*(b+c))-(b*c/d);
	if(den<0.0000001)
		return(-1.0);
	Total=num/den;

	// Delete the vectors
	delete[] VectorRows;
	delete[] VectorCols;
	delete[] VectorColsTemp;

	// Return the result
	return(Total);
}
		
		
//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroups<cGroup,cObj,cGroups>::~RGroups(void)
{
	if(ObjectsAss)
		delete[] ObjectsAss;
	if(OrdObjectsAss)
		delete[] OrdObjectsAss;
	if(NewUsedId)
		delete[] NewUsedId;
}
