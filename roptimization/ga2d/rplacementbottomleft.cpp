/*

	Rainbow Library Project

	RPlacementBottomLeft.cpp

	Bottom Left Heuristic for Placement - Implementation

	(C) 1998-2000 by By P. Francq.

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
// include files for Rainbow
#include "rplacementbottomleft.h"
using namespace RGA;



//-----------------------------------------------------------------------------
//
//	RPlacementBottomLeft
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPlacementBottomLeft::RPlacementBottomLeft(unsigned int maxobjs,bool calcfree)
	: RPlacementHeuristic(maxobjs,calcfree)
{
}


//-----------------------------------------------------------------------------
void RPlacementBottomLeft::Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs)
{
	RPlacementHeuristic::Init(limits,grid,objs,infos,nbobjs);
	Actual.Set(0,0);
	Last.Set(0,0);

}


//-----------------------------------------------------------------------------
bool RPlacementBottomLeft::NextObjectOri(void)
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
 	{
		return(false);
 	}


 	// Assign the object to the current position
	CurInfo->Assign(Pos,Grid);
		

 	// Calculate Next position
	Last=Actual;
 	if(Pos.X+CurInfo->Width()>Actual.X)
 		Actual.X=Pos.X+CurInfo->Width();
	Last.Y+=CurInfo->Height();


	// Verify ActLimits
 	if(Pos.X+CurInfo->Width()>Result.Pt2.X)
 		Result.Pt2.X=Pos.X+CurInfo->Width();
 	if(Pos.Y+CurInfo->Height()>Result.Pt2.Y)
 		Result.Pt2.Y=Pos.Y+CurInfo->Height();

	// Ok
	return(true);
}