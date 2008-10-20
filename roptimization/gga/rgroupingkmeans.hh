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
	: Name(n), Debug(debug), Rand(r), Objs(0), NbObjs(objs.GetNb()), ObjsUsed(0), NbObjsUsed(objs.GetNb()), Protos(40)
{
	cObj** ptr;
	ptr=Objs=ObjsUsed=new cObj*[NbObjs];
	for(objs.Start();!objs.End();objs.Next())
		(*(ptr++))=objs();
}


//---------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double R::RGroupingKMeans<cGroup,cObj,cGroups>::ComputeSumSim(cGroup* group,const cObj* obj)
{
	double Sum;

	if(!group->GetNbObjs())
		return(0.0);
	R::RCursor<cObj> ptr(Groups->GetObjs(*group));
	for(ptr.Start(),Sum=0.0;!ptr.End();ptr.Next())
	{
		if(ptr()==obj) continue;
		Sum+=Similarity(obj,ptr());
	}
	return(Sum);
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::ComputePrototype(cGroup* group)

{
	double SumSim;
	Centroid* Relevant(Protos[group->GetId()]);

	// If no objects -> No relevant one.
	if(!group->GetNbObjs())
		throw RException("KMeans : Cannot compute prototype for an empty group");

	// Suppose the first object is the most relevant.
	RCursor<cObj> ptr(Groups->GetObjs(*group));
	ptr.Start();
	Relevant->Obj=(ptr());
	if(group->GetNbObjs()==1)
	{
		Relevant->AvgSim=1.0;
		return;
	}
	Relevant->AvgSim=ComputeSumSim(group,ptr());

	// Look if in the other objects, there is a better one
	for(ptr.Next();!ptr.End();ptr.Next())
	{
		SumSim=ComputeSumSim(group,ptr());
		if(SumSim>Relevant->AvgSim)
		{
			Relevant->Obj=(ptr());
			Relevant->AvgSim=SumSim;
		}
	}
	Relevant->AvgSim/=(group->GetNbObjs()-1);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::InitRandom(size_t nb)
{
	size_t i;
	cGroup* grp;

	if(nb>NbObjsUsed)
		throw RException("KMeans : Cannot find valid prototypes");

	// Reserve the correct number of groups and allocate one object to each group
	cObj** ptr(ObjsUsed);
	Groups->ClearGroups();
	for(i=nb+1;--i;ptr++)
	{
		grp=Groups->ReserveGroup();
		grp->Insert(*ptr);
		Protos[grp->GetId()]->SetObj(*ptr);
	}
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::InitRefine(size_t nb,size_t max,size_t nbsub,int level)
{
	Group* g;
	double cost,bestcost(-2.0);
	Group* Final;
	size_t NbObjsSub(static_cast<size_t>(NbObjs*level/100));

	while(NbObjsSub*nbsub>NbObjs)
		nbsub--;
	if((!nbsub)||(nb>NbObjsSub))
		throw RException("KMeans : Cannot refine the centers");

	R::RContainer<Group,true,false> SubSamples(nbsub);
	R::RContainer<Group,true,false> InitialCenters(nbsub);
	R::RContainer<cObj,false,false> AllCenters(nbsub*NbObjsSub);
	cObj** Tab=new cObj*[NbObjsSub];

	// Run 'nbsub' kMeans and remember all centroid
	cObj** ptr(Objs);
	for(size_t k=0;k<nbsub;k++)
	{
		std::cout<<k<<" sub-kMeans"<<std::endl;

		// Construct the samples
		g=new Group(k,NbObjsSub);
		for(size_t i=NbObjsSub+1;--i;ptr++)
			g->InsertPtr(*ptr);
		SubSamples.InsertPtr(g);
		NbObjsUsed=NbObjsSub;
		g->GetTab(Tab,NbObjsSub);
		ObjsUsed=Tab;

		// Initialize randomly
		InitRandom(nb);
		DokMeans(max);

		// Save the centers
		g=new Group(k,nb);
		InitialCenters.InsertPtr(g);
		RCursor<Centroid> Cur(Protos);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			g->InsertPtr(Cur()->Obj);
			AllCenters.InsertPtr(Cur()->Obj);
		}
	}
	delete[] Tab;

	// Executes a kMeans over all the estimates of the centroids
	NbObjsUsed=NbObjsSub*nbsub;
	ObjsUsed=Objs;
	R::RCursor<Group> Cur(InitialCenters);
	size_t i;
	for(Cur.Start(),i=0;!Cur.End();Cur.Next(),i++)
	{
		std::cout<<i<<" kMeans with centroid"<<std::endl;
		// Initializing the prototypes with a set of found initial centers
		Groups->ClearGroups();
		R::RCursor<cObj> Cur2(*Cur());
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			cGroup* grp=Groups->ReserveGroup();
			grp->Insert(Cur2());
			Protos[grp->GetId()]->SetObj(Cur2());
		}

		// Executes a kMeans over all the centers with these initial centers
		DokMeans(max);

		// Evaluate the distortion
		cost=Fitness();
		std::cout<<i<<" : cost="<<cost<<std::endl;
		if(cost>bestcost)
		{
			bestcost=cost;
			Final=Cur();
		}
	}

	// Set the final initial centers as prototypes.
	Groups->ClearGroups();
	R::RCursor<cObj> Cur2(*Final);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
	{
		cGroup* grp=Groups->ReserveGroup();
		grp->Insert(Cur2());
		Protos[grp->GetId()]->SetObj(Cur2());
	}
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::InitkMeansPlusPlus(size_t nb)
{
	if(nb>NbObjsUsed)
		throw RException("KMeans : Cannot find valid prototypes");

	// Init part
	cObj** ptr(ObjsUsed);
	size_t nbobjs(NbObjsUsed);
	double* Probas=new double[NbObjsUsed-1];

	// Choose as first prototype, a random object
	cGroup* grp=Groups->ReserveGroup();
	grp->Insert(*ptr);
	Protos[grp->GetId()]->SetObj(*ptr);

	// Find the rest of the centroids
	for(size_t used=nb;--used;)
	{
		std::cout<<used<<" centroids left to find"<<std::endl;

		// Skip the next object
		ptr++;
		nbobjs--;

		// For each object, compute the maximal similarity with the centroids
		double total;
		cObj** left;
		double* obj;
		size_t nbleft;
		for(total=0.0,left=ptr,nbleft=nbobjs+1,obj=Probas;--nbleft;left++,obj++)
		{
			// Compute the maximal similarity with the centroids
			double maxsim=-2.0;
			RCursor<Centroid> Cur(Protos);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				if(!Cur()->Obj)
					continue;
				double sim=Similarity(*left, Cur()->Obj);
				if(sim<0.0)
					sim=0.0;
				if(sim>maxsim)
					maxsim=sim;
			}
			(*obj)=maxsim;
			total+=maxsim;
		}

		// Find a random number in [0,total]
		double rand=Rand->GetValue()*total;

		// Find the object having the right similarity
		for(total=0.0,left=ptr,nbleft=nbobjs,obj=Probas;--nbleft;left++,obj++)
		{
			total+=(*obj);
			if((total<=rand)&&(total+(*(obj+1))>rand))
				break;
		}
		if(!nbleft)
			left++;

		// left points to the new centroid
		cObj* newcentroid=(*left);
		(*left)=(*ptr);
		(*ptr)=newcentroid;
		grp=Groups->ReserveGroup();
		grp->Insert(*ptr);
		Protos[grp->GetId()]->SetObj(*ptr);
	}

	// Delete
	delete[] Probas;
}


//-----------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::ReAllocate(void)
{
	double sim,maxsim;
	cGroup* grp;
	cObj** Cur;

	// Put the prototypes in each group
	Groups->ClearGroups();
	R::RCursor<Centroid> Grp(Protos);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		cGroup* grp=Groups->ReserveGroup();
		grp->Insert(Grp()->Obj);
	}

	// Go through the randomly ordered objects and put them in the group of the
	// most similar prototype.
	size_t nb;
	for(Cur=ObjsUsed,nb=NbObjsUsed+1;--nb;Cur++)
	{
		// If the object is a prototype -> already in a group
		if(Protos.IsIn(*Cur))
			continue;

		// Find the group with the most similar prototype
		R::RCursor<cGroup> Grp(Groups->Used);
		for(Grp.Start(),maxsim=-2.0,grp=0;!Grp.End();Grp.Next())
		{
			// If all the hard constraints are not respected -> skip the group.
			if(!Grp()->CanInsert(*Cur))
				continue;

			// Compute similarity with the relevant object of the group.
			sim=Similarity(Protos[Grp()->GetId()]->Obj,*Cur);
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
			std::cout<<"Group number will changed"<<std::endl;
			grp=Groups->ReserveGroup();
			Protos.InsertPtr(new Centroid());
			Protos[grp->GetId()]->SetObj(*Cur);
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
		cObj* OldProto(Protos[Grp()->GetId()]->Obj);
		ComputePrototype(Grp());
		if(OldProto!=Protos[Grp()->GetId()]->Obj)
			count++;
	}
	return(count);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	double RGroupingKMeans<cGroup,cObj,cGroups>::Fitness(void)
{
	if(!Groups->Used.GetNb())
		return(0.0);

	double J(0.0);
	double max(-2.0),tmp;
	RCursor<Centroid> Grp(Protos);
	RCursor<Centroid> Grp2(Protos);
	for(Grp.Start();!Grp.End();Grp.Next())
	{
		J+=Grp()->AvgSim;
		double avg(0.0);
		for(Grp2.Start();!Grp2.End();Grp2.Next())
		{
			if(Grp()==Grp2()) continue;
			tmp=Similarity(Grp()->Obj,Grp2()->Obj);
			avg+=tmp;
		}
		avg/=static_cast<double>(Protos.GetNb()-1);
		if(avg>max)
			max=avg;
	}
	return(J/(Protos.GetNb()*max));
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::DokMeans(size_t max)
{
	double minerror(0.0);
	double error(1.0);
	for(NbIterations=0,error=1.0;((!max)||(max&&(NbIterations<max)))&&(error>minerror);NbIterations++)
	{
		ReAllocate();
		error=static_cast<double>(CalcNewProtosNb())/static_cast<double>(Protos.GetNb());
		std::cout<<"Iterations="<<NbIterations<<": "<<Fitness()<<std::endl;
	}
}



//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	void R::RGroupingKMeans<cGroup,cObj,cGroups>::Run(cGroups* groups,size_t max,size_t nb,tInitial start)
{
	// Verify that some objects must be grouped
	if(!NbObjs)
		return;

	// Initialize randomly
	Groups=groups;
	Rand->RandOrder(Objs,NbObjs);

	// Create the prototypes
	Protos.Clear();
	for(size_t i=nb+1;--i;)
		Protos.InsertPtr(new Centroid());

	// Initialization
	switch(start)
	{
		case Random:
			InitRandom(nb);
			break;
		case Refine:
			InitRefine(nb,max,5,5);
			break;
		case kMeansPlusPlus:
			InitkMeansPlusPlus(nb);
			break;
		case Incremental:
			R::RCursor<cGroup> Grp(Groups->Used);
			for(Grp.Start();!Grp.End();Grp.Next())
				ComputePrototype(Grp());
			break;
	}

	std::cout<<"Do Main kMeans"<<std::endl;
	ObjsUsed=Objs;
	NbObjsUsed=NbObjs;
	DokMeans(max);
}


//------------------------------------------------------------------------------
template<class cGroup,class cObj,class cGroups>
	R::RGroupingKMeans<cGroup,cObj,cGroups>::~RGroupingKMeans(void)
{
	if(Objs)
		delete[] Objs;
}
