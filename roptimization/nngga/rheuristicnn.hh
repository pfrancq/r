/*

	R Project Library

	RHeuristicNN.h

	NNGGA Heuristic - Implementation

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
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
// class RHeuristicNN<cGroup,cObj,cGroups>::Local
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	class RHeuristicNN<cGroup,cObj,cGroups>::Local
{
public:
	size_t Id;        // Identifier of the group
	size_t Nb;        // Number of elements in the group

	Local(size_t id) : Id(id), Nb(1) {}
	int Compare(const Local& el) const {return(-1);}
	static int SortOrder(const void* a,const void* b)
	{
		Local* ptrA(*((Local**)a));
		Local* ptrB(*((Local**)b));

		if(ptrA)
		{
			if(ptrB)
				return(-CompareIds(ptrA->Nb,ptrB->Nb));
			else
				return(-1);
		}
		else
		{
			if(ptrB)
				return(1);
			else
				return(0);
		}
	}
};



//-----------------------------------------------------------------------------
//
// class RHeuristicNN<cGroup,cObj,cGroups>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RHeuristicNN<cGroup,cObj,cGroups>::RHeuristicNN(RRandom& r,RCursor<cObj> objs,RParamsNN* params,RDebug* debug)
	: RGroupingHeuristic<cGroup,cObj,cGroups>("NNFirstFit",r,objs,debug),
	  ToDel(Objs.GetNb()<11?10:Objs.GetNb()/4), ToReAssign(100), Params(params), NbNearestNeighbors(100)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RHeuristicNN<cGroup,cObj,cGroups>::Init(cGroups* groups)
{
	RGroupingHeuristic<cGroup,cObj,cGroups>::Init(groups);
	NbPossibleGrps=0;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* RHeuristicNN<cGroup,cObj,cGroups>::FindGroup(cObj* obj)
{
	// Look first if an object with a high agreement ratio with the current object
	// is already grouped
	RCursor<RMaxValue> Agree(*Groups->Instance->GetAgreementRatios(obj));
	for(Agree.Start();(!Agree.End())&&(Agree()->Value>=Params->MinAgreement);Agree.Next())
	{
		size_t GroupId(Groups->ObjectsAss[Agree()->Id]);
		if(GroupId!=cNoRef)
			return((*Groups)[GroupId]);
	}

	// Count for each existing group the one containing the maximum number of nearest neighbors
	NbNearestNeighbors.Clear();
	RCursor<RMaxValue> Sim(*Groups->Instance->GetSims(obj));
	for(Sim.Start();!Sim.End();Sim.Next())
	{
		size_t GroupId(Groups->ObjectsAss[Sim()->Id]);
		if(GroupId!=cNoRef)
		{
			if((!NbNearestNeighbors.GetNb())||(GroupId>=NbNearestNeighbors.GetMaxPos()))
			{
				NbNearestNeighbors.InsertPtrAt(new Local(GroupId),GroupId);
			}
			else
			{
				Local* ptr(NbNearestNeighbors[GroupId]);
				if(ptr)
					ptr->Nb++;
				else
					NbNearestNeighbors.InsertPtrAt(new Local(GroupId),GroupId);
			}
		}
	}

	// Order the groups in descending order of number of elements
	NbNearestNeighbors.ReOrder(Local::SortOrder);

	// Find the first compatible group
	NbPossibleGrps+=NbNearestNeighbors.GetNb();
	RCursor<Local> Grps(NbNearestNeighbors);
	for(Grps.Start();!Grps.End();Grps.Next())
	{
		cGroup* grp((*Groups)[Grps()->Id]);
		if(grp->CanInsert(obj))
			return(grp);
	}

	// No group was found -> create a new one
	return(Groups->ReserveGroup());
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RHeuristicNN<cGroup,cObj,cGroups>::PostRun(void)
{
	std::cout<<"Average number of possible groups "<<static_cast<double>(NbPossibleGrps)/static_cast<double>(NbObjs)<<std::endl;
	R::RCursor<cGroup> Cur1(Groups->Used);//,Cur2(Groups->Used);

	// Look for the groups to delete
	ToDel.Clear();
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		size_t CurGroupId(Cur1()->GetId());

		// Verify if the number of objects is greater than the minimum
		if(Cur1()->GetNbObjs()>=Params->NbMinObjs)
		{
			// Look if this group contains only 1 social profile
			if(Cur1()->GetNbObjs()!=1)
				continue;
			if(!Groups->Instance->IsSocial(Cur1()->GetObjPos(0)))
				continue;
		}

		// The current group must be emptied and the objects reassigned
		RCursor<cObj> Objs(Cur1()->GetObjs());
		ToReAssign.Clear();
		for(Objs.Start();!Objs.End();Objs.Next())
			ToReAssign.InsertPtr(Objs());
		Objs.Set(ToReAssign);
		for(Objs.Start();!Objs.End();Objs.Next())
		{
			cGroup* grp(0); // Group to find

			// Find the first object with a high agreement ratio not in the same group
			RCursor<RMaxValue> Agree(*Groups->Instance->GetAgreementRatios(Objs()));
			for(Agree.Start();(!Agree.End())&&(!grp);Agree.Next())
			{
				size_t GroupId(Groups->ObjectsAss[Agree()->Id]);
				if(GroupId!=CurGroupId)
					grp=(*Groups)[GroupId];
			}

			if(!grp)
			{
				// Find the first object with a high similarity not in the same group
				RCursor<RMaxValue> Sim(*Groups->Instance->GetSims(Objs()));
				for(Sim.Start();(!Sim.End())&&(!grp);Sim.Next())
				{
					size_t GroupId(Groups->ObjectsAss[Sim()->Id]);
					if(GroupId!=CurGroupId)
						grp=(*Groups)[GroupId];
				}
			}


			// Try to find a new group for obj
//			cGroup* grp(0);
//			double max(-1.0);
//			for(Cur2.Start();!Cur2.End();Cur2.Next())
//			{
//				if((Cur1()==Cur2())||(!Cur2()->GetNbObjs()))
//					continue;
//				if(Cur2()->HasSameUser(Objs()))
//					continue;
//				double tmp(Cur2()->ComputeRelSim(Objs()));
//				if(tmp>max)
//				{
//					max=tmp;
//					grp=Cur2();
//				}
//			}

			// If a group was found -> put the object in it
			if(grp)
			{
				Cur1()->Delete(Objs());
				grp->Insert(Objs());
			}
		}

		// If Cur1() has no objects anymore -> delete it
		if(!Cur1()->GetNbObjs())
			ToDel.InsertPtr(Cur1());
	}

	// Delete the groups
	Cur1.Set(ToDel);
	for(Cur1.Start();!Cur1.End();Cur1.Next())
		Groups->ReleaseGroup(Cur1());
}
