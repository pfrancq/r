/*

	R Project Library

	RGroup.hh

	Group of a GGA - Header.

	Copyright 2001-2012 by Pascal Francq (pascal@francq.info).
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
// class RGroup<cGroup,cObj,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroup<cGroup,cObj,cGroups>::RGroup(RGroup* grp)
		: Id(grp->Id), Owner(grp->Owner)
{
	NbSubObjects= grp->NbSubObjects;
	SubObjects = grp->SubObjects;
	Reserved=grp->Reserved;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroup<cGroup,cObj,cGroups>::RGroup(cGroups* owner,const size_t id)
		: Id(id), Owner(owner)
{
	NbSubObjects= 0;
	SubObjects = cNoRef;
	Reserved=false;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::Verify(void)
{
	size_t NbObjects=Owner->ObjsAss.GetNb();

	// Each group must have a parent.
	if(!Owner)
		mThrowRGAException("Verify","No Owner for used group "+RString::Number(Id));

	// Verify objects attached.
	if(SubObjects!=cNoRef)
	{
		// The number of objects attached can't be null.
		if(!NbSubObjects)
			mThrowRGAException("Verify","!NbSubObjects for used group "+RString::Number(Id));

		// The index of the first objects attached can not exceed the total number of objects.
		if(SubObjects>NbObjects)
			mThrowRGAException("Verify","SubObjects>NbObjects for group "+RString::Number(Id));

		// The index of the last objects attached can not exceed the total number of objects.
		if(SubObjects+NbSubObjects>NbObjects+1)
			mThrowRGAException("Verify","SubObjects+NbSubObjects>NbObjects+1 for group "+RString::Number(Id));
	}
	else
	{
		// The number of objects attached must be null.
		if(NbSubObjects)
			mThrowRGAException("Verify","NbSubObjects for empty group "+RString::Number(Id));
	}

	// Verify coherence with the objects.
	RCursor<cObj> ptr(Owner->GetObjs(*static_cast<cGroup*>(this)));
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(!ptr())
			mThrowRGAException("Verify","Owner->ObjsAss is null for group "+RString::Number(Id));
		if(Owner->ObjectsAss[ptr()->GetId()]!=Id)
			mThrowRGAException("Verify","Owner->ObjsAss[(*obj)->GetId()]!=Id for group "+RString::Number(Id)+" and object "+RString::Number(ptr()->GetId()));
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::Clear(void)
{
	Reserved=false;
	SubObjects=cNoRef;
	NbSubObjects=0;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::PostInsert(const cObj*)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::PreInsertAll(const cGroup*)
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::PostDelete(const cObj*)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::PreDeleteAll(void)
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::CanInsert(const cObj*)
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::IsCompatible(const cObj*)
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::CanDelete(const cObj*)
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::IsCompatible(const cGroup*) const
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::IsIn(const size_t id) const
{
	if(!NbSubObjects)
		return(false);

	size_t i;
	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next())
		if(ptr()->GetId()==id) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::CommonObjs(const cGroup* grp) const
{
	if(!NbSubObjects)
		return(false);

	size_t i;
	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next())
		if(grp->IsIn(ptr()->GetId())) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::SameObjs(const cGroup* grp) const
{
	if((!NbSubObjects)||(NbSubObjects!=grp->NbSubObjects))
		return(false);
	size_t i;
	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next())
		if(!grp->IsIn(ptr()->GetId())) return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	size_t* RGroup<cGroup,cObj,cGroups>::GetObjectsId(void) const
{
	size_t* tmp;
	size_t* tmp2;
	size_t i;

	tmp2=tmp=new size_t[NbSubObjects+1];
	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next(),tmp2++)
		(*tmp2)=ptr()->GetId();
	(*tmp2)=cNoRef;
	return(tmp);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cObj* RGroup<cGroup,cObj,cGroups>::GetObjPos(const size_t pos)
{
	if(pos>NbSubObjects)
			throw std::range_error("cObj* RGroup::GetObjPos(size_t) : group '"+RString::Number(Id)+"' has only "+RString::Number(NbSubObjects)+" objects");
	return(Owner->GetObj(SubObjects+pos));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroup<cGroup,cObj,cGroups>::~RGroup(void)
{
}
