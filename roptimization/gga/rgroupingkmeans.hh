/*

	R Project Library

	RGroupingKMeans.hh

	k-Means Algorithm - Inline Implemenation

	Copyright 2003-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
// class RGroupingKMeans<cGroup,cObj,cGroups>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupingKMeans<cGroup,cObj,cGroups>::RGroupingKMeans(const RString& n, RRandom* r,RCursor<cObj> objs,RDebug* debug)
	: Name(n), Debug(debug), Rand(r), Objs(objs), RandObjects(0), Protos(40)
{
	Groups=0;
}
		
	
//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroups>::ComputeSumSim(cGroup* group,cObj* obj)
{
	double Sum;
	double tmp;

	if(!group->GetNbObjs())
		return(0.0);
	RCursor<cObj> ptr(Groups->GetObjs(*group));
	for(ptr.Start(),Sum=0.0;!ptr.End();ptr.Next())
	{
		if(ptr()==obj) continue;
		tmp=Similarity(obj,ptr());
		Sum+=tmp*tmp;
	}
	return(Sum);
}
	

//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	cObj* RGroupingKMeans<cGroup,cObj,cGroups>::ComputePrototype(cGroup* group)
 
{
	double SumSim;
	cObj* Relevant;
	
	// If no objects -> No relevant one.
	if(!group->GetNbObjs())
		return(0);

	// Suppose the first element is the most relevant.
	RCursor<cObj> ptr(Groups->GetObjs(*group));
	ptr.Start();
	Relevant=(ptr());
	double BestSumSim=ComputeSumSim(group,ptr());

	// Look if in the other objects, there is a better one
	for(ptr.Next();!ptr.End();ptr.Next())
	{
		SumSim=ComputeSumSim(group,ptr());
		if(SumSim>BestSumSim)
		{
			Relevant=(ptr());
			BestSumSim=SumSim;
		}
	}
	return(Relevant);	
}
		
		
//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroups>::InitRandom(size_t nbclusters)
{
	cObj** ptr;
	size_t i,nbtoplace;
	double maxsim;
	cGroup* grp;
	
	if(nbclusters>Objs.GetNb())
		throw RException("KMeans : Cannot find valid prototypes");	
	
	Protos.Clear();

	// Reserve the correct number of groups and allocate one object to each group
	Groups->ClearGroups();
	for(i=nbclusters+1,ptr=RandObjects,nbtoplace=0;--i;ptr++,nbtoplace++)
	{
		grp=Groups->ReserveGroup();
		grp->Insert(*ptr);
		Protos.InsertPtr(*ptr);
	}
	
	// Allocate the rest of the objects
	R::RCursor<cGroup> Grp(Groups->Used);
	R::RCursor<cObj> Proto(Protos);
	for(;nbtoplace<Objs.GetNb();ptr++,nbtoplace++)
	{		
		for(Grp.Start(),Proto.Start(),maxsim=-1.0,grp=0;!Grp.End();Grp.Next(),Proto.Next())
		{
			double sim=Similarity(*ptr,Proto());
			if(sim>maxsim)
			{
				maxsim=sim;
				grp=Grp();
			}
		}
		grp->Insert(*ptr);
	}
}

	
//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroups>::ReAllocate(void)
{
	double sim,maxsim;
	cGroup* grp;
	cObj** Cur;

	// Put the prototypes in Protos
	Protos.Clear();
	R::RCursor<cGroup> Grp(*Groups);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		cObj* obj=ComputePrototype(Grp());
		Protos.InsertPtr(obj);
	}

	// Clear the groups
	Groups->ClearGroups();

	// Insert the Prototypes in a group
	RCursor<cObj> CurP(Protos);
	for(CurP.Start();!CurP.End();CurP.Next())
		Groups->ReserveGroup()->Insert(CurP());

	// Go through the ranbomly ordered subprofiles and put them in the group of the
	// most similar prototype.
	unsigned nb;
	for(Cur=RandObjects,nb=Objs.GetNb()+1;--nb;Cur++)
	{
		// If the subprofile is a prototype -> already in a group
		if(Protos.IsIn(*Cur))
			continue;

		// Find the group with the most similar prototype
		R::RCursor<cGroup> Grp(Groups->Used);
		for(Grp.Start(),CurP.Start(),maxsim=-1.0;!Grp.End();Grp.Next(),CurP.Next())
		{
			// If all the hard constraints are not respected -> skip the group.
			if(!Grp()->CanInsert(*Cur))
				continue;

			// Compute similarity with the relevant profile of the group.
			sim=Similarity(CurP(),*Cur);
			if(sim>maxsim)
			{
				maxsim=sim;
				grp=Grp();
			}
		}

		// If no group find -> Create a new group and make the current subprofile the
		// prototype of it.
		if(!grp)
		{
			grp=Groups->ReserveGroup();
			Protos.InsertPtrAt(*Cur,grp->GetId());						
		}

		// Insert the subprofile in the current group.
		grp->Insert(*Cur);
	}
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	unsigned int RGroupingKMeans<cGroup,cObj,cGroups>::CalcNewProtosNb(void)
{
	unsigned int count;
	R::RCursor<cGroup> Grp;
	cObj* OldProto;

	// Computed the prototypes for each groups and count the number in Protos
	Grp.Set(Groups->Used);
	RCursor<cObj> CurP(Protos);
	for(Grp.Start(),CurP.Start(),count=0;!Grp.End();Grp.Next(),CurP.Next())
	{
		OldProto=CurP();
		cObj* obj=ComputePrototype(Grp());
		Protos.InsertPtrAt(obj,Grp()->GetId(),true);
		if(OldProto!=obj)
			count++;
	}
	return(count);
}	

	
//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroups>::DisplayInfos(void)
{
}

	
//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void RGroupingKMeans<cGroup,cObj,cGroups>::Run(cGroups* groups,unsigned int itermax,size_t nbclusters)
{
	// Verify that some objects must be grouped
	if(!Objs.GetNb())
		return;

	// Init part.
	Groups=groups;
	cObj** ptr;
	if(RandObjects)
	{
		delete RandObjects;
		RandObjects=0;
	}
	ptr=RandObjects=new cObj*[Objs.GetNb()];
	for(Objs.Start();!Objs.End();Objs.Next())
	 (*(ptr++))=Objs();
	Rand->RandOrder(RandObjects,Objs.GetNb());
	
	// Initialization 		
	InitRandom(nbclusters);
		
	// Max Iterations
	double minerror=0.0;
	double error;
	for(NbIterations=0,error=1.0;(NbIterations<itermax)&&(error>minerror);NbIterations++)
	{
		ReAllocate();
		error=static_cast<double>(CalcNewProtosNb())/static_cast<double>(Protos.GetNb());
	}	
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	RGroupingKMeans<cGroup,cObj,cGroups>::~RGroupingKMeans(void)
{
	delete RandObjects;
}
