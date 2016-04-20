/*

	R Project Library

	RPlacementBottomLeft.cpp

	Bottom Left Heuristic for Placement - Implementation

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
#include <rplacementbottomleft.h>
using namespace R;



//------------------------------------------------------------------------------
//
//	RPlacementBottomLeft
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPlacementBottomLeft::RPlacementBottomLeft(size_t maxobjs,bool calc,bool use,RRandom& r,RParamStruct* dist,RParamStruct* area,bool firstrandom,bool ori)
	: RPlacementHeuristic(maxobjs,calc,use,r,dist,area,firstrandom,ori)
{
}


//------------------------------------------------------------------------------
void RPlacementBottomLeft::Init(RProblem2D* prob,RLayout* layout,RGrid* grid)
{
	RPlacementHeuristic::Init(prob,layout,grid);
	NextObj.Set(0,0);
	NextLine.Set(0,0);
}


//------------------------------------------------------------------------------
void RPlacementBottomLeft::SearchValidPositions(RGeoInfo* info)
{
	// Do a local optimization at the actual position
	RPoint Pos(NextLine);
	info->PushBottomLeft(Pos,Limits,Grid);
	if((Pos.Y+info->GetConfig()->GetHeight()>Result.GetY2())||(Pos.X+info->GetConfig()->GetWidth()>NextObj.X))
	{
		Pos=NextObj;
		info->PushBottomLeft(Pos,Limits,Grid);
	}

	// If to long than begin from left again
	if(Pos.X+info->GetConfig()->GetWidth()>Limits.GetWidth())
	{
		NextObj.Set(0,Result.GetY2());
		Pos=NextObj;
		info->PushBottomLeft(Pos,Limits,Grid);
	}

	// If too high, generate an exception
	if((Pos.X+info->GetConfig()->GetWidth()>Limits.GetWidth())||(Pos.Y+info->GetConfig()->GetHeight()>Limits.GetHeight()))
		mThrowRException("Objects does not fit in the actual limits");

	AddValidPosition(Pos);
}


//------------------------------------------------------------------------------
void RPlacementBottomLeft::PostPlace(RGeoInfo* info,const RPoint& pos)
{
	// Calculate Next position
	NextLine=NextObj;
	if(pos.X+info->GetConfig()->GetWidth()>NextObj.X)
		NextObj.X=pos.X+info->GetConfig()->GetWidth();
	NextLine.Y+=info->GetConfig()->GetHeight();

	// Verify ActLimits
	tCoord X2(Result.GetX2()),Y2(Result.GetY2());
	if(pos.X+info->GetConfig()->GetWidth()>X2)
		X2=pos.X+info->GetConfig()->GetWidth();
	if(pos.Y+info->GetConfig()->GetHeight()>Y2)
		Y2=pos.Y+info->GetConfig()->GetHeight();
	Result.Set(Result.GetX1(),Result.GetY1(),X2,Y2);
}
