/*

	R Project Library

	RPlacementCenter.cpp

	Center Heuristic for Placement - Implemenation

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
#include <rga2d/rplacementcenter.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
//
//	RPlacementCenter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RPlacementCenter::RPlacementCenter(unsigned int maxobjs,bool calc,bool use,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,ori), CalcPos(0)
{
	MaxCalcPos=200;
	CalcPos=new RPoint[MaxCalcPos];
	AreaParams.P=AreaParams.Q=DistParams.P=DistParams.Q=0.0;
	AreaParams.Weight=2.0;
	DistParams.Weight=1.0;	
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementCenter::Init(RProblem2D* prob,RGeoInfo** infos,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,infos,grid);
	HoldLimits=Limits;
	Max=Limits+Limits;
	//Limits=Max;
	Attraction.Set(Max.X/2,Max.Y/2);
	Union.Clear();
	Sol.Clear();
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementCenter::Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	Init(prob,infos->Tab,grid);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementCenter::AddPosition(RPromKernel& k,RPromCriterion *area,RPromCriterion *dist,RPoint& pos)
{
  	RPoint Center;
  	RRect CurRect(Result);
  	RPromSol *sol;
  	 	
  	// Push it to the center
  	CurInfo->PushCenter(pos,Max,Grid);
  	
  	// Verify the the position is a valid one (in the limits)
  	if((pos.X<0)||(pos.X+CurInfo->Width()>=Max.X)||(pos.Y<0)||(pos.Y+CurInfo->Height()>=Max.Y))	return;
  	
  	// Verify CalcPos
  	if(NbCalcPos==MaxCalcPos)
  	{
  		// Allocate new CalcPos
  	}
  	
  	// Add new solution for Prométhée
	CalcPos[NbCalcPos]=pos;
	CurInfo->Assign(pos);	// Temporary assignment
  	Center.Set(CurInfo->Width()/2,CurInfo->Height()/2);
	if(pos.X<CurRect.Pt1.X) CurRect.Pt1.X=pos.X;
  	if(pos.Y<CurRect.Pt1.Y) CurRect.Pt1.Y=pos.Y;
  	if(pos.X+CurInfo->Width()>CurRect.Pt2.X) CurRect.Pt2.X=pos.X+CurInfo->Width();
  	if(pos.Y+CurInfo->Height()>CurRect.Pt2.Y) CurRect.Pt2.Y=pos.Y+CurInfo->Height();
  	if((CurRect.Width()>Max.X/2)||(CurRect.Height()>Max.Y/2))
  		return;
  	sol=k.NewSol();
  	k.Assign(sol,area,CurRect.Width()*CurRect.Height());
	k.Assign(sol,dist,Connections->GetDistances(Infos));
  	NbCalcPos++;
}


//-----------------------------------------------------------------------------
RPoint& RGA2D::RPlacementCenter::NextObjectOri(void)
{
	RPoint Pos;									// Position to test (X,Y).
	RPoint *Best=RPoint::GetPoint(); 	// Temporary point to hold best position.
	unsigned int NbPos;
	RPoint *Actual,*Last;
	unsigned int i;
	bool LookX;
	bool LookLeft,LookBottom;
	RPromKernel Prom("PlacementCenter",100,2);
	RPromCriterion *area,*dist;

	// If first object -> place it in the middle
	if(!NbObjsOk)
	{
 		Best->X=(Max.X-CurInfo->Width())/2;
 		Best->Y=(Max.Y-CurInfo->Height())/2;
	 	return(*Best);
	}

	// Init Prométhée
	NbCalcPos=0;
	area=Prom.NewCriterion(Minimize,AreaParams);
	dist=Prom.NewCriterion(Minimize,DistParams);
		
	// Find the Bottom-Left coordinate of the boundary rectangle
	Actual=Union.GetBottomLeft();
	i=Union.NbPtr+1;	
	LookX=true;					// Go anti-clockwise, begin with X-Axis and then right.
	LookBottom=true;	    	// Look (X,Y-1) or (X,Y+1) ?
	LookLeft=true;				// Look (X-1,Y) or (X+1,Y) ?


	// Try all vertices
	while(--i)
	{
		// Calculate possible positions at "Actual" position
		NbPos=0;
		if(LookLeft)
		{
			
			// Look to the Left -> Test (X-1,Y)
			if(Grid->IsFree(Actual->X-1,Actual->Y))
			{
				Pos.Set(Actual->X-CurInfo->Width(),Actual->Y);
		  		if(CurInfo->Test(Pos,Max,Grid))
					AddPosition(Prom,area,dist,Pos);
			}
			if(LookBottom)
			{				
				// If look to bottom -> Test (X,Y-1)
				if(Grid->IsFree(Actual->X,Actual->Y-1))
				{
					Pos.Set(Actual->X,Actual->Y-CurInfo->Height());
   		  			if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual->X-CurInfo->Width(),Actual->Y-CurInfo->Height());
	   		  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
			}
			else
			{
				// If look to up -> Test (X,Y+1)				
				if(Grid->IsFree(Actual->X,Actual->Y+1))
				{
					Pos.Set(Actual->X,Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual->X-CurInfo->Width(),Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
			}
		
		}			
		else
		{
			
			// Look to the right -> Test (X+1,Y)
			if(Grid->IsFree(Actual->X+1,Actual->Y))
			{
				Pos.Set(Actual->X+1,Actual->Y);
		  		if(CurInfo->Test(Pos,Max,Grid))
					AddPosition(Prom,area,dist,Pos);
			}
			if(LookBottom)
			{
				// If look to bottom -> Test (X,Y-1)				
				if(Grid->IsFree(Actual->X,Actual->Y-1))
				{
					Pos.Set(Actual->X,Actual->Y-CurInfo->Height());
			  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
				// If no positions -> Test(X,Y)				
				if(NbPos==0)
				{
					Pos.Set(Actual->X+1,Actual->Y-CurInfo->Height());
			  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
			}
			else
			{
				// If look to up -> Test (X,Y+1)				
				if(Grid->IsFree(Actual->X,Actual->Y+1))
				{
					Pos.Set(Actual->X,Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual->X+1,Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						AddPosition(Prom,area,dist,Pos);
				}
			}
		
		}

		// Choose next vertice
		Last=Actual;
		if(LookX)
		{
			Actual=Union.GetConX(Last);
			if(Actual->X<Last->X)
			{
				LookBottom=false;
				LookLeft=true;
			}
			else
			{
				LookBottom=true;
				LookLeft=false;
			}
		}
		else
		{
			Actual=Union.GetConY(Last);
			if(Actual->Y<Last->Y)
			{
				LookLeft=true;
				LookBottom=true;
			}
			else
			{
				LookLeft=false;
				LookBottom=false;
			}
		}
		LookX=!LookX; // If X-Axis -> Next time : Y-Axis.				
	}

	// Calculate Prométhée II and return the "best"	solution.
	if(NbCalcPos==0)
	{
		Best->Set(MaxCoord,MaxCoord);
		return(*Best);
	}
	Prom.ComputePrometheeII();
	(*Best)=CalcPos[Prom.GetBestSolId()];	
	return(*Best);
}


//-----------------------------------------------------------------------------
void RGA2D::RPlacementCenter::Place(RPoint& pos)
{
	// Assign it
	CurInfo->Assign(pos,Grid);
	Sol.Clear();
	Sol.InsertPtr(new RPolygon(Union));
	CurInfo->Add(Sol);
	Sol.Union(&Union);
 	Union.Boundary(Result);
}


//----------------------------------------------------------------------------
void RGA2D::RPlacementCenter::PostRun(RPoint &limits)
{
	RGeoInfo **info;
	unsigned int i;
	RPoint tmp;

	Sol.Clear();
	for(i=NbObjs+1,info=Infos;--i;info++)
		Sol.InsertPtr(new RPolygon((*info)->GetPolygon()));
	Sol.Union(&Union);
	Union.Boundary(Result);
	limits=HoldLimits;
	Grid->Clear();
	for(i=NbObjs+1,info=Infos;--i;info++)
	{
		tmp=(*info)->GetPos()-Result.Pt1;
		(*info)->Assign(tmp,Grid);
	}
	for(Free.Start();!Free.End();Free.Next())
		(*Free())-=Result.Pt1;
}

	
//----------------------------------------------------------------------------
RGA2D::RPlacementCenter::~RPlacementCenter(void)
{
	if(CalcPos) delete[] CalcPos;
}
