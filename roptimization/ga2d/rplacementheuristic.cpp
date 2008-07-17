/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Implemenation

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
// include files for R Project
#include <rpromkernel.h>
#include <rplacementheuristic.h>
#include <random.h>
#include <rxmlstruct.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPlacementHeuristic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementHeuristic::RPlacementHeuristic(size_t maxobjs,bool calc,bool use,RRandom* r,bool ori)
	: Random(r), Free(), CalcFree(calc), UseFree(calc&&use), AllOri(ori)
{
	Order=new RGeoInfo*[maxobjs];
	MaxPromSol=500;
	Sols=new ObjectPos[MaxPromSol];
	AreaParams=RPromLinearCriterion::CreateParam("Area");
	DistParams=RPromLinearCriterion::CreateParam("Dist");	
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	// Assign
	Limits=prob->Limits;
	Grid=grid;
	Infos=infos;
	NbObjs=Infos->RealNb;
	Connections=&prob->Cons;

	// Init the data for a placement
	NbObjsOk=0;
	Grid->Clear();
	Free.Clear();
	Result.Set(0,0,0,0);
	Distances=0.0;

	// Calculate an order
	for(size_t i=0;i<NbObjs;i++)
		Order[i]=((*Infos)[i]);
	Random->RandOrder<RGeoInfo*>(Order,NbObjs);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SetDistParams(double p,double q,double w)
{
	dynamic_cast<RParamStruct*>(DistParams)->Get<RParamValue>("P")->SetDouble(p);
	dynamic_cast<RParamStruct*>(DistParams)->Get<RParamValue>("Q")->SetDouble(q);
	dynamic_cast<RParamStruct*>(DistParams)->Get<RParamValue>("Weight")->SetDouble(w);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SetDistParams(RParam* params)
{
	DistParams=params;
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SetAreaParams(RParam* params)
{
	AreaParams=params;
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SetAreaParams(double p,double q,double w)
{
	dynamic_cast<RParamStruct*>(AreaParams)->Get<RParamValue>("P")->SetDouble(p);
	dynamic_cast<RParamStruct*>(AreaParams)->Get<RParamValue>("Q")->SetDouble(q);
	dynamic_cast<RParamStruct*>(AreaParams)->Get<RParamValue>("Weight")->SetDouble(w);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SelectNextObject(void)
{
	if(!NbObjsOk)
	{
		CurInfo=Order[0];
		return;
	}

	// Find the most connected object
	CurInfo=Connections->GetMostConnected(Infos,NbObjs,Order,NbObjsOk);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::AddValidPosition(RPoint& pos)
{
	RRect CurRect(Result);
	ObjectPos *p;
	RPromSol* sol;
	size_t i;

	// Verify if solution not already exists
	for(i=NbPromSol+1,p=Sols;--i;p++)
		if((p->Pos==pos)&&(p->Ori==CurInfo->GetOri()))
			return;

	// Verify CalcPos
	if(NbPromSol==MaxPromSol)
	{
		// Allocate new Sols
	}

	// Compute the bounding rectangle of all placed objects and the current
	// one at the given position
	if(pos.X<CurRect.X1) CurRect.X1=pos.X;
	if(pos.Y<CurRect.Y1) CurRect.Y1=pos.Y;
	if(pos.X+CurInfo->Width()>CurRect.X2) CurRect.X2=pos.X+CurInfo->Width();
	if(pos.Y+CurInfo->Height()>CurRect.Y2) CurRect.Y2=pos.Y+CurInfo->Height();
	if((CurRect.X1<0)||(CurRect.Y1<0)||(CurRect.X2>=Limits.X)||(CurRect.Y2>=Limits.Y))
	{
		return;
	}

	// Add new solution for Proméhée
	p=&Sols[NbPromSol];
	p->Ori=CurInfo->GetOri();
	p->Pos=pos;
	sol=Prom->NewSol();
	Prom->Assign(sol,area,CurRect.GetWidth()*CurRect.GetHeight());
	Prom->Assign(sol,dist,Infos->Cons.GetDistances(CurInfo,pos));
//	cout<<NbPromSol<<": Pos=("<<pos.X<<","<<pos.Y<<") ; Area="<<CurRect.Width()*CurRect.Height();
//	cout<<" ; Dist="<<Connections->GetDistances(Infos)<<endl;
	NbPromSol++;
}


//------------------------------------------------------------------------------
RGeoInfo* RPlacementHeuristic::NextObject(void)
{
	RPoint pos;
	RObj2D* obj;
	size_t best;

	SelectNextObject();
	CurInfo->SetOrder(NbObjsOk);
	obj=CurInfo->GetObj();
	NbPromSol=0;

	// Choose best free polygon (if any).
	pos.Set(MaxCoord,MaxCoord);
	if(UseFree)
		pos=Free.CanPlace(CurInfo);

	// Find the best position for current orientation if not already found.
	if(!pos.IsValid())
	{
		// Init Part
		Prom=new RPromKernel("Orientations",200,2);
		Prom->AddCriterion(area=new RPromLinearCriterion(RPromCriterion::Maximize,AreaParams,"Weight"));
		Prom->AddCriterion(dist=new RPromLinearCriterion(RPromCriterion::Minimize,DistParams,"Distance"));
		
		if(AllOri&&(obj->NbPossOri>1))
		{
			// Compute all orientations
			for(char o=0;o<obj->NbPossOri;o++)
			{
				// Set an orientation & Find a position
				CurInfo->SetOri(o);
				NextObjectOri();
			}
		}
		else
		{
			// Set an orientation & Find a position
			CurInfo->SetOri(0);
			NextObjectOri();
		}

		// Run Proméhéé
		if(NbPromSol)
		{
			if(NbPromSol>1)
			{
				Prom->ComputePrometheeII();
				best=Prom->GetBestSolId();
				pos=Sols[best].Pos;
				CurInfo->SetOri(Sols[best].Ori);
			}
			else
			{
				pos=Sols[0].Pos;
				CurInfo->SetOri(Sols[0].Ori);
			}
		}
		delete Prom;
	}

	// Place it
	if(!pos.IsValid())
	{
		throw RGA2DException("Can't place an object!");
	}
	Place(pos);

	// Look for free polygons
	if(CalcFree)
	{
		if(NbObjsOk==26)
		{
			RTrace* m=RTrace::LookMsg("Random");
			if(m)
			{
				RTrace::DeleteMsg(m);
				RTrace::InsertMsg("Debug");
			}
		}
		Grid->AddFreePolygons(CurInfo,&Free,Result);
	}

	// Next Object
	NbObjsOk++;
	return(CurInfo);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::Run(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	Init(prob,infos,grid);
	while(NbObjsOk<NbObjs)
	{
		NextObject();
	}
	PostRun(Limits);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::PostRun(RPoint&)
{
	// Compute all connections part again
	Distances=0.0;
	Infos->Cons.UnComplete();
	RCursor<RGeoInfoConnection> Cons(Infos->Cons);
	for(Cons.Start();!Cons.End();Cons.Next())
	{
		Cons()->ComputeMinDist(Infos);
		Distances+=Cons()->Dist;
	}
}


//------------------------------------------------------------------------------
RRect RPlacementHeuristic::GetResult(void)
{
	return(Result);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::CreateProblem(void)
{

}


//------------------------------------------------------------------------------
RPlacementHeuristic::~RPlacementHeuristic(void)
{
	delete[] Order;
	delete DistParams;
	delete AreaParams;
}
