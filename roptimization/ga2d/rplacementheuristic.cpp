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
void RGA2D::RPlacementHeuristic::SelectNextObject(void)
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
RGeoInfo* RGA2D::RPlacementHeuristic::NextObject(void)
{
	RPoint pos;
	char nb;
	RObj2D* obj;	
	
	SelectNextObject();
	obj=CurInfo->GetObj();
	
	// Choose best free polygon (if any).
	pos.Set(MaxCoord,MaxCoord);
	if(UseFree)
		pos=Free.CanPlace(CurInfo);

  	// Find the best position for current orientation if not already found.	
	if(!pos.IsValid())		
	{
	   	if(AllOri&&(obj->NbPossOri>1))
   		{
   			RPromKernel Prom("Orientations",8,2);
	   		RPromCriterion *area,*dist;
   			RPromSol *sol;
   			char o;

	   		// Init Part
   			area=Prom.NewCriterion(Minimize,AreaParams);
   			dist=Prom.NewCriterion(Minimize,DistParams);
			nb=0;
			   		
	   		// Compute all orientations
   			for(o=0;o<obj->NbPossOri;o++)
   			{
	   			// Set an orientation & Find a position
	   			CurInfo->SetOri(o);
				pos=NextObjectOri();
   				if(pos.IsValid())
   				{
   					Ori[nb]=o;
	   				OriPos[nb]=pos;
   					OriResult[nb++]=Result;
   					CurInfo->Assign(pos);		// Temporary assignement
   					sol=Prom.NewSol();
	   				Prom.Assign(sol,area,Result.Width()*Result.Height());
   					Prom.Assign(sol,dist,Connections->GetDistances(Infos));
   				}
	   		}
   			
   			// Run Prométhée
	  		Prom.ComputePrometheeII();
  			nb=Prom.GetBestSolId();
			pos=OriPos[nb];	
			Result=OriResult[nb];
			CurInfo->SetOri(Ori[nb]);
	   	}
   		else
	   	{
   			// Set an orientation & Find a position
   			CurInfo->SetOri(0);
			pos=NextObjectOri();
	   	}
	}
			
	// Place it	
	if(!pos.IsValid())
	{
		cout<<"Problem"<<endl;
		throw;
	}
	Place(pos);

	// Look for free polygons
	if(CalcFree)
		Grid->AddFreePolygons(CurInfo,&Free,Result);

	// Next Object
	NbObjsOk++;
	return(CurInfo);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::Run(RProblem2D* prob,RGeoInfo** infos,RGrid* grid)
{
	Init(prob,infos,grid);
	while(NbObjsOk<NbObjs)
	{
		NextObject();
	}
	PostRun(Limits);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementHeuristic::Run(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
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
