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
template<class cGroup>
	RGGA::RGroups<cGroup>::RGroups(const unsigned max) throw(bad_alloc)
		: RStd::RContainer<cGroup,unsigned int,true,false>(max+1,max/2),
		  Used(max+1,max/2)
{
	unsigned int i;

	for(i=0;i<max;i++)
		InsertPtr(new cGroup(static_cast<RGroups<cGroup>*>(this),i));
}


//---------------------------------------------------------------------------
template<class cGroup>
	void RGGA::RGroups<cGroup>::Clear(void)
{
	cGroup **G;
	unsigned int i;

	for(i=NbPtr+1,G=Tab;--i;G++)
		(*G)->Clear();
	Used.Clear();
}


//---------------------------------------------------------------------------
template<class cGroup>
	cGroup* RGGA::RGroups<cGroup>::ReserveGroup(void)
{
	cGroup **ptr=Tab;

	if(NbGroups+1>NbPtr)
	{
		InsertPtr(new cGroup(static_cast<RGroups<cGroup>*>(this),NbPtr));
	}
	while((*ptr)->Reserved)
		ptr++;
	(*ptr)->Reserved=true;
	NbGroups++;
	Used.InsertPtr(*ptr);
	return((*ptr));
}


//---------------------------------------------------------------------------
template<class cGroup>
	void RGGA::RGroups<cGroup>::ReleaseGroup(const unsigned int grp)
{
	cGroup *G=Tab[grp];

	Used.DeletePtr(grp);
	G->Clear();
	NbGroups--;
}


//---------------------------------------------------------------------------
template<class cGroup>
	bool RGGA::RGroups<cGroup>::Verify(void)
{
	unsigned int i;
	cGroup** G;

	for(i=NbGroups+1,G=Tab;--i;G++)
		if(!(*G)->Verify())
			return(false);
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup>
	RGGA::RGroups<cGroup>& RGGA::RGroups<cGroup>::operator=(const RGroups& grps)
{
	RStd::RContainer<cGroup,unsigned int,true,false>::operator=(grps);
	Used=grps.Used;
	return(*this);
}


//---------------------------------------------------------------------------
template<class cGroup>
	RGGA::RGroups<cGroup>::~RGroups(void)
{
}
