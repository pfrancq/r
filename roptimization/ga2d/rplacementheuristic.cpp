/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Implementation

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
// include files for R Project
#include <rplacementheuristic.h>
#include <rrandom.h>
#include <rxmlstruct.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RPlacementHeuristic::ObjecPos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RPlacementHeuristic::ObjectPos
{
public:
	/** Position of an object.*/
	RPoint Pos;

	/**Orientation */
	tOrientation Ori;

	ObjectPos(void) {}
	int Compare(const ObjectPos&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
// class RPlacementHeuristic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementHeuristic::RPlacementHeuristic(size_t maxobjs,bool calc,bool use,RRandom& r,RParamStruct* dist,RParamStruct* area,bool ori)
	: Random(r), Free(), CalcFree(calc), UseFree(calc&&use), AllOri(ori), ValidPos(500), Prom("Orientations",100,2),
	  Dist(0), DistParams(dist), Area(0), AreaParams(area)
{
	Order=new RGeoInfo*[maxobjs];
	for(size_t i=101;--i;)
		ValidPos.InsertPtr(new ObjectPos());

	// Initialize PROMTEHEE Criteria
	if(!AreaParams)
		mThrowRException("PROMETHEE parameters for the area criterion not specified");
	Prom.AddCriterion(Area=new RPromLinearCriterion(RPromCriterion::Maximize,AreaParams,"Weight"));
	if(!DistParams)
		mThrowRException("PROMETHEE parameters for the distance criterion not specified");
	Prom.AddCriterion(Dist=new RPromLinearCriterion(RPromCriterion::Minimize,DistParams,"Distance"));
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::Init(RProblem2D* prob,RLayout* layout,RGrid* grid)
{
	// Assign
	Limits=prob->GetLimits();
	Grid=grid;
	Layout=layout;
	WeightedDistances=prob->MustWeightedDistances();

	// Init the data for a placement
	NbObjsOk=0;
	Grid->Clear();
	Free.Clear();
	Result.Set(0,0,0,0);
	Distances=0.0;

	// Calculate an order
	RCursor<RGeoInfo> Objs(Layout->GetInfos());
	RGeoInfo** ptr(Order);
	for(Objs.Start(),NbObjs=0;!Objs.End();Objs.Next())
		if(!Objs()->GetContainer())
		{
			(*ptr)=Objs();
			ptr++;
			NbObjs++;
		}
	Random.RandOrder<RGeoInfo*>(Order,NbObjs);
}


//------------------------------------------------------------------------------
RGeoInfo* RPlacementHeuristic::SelectNextObject(void)
{
	if(!NbObjsOk)
		return(Order[0]);

	// Find the most connected object
	return(Layout->GetMostConnected(Order,NbObjs,NbObjsOk,WeightedDistances));
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::AddValidPosition(RPoint& pos)
{
	// Verify if the solution is not already existing
	if(Prom.GetNbSols())
	{
		RCursor<ObjectPos> Cur(ValidPos,0,Prom.GetNbSols()-1);
		for(Cur.Start();!Cur.End();Cur.Next())
			if((Cur()->Pos==pos)&&(Cur()->Ori==CurInfo->GetConfig()->GetOrientation()))
				return;
	}

	// Compute the bounding rectangle of all placed objects and the current
	// one at the given position
	RRect CurRect;
	if(NbObjsOk)
	{
		// Objects already placed: start from existing rectangle and adapt it
		CurRect=Result;
		tCoord X1(CurRect.GetX1()), Y1(CurRect.GetY1()), X2(CurRect.GetX2()), Y2(CurRect.GetY2());
		if(pos.X<X1) X1=pos.X;
		if(pos.Y<Y1) Y1=pos.Y;
		if(pos.X+CurInfo->GetConfig()->GetWidth()>X2)
			X2=pos.X+CurInfo->GetConfig()->GetWidth();
		if(pos.Y+CurInfo->GetConfig()->GetHeight()>Y2)
			Y2=pos.Y+CurInfo->GetConfig()->GetHeight();
		CurRect.Set(X1,Y1,X2,Y2);
	}
	else
	{
		// No objects placed: boundary corresponds to the first object
		CurRect.Set(pos.X,pos.Y,pos.X+CurInfo->GetConfig()->GetWidth(),pos.Y+CurInfo->GetConfig()->GetHeight());
	}
	if((CurRect.GetX1()<0)||(CurRect.GetY1()<0)||(CurRect.GetX2()>=Limits.GetWidth())||(CurRect.GetY2()>=Limits.GetHeight()))
		return;

	// Verify if the container contains enough elements
	if(Prom.GetNbSols()==ValidPos.GetNb())
	{
		for(size_t i=101;--i;)
			ValidPos.InsertPtr(new ObjectPos());
	}

	// Add a new valid position and a new PROMETHEE solution
	ObjectPos* ptr(ValidPos[Prom.GetNbSols()]);
	ptr->Ori=CurInfo->GetConfig()->GetOrientation();
	ptr->Pos=pos;
	RPromSol* Sol(Prom.NewSol());
	Prom.Assign(Sol,Area,CurRect.GetWidth()*CurRect.GetHeight());
	Prom.Assign(Sol,Dist,Layout->ComputeDist(CurInfo,WeightedDistances,pos));
}


//------------------------------------------------------------------------------
RGeoInfo* RPlacementHeuristic::NextObject(void)
{
	if(NbObjsOk>=NbObjs)
		mThrowRException("All objects placed");

	// Init
	RPoint pos(RPoint::Null);
	CurInfo=SelectNextObject();
	RObj2D* obj(CurInfo->GetObj());

	// Choose best free polygon (if any).
	if(UseFree)
		pos=Free.CanPlace(CurInfo);

	// Find the best position for current orientation if not already found.
	if(!pos.IsValid())
	{
		// Suppose no position
		Prom.ClearSols();

		if(AllOri&&(obj->GetNb()>1))
		{
			// Compute all orientations
			RCursor<RObj2DConfig> Cur(*obj);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				// Set an orientation & Find a position
				CurInfo->SetConfig(Cur()->GetOrientation());
				SearchValidPositions(CurInfo);
			}
		}
		else
		{
			// Set an orientation & find a position
			CurInfo->SetConfig(oNormal);
			SearchValidPositions(CurInfo);
		}

		// Is there at least one valid position
		if(Prom.GetNbSols())
		{
			ObjectPos* Best;

			// If multiple positions -> Run PROMETHEE
			if(Prom.GetNbSols()>1)
			{
				Prom.ComputePrometheeII();
				Best=ValidPos[Prom.GetBestSolId()];
			}
			else
				Best=ValidPos[0];
			pos=Best->Pos;
			CurInfo->SetConfig(Best->Ori);
		}
	}

	// Place it
	if(!pos.IsValid())
		mThrowRException("Can't place an object!");

	// Assign the object to the current position
	CurInfo->Assign(pos,Grid,NbObjsOk);
	PostPlace(CurInfo,pos);

	// Look for free polygons
	if(CalcFree)
		Grid->AddFreePolygons(CurInfo,&Free,Result);

	// Next Object
	NbObjsOk++;
	return(CurInfo);
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::Run(RProblem2D* prob,RLayout* layout,RGrid* grid)
{
	Init(prob,layout,grid);
	while(NbObjsOk<NbObjs)
	{
		NextObject();
	}
	PostRun();
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::PostRun(void)
{
	// Compute all connections part again
	Distances=Layout->ComputeConnections(WeightedDistances);
}


//------------------------------------------------------------------------------
RRect RPlacementHeuristic::GetResult(void)
{
	return(Result);
}


//------------------------------------------------------------------------------
RPlacementHeuristic::~RPlacementHeuristic(void)
{
	delete[] Order;
}
