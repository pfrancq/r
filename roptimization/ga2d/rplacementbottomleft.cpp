/*

	R Project Library

	RPlacementBottomLeft.cpp

	Bottom Left Heuristic for Placement - Implementation

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
#include <rga2d/rplacementbottomleft.h>
using namespace R;



//------------------------------------------------------------------------------
//
//	RPlacementBottomLeft
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementBottomLeft::RPlacementBottomLeft(unsigned int maxobjs,bool calc,bool use,RRandom* r,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,r,ori)
{
}


//------------------------------------------------------------------------------
void RPlacementBottomLeft::Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,infos,grid);
	Actual.Set(0,0);
	Last.Set(0,0);
}


//------------------------------------------------------------------------------
void RPlacementBottomLeft::NextObjectOri(void) throw(RPlacementHeuristicException)
{
	RPoint Pos;

	// Do a local optimisationn at actual position
	Pos=Last;
	CurInfo->PushBottomLeft(Pos,Limits,Grid);
	if((Pos.Y+CurInfo->Height()>Result.Pt2.Y)||(Pos.X+CurInfo->Width()>Actual.X))
	{
		Pos=Actual;
		CurInfo->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to long than begin from left again
	if(Pos.X+CurInfo->Width()>Limits.X)
	{
		Actual.Set(0,Result.Pt2.Y);
		Pos=Actual;
		CurInfo->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to high than try to switch objects and place another one
	if(Pos.Y+CurInfo->Height()>Limits.Y)
		return;

	AddValidPosition(Pos);
}


//------------------------------------------------------------------------------
void RPlacementBottomLeft::Place(RPoint& pos) throw(RPlacementHeuristicException)
{
	// Assign the object to the current position
	CurInfo->Assign(pos,Grid);

	// Calculate Next position
	Last=Actual;
	if(pos.X+CurInfo->Width()>Actual.X)
		Actual.X=pos.X+CurInfo->Width();
	Last.Y+=CurInfo->Height();

	// Verify ActLimits
	if(pos.X+CurInfo->Width()>Result.Pt2.X)
		Result.Pt2.X=pos.X+CurInfo->Width();
	if(pos.Y+CurInfo->Height()>Result.Pt2.Y)
		Result.Pt2.Y=pos.Y+CurInfo->Height();
}
