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
RPlacementCenter::RPlacementCenter(unsigned int maxobjs,bool calcfree,RCoord mindist)
	: RPlacementHeuristic(maxobjs,calcfree), MinDist(mindist)
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
void RPlacementCenter::CalcPositions(void)
{
	RPoint Pos;
	RPoint Center;
	unsigned int nb,NbPos;
	ICalcPos *pos;
	RPoint *Actual,*Last;
	unsigned int i;
	bool LookX;
	bool LookLeft,LookBottom;
	RRect CurRect;

	// Init
	NbCalcPos=0;

	// Find the Bottom-Left coordinate of the boundary rectangle
	Actual=Union.GetBottomLeft();
	i=Union.NbPtr+1;	
	LookX=true;
	LookLeft=true;
	LookBottom=true;

	// Try vertices
	while(--i)
	{
		// Calculate possible positions at "Actual" position
		NbPos=0;
		if(LookLeft)
		{
			if(Grid->IsFree(Actual->X-1,Actual->Y))
			{
				Pos.Set(Actual->X-CurInfo->Width(),Actual->Y);
		  		if(CurInfo->Test(Pos,Max,Grid))
					CalcPos[NbCalcPos++].Pos=Pos;
			}
			if(LookBottom)
			{				
				if(Grid->IsFree(Actual->X,Actual->Y-1))
				{
					Pos.Set(Actual->X,Actual->Y-CurInfo->Height());
   		  		if(CurInfo->Test(Pos,Max,Grid))
   					CalcPos[NbCalcPos++].Pos=Pos;
				}
				if(NbPos==0)
				{
					Pos.Set(Actual->X-CurInfo->Width(),Actual->Y-CurInfo->Height());
   		  		if(CurInfo->Test(Pos,Max,Grid))
   					CalcPos[NbCalcPos++].Pos=Pos;
				}
			}
			else
			{
				if(Grid->IsFree(Actual->X,Actual->Y+1))
				{
					Pos.Set(Actual->X,Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						CalcPos[NbCalcPos++].Pos=Pos;
				}
				if(NbPos==0)
				{
					Pos.Set(Actual->X-CurInfo->Width(),Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						CalcPos[NbCalcPos++].Pos=Pos;
				}
			}
		}			
		else
		{
			if(Grid->IsFree(Actual->X+1,Actual->Y))
			{
				Pos.Set(Actual->X+1,Actual->Y);
		  		if(CurInfo->Test(Pos,Max,Grid))
					CalcPos[NbCalcPos++].Pos=Pos;
			}
			if(LookBottom)
			{
				if(Grid->IsFree(Actual->X,Actual->Y-1))
				{
					Pos.Set(Actual->X,Actual->Y-CurInfo->Height());
			  		if(CurInfo->Test(Pos,Max,Grid))
						CalcPos[NbCalcPos++].Pos=Pos;
				}
				if(NbPos==0)
				{
					Pos.Set(Actual->X+1,Actual->Y-CurInfo->Height());
			  		if(CurInfo->Test(Pos,Max,Grid))
						CalcPos[NbCalcPos++].Pos=Pos;
				}
			}
			else
			{
				if(Grid->IsFree(Actual->X,Actual->Y+1))
				{
					Pos.Set(Actual->X,Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						CalcPos[NbCalcPos++].Pos=Pos;
				}
				if(NbPos==0)
				{
					Pos.Set(Actual->X+1,Actual->Y+1);
			  		if(CurInfo->Test(Pos,Max,Grid))
						CalcPos[NbCalcPos++].Pos=Pos;
				}
			}
		}

		// Choose next vertice
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


  	// Try all the possibilities
  	for(nb=NbCalcPos+1,pos=CalcPos;--nb;pos++)
  	{
		CurInfo->PushCenter(pos->Pos,Max,Grid);
		Center.Set(CurInfo->Width()/2,CurInfo->Height()/2);
		pos->Dist=Attraction.ManhattanDist(pos->Pos+Center);				
		CurRect=Result;
		if(pos->Pos.X<CurRect.Pt1.X) CurRect.Pt1.X=pos->Pos.X;
		if(pos->Pos.Y<CurRect.Pt1.Y) CurRect.Pt1.Y=pos->Pos.Y;
		if(pos->Pos.X+CurInfo->Width()>CurRect.Pt2.X) CurRect.Pt2.X=pos->Pos.X+CurInfo->Width();
		if(pos->Pos.Y+CurInfo->Height()>CurRect.Pt2.Y) CurRect.Pt2.Y=pos->Pos.Y+CurInfo->Height();
		pos->Area=CurRect.Width()*CurRect.Height();
  	}
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
	RRect CurRect;
	RCoord Area,BestArea;
	unsigned count=0;

	// If first object -> place it in the middle
	if(!NbObjsOk)
	{
 		Best.X=(Max.X-CurInfo->Width())/2;
 		Best.Y=(Max.Y-CurInfo->Height())/2;
		CurInfo->Assign(Best,Grid);
		CurInfo->Add(Sol);
		// Calculate Union of all placed polygons
		Sol.Union(&Union);
 		Union.Boundary(Result);
	 	return(true);
	}


	// Find the Bottom-Left coordinate of the boundary rectangle
	Center.Set(CurInfo->Width()/2,CurInfo->Height()/2);
	Actual=Union.GetBottomLeft();
	i=Union.NbPtr+1;	
	LookX=true;
	LookLeft=true;
	LookBottom=true;
	BestArea=MaxCoord;
	BestDist=MaxCoord;

	// Try vertices
	while(--i)
	{
		// Calculate possible positions at "Actual" position
		NbPos=0;
		if(LookLeft)
		{
			if(Grid->IsFree(Actual->X-1,Actual->Y))
			{
				count++;
				Pos[NbPos++].Set(Actual->X-CurInfo->Width(),Actual->Y);
			}
			if(LookBottom)
			{				
				if(Grid->IsFree(Actual->X,Actual->Y-1))
				{
					count++;
					Pos[NbPos++].Set(Actual->X,Actual->Y-CurInfo->Height());
				}
				if(NbPos==0)
				{
					count++;
					Pos[NbPos++].Set(Actual->X-CurInfo->Width(),Actual->Y-CurInfo->Height());
				}
			}
			else
			{
				if(Grid->IsFree(Actual->X,Actual->Y+1))
				{
					count++;
					Pos[NbPos++].Set(Actual->X,Actual->Y+1);
				}
				if(NbPos==0)
				{
					Pos[NbPos++].Set(Actual->X-CurInfo->Width(),Actual->Y+1);
					count++;
				}
			}
		}			
		else
		{
			if(Grid->IsFree(Actual->X+1,Actual->Y))
			{
				Pos[NbPos++].Set(Actual->X+1,Actual->Y);
				count++;
			}
			if(LookBottom)
			{
				if(Grid->IsFree(Actual->X,Actual->Y-1))
				{
					Pos[NbPos++].Set(Actual->X,Actual->Y-CurInfo->Height());
					count++;
				}
				if(NbPos==0)
				{
					Pos[NbPos++].Set(Actual->X+1,Actual->Y-CurInfo->Height());
					count++;
				}
			}
			else
			{
				if(Grid->IsFree(Actual->X,Actual->Y+1))
				{
					Pos[NbPos++].Set(Actual->X,Actual->Y+1);
					count++;
				}
				if(NbPos==0)
				{
					count++;
					Pos[NbPos++].Set(Actual->X+1,Actual->Y+1);
				}
			}
		}

		// Try the possibilities and see if one of them is better
		for(nb=0;nb<NbPos;nb++)
		{
			if(CurInfo->Test(Pos[nb],Max,Grid))
			{
				CurInfo->PushCenter(Pos[nb],Max,Grid);
				Dist2=Attraction.ManhattanDist(Pos[nb]+Center);				
 				CurRect=Result;
 				if(Pos[nb].X<CurRect.Pt1.X) CurRect.Pt1.X=Pos[nb].X;
 				if(Pos[nb].Y<CurRect.Pt1.Y) CurRect.Pt1.Y=Pos[nb].Y;
 				if(Pos[nb].X+CurInfo->Width()>CurRect.Pt2.X) CurRect.Pt2.X=Pos[nb].X+CurInfo->Width();
 				if(Pos[nb].Y+CurInfo->Height()>CurRect.Pt2.Y) CurRect.Pt2.Y=Pos[nb].Y+CurInfo->Height();
 				Area=CurRect.Width()*CurRect.Height();
				if(((labs(Dist2-BestDist)<MinDist)&&(Area<BestArea))||(Dist2<BestDist))
				{
					BestArea=Area;
					Best=Pos[nb];
					BestDist=Dist2;						
				}
			}
		}

		// Choose next vertice
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
//	CalcPositions();


	// Assign it
	CurInfo->Assign(Best,Grid);
	Sol.Clear();
	Sol.InsertPtr(new RPolygon(Union));
	CurInfo->Add(Sol);

	// Calculate Union of all placed polygons
	Sol.Union(&Union);
 	Union.Boundary(Result);
	cout<<"Result: ("<<Result.Pt1.X<<","<<Result.Pt1.Y<<") - ("<<Result.Pt2.X<<","<<Result.Pt2.Y<<")"<<endl;

	// Ok
	cout<<count<<endl;
	return(true);
}


//-----------------------------------------------------------------------------
void RPlacementCenter::PostRun(RPoint &limits)
{
	RGeoInfo **info;
	unsigned int i;
   RPoint tmp;

	Sol.Clear();
	for(i=NbObjs+1,info=Infos;--i;info++)
		Sol.InsertPtr(new RPolygon((*info)->GetPolygon()));
	Sol.Union(&Union);
	Union.Boundary(Result);
	limits=HoldLimits;
	Grid->Clear();
	for(i=NbObjs+1,info=Infos;--i;info++)
	{
		tmp=(*info)->GetPos()-Result.Pt1;
		(*info)->Assign(tmp,Grid);
	}
	for(Free.Start();!Free.End();Free.Next())
		(*Free())-=Result.Pt1;
}


















