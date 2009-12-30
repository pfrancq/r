/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Implementation

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
// include files for R Project
#include <rplacementheuristic.h>
#include <random.h>
#include <rxmlstruct.h>
using namespace R;


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
	char Ori;

	ObjectPos(void) {}
	int Compare(const ObjectPos&) const {return(-1);}
};



//------------------------------------------------------------------------------
//
// class RPlacementHeuristic
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementHeuristic::RPlacementHeuristic(size_t maxobjs,bool calc,bool use,RRandom* r,bool ori)
	: Random(r), Free(), CalcFree(calc), UseFree(calc&&use), AllOri(ori), ValidPos(500), Prom("Orientations",100,2),
	  DistParams(0), AreaParams(0)
{
	Order=new RGeoInfo*[maxobjs];
	for(size_t i=101;--i;)
		ValidPos.InsertPtr(new ObjectPos());
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SetDistParams(const RParam* params)
{
	DistParams=params;
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::SetAreaParams(const RParam* params)
{
	AreaParams=params;
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	// Initialize PROMTEHEE Criteria
	if(!AreaParams)
		ThrowRException("PROMETHEE parameters for the area criterion not specified");
	if(!DistParams)
		ThrowRException("PROMETHEE parameters for the distance criterion not specified");
	Prom.AddCriterion(Area=new RPromLinearCriterion(RPromCriterion::Maximize,AreaParams,"Weight"));
	Prom.AddCriterion(Dist=new RPromLinearCriterion(RPromCriterion::Minimize,DistParams,"Distance"));

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
RGeoInfo* RPlacementHeuristic::SelectNextObject(void)
{
	if(!NbObjsOk)
		return(Order[0]);

	// Find the most connected object
	return(Connections->GetMostConnected(Infos,NbObjs,Order,NbObjsOk));
}


//------------------------------------------------------------------------------
void RPlacementHeuristic::AddValidPosition(RPoint& pos)
{
	// Verify if the solution is not already existing
	if(Prom.GetNbSols())
	{
		RCursor<ObjectPos> Cur(ValidPos,0,Prom.GetNbSols()-1);
		for(Cur.Start();!Cur.End();Cur.Next())
			if((Cur()->Pos==pos)&&(Cur()->Ori==CurInfo->GetOri()))
				return;
	}

	// Compute the bounding rectangle of all placed objects and the current
	// one at the given position
	RRect CurRect(Result);
	if(pos.X<CurRect.X1) CurRect.X1=pos.X;
	if(pos.Y<CurRect.Y1) CurRect.Y1=pos.Y;
	if(pos.X+CurInfo->Width()>CurRect.X2) CurRect.X2=pos.X+CurInfo->Width();
	if(pos.Y+CurInfo->Height()>CurRect.Y2) CurRect.Y2=pos.Y+CurInfo->Height();
	if((CurRect.X1<0)||(CurRect.Y1<0)||(CurRect.X2>=Limits.X)||(CurRect.Y2>=Limits.Y))
		return;

	// Verify if the container contains enough elements
	if(Prom.GetNbSols()==ValidPos.GetNb())
	{
		for(size_t i=101;--i;)
			ValidPos.InsertPtr(new ObjectPos());
	}

	// Add a new valid position and a new PROMETHEE solution
	ObjectPos* ptr(ValidPos[Prom.GetNbSols()]);
	ptr->Ori=CurInfo->GetOri();
	ptr->Pos=pos;
	RPromSol* Sol(Prom.NewSol());
	Prom.Assign(Sol,Area,CurRect.GetWidth()*CurRect.GetHeight());
	Prom.Assign(Sol,Dist,Infos->Cons.GetDistances(CurInfo,pos));
}


//------------------------------------------------------------------------------
RGeoInfo* RPlacementHeuristic::NextObject(void)
{
	if(NbObjsOk>=NbObjs)
		ThrowRException("All objects placed");

	// Init
	RPoint pos(MaxCoord,MaxCoord);
	CurInfo=SelectNextObject();
	CurInfo->SetOrder(NbObjsOk);
	RObj2D* obj(CurInfo->GetObj());

	// Choose best free polygon (if any).
	if(UseFree)
		pos=Free.CanPlace(CurInfo);

	// Find the best position for current orientation if not already found.
	if(!pos.IsValid())
	{
		// Suppose no position
		Prom.ClearSols();

		if(AllOri&&(obj->NbPossOri>1))
		{
			// Compute all orientations
			for(char o=0;o<obj->NbPossOri;o++)
			{
				// Set an orientation & Find a position
				CurInfo->SetOri(o);
				SearchValidPositions(CurInfo);
			}
		}
		else
		{
			// Set an orientation & find a position
			CurInfo->SetOri(0);
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
			CurInfo->SetOri(Best->Ori);
		}
	}

	// Place it
	if(!pos.IsValid())
		ThrowRException("Can't place an object!");

	// Assign the object to the current position
	CurInfo->Assign(pos,Grid);
	PostPlace(CurInfo,pos);

	// Look for free polygons
	if(CalcFree)
		Grid->AddFreePolygons(CurInfo,&Free,Result);

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
RPlacementHeuristic::~RPlacementHeuristic(void)
{
	delete[] Order;
}
