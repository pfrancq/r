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
// class RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RGroup(RGroups<cGroup>* owner,unsigned id)
		: Id(id), Owner(owner), Groups(owner->Groups)
{
	NbSubObjects= 0;
	SubObjects = NoObject;
	Reserved=false;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	bool RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Verify(void)
{
	unsigned int j,*obj;
	unsigned int NbObjects=Owner->Instance->NbObjects;
	unsigned int NbGroups=Owner->NbGroups;
	bool Ok=true;
	
	// Each group must have a parent.
	if(!Owner)
		throw(new eGAVerify);

	// The groups array must always pointed to the one of the parent.
	if(Owner->Groups!=Groups)
		Ok=false;

	// Verify objects attached.
	if(SubObjects!=NoObject)
	{
		// The number of objects attached can't be null.
		if(!NbSubObjects)
			Ok=false;

		// The index of the first objects attached can npt exceed the total number of objects.
		if(SubObjects>NbObjects)
			Ok=false;

		// The index of the last objects attached can npt exceed the total number of objects.
		if(SubObjects+NbSubObjects>NbObjects+1)
			Ok=false;
	}
	else
	{
		// The number of objects attached must be null.
		if(NbSubObjects)
			Ok=false;
	}

	// Verify coherence with the objects.
	for(j=NbSubObjects+1,obj=&(Owner->ObjectsList[SubObjects]);--j;obj++)
		if(Owner->ObjectsAss[*obj]!=Id)
			Ok=false;

	// return the value of the verification.
	return(Ok);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Clear(void)
{
	Reserved=false;
	SubObjects=NoObjects;
	NbSubObjects=0;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	bool RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::IsSameObjs(const RGroup* /*grp*/) const
{
	return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>& RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::operator=(const RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>& grp)

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
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	RGGA::RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RGroup(void)
{
}
