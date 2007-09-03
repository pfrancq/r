/*

	R Project Library

	RPlacementCenter.cpp

	Center Heuristic for Placement - Implemenation

	Copyright 1998-2005 by the Université Libre de Bruxelles.

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
#include <rplacementcenter.h>
using namespace R;


//------------------------------------------------------------------------------
// defines
//#define DOUBLESPACE



//------------------------------------------------------------------------------
//
// class RPlacementCenter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementCenter::RPlacementCenter(unsigned int maxobjs,bool calc,bool use,RRandom* r,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,r,ori)
{
	AreaParams.Set(0.2,0.05,2.0);
	DistParams.Set(0.2,0.05,1.0);
}


//------------------------------------------------------------------------------
void RPlacementCenter::Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,infos,grid);
	HoldLimits=Limits;
	#ifdef DOUBLESPACE
		Max=Limits+Limits;
	#else
		Max=Limits;
	#endif
	Attraction.Set(Max.X/2,Max.Y/2);
	Union.Clear();
	Sol.Clear();
}


//------------------------------------------------------------------------------
void RPlacementCenter::NextObjectOri(void)
{
	RPoint Pos;                     // Position to test (X,Y).
	unsigned int NbPos;
	RPoint Actual,Last;
	unsigned int i;
	bool LookX;
	bool LookLeft,LookBottom;

	// If first object -> place it in the middle
	if(!NbObjsOk)
	{
		RPoint Best((Max.X-CurInfo->Width())/2,(Max.Y-CurInfo->Height())/2);
		AddValidPosition(Best);
		return;
	}

	// Find the Bottom-Left coordinate of the boundary rectangle
	Actual=Union.GetBottomLeft();
	i=Union.GetNb()+1;
	LookX=true;                 // Go anti-clockwise, begin with X-Axis and then right.
	LookBottom=true;            // Look (X,Y-1) or (X,Y+1) ?
	LookLeft=true;              // Look (X-1,Y) or (X+1,Y) ?

	// Try all vertices
	while(--i)
	{
		// Calculate possible positions at "Actual" position
		NbPos=0;
		if(LookLeft)
		{

			// Look to the Left -> Test (X-1,Y)
			if(Grid->IsFree(Actual.X-1,Actual.Y))
			{
				Pos.Set(Actual.X-CurInfo->Width(),Actual.Y);
				if(CurInfo->Test(Pos,Grid))
				{
					CurInfo->PushCenter(Pos,Limits,Grid);
					AddValidPosition(Pos);
				}
			}
			if(LookBottom)
			{
				// If look to bottom -> Test (X,Y-1)
				if(Grid->IsFree(Actual.X,Actual.Y-1))
				{
					Pos.Set(Actual.X,Actual.Y-CurInfo->Height());
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X-CurInfo->Width(),Actual.Y-CurInfo->Height());
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
			}
			else
			{
				// If look to up -> Test (X,Y+1)
				if(Grid->IsFree(Actual.X,Actual.Y+1))
				{
					Pos.Set(Actual.X,Actual.Y+1);
					if(CurInfo->Test(Pos,Grid))
						AddValidPosition(Pos);
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X-CurInfo->Width(),Actual.Y+1);
					Pos.Set(Actual.X-CurInfo->Width(),Actual.Y-CurInfo->Height());
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
			}

		}
		else
		{
			// Look to the right -> Test (X+1,Y)
			if(Grid->IsFree(Actual.X+1,Actual.Y))
			{
				Pos.Set(Actual.X+1,Actual.Y);
				if(CurInfo->Test(Pos,Grid))
				{
					CurInfo->PushCenter(Pos,Limits,Grid);
					AddValidPosition(Pos);
				}
			}
			if(LookBottom)
			{
				// If look to bottom -> Test (X,Y-1)
				if(Grid->IsFree(Actual.X,Actual.Y-1))
				{
					Pos.Set(Actual.X,Actual.Y-CurInfo->Height());
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X+1,Actual.Y-CurInfo->Height());
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
			}
			else
			{
				// If look to up -> Test (X,Y+1)
				if(Grid->IsFree(Actual.X,Actual.Y+1))
				{
					Pos.Set(Actual.X,Actual.Y+1);
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X+1,Actual.Y+1);
					if(CurInfo->Test(Pos,Grid))
					{
						CurInfo->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
			}
		}

		// Choose next vertice
		Last=Actual;
		if(LookX)
		{
			Actual=Union.GetConX(&Last);
			if(Actual.X<Last.X)
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
			Actual=Union.GetConY(&Last);
			if(Actual.Y<Last.Y)
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
}


//------------------------------------------------------------------------------
void RPlacementCenter::Place(RPoint& pos)
{
	// Assign it
	CurInfo->Assign(pos,Grid);
	Sol.Clear();
	Sol.InsertPtr(new RPolygon(Union));
	CurInfo->Add(Sol);
	Sol.Union(&Union);
	Union.Boundary(Result);
}


//------------------------------------------------------------------------------
void RPlacementCenter::PostRun(RPoint& limits)
{
	RPlacementHeuristic::PostRun(limits);
	Sol.Clear();
	RCursor<RGeoInfo> info(*Infos);
	for(info.Start();!info.End();info.Next())
		Sol.InsertPtr(new RPolygon(info()->GetPolygon()));
	Sol.Union(&Union);
	Union.Boundary(Result);
	limits=HoldLimits;
}


//------------------------------------------------------------------------------
RPlacementCenter::~RPlacementCenter(void)
{
}
