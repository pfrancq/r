/*

	R Project Library

	RChromo2D.hh

	Chromosome for 2D placement GA - Inline Implementation

	(C) 1999-2001 by P. Francq.

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
// RChromo2D<cInst,cChromo,cFit,cInfo>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RChromo2D(cInst *inst,unsigned int id) throw(bad_alloc)
			: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
			  RGeoInfos(inst->Problem,true),
			  Heuristic(0), Grid(0),
			  Objs(0), NbObjs(0), thOrder(0), thObjs(0),
			  thObj1(0), thObj2(0), Limits()
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Init(cThreadData *thData) throw(bad_alloc)
{
	// Call the initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// General Initialisation
	Objs=Instance->Objs;
	NbObjs=Instance->NbObjs;

	// Init "thread-dependent" data
	thOrder=thData->Order;
	thOrder2=thData->Order2;
	thObjs=thData->tmpObjs;
	thObj1=thData->tmpObj1;
	thObj2=thData->tmpObj2;
	thInfos=thData->tmpInfos;
	Heuristic=thData->Heuristic;

	// If objects to place -> create the information and selection
	if(NbObjs)
	{
		// Create Selected
		Selected=new bool[NbObjs];
		memset(Selected,0,NbObjs*sizeof(bool));
	}

	// Init Limits
	Limits=Instance->GetLimits();

	// Init Occupied	(Add 1 Because <= Limits)
	Grid=new RGrid(Limits);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RandomConstruct(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Crossover(cChromo* parent1,cChromo* parent2)
{
	unsigned int NbRealInfos,i;
	RGeoInfo *info,*info1=0,*info2=0,*infoprob;
	double Distances;
	RPoint T(-Problem->Translation.X,-Problem->Translation.Y);

	#ifdef RGADEBUG
		char tmpmsg[100];
		if(Instance->Debug)
			Instance->Debug->BeginFunc("Crossover","RChromo2D");
	#endif

	// Init
	NbRealInfos=0;
	memset(Selected,0,NbObjs*sizeof(bool));
	thInfos->Clear();
	ClearInfos();

	// Select the objects from the parents
	parent1->GetSetInfos(thObj1,Grid,Selected);
	#ifdef RGADEBUG
		if(Instance->Debug)
		{
			sprintf(tmpmsg,"%u objects selected from %u",thObj1->NbPtr,parent1->Id);
			Instance->Debug->PrintInfo(tmpmsg);
		}
	#endif
	parent2->GetSetInfos(thObj2,Grid,Selected);
	#ifdef RGADEBUG
		if(Instance->Debug)
		{
			sprintf(tmpmsg,"%u objects selected from %u",thObj2->NbPtr,parent2->Id);
			Instance->Debug->PrintInfo(tmpmsg);
		}
	#endif

	// Add the geometric informations
	for(i=0;i<RealNb;i++)
	{
		if(!Selected[i])
		{
			NbRealInfos++;
			thInfos->InsertPtr(new cInfo(Objs[Tab[i]->GetObj()->GetId()]));
		}
	}
	thInfos->RealNb=NbRealInfos;
	if(thObj1->NbPtr)
	{
		thInfos->RealNb++;
		thInfos->InsertPtr(info1=new cInfo(thObj1));
	}
	if(thObj2->NbPtr)
	{
		thInfos->RealNb++;
		thInfos->InsertPtr(info2=new cInfo(thObj2));
	}
	thInfos->InsertPtr(new cInfo(&Problem->Problem));

	// Calculate Positions
	try
	{
		Heuristic->Run(Instance->Problem,thInfos,Grid);
	}
	catch(RPlacementHeuristicException& e)
	{
		return(false);
	}

	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());

	// Assign the "real" geometric information
	for(i=0;i<NbRealInfos;i++)
	{
		info=GetPtr<unsigned int>(thInfos->Tab[i]->GetObj()->GetId());
		(*info)=(*thInfos->Tab[i]);
	}
	if(info1)
		thObj1->Assign(this,info1->GetPos(),Grid,info1->GetOrder());
	if(info2)
		thObj2->Assign(this,info2->GetPos(),Grid,info2->GetOrder());
	infoprob=Tab[RealNb];
	infoprob->SetOri(0);
	infoprob->Assign(T);

	// Compute all connections
	Distances=0.0;
	Cons.UnComplete();
	for(Cons.Start();!Cons.End();Cons.Next())
	{
		Cons()->ComputeMinDist(this);
		Distances+=Cons()->Dist;
	}

	#ifdef RGADEBUG
		if(Instance->Debug)
			Instance->Debug->EndFunc("Crossover","RChromo2D");
	#endif

	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Mutation(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	bool RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Verify(void)
{
	unsigned int i,j;
	RGeoInfo **infoi,**infoj;
	char Tmp[200];

	for(i=0,infoi=Tab;i<NbObjs-1;i++,infoi++)
	{
		for(j=i+1,infoj=&Tab[i+1];j<NbObjs;j++,infoj++)
		{
			if(((*infoi)->IsValid())&&((*infoj)->IsValid()))
			{
				if((*infoi)->Overlap(*infoj))
				{
					cerr<<"Overlapping Problem: (Id=="<<Id<<")"<<endl<<"  "<<i<<" : ";
					for((*infoi)->Start();!(*infoi)->End();(*infoi)->Next())
						cerr<<"("<<((**infoi)()).X<<","<<((**infoi)()).Y<<")     ";
					cerr<<endl<<"  "<<j<<" : ";
					for((*infoj)->Start();!(*infoi)->End();(*infoj)->Next())
						cerr<<"("<<((**infoj)()).X<<","<<((**infoj)()).Y<<")     ";
					cerr<<endl;
					#ifdef RGADEBUG
						if(Instance->Debug)
						{
							sprintf(Tmp,"Overlapping Problem (Id==%u) between %s and %s",Id,(*infoi)->GetObj()->Name(),(*infoj)->GetObj()->Name());
							Instance->Debug->PrintInfo(Tmp);
						}
					#endif
					return(false);
				}
			}
		}
	}
	return(true);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>&
		RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::operator=(const RChromo2D &chromo)
{
	RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
	RGeoInfos::operator=(chromo);
	memcpy(Selected,chromo.Selected,NbObjs*sizeof(bool));
	return(*this);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RObj2D* RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetObj(RCoord X,RCoord Y)
{
	unsigned int obj;

	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=NoObject)
		return(Objs[obj]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	cInfo* RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetInfo(RCoord X,RCoord Y)
{
	unsigned int obj;

	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=NoObject)
		return(Infos[obj]);
	else
		return(0);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetActLimits(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=ActLimits;
	return(*pt);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLevel(unsigned int i)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Levels[i];
	return(*pt);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RGA2D::RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::~RChromo2D(void)
{
	if(Grid) delete Grid;
}
