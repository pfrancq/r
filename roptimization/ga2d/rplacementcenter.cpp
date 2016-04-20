/*

	R Project Library

	RPlacementCenter.cpp

	Center Heuristic for Placement - Implementation

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
#include <rpromkernel.h>
#include <rplacementcenter.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RPlacementCenter
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementCenter::RPlacementCenter(size_t maxobjs,bool calc,bool use,RRandom& r,RParamStruct* dist,RParamStruct* area,bool firstrandom,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,r,dist,area,firstrandom,ori)
{
}


//------------------------------------------------------------------------------
void RPlacementCenter::Init(RProblem2D* prob,RLayout* layout,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,layout,grid);
	Attraction.Set(Limits.GetWidth()/2,Limits.GetHeight()/2);
	Union.Clear();
	Sol.Clear();
}


//------------------------------------------------------------------------------
void RPlacementCenter::SearchValidPositions(RGeoInfo* info)
{
	RPoint Pos;                     // Position to test (X,Y).
	size_t NbPos;
	RPoint Actual,Last;
	size_t i;
	bool LookX;
	bool LookLeft,LookBottom;

	// If first object -> place it in the middle
	if(!NbObjsOk)
	{
		RPoint Best((Limits.GetWidth()-info->GetConfig()->GetWidth())/2,(Limits.GetHeight()-info->GetConfig()->GetHeight())/2);
		AddValidPosition(Best);
		return;
	}

	// Find the Bottom-Left coordinate of the boundary rectangle
	Actual=Union.GetBottomLeft();
	i=Union.GetNbVertices()+1;
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
				Pos.Set(Actual.X-info->GetConfig()->GetWidth(),Actual.Y);
				if(info->Test(Pos,Grid))
				{
					info->PushCenter(Pos,Limits,Grid);
					AddValidPosition(Pos);
				}
			}
			if(LookBottom)
			{
				// If look to bottom -> Test (X,Y-1)
				if(Grid->IsFree(Actual.X,Actual.Y-1))
				{
					Pos.Set(Actual.X,Actual.Y-info->GetConfig()->GetHeight());
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X-info->GetConfig()->GetWidth(),Actual.Y-info->GetConfig()->GetHeight());
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
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
					if(info->Test(Pos,Grid))
						AddValidPosition(Pos);
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X-info->GetConfig()->GetWidth(),Actual.Y+1);
					Pos.Set(Actual.X-info->GetConfig()->GetWidth(),Actual.Y-info->GetConfig()->GetHeight());
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
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
				if(info->Test(Pos,Grid))
				{
					info->PushCenter(Pos,Limits,Grid);
					AddValidPosition(Pos);
				}
			}
			if(LookBottom)
			{
				// If look to bottom -> Test (X,Y-1)
				if(Grid->IsFree(Actual.X,Actual.Y-1))
				{
					Pos.Set(Actual.X,Actual.Y-info->GetConfig()->GetHeight());
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X+1,Actual.Y-info->GetConfig()->GetHeight());
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
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
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
				// If no positions -> Test(X,Y)
				if(NbPos==0)
				{
					Pos.Set(Actual.X+1,Actual.Y+1);
					if(info->Test(Pos,Grid))
					{
						info->PushCenter(Pos,Limits,Grid);
						AddValidPosition(Pos);
					}
				}
			}
		}

		// Choose next vertex
		Last=Actual;
		if(LookX)
		{
			Actual=Union.GetConX(Last);
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
			Actual=Union.GetConY(Last);
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
void RPlacementCenter::PostPlace(RGeoInfo* info,const RPoint&)
{
	Sol.Clear();
	if(Union.GetNbVertices())
		Sol.InsertPtr(new RPolygon(Union));
	info->Add(Sol);
	Sol.Union(Union);
	Union.Boundary(Result);
}


//------------------------------------------------------------------------------
void RPlacementCenter::PostRun(void)
{
	RPlacementHeuristic::PostRun();
	Sol.Clear();
	RCursor<RGeoInfo> info(Layout->GetInfos());
	for(info.Start();!info.End();info.Next())
		Sol.InsertPtr(new RPolygon(info()->GetPlacedPolygon()));
	Sol.Union(Union);
	Union.Boundary(Result);
}


//------------------------------------------------------------------------------
RPlacementCenter::~RPlacementCenter(void)
{
}
