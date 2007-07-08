/*

	R Project Library

	RGroup.hh

	Group of a GGA - Header.

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
	RGroup<cGroup,cObj,cGroups>::RGroup(cGroups* owner,const unsigned int id)
		: Id(id), Owner(owner)
{
	NbSubObjects= 0;
	SubObjects = NoObject;
	Reserved=false;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::Verify(void)
{
	unsigned int NbObjects=Owner->ObjsAss.GetNb();
	char tmp[200];

	// Each group must have a parent.
	if(!Owner)
	{
		sprintf(tmp,"No Owner for group %u",Id);
		throw RGAException(tmp,RGAException::eGAVerify);
	}

	// Verify objects attached.
	if(SubObjects!=NoObject)
	{
		// The number of objects attached can't be null.
		if(!NbSubObjects)
		{
			sprintf(tmp,"!NbSubObjects for group %u",Id);
			throw RGAException(tmp,RGAException::eGAVerify);
		}

		// The index of the first objects attached can not exceed the total number of objects.
		if(SubObjects>NbObjects)
		{
			sprintf(tmp,"SubObjects>NbObjects for group %u",Id);
			throw RGAException(tmp,RGAException::eGAVerify);
		}

		// The index of the last objects attached can not exceed the total number of objects.
		if(SubObjects+NbSubObjects>NbObjects+1)
		{
			sprintf(tmp,"SubObjects+NbSubObjects>NbObjects+1 for group %u",Id);
			throw RGAException(tmp,RGAException::eGAVerify);
		}
	}
	else
	{
		// The number of objects attached must be null.
		if(NbSubObjects)
		{
			sprintf(tmp,"NbSubObjects for group %u",Id);
			throw RGAException(tmp,RGAException::eGAVerify);
		}
	}

	// Verify coherence with the objects.
	RCursor<cObj> ptr(this->Owner->GetObjs(*static_cast<cGroup*>(this)));
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		if(!ptr())
		{
			sprintf(tmp,"Owner->ObjsAss is null for group %u",Id);
			throw RGAException(tmp,RGAException::eGAVerify);
		}
		if(Owner->ObjectsAss[ptr()->GetId()]!=Id)
		{
			sprintf(tmp,"Owner->ObjsAss[(*obj)->GetId()]!=Id for group %u and object %u",Id,ptr()->GetId());
			throw RGAException(tmp,RGAException::eGAVerify);
		}
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::Clear(void)
{
	Reserved=false;
	SubObjects=NoObject;
	NbSubObjects=0;
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::Copy(const cGroup* grp)
{
	RReturnIfFail(Owner!=grp->Owner);
	RCursor<cObj> ptr(grp->Owner->GetObjs(*grp));
	for(ptr.Start();!ptr.End();ptr.Next())
		Insert(ptr());

}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::PostInsert(const cObj*)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroup<cGroup,cObj,cGroups>::PostDelete(const cObj*)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::CanInsert(const cObj*) const
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::IsCompatible(const cObj*) const
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::CanDelete(const cObj*) const
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroup<cGroup,cObj,cGroups>& RGroup<cGroup,cObj,cGroups>::operator=(const RGroup<cGroup,cObj,cGroups>&)
{
	return(*this);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::IsCompatible(const cGroup*) const
{
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::IsIn(const unsigned int id) const
{
	unsigned int i;

	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next())
		if(ptr()->GetId()==id) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::CommonObjs(const cGroup* grp) const
{
	unsigned int i;

	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next())
		if(grp->IsIn(ptr()->GetId())) return(true);
	return(false);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroup<cGroup,cObj,cGroups>::SameObjs(const cGroup* grp) const
{
	unsigned int i;

	if(NbSubObjects!=grp->NbSubObjects) return(false);
	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next())
		if(!grp->IsIn(ptr()->GetId())) return(false);
	return(true);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	unsigned int* RGroup<cGroup,cObj,cGroups>::GetObjectsId(void) const
{
	unsigned int* tmp;
	unsigned int* tmp2;
	unsigned int i;

	tmp2=tmp=new unsigned int[NbSubObjects+1];
	RCursor<cObj> ptr(Owner->ObjsAss);
	for(i=NbSubObjects+1,ptr.GoTo(SubObjects);--i;ptr.Next(),tmp2++)
		(*tmp2)=ptr()->GetId();
	(*tmp2)=NoObject;
	return(tmp);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroup<cGroup,cObj,cGroups>::~RGroup(void)
{
}
