/*

	R Project Library

	RChromo2D.hh

	Chromosome for 2D placement GA - Inline Implementation

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
		RChromo2D(cInst *inst,size_t id)
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


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::RandomConstruct(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.GetWidth(),r.GetHeight());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Crossover(cChromo* parent1,cChromo* parent2)
{
	size_t NbRealInfos,i;
	RGeoInfo *info,*info1=0,*info2=0,*infoprob;
	double Distances;
	RPoint T(-Problem->Translation.X,-Problem->Translation.Y);

	if(Instance->Debug)
		Instance->Debug->BeginFunc("Crossover","RChromo2D");

	// Init
	NbRealInfos=0;
	memset(Selected,0,NbObjs*sizeof(bool));
	thInfos->Clear();
	ClearInfos();

	// Select the objects from the parents
	parent1->GetSetInfos(thObj1,Grid,Selected);
	if(Instance->Debug)
		Instance->Debug->PrintInfo(RString::Number(thObj1->GetNb())+" objects selected from "+RString::Number(parent1->Id));
	parent2->GetSetInfos(thObj2,Grid,Selected);
	if(Instance->Debug)
		Instance->Debug->PrintInfo(RString::Number(thObj2->GetNb())+" objects selected from "+RString::Number(parent2->Id));

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
	Heuristic->Run(Instance->Problem,thInfos,Grid);

	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.GetWidth(),r.GetHeight());

	// Assign the "real" geometric information
	for(i=0;i<NbRealInfos;i++)
	{
		info=GetPtr<size_t>(((*thInfos)[i])->GetObj()->GetId());
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

	if(Instance->Debug)
		Instance->Debug->EndFunc("Crossover","RChromo2D");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Mutation(void)
{
	memset(Selected,0,NbObjs*sizeof(bool));
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.GetWidth(),r.GetHeight());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Verify(void)
{
	size_t i;

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
					RString Tmp("Overlapping Problem (Id=="+RString::Number(Id)+") between "+infoi()->GetObj()->Name+" and "+infoj()->GetObj()->Name);
					if(Instance->Debug)
						Instance->Debug->PrintInfo(Tmp);
					throw RGAException(Tmp,RGAException::eGAVerify);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Copy(const cChromo &chromo)
{
	RChromo<cInst,cChromo,cFit,cThreadData>::Copy(chromo);
	RGeoInfos::operator=(chromo);
	memcpy(Selected,chromo.Selected,NbObjs*sizeof(bool));
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RObj2D* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetObj(tCoord X,tCoord Y)
{
	size_t obj;

	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=cNoRef)
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
	size_t obj;

	if((X>Limits.X)||(Y>Limits.Y)) return(0);
	obj=Grid->GetObjId(X,Y);
	if(obj!=cNoRef)
		//return(Infos[obj]);
		return((*this)[obj]);
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
/*template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLevel(size_t i)
{
	RPoint pt;

	pt=Levels[i];
	return(pt);
}
*/

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::~RChromo2D(void)
{
	if(Grid) delete Grid;
}
