/*

	R Project Library

	RGroups.h

	Groups - Implementation.

	Copyright 2001-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
	R::RGroups<cGroup,cObj,cGroups>::RGroups(R::RCursor<cObj> objs,size_t max)
		: RContainer<cGroup,true,false>(max,max<20?20:max/2), Used(max,max<20?20:max/2),
		  Objs(objs), ObjsAss(objs.GetNb()), ObjsNoAss(objs.GetNb()),
		  OrdObjectsAss(0), NewUsedId(0)
{
	ObjectsAss = new size_t[Objs.GetMaxPos()+1];
	memset(ObjectsAss,0xFF,(Objs.GetMaxPos()+1)*sizeof(size_t));

	// Init of the arrays needed.
	OrdObjectsAss=new size_t[Objs.GetMaxPos()+1];
	NewUsedId=new size_t[GetMaxNb()];

	// Suppose no object is assigned
	for(Objs.Start();!Objs.End();Objs.Next())
		ObjsNoAss.InsertPtr(Objs());
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::Init(void)
{
	size_t i;

	// Create the groups
	for(i=0;i<GetMaxNb();i++)
		InsertPtr(new cGroup(static_cast<cGroups*>(this),i));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::ClearGroups(void)
{
	R::RCursor<cGroup> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Clear();
	Used.Clear();
	ObjsAss.Clear();
	ObjsNoAss.Clear();
	for(Objs.Start();!Objs.End();Objs.Next())
		ObjsNoAss.InsertPtr(Objs());
	memset(ObjectsAss,0xFF,(Objs.GetMaxPos()+1)*sizeof(size_t));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::VerifyGroups(size_t id)
{
	if(id+1>GetMaxNb())
	{
		// Compute the new size of the groups
		size_t NewSize(GetMaxNb()+GetIncNb());
		if(NewSize<id+1)
			NewSize=id+1;

		// Recreate a new NewUsedId array with the new size
		size_t *n(new size_t[NewSize]);
		memcpy(n,NewUsedId,sizeof(size_t)*GetMaxNb());
		delete[] NewUsedId;
		NewUsedId=n;

		// Create New groups
		for(size_t i=GetMaxNb();i<NewSize;i++)
			InsertPtr(new cGroup(static_cast<cGroups*>(this),i));
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* R::RGroups<cGroup,cObj,cGroups>::ReserveGroup(size_t id)
{
	cGroup* Grp(0);
	if(id==cNoRef)
	{
		VerifyGroups(Used.GetNb());
		R::RCursor<cGroup> ptr(*this);
		ptr.Start();
		while(ptr()->Reserved)
			ptr.Next();
		Grp=ptr();
	}
	else
	{
		VerifyGroups(id);
		Grp=(*this)[id];
		if(Grp->Reserved)
			ThrowRException("Group '"+RString(id)+"' is already used.");
	}
	Grp->Reserved=true;
	Used.InsertPtr(Grp);
	return(Grp);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::ReleaseGroup(cGroup* grp)
{
	if(!grp->Reserved)
		return;
	DeleteObjs(grp);
	Used.DeletePtr(grp);
	grp->Clear();
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::InsertObj(cGroup* to,const cObj* obj)
{
	size_t tmp,j;

	if(to->NbSubObjects)
	{
		tmp=to->SubObjects+to->NbSubObjects;
		j=to->SubObjects;
		R::RCursor<cGroup> G(Used);
		for(G.Start();!G.End();G.Next())
			if((G()->SubObjects>j)&&(G()->SubObjects!=cNoRef))
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
	void R::RGroups<cGroup,cObj,cGroups>::InsertObjs(const cGroup* from,cGroup* to)
{
	RReturnIfFail(from->Owner!=to->Owner);
	RReturnIfFail(!to->NbSubObjects);
	if(!(from->NbSubObjects)) return;
	bool CallPostInsert(to->PreInsertAll(from));
	to->SubObjects=ObjsAss.GetNb();
	to->NbSubObjects=from->NbSubObjects;
	RCursor<cObj> ptr(from->GetObjs());
	size_t pos;
	for(ptr.Start(),pos=to->SubObjects;!ptr.End();ptr.Next(),pos++)
	{
		ObjsAss.InsertPtrAt(ptr(),pos,false);
		ObjectsAss[ptr()->GetId()]=to->Id;
		ObjsNoAss.DeletePtr(ptr());
		if(CallPostInsert)
			to->PostInsert(ptr());
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::DeleteObj(cGroup* from,const cObj* obj)
{
	size_t j;

	j=from->SubObjects;
	ObjectsAss[obj->GetId()]=cNoRef;
	ObjsNoAss.InsertPtr(obj);
	ObjsAss.DeletePtr(obj);
	if(!(--from->NbSubObjects))
		from->SubObjects=cNoRef;
	from->PostDelete(obj);
	R::RCursor<cGroup> G(Used);
	for(G.Start();!G.End();G.Next())
		if((G()->SubObjects>j)&&(G()->SubObjects!=cNoRef))
			G()->SubObjects--;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::DeleteObjs(cGroup* from)
{
	size_t j,i,tmp;
	cObj* tmpObj;

	if(!(from->NbSubObjects)) return;
	bool CallPostDelete(from->PreDeleteAll());
	tmp=from->NbSubObjects;
	j=from->SubObjects;
	R::RCursor<cObj> obj(ObjsAss);
	for(i=from->NbSubObjects+1,obj.GoTo(from->SubObjects);--i;)
	{
		 // No need to increment obj, because delete moves everything
		tmpObj=obj();
		ObjectsAss[tmpObj->GetId()]=cNoRef;
		ObjsNoAss.InsertPtr(tmpObj);
		ObjsAss.DeletePtr(tmpObj);
		if(!(--from->NbSubObjects))
			from->SubObjects=cNoRef;
		if(CallPostDelete)
			from->PostDelete(tmpObj);
	}
	R::RCursor<cGroup> G(Used);
	for(G.Start();!G.End();G.Next())
		if((G()->SubObjects>j)&&(G()->SubObjects!=cNoRef))
			G()->SubObjects-=tmp;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::Verify(void)
{
	R::RCursor<cGroup> Cur(Used);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Verify();
	if(ObjsAss.GetNb()+ObjsNoAss.GetNb()!=Objs.GetNb())
		throw RGAException("Problem with the number of objects: ObjsAss="+RString::Number(ObjsAss.GetNb())+" and ObjsNoAss="+RString::Number(ObjsNoAss.GetNb()),RGAException::eGAVerify);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* R::RGroups<cGroup,cObj,cGroups>::GetGroup(size_t id) const
{
	size_t idx=ObjectsAss[id];

	if(idx==cNoRef) return(0);
	return((*const_cast<RGroups<cGroup,cObj,cGroups>*>(this))[idx]);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* R::RGroups<cGroup,cObj,cGroups>::GetGroup(const cObj* obj) const
{
	size_t idx=ObjectsAss[obj->GetId()];

	if(idx==cNoRef) return(0);
	return((*const_cast<RGroups<cGroup,cObj,cGroups>*>(this))[idx]);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RCursor<cObj> R::RGroups<cGroup,cObj,cGroups>::GetObjs(const RGroup<cGroup,cObj,cGroups>& grp) const
{
	if(!grp.NbSubObjects)
		return(R::RCursor<cObj>());
	return(R::RCursor<cObj>(ObjsAss,grp.SubObjects,grp.SubObjects+grp.NbSubObjects-1));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::CopyGrouping(const RGroups& grps)
{
	ClearGroups();
	R::RCursor<cGroup> G(grps.Used);
	for(G.Start();!G.End();G.Next())
	{
		cGroup* ptr(ReserveGroup(G()->Id));  // Reserve a new group
		InsertObjs(G(),ptr);
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroups<cGroup,cObj,cGroups>::ComputeOrd(void)
{
	size_t *oldo,*newo;
	size_t i,id,nbgrp,j;

	memset(NewUsedId,0xFF,sizeof(size_t)*GetMaxNb());
	memset(OrdObjectsAss,0xFF,sizeof(size_t)*(Objs.GetMaxPos()+1));
	for(i=Objs.GetMaxPos()+2,oldo=ObjectsAss,newo=OrdObjectsAss,nbgrp=0,j=0;--i;oldo++,newo++,j++)
	{
		if((*oldo)==R::cNoRef)
			continue;
		id=NewUsedId[*oldo];
		if(id==cNoRef)
			id=NewUsedId[*oldo]=(nbgrp++);
		(*newo)=id;
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool R::RGroups<cGroup,cObj,cGroups>::SameGroupment(const RGroups* grps) const
{
	size_t i,*ass,*cass;

	for(i=Objs.GetMaxPos()+2,ass=OrdObjectsAss,cass=grps->OrdObjectsAss;--i;ass++,cass++)
		if((*ass)!=(*cass))
			return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RCursor<cObj> R::RGroups<cGroup,cObj,cGroups>::GetObjsCursor(void) const
{
	return(R::RCursor<cObj>(ObjsAss));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double R::RGroups<cGroup,cObj,cGroups>::ComputeAdjustedRandIndex(const cGroups& groups) const
{
	size_t NbRows,NbCols;                   // Rows and Cols for matrix
	size_t NbObjs;                          // Total Number of objects
	size_t col;
	cGroup* Group2;
	double a,b,c,d,num,den;
	double* VectorRows=0;                         // Sum of the rows of the matrix
	double* VectorCols=0;                         // Sum of the columns of the matrix
	double* VectorColsTemp=0;                     // temp sum of the columns of the matrix
	double* ptr;
	double Total;

	// Initialize
	Total=0.0;
	NbObjs=0;

	// Go through the languages to define the maximal sizes and allocate the matrix
	NbRows=Used.GetNb();
	NbCols=groups.Used.GetNb();
	if((!NbRows)||(!NbCols))
		return(-1.0);
	VectorRows=new double[NbRows];
	VectorCols=new double[NbCols];
	VectorColsTemp=new double[NbCols];

	// Construction of the container for relation between id and column in the matrix.
	R::RContainer<GroupId,true,true> GroupsId(NbCols,NbCols/2);
	R::RCursor<cGroup> Cur2(groups.Used);
	for(Cur2.Start(),col=0;!Cur2.End();Cur2.Next())
		GroupsId.InsertPtr(new GroupId(Cur2()->GetId(),col++));

	// Initialization of the variable used for computing the sub-totals.
	a=b=c=d=0.0;

	// Initialization of the vectors
	memset(VectorRows,0,NbRows*sizeof(double));
	memset(VectorCols,0,NbCols*sizeof(double));

	// For each group of the current chromosome and for each object in this group
	// -> Compute the different terms of the total
	int row,position;
	row=0;

	R::RCursor<cGroup> Cur1(Used);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		memset(VectorColsTemp,0,NbCols*sizeof(double));
		R::RCursor<cObj> Objs(GetObjs(*Cur1()));
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
	R::RGroups<cGroup,cObj,cGroups>::~RGroups(void)
{
	if(ObjectsAss)
		delete[] ObjectsAss;
	if(OrdObjectsAss)
		delete[] OrdObjectsAss;
	if(NewUsedId)
		delete[] NewUsedId;
}
