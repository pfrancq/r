/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Implemenation

	(C) 1998-2001 by By P. Francq.

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
// include files for R Project
#include <rpromethee/rpromkernel.h>
using namespace RPromethee;
#include <rga2d/rplacementheuristic.h>
using namespace RGA2D;
#include <rmath/random.h>
using namespace RMath;



//-----------------------------------------------------------------------------
//
//	RPlacementHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RPlacementHeuristic::RPlacementHeuristic(unsigned int maxobjs,bool calc,bool use,bool ori)
	: Free(), CalcFree(calc), UseFree(calc&&use), AllOri(ori)
{
	Order=new unsigned int[maxobjs];
	MaxPromSol=500;
	Sols=new ObjectPos[MaxPromSol];
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::Init(RProblem2D *prob,RGeoInfo** infos,RGrid *grid)
{
	// Assign
	Limits=prob->Limits;
	Grid=grid;
	Infos=infos;
	NbObjs=prob->Objs.NbPtr;
	Connections=&prob->Cons;
	
	// Init the data for a placement
	NbObjsOk=0;
	Grid->Clear();
	Free.Clear();
	Result.Pt1.Set(0,0);
	Result.Pt2.Set(0,0);

	// Calculate an order
	for(unsigned int i=0;i<NbObjs;i++)
		Order[i]=i;
	RRandom::randorder<unsigned int>(Order,NbObjs);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::Init(RProblem2D *prob,RGeoInfos *infos,RGrid *grid)
{
	Init(prob,infos->Tab,grid);
}


//----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::SetDistParams(double p,double q,double w)
{
	DistParams.P=p;
	DistParams.Q=q;
	DistParams.Weight=w;
}


//-----------------------------------------------------------------------------	
void RGA2D::RPlacementHeuristic::SetDistParams(const RPromCriterionParams& params)
{
	DistParams=params;
}


//----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::SetAreaParams(const RPromCriterionParams& params)
{
	AreaParams=params;
}

		
//----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::SetAreaParams(double p,double q,double w)
{
	AreaParams.P=p;
	AreaParams.Q=q;
	AreaParams.Weight=w;
}

	
//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::SelectNextObject(void) throw(RPlacementHeuristicException)
{
	if(!NbObjsOk)
	{
		CurInfo=Infos[Order[0]];
		return;
	}
	
	// Find the most connected object
	CurInfo=Connections->GetMostConnected(Infos,NbObjs,Order,NbObjsOk);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::AddValidPosition(RPoint& pos)
{
	RRect CurRect(Result);
	ObjectPos *p;
	RPromSol *sol;
	unsigned int i;

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
	if(pos.X<CurRect.Pt1.X) CurRect.Pt1.X=pos.X;
	if(pos.Y<CurRect.Pt1.Y) CurRect.Pt1.Y=pos.Y;
	if(pos.X+CurInfo->Width()>CurRect.Pt2.X) CurRect.Pt2.X=pos.X+CurInfo->Width();
	if(pos.Y+CurInfo->Height()>CurRect.Pt2.Y) CurRect.Pt2.Y=pos.Y+CurInfo->Height();
	if((CurRect.Pt1.X<0)||(CurRect.Pt1.Y<0)||(CurRect.Pt2.X>=Limits.X)||(CurRect.Pt2.Y>=Limits.Y))
	{
		return;
	}
	
	// Add new solution for Prométhée
	p=&Sols[NbPromSol];
	p->Ori=CurInfo->GetOri();
	p->Pos=pos;
	sol=Prom->NewSol();
	Prom->Assign(sol,area,CurRect.Width()*CurRect.Height());
	Prom->Assign(sol,dist,Connections->GetDistances(Infos));
//	cout<<NbPromSol<<": Pos=("<<pos.X<<","<<pos.Y<<") ; Area="<<CurRect.Width()*CurRect.Height();
//	cout<<" ; Dist="<<Connections->GetDistances(Infos)<<endl;
	NbPromSol++;
}


//-----------------------------------------------------------------------------
RGeoInfo* RGA2D::RPlacementHeuristic::NextObject(void) throw(RPlacementHeuristicException)
{
	RPoint pos;
	RObj2D* obj;	
	unsigned int best;

	SelectNextObject();
	CurInfo->SetOrder(NbObjsOk);
	obj=CurInfo->GetObj();
	NbPromSol=0;

	// Choose best free polygon (if any).
	pos.Set(MaxCoord,MaxCoord);
	if(UseFree)
		pos=Free.CanPlace(CurInfo);

//	cout<<NbObjsOk<<endl;
	// Find the best position for current orientation if not already found.
	if(!pos.IsValid())
	{
		// Init Part
		Prom=new RPromKernel("Orientations",200,2);
		area=Prom->NewCriterion(Minimize,AreaParams);
		dist=Prom->NewCriterion(Minimize,DistParams);

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

		// Run Prométhée
		if(NbPromSol)
		{
			if(NbPromSol>1)
			{
				Prom->ComputePrometheeII();
				best=Prom->GetBestSolId();
//				cout<<endl<<"Best="<<best<<endl;
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
//	cout<<endl<<endl;

	// Place it	
	if(!pos.IsValid())
	{
		throw RPlacementHeuristicException("Can't place an object!");
	}
	Place(pos);
	CurInfo->Assign(pos);
	
	// Look for free polygons
	if(CalcFree)
		Grid->AddFreePolygons(CurInfo,&Free,Result);

	// Next Object
	NbObjsOk++;
	return(CurInfo);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::Run(RProblem2D* prob,RGeoInfo** infos,RGrid* grid) throw(RPlacementHeuristicException)
{
	Init(prob,infos,grid);
	while(NbObjsOk<NbObjs)
	{
		NextObject();
	}
	PostRun(Limits);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::Run(RProblem2D* prob,RGeoInfos* infos,RGrid* grid) throw(RPlacementHeuristicException)
{
	Run(prob,infos->Tab,grid);
}


//-----------------------------------------------------------------------------
RRect& RGA2D::RPlacementHeuristic::GetResult(void)
{
	RRect *rect=RRect::GetRect();
	
	(*rect)=Result;
	return(*rect);
}


//-----------------------------------------------------------------------------
RGA2D::RPlacementHeuristic::~RPlacementHeuristic(void)
{
	if(Order) delete[] Order;
}
