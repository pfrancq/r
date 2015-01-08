/*

	R Project Library

	RHeuristicSG.h

	Similarity-based Grouping Genetic Algorithm - Implementation

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
// class RHeuristicSG<cGroup,cObj,cGroups>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RHeuristicSG<cGroup,cObj,cGroups>::RHeuristicSG(RRandom& r,RCursor<cObj> objs,RParamsSG* params,RDebug* debug)
	: RGroupingHeuristic<cGroup,cObj,cGroups>("SCGA Heuristic",r,objs,debug),
	  ToDel(Objs.GetNb()<11?10:Objs.GetNb()/4), ToReAssign(100), Params(params)
{
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RHeuristicSG<cGroup,cObj,cGroups>::Init(cGroups* groups)
{
	RGroupingHeuristic<cGroup,cObj,cGroups>::Init(groups);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cGroup* RHeuristicSG<cGroup,cObj,cGroups>::FindGroup(cObj* obj)
{
	double maxsim(-2.0);

	// Look first if one of the object with a ratio is already grouped
	// -> If yes, return the group
	cGroup* grp(0);
	Params->FindBestGroup<cGroup,cGroups>(obj->GetId(),Groups,grp);
	if(grp)
		return(grp);

	// Go through each groups
	R::RCursor<cGroup> Cur(Groups->Used);
	for(Cur.Start(),maxsim=-2.0;!Cur.End();Cur.Next())
	{
		// If all the hard constraints are not respected -> skip the group.
		if(!Cur()->CanInsert(obj))
			continue;

		// Compute best similarity with the objects already in the group.
		double sim(Cur()->GetLastMaxSim());
		if(sim>maxsim)
		{
			maxsim=sim;
			grp=Cur();
		}
	}

	// If no group find -> Reserve another one.
	if(!grp)
		grp=Groups->ReserveGroup();

	// Return the group.
	return(grp);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RHeuristicSG<cGroup,cObj,cGroups>::PostRun(void)
{
	R::RCursor<cGroup> Cur1(Groups->Used),Cur2(Groups->Used);

	// Look for the groups to delete
	ToDel.Clear();
	for(Cur1.Start();!Cur1.End();Cur1.Next())
	{
		// Verify if the number of objects is greater than the minimum
		if(Cur1()->GetNbObjs()>=Params->NbMinObjs)
		{
			// Look if this group contains only 1 social object
			if(Cur1()->GetNbObjs()!=1)
				continue;
			if(!Params->IsSocial(Cur1()->GetObjPos(0)->GetId()))
				continue;
		}

		// The current group must be emptied
		RCursor<cObj> Objs(Cur1()->GetObjs());
		ToReAssign.Clear();
		for(Objs.Start();!Objs.End();Objs.Next())
			ToReAssign.InsertPtr(Objs());
		Objs.Set(ToReAssign);
		for(Objs.Start();!Objs.End();Objs.Next())
		{
			// Try to find a new group for obj
			cGroup* grp(0);
			double max(-1.0);
			for(Cur2.Start();!Cur2.End();Cur2.Next())
			{
				if((Cur1()==Cur2())||(!Cur2()->GetNbObjs()))
					continue;
				if(Cur2()->HasSameUser(Objs()))
					continue;
				double tmp(Cur2()->ComputeRelSim(Objs()));
				if(tmp>max)
				{
					max=tmp;
					grp=Cur2();
				}
			}

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


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RHeuristicSG<cGroup,cObj,cGroups>::~RHeuristicSG(void)
{
}
