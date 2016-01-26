/*

	R Project Library

	RChromo2D.hh

	Chromosome for 2D placement GA - Inline Implementation

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
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
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::RChromo2D(cInst *inst,size_t id)
		: RChromo<cInst,cChromo,cFit,cThreadData>(inst,id),
		  RLayout(inst->Problem,true),
		  Heuristic(0), Grid(new RGrid(inst->Problem->GetLimits())), Kernel(0)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		Init(cThreadData *thData)
{
	// Call the initialization of the parent
	RChromo<cInst,cChromo,cFit,cThreadData>::Init(thData);

	// Init "thread-dependent" data
	Selected=thData->Selected;
	Heuristic=thData->Heuristic;
	Kernel=&thData->Kernel;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::RandomConstruct(void)
{
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.GetWidth(),r.GetHeight());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Crossover(cChromo* parent1,cChromo* parent2)
{
	if(Instance->Debug)
		Instance->Debug->BeginFunc("Crossover","RChromo2D");

	// Initialization
	memset(Selected,0,Problem->GetNbObjs()*sizeof(bool));
	ClearInfos();

	// Select the objects from the parents (parent1 -> thObj1,parent2 -> thObj2);
	RObj2DContainer* thObj1(GetNewAggregator());
	parent1->FillAggregator(thObj1,Selected,Kernel,Instance->Random,Instance->GetProblem()->MustWeightedDistances());
	if(Instance->Debug)
		Instance->Debug->PrintInfo(RString::Number(thObj1->GetNbObjs())+" objects selected from "+RString::Number(parent1->Id));
	RObj2DContainer* thObj2(GetNewAggregator());
	parent2->FillAggregator(thObj2,Selected,Kernel,Instance->Random,Instance->GetProblem()->MustWeightedDistances());
	if(Instance->Debug)
		Instance->Debug->PrintInfo(RString::Number(thObj2->GetNbObjs())+" objects selected from "+RString::Number(parent2->Id));

	// Use the heuristic to place the two aggregators and the non-aggregated objects
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.GetWidth(),r.GetHeight());

	// Destroy the aggregators
	DestroyAggregators();

	// Compute all connections
	ComputeConnections(Instance->GetProblem()->MustWeightedDistances());

	if(Instance->Debug)
		Instance->Debug->EndFunc("Crossover","RChromo2D");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Mutation(void)
{
	memset(Selected,0,Problem->GetNbObjs()*sizeof(bool));
	Heuristic->Run(Instance->Problem,this,Grid);
	RRect r=Heuristic->GetResult();
	ActLimits.Set(r.GetWidth(),r.GetHeight());
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Verify(void)
{
	size_t i;

	RCursor<RGeoInfo> infoi(GetInfos());
	RCursor<RGeoInfo> infoj(GetInfos());
	for(i=0,infoi.Start();i<Problem->GetNbObjs()-1;i++,infoi.Next())
	{
		for(infoj.GoTo(i+1);!infoj.End();infoj.Next())
		{
			if((infoi()->IsValid())&&(infoj()->IsValid()))
			{
				if(infoi()->Overlap(infoj()))
				{
					RString Tmp("Overlapping Problem (Id=="+RString::Number(Id)+") between "+infoi()->GetObj()->GetName()+" and "+infoj()->GetObj()->GetName());
					if(Instance->Debug)
						Instance->Debug->PrintInfo(Tmp);
					mThrowRGAException("Verify",Tmp);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
  void RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::Copy(const cChromo &chromo)
{
	// Copy the information of the chromosome
	RChromo<cInst,cChromo,cFit,cThreadData>::Copy(chromo);
	ActLimits=chromo.ActLimits;

	// Clear the grid and copy the layout
	Grid->Clear();
	RLayout::Copy(chromo,Grid);

	// Remember the selection
	memcpy(Selected,chromo.Selected,Problem->GetNbObjs()*sizeof(bool));
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RObj2D* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetObj(tCoord X,tCoord Y)
{
	if((X>Problem->GetLimits().GetWidth())||(Y>Problem->GetLimits().GetHeight())) return(0);
	size_t obj(Grid->GetObjId(X,Y));
	if(obj!=cNoRef)
	{
		return((*this)[obj]->GetObj());
	}
	else
		return(0);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	cInfo* RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetInfo(tCoord X,tCoord Y)
{
	size_t obj;

	if((X>Problem->GetLimits().GetWidth())||(Y>Problem->GetLimits().GetHeight())) return(0);
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
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RChromo2D<cInst,cChromo,cFit,cThreadData,cInfo>::~RChromo2D(void)
{
	if(Grid) delete Grid;
}
