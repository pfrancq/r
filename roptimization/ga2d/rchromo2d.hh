/*

	R Project Library

	RChromo2D.hh

	Chromosome for 2D placement GA - Inline Implementation

	Copyright 1999-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// RChromo2D<cInst,cChromo,cFit,cInfo>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RChromo2D(cInst *inst,unsigned int id)
			: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
			  RGeoInfos(inst->Problem,true),
			  Heuristic(0), Grid(0), Objs(),
			  NbObjs(0), thOrder(0), thObjs(0),
			  thObj1(0), thObj2(0), Limits()
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Init(cThreadData *thData)
{
	// Call the initialisation of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// General Initialisation
	Objs=this->Instance->Objs;
	NbObjs=this->Instance->NbObjs;

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
	Limits=this->Instance->GetLimits();

	// Init Occupied	(Add 1 Because <= Limits)
	Grid=new RGrid(Limits);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::RandomConstruct(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(this->Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Crossover(cChromo* parent1,cChromo* parent2)
{
	unsigned int NbRealInfos,i;
	RGeoInfo *info,*info1=0,*info2=0,*infoprob;
	double Distances;
	RPoint T(-Problem->Translation.X,-Problem->Translation.Y);

	#ifdef RGADEBUG
		char tmpmsg[100];
		if(this->Instance->Debug)
			this->Instance->Debug->BeginFunc("Crossover","RChromo2D");
	#endif

	// Init
	NbRealInfos=0;
	memset(Selected,0,NbObjs*sizeof(bool));
	thInfos->Clear();
	ClearInfos();

	// Select the objects from the parents
	parent1->GetSetInfos(thObj1,Grid,Selected);
	#ifdef RGADEBUG
		if(this->Instance->Debug)
		{
			sprintf(tmpmsg,"%u objects selected from %u",thObj1->GetNb(),parent1->Id);
			this->Instance->Debug->PrintInfo(tmpmsg);
		}
	#endif
	parent2->GetSetInfos(thObj2,Grid,Selected);
	#ifdef RGADEBUG
		if(this->Instance->Debug)
		{
			sprintf(tmpmsg,"%u objects selected from %u",thObj2->GetNb(),parent2->Id);
			this->Instance->Debug->PrintInfo(tmpmsg);
		}
	#endif

	// Add the geometric informations
	for(i=0;i<RealNb;i++)
	{
		if(!Selected[i])
		{
			NbRealInfos++;
			Objs.GoTo(((*this)[i])->GetObj()->GetId());
			thInfos->InsertPtr(new cInfo(Objs()));
		}
	}
	thInfos->RealNb=NbRealInfos;
	if(thObj1->GetNb())
	{
		thInfos->RealNb++;
		thInfos->InsertPtr(info1=new cInfo(thObj1));
	}
	if(thObj2->GetNb())
	{
		thInfos->RealNb++;
		thInfos->InsertPtr(info2=new cInfo(thObj2));
	}
	thInfos->InsertPtr(new cInfo(&Problem->Problem));

	// Calculate Positions
	Heuristic->Run(this->Instance->Problem,thInfos,Grid);

	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());

	// Assign the "real" geometric information
	for(i=0;i<NbRealInfos;i++)
	{
		info=GetPtr<unsigned int>(((*thInfos)[i])->GetObj()->GetId());
		(*info)=(*((*thInfos)[i]));
	}
	if(info1)
		thObj1->Assign(this,info1->GetPos(),Grid,info1->GetOrder());
	if(info2)
		thObj2->Assign(this,info2->GetPos(),Grid,info2->GetOrder());
	infoprob=(*this)[RealNb];
	infoprob->SetOri(0);
	infoprob->Assign(T);

	// Compute all connections
	Distances=0.0;
	Cons.UnComplete();
	RCursor<RGeoInfoConnection> Cur(Cons);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->ComputeMinDist(this);
		Distances+=Cur()->Dist;
	}

	#ifdef RGADEBUG
		if(this->Instance->Debug)
			this->Instance->Debug->EndFunc("Crossover","RChromo2D");
	#endif
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Mutation(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(this->Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.Width(),r.Height());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Verify(void)
{
	unsigned int i;
	char Tmp[200];

	RCursor<RGeoInfo> infoi(*this);
	RCursor<RGeoInfo> infoj(*this);
	for(i=0,infoi.Start();i<NbObjs-1;i++,infoi.Next())
	{
		for(infoj.GoTo(i+1);!infoj.End();infoj.Next())
		{
			if((infoi()->IsValid())&&(infoj()->IsValid()))
			{
				if(infoi()->Overlap(infoj()))
				{
					sprintf(Tmp,"Overlapping Problem (Id==%u) between %s and %s",this->Id,infoi()->GetObj()->Name.Latin1(),infoj()->GetObj()->Name.Latin1());
					#ifdef RGADEBUG
						if(this->Instance->Debug)
							this->Instance->Debug->PrintInfo(Tmp);
					#endif
					throw eGAVerify(Tmp);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>&
		RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::operator=(const RChromo2D &chromo)
{
	RChromo<cInst,cChromo,cFit,cThreadData>::operator=(chromo);
	RGeoInfos::operator=(chromo);
	memcpy(Selected,chromo.Selected,NbObjs*sizeof(bool));
	return(*this);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RObj2D* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetObj(tCoord X,tCoord Y)
{
	unsigned int obj;

	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=NoObject)
	{
		Objs.GoTo(obj);
		return(Objs());
	}
	else
		return(0);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	cInfo* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetInfo(tCoord X,tCoord Y)
{
	unsigned int obj;

	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=NoObject)
		return(this->Infos[obj]);
	else
		return(0);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetActLimits(void)
{
	RPoint pt;

	pt=ActLimits;
	return(pt);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLevel(unsigned int i)
{
	RPoint pt;

	pt=this->Levels[i];
	return(pt);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::~RChromo2D(void)
{
	if(Grid) delete Grid;
}
