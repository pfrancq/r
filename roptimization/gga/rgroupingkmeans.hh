/*

	R Project Library

	RGroupingKMeans.hh

	k-Means Algorithm - Inline Implemenation

	Copyright 2003-2008 by the Université Libre de Bruxelles.

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
	R::RGroupingKMeans<cGroup,cObj,cGroups>::RGroupingKMeans(const R::RString& n,R::RRandom* r,R::RCursor<cObj> objs,R::RDebug* debug)
	: Name(n), Debug(debug), Rand(r), Objs(objs), RandObjects(0), Protos(40)
{
	Groups=0;
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double R::RGroupingKMeans<cGroup,cObj,cGroups>::ComputeSumSim(cGroup* group,cObj* obj)
{
	double Sum;
	double tmp;

	if(!group->GetNbObjs())
		return(0.0);
	R::RCursor<cObj> ptr(Groups->GetObjs(*group));
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
	cObj* R::RGroupingKMeans<cGroup,cObj,cGroups>::ComputePrototype(cGroup* group)

{
	double SumSim;
	cObj* Relevant;

	// If no objects -> No relevant one.
	if(!group->GetNbObjs())
		return(0);

	// Suppose the first object is the most relevant.
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
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::InitRandom(size_t nbclusters)
{
	cObj** ptr(RandObjects);
	size_t i,nbtoplace;
	double maxsim;
	cGroup* grp;

	if(nbclusters>Objs.GetNb())
		throw RException("KMeans : Cannot find valid prototypes");

	// Reserve the correct number of groups and allocate one object to each group
	Protos.Clear();
	Groups->ClearGroups();
	for(i=nbclusters+1,ptr=RandObjects,nbtoplace=0;--i;ptr++,nbtoplace++)
	{
		grp=Groups->ReserveGroup();
		grp->Insert(*ptr);
		Protos.InsertPtrAt(*ptr,grp->GetId(),true);
	}

	// Allocate the rest of the objects
	R::RCursor<cGroup> Grp(Groups->Used);
	for(;nbtoplace<Objs.GetNb();ptr++,nbtoplace++)
	{
		for(Grp.Start(),maxsim=-1.0,grp=0;!Grp.End();Grp.Next())
		{
			double sim=Similarity(*ptr,Protos[Grp()->GetId()]);
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
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::ReAllocate(void)
{
	double sim,maxsim;
	cGroup* grp;
	cObj** Cur;

	// Put the prototypes in each group
	R::RCursor<cGroup> Grp(*Groups);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		Groups->DeleteObjs(Grp());
		Grp()->Insert(Protos[Grp()->GetId()]);
	}

	// Go through the randomly ordered objects and put them in the group of the
	// most similar prototype.
	size_t nb;
	for(Cur=RandObjects,nb=Objs.GetNb()+1;--nb;Cur++)
	{
		// If the object is a prototype -> already in a group
		if(Protos.IsIn(*Cur))
			continue;

		// Find the group with the most similar prototype
		R::RCursor<cGroup> Grp(Groups->Used);
		for(Grp.Start(),maxsim=-1.0;!Grp.End();Grp.Next())
		{
			// If all the hard constraints are not respected -> skip the group.
			if(!Grp()->CanInsert(*Cur))
				continue;

			// Compute similarity with the relevant object of the group.
			sim=Similarity(Protos[Grp()->GetId()],*Cur);
			if(sim>maxsim)
			{
				maxsim=sim;
				grp=Grp();
			}
		}

		// If no group find -> Create a new group and make the current object the
		// prototype of it.
		if(!grp)
		{
			grp=Groups->ReserveGroup();
			Protos.InsertPtrAt(*Cur,grp->GetId());
		}

		// Insert the object in the current group.
		grp->Insert(*Cur);
	}
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	size_t R::RGroupingKMeans<cGroup,cObj,cGroups>::CalcNewProtosNb(void)
{
	// Computed the prototypes for each groups and count the number in Protos
	size_t count;
	R::RCursor<cGroup> Grp(Groups->Used);
	for(Grp.Start(),count=0;!Grp.End();Grp.Next())
	{
		cObj* OldProto(Protos[Grp()->GetId()]);
		cObj* obj=ComputePrototype(Grp());
		Protos.InsertPtrAt(obj,Grp()->GetId(),true);
		if(OldProto!=obj)
			count++;
	}
	return(count);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::DisplayInfos(void)
{
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::Run(cGroups* groups,size_t itermax,size_t nbclusters)
{
	// Verify that some objects must be grouped
	if(!Objs.GetNb())
		return;

	// Init part.
	Groups=groups;
	cObj** ptr;
	if(RandObjects)
	{
		delete[] RandObjects;
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
	R::RGroupingKMeans<cGroup,cObj,cGroups>::~RGroupingKMeans(void)
{
	if(RandObjects)
		delete[] RandObjects;
}
