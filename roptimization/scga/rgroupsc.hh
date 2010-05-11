/*

	R Project Library

	RGroupSC.hh

	Similarity-based Clustering Group - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
// class RGroupSC<cGroup,cObj,cGroups>
//


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>::RGroupSC(RGroupSC* grp)
	: RGroup<cGroup,cObj,cGroups>(grp), ToEval(true)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>::RGroupSC(cGroups* owner,const size_t id)
	: RGroup<cGroup,cObj,cGroups>(owner,id), ToEval(true)
{
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::Clear(void)
{
	RGroup<cGroup,cObj,cGroups>::Clear();
	ToEval=true;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSC<cGroup,cObj,cGroups>::HasSameUser(const cObj* obj) const
{
	size_t usr=obj->GetParentId();
	if(!usr)
		return(false);
	RCursor<cObj> ptr(Owner->GetObjs(*this));
	for(ptr.Start();!ptr.End();ptr.Next())
		if(usr==ptr()->GetParentId())
			return(true);
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSC<cGroup,cObj,cGroups>::CanInsert(const cObj* obj)
{
	// If no objects -> OK
	if(!RGroup<cGroup,cObj,cGroups>::NbSubObjects)
		return(true);

	// If too many objects -> No
	if(RGroup<cGroup,cObj,cGroups>::NbSubObjects>Owner->Instance->Params->NbMaxObjs)
		return(false);

	// Verify that the maximal disagreement ratio is respected
	RCursor<RMaxValue> Disagree(*Owner->Instance->GetDisagreementRatios(obj));
	for(Disagree.Start();(!Disagree.End())&&(Disagree()->Value>=Owner->Instance->Params->MinDisagreement);Disagree.Next())
	{
		if(Owner->ObjectsAss[Disagree()->Id]==Id)
			return(false);
	}

	// Verify that the minimum similarity is respected
	RCursor<RMaxValue> Sim(*Owner->Instance->GetSims(obj));
	for(Sim.StartAtEnd();(!Sim.Begin())&&(Sim()->Value<=Owner->Instance->Params->MinSimLevel);Sim.Prev())
	{
		if(Owner->ObjectsAss[Sim()->Id]==Id)
			return(false);
	}

	// Ok the group is a valid one.
	return(true);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::PostInsert(const cObj* /*obj*/)
{
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSC<cGroup,cObj,cGroups>::PreInsertAll(const cGroup* /*from*/)
{
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupSC<cGroup,cObj,cGroups>::PostDelete(const cObj* /*obj*/)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	bool RGroupSC<cGroup,cObj,cGroups>::PreDeleteAll(void)
{
	ToEval=true;
	return(false);
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupSC<cGroup,cObj,cGroups>::~RGroupSC(void)
{
}
