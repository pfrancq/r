/*

	R Project Library

	RPlacementEdge.h

	Edge Heuristic for Placement - Implementation

	Copyright 1998-2003 by the Université Libre de Bruxelles.

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
#include <rga2d/rplacementedge.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RPlacementEdge
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementEdge::RPlacementEdge(unsigned int maxobjs,bool calc,bool use,RRandom* r,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,r,ori)
{
}


//------------------------------------------------------------------------------
void RPlacementEdge::Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,infos,grid);
	for(unsigned i=0;i<40;i++)
		Levels[i].Set(0,0);
	NbLevels=0;
	CurLevel=0;
	Actual.Set(0,0);
	Max.Set(0,0);
	Last.Set(0,0);
}


//------------------------------------------------------------------------------
void RPlacementEdge::NextObjectOri(void) throw(RPlacementHeuristicException)
{
	double FactorX,FactorY;
	RPoint Pos;

	// Verify if add normally or on bottom
	if(((Result.Pt2.X+1)<Limits.X)&&(CurLevel==NbLevels))
	{
		FactorX=(static_cast<double>(Result.Pt2.X))/(static_cast<double>(Limits.X));
		FactorY=(static_cast<double>(Result.Pt2.Y))/(static_cast<double>(Limits.Y));
		if(FactorX<FactorY)
		{
			Levels[CurLevel++]=Actual;
			if(CurLevel>=NbLevels)
				NbLevels=CurLevel;
			Actual.Set(Result.Pt2.X,0);
			Max.X=Result.Pt2.X+CurInfo->Width()+1;
			if(Max.X>Limits.X)
			Max.X=Limits.X;
			CurLevel=0;
		}
	}

	// Do a local optimisationn at actual position
	Pos=Last;
	CurInfo->PushBottomLeft(Pos,Limits,Grid);
	if((Pos.Y+CurInfo->Height()>Levels[CurLevel].Y)||(Pos.X+CurInfo->Width()>Actual.X))
	{
		Pos=Actual;
		CurInfo->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to long than begin from left again
	while((Pos.X>0)&&(Pos.X+CurInfo->Width()>Max.X))
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
		CurInfo->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to high than try to switch objects and place another one
	if(Pos.Y+CurInfo->Height()>Limits.Y)
	{
		Pos.Set(MaxCoord,MaxCoord);
		return;
	}

	AddValidPosition(Pos);
}


//------------------------------------------------------------------------------
void RPlacementEdge::Place(RPoint& pos) throw(RPlacementHeuristicException)
{
	unsigned int i,l;

	// Assign the object to the current position
	CurInfo->Assign(pos,Grid);

	// Calculate Next position
	Last=Actual;
	if(pos.X+CurInfo->Width()>Actual.X)
		Actual.X=pos.X+CurInfo->Width();
	if((pos.X==0)&&(pos.X+CurInfo->Width())>Max.X)
		Max.X=pos.X+CurInfo->Width();
	if(pos.Y+CurInfo->Height()>Max.Y)
		Max.Y=pos.Y+CurInfo->Height();
	Last.Y+=CurInfo->Height();


	// Verify if down level needed update
	for(i=0;i<CurLevel;i++)
		if(Levels[i].X<Max.X) Levels[i].X=Max.X;


	// Verify if some levels must be skipped
	i=CurLevel+1;
	while((i<NbLevels)&&(Levels[i].Y<pos.Y+CurInfo->Height()))
	{
		NbLevels--;
		for(l=i;l<NbLevels;l++)
			Levels[l]=Levels[l+1];
	}

	// Verify ActLimits
	if(pos.X+CurInfo->Width()>Result.Pt2.X)
	{
		Result.Pt2.X=pos.X+CurInfo->Width();
		if(Max.X==0) Max.X=Result.Pt2.X;
	}
	if(pos.Y+CurInfo->Height()>Result.Pt2.Y)
		Result.Pt2.Y=pos.Y+CurInfo->Height();
}
