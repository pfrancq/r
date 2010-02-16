/*

	R Project Library

	RPlacementEdge.h

	Edge Heuristic for Placement - Implementation

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
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
#include <rplacementedge.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPlacementEdge
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementEdge::RPlacementEdge(size_t maxobjs,bool calc,bool use,RRandom* r,RParamStruct* dist,RParamStruct* area,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,r,dist,area,ori)
{
}


//------------------------------------------------------------------------------
void RPlacementEdge::Init(RProblem2D* prob,RLayout* layout,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,layout,grid);
	for(unsigned i=0;i<40;i++)
		Levels[i].Set(0,0);
	NbLevels=0;
	CurLevel=0;
	Actual.Set(0,0);
	Max.Set(0,0);
	Last.Set(0,0);
}


//------------------------------------------------------------------------------
void RPlacementEdge::SearchValidPositions(RGeoInfo* info)
{
	double FactorX,FactorY;
	RPoint Pos;

	// Verify if add normally or on bottom
	if(((Result.GetX2()+1)<Limits.GetWidth())&&(CurLevel==NbLevels))
	{
		FactorX=(static_cast<double>(Result.GetX2()))/(static_cast<double>(Limits.GetWidth()));
		FactorY=(static_cast<double>(Result.GetY2()))/(static_cast<double>(Limits.GetHeight()));
		if(FactorX<FactorY)
		{
			Levels[CurLevel++]=Actual;
			if(CurLevel>=NbLevels)
				NbLevels=CurLevel;
			Actual.Set(Result.GetX2(),0);
			Max.X=Result.GetX2()+info->GetConfig()->GetWidth()+1;
			if(Max.X>Limits.GetWidth())
				Max.X=Limits.GetWidth();
			CurLevel=0;
		}
	}

	// Do a local optimization at actual position
	Pos=Last;
	info->PushBottomLeft(Pos,Limits,Grid);
	if((Pos.Y+info->GetConfig()->GetHeight()>Levels[CurLevel].Y)||(Pos.X+info->GetConfig()->GetWidth()>Actual.X))
	{
		Pos=Actual;
		info->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to long than begin from left again
	while((Pos.X>0)&&(Pos.X+info->GetConfig()->GetWidth()>Max.X))
	{
		Levels[CurLevel++]=Actual;
		if(CurLevel>=NbLevels)
		{
			NbLevels=CurLevel;
			Actual.Set(0,Max.Y);
		}
		else
			Actual=Levels[CurLevel];
		Pos=Actual;
		info->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to high than try to switch objects and place another one
	if(Pos.Y+info->GetConfig()->GetHeight()>Limits.GetHeight())
	{
		Pos=RPoint::Null;
		return;
	}

	AddValidPosition(Pos);
}


//------------------------------------------------------------------------------
void RPlacementEdge::PostPlace(RGeoInfo* info,const RPoint& pos)
{
	size_t i,l;

	// Calculate Next position
	Last=Actual;
	if(pos.X+info->GetConfig()->GetWidth()>Actual.X)
		Actual.X=pos.X+info->GetConfig()->GetWidth();
	if((pos.X==0)&&(pos.X+info->GetConfig()->GetWidth())>Max.X)
		Max.X=pos.X+info->GetConfig()->GetWidth();
	if(pos.Y+info->GetConfig()->GetHeight()>Max.Y)
		Max.Y=pos.Y+info->GetConfig()->GetHeight();
	Last.Y+=info->GetConfig()->GetHeight();

	// Verify if down level needed update
	for(i=0;i<CurLevel;i++)
		if(Levels[i].X<Max.X) Levels[i].X=Max.X;

	// Verify if some levels must be skipped
	i=CurLevel+1;
	while((i<NbLevels)&&(Levels[i].Y<pos.Y+info->GetConfig()->GetHeight()))
	{
		NbLevels--;
		for(l=i;l<NbLevels;l++)
			Levels[l]=Levels[l+1];
	}

	// Verify ActLimits
	tCoord X2(Result.GetX2()),Y2(Result.GetY2());
	if(pos.X+info->GetConfig()->GetWidth()>X2)
	{
		X2=pos.X+info->GetConfig()->GetWidth();
		if(Max.X==0) Max.X=X2;
	}
	if(pos.Y+info->GetConfig()->GetHeight()>Y2)
		Y2=pos.Y+info->GetConfig()->GetHeight();
	Result.Set(Result.GetX1(),Result.GetY1(),X2,Y2);
}
