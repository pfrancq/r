/*

	R Project Library

	RNodeGA.hh

	GA Node - Header.

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
// class RGroup<cGroup,cObj,cGroupData>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroup<cGroup,cObj,cGroupData>::RGroup(RGroup* grp)
		: Id(grp->Id), Owner(grp->Owner)
{
	NbSubObjects= grp->NbSubObjects;
	SubObjects = grp->SubObjects;
	Reserved=grp->Reserved;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroup<cGroup,cObj,cGroupData>::RGroup(RGroups<cGroup,cObj,cGroupData>* owner,const unsigned int id,const cGroupData*)
		: Id(id), Owner(owner)
{
	NbSubObjects= 0;
	SubObjects = NoObject;
	Reserved=false;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	bool RGroup<cGroup,cObj,cGroupData>::Verify(void)
{
	unsigned int i,*obj;
	unsigned int NbObjects=Owner->NbObjectsList;
	unsigned int NbGroups=Owner->Used.NbPtr;
	
	// Each group must have a parent.
	if(!Owner)
		return(false);

	// Verify objects attached.
	if(SubObjects!=NoObject)
	{
		// The number of objects attached can't be null.
		if(!NbSubObjects)
			return(false);

		// The index of the first objects attached can not exceed the total number of objects.
		if(SubObjects>NbObjects)
			return(false);

		// The index of the last objects attached can not exceed the total number of objects.
		if(SubObjects+NbSubObjects>NbObjects+1)
			return(false);
	}
	else
	{
		// The number of objects attached must be null.
		if(NbSubObjects)
			return(false);
	}

	// Verify coherence with the objects.
	for(i=NbSubObjects+1,obj=&(Owner->ObjectsList[SubObjects]);--i;obj++)
		if(Owner->ObjectsAss[*obj]!=Id)
			return(false);

	// return the value of the verification.
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroup<cGroup,cObj,cGroupData>::Clear(void)
{
	Reserved=false;
	SubObjects=NoObject;
	NbSubObjects=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	void RGroup<cGroup,cObj,cGroupData>::Insert(cObj** objs,const cGroup* grp)
{
	unsigned int i;
	unsigned int* list;

	RReturnIfFail(Owner!=grp->Owner);
	for(i=grp->NbSubObjects+1,list=&grp->Owner->ObjectsList[grp->SubObjects];--i;list++)
		Insert(objs[*list]);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroup<cGroup,cObj,cGroupData>& RGGA::RGroup<cGroup,cObj,cGroupData>::operator=(const RGroup<cGroup,cObj,cGroupData>& grp)
{
	if(this!=&grp)
	{
		SubObjects=node.SubObjects;
		NbSubObjects=node.NbSubObjects;
		Reserved=node.Reserved;
		Id=node.Id;
	}
	return(*this);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	bool RGroup<cGroup,cObj,cGroupData>::IsIn(const unsigned int id) const
{
	unsigned int i;
	unsigned int* list;

	for(i=NbSubObjects+1,list=&Owner->ObjectsList[SubObjects];--i;list++)
		if((*list)==id) return(true);
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	bool RGroup<cGroup,cObj,cGroupData>::CommonObjs(const cGroup* grp) const
{
	unsigned int i;
	unsigned int* list;

	for(i=NbSubObjects+1,list=&Owner->ObjectsList[SubObjects];--i;list++)
		if(grp->IsIn(*list)) return(true);
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	bool RGroup<cGroup,cObj,cGroupData>::SameObjs(const cGroup* grp) const
{
	unsigned int i;
	unsigned int* list;

	if(NbSubObjects!=grp->NbSubObjects) return(false);
	for(i=NbSubObjects+1,list=&Owner->ObjectsList[SubObjects];--i;list++)
		if(!grp->IsIn(*list)) return(false);
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	unsigned int* RGroup<cGroup,cObj,cGroupData>::GetObjectsId(void) const
{
	unsigned int* tmp=new unsigned int[NbSubObjects+1];
	memcpy(tmp,&Owner->ObjectsList[SubObjects],NbSubObjects*sizeof(unsigned int));
	tmp[NbSubObjects]=NoObject;
	return(tmp);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroupData>
	RGroup<cGroup,cObj,cGroupData>::~RGroup(void)
{
}
