/*

	Rainbow Library Project

	RPlacementCenter.cpp

	Center Heuristic for Placement - Implemenation

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
#include "rplacementcenter.h"
using namespace RGA;



//-----------------------------------------------------------------------------
//
//	RPlacementCenter
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPlacementCenter::RPlacementCenter(unsigned int maxobjs)
	: RPlacementHeuristic(maxobjs)
{
}


//-----------------------------------------------------------------------------
void RPlacementCenter::Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs)
{
	RPlacementHeuristic::Init(limits,grid,objs,infos,nbobjs);
	HoldLimits=limits;
	Max=Limits+Limits;
	limits=Max;
	Attraction.Set(Max.X/2,Max.Y/2);
	Sol.Clear();
}


//-----------------------------------------------------------------------------
bool RPlacementCenter::NextObjectOri(void)
{
	RCoord BestDist,Dist2;
	RPoint Best;
	RPoint Pos[2];
	RPoint Center;
	char NbPos,nb;
	RPoint *Actual,*Last;
	unsigned int i;
	bool LookX;
	bool LookLeft,LookBottom;
//	RRect Rect;
	RRect CurRect;
	RCoord Area,BestArea;


	// If first object -> place it in the middle
	if(!NbObjsOk)
	{
 		Best.X=(Max.X-CurInfo->Width())/2;
 		Best.Y=(Max.Y-CurInfo->Height())/2;
		CurInfo->Assign(Best,Grid);
		CurInfo->Add(Sol);
		NbObjsOk++;
		// Calculate Union of all placed polygons
		Sol.Union(&Union);
 		Union.Boundary(Result);
	 	return(true);
  }




	// Find the Bottom-Left
	Actual=Union.GetBottomLeft();
	i=Union.NbPtr+1;	
	LookX=true;
	LookLeft=true;
	LookBottom=true;
	BestArea=MaxCoord;
	BestDist=MaxCoord;

	// Try edges
	while(--i)
	{
		// Calculate possible positions at "Actual" position
		NbPos=0;
		if(LookLeft)
		{
			if(Grid->IsFree(Actual->X-1,Actual->Y))
				Pos[NbPos++].Set(Actual->X-CurInfo->Width(),Actual->Y);
			if(LookBottom)
			{				
				if(Grid->IsFree(Actual->X,Actual->Y-1))
					Pos[NbPos++].Set(Actual->X,Actual->Y-CurInfo->Height());
				if(NbPos==0)
					Pos[NbPos++].Set(Actual->X-CurInfo->Width(),Actual->Y-CurInfo->Height());
			}
			else
			{
				if(Grid->IsFree(Actual->X,Actual->Y+1))
					Pos[NbPos++].Set(Actual->X,Actual->Y+1);
				if(NbPos==0)
					Pos[NbPos++].Set(Actual->X-CurInfo->Width(),Actual->Y+1);
			}
		}			
		else
		{
			if(Grid->IsFree(Actual->X+1,Actual->Y))
				Pos[NbPos++].Set(Actual->X+1,Actual->Y);
			if(LookBottom)
			{
				if(Grid->IsFree(Actual->X,Actual->Y-1))
					Pos[NbPos++].Set(Actual->X,Actual->Y-CurInfo->Height());
				if(NbPos==0)
					Pos[NbPos++].Set(Actual->X+1,Actual->Y-CurInfo->Height());
			}
			else
			{
				if(Grid->IsFree(Actual->X,Actual->Y+1))
					Pos[NbPos++].Set(Actual->X,Actual->Y+1);
				if(NbPos==0)
					Pos[NbPos++].Set(Actual->X+1,Actual->Y+1);
			}
		}

		// Try the possibilities and see if one of them is better
		for(nb=0;nb<NbPos;nb++)
		{
			if(CurInfo->Test(Pos[nb],Max,Grid))
			{
				CurInfo->PushCenter(Pos[nb],Max,Grid);
				Dist2=Attraction.ManhattanDist(Pos[nb]+Center);				
 				CurRect=Result;            // Rect
 				if(Pos[nb].X<CurRect.Pt1.X) CurRect.Pt1.X=Pos[nb].X;
 				if(Pos[nb].Y<CurRect.Pt1.Y) CurRect.Pt1.Y=Pos[nb].Y;
 				if(Pos[nb].X+CurInfo->Width()>CurRect.Pt2.X) CurRect.Pt2.X=Pos[nb].X+CurInfo->Width();
 				if(Pos[nb].Y+CurInfo->Height()>CurRect.Pt2.Y) CurRect.Pt2.Y=Pos[nb].Y+CurInfo->Height();
 				Area=CurRect.Width()*CurRect.Height();
				if(((labs(Dist2-BestDist)<15)&&(Area<BestArea))||(Dist2<BestDist))
				{
					BestArea=Area;
					Best=Pos[nb];
					BestDist=Dist2;						
				}
			}
		}

		// Choose next edge
		Last=Actual;
		LookX=!LookX;
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

	}


	// Assign it
	CurInfo->Assign(Best,Grid);
	Sol.Clear();
	Sol.InsertPtr(new RPolygon(Union));
	CurInfo->Add(Sol);

	// Calculate Union of all placed polygons
	Sol.Union(&Union);
 	Union.Boundary(Result);

	// Ok
	return(true);
}


//-----------------------------------------------------------------------------
void RPlacementCenter::PostRun(RPoint &limits)
{
	Sol.Union(&Union);
	Union.Boundary(Result);
	limits=HoldLimits;
}