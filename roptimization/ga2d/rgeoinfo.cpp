/*

	Rainbow Library Project

	RGeoInfo.cpp

	Geometric information - Implementation.

	(C) 1999-2000 by P. Francq.

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
#include "rgeoinfo.h"
using namespace RGA;



//-----------------------------------------------------------------------------
//
// Class "RGeoInfoConnector"
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeoInfoConnector::RGeoInfoConnector(RObj2DConnector *con,RGeoInfo* owner,const RPoint &pos)
	: Con(con), Owner(owner), Pos(pos)
{
}

//-----------------------------------------------------------------------------
RPoint& RGeoInfoConnector::GetPos(void)
{
	RPoint *pt=RPoint::GetPoint();
	
	(*pt)=Pos;
	return(*pt);
}



//-----------------------------------------------------------------------------
//
// Class "RGeoInfo"
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeoInfo::RGeoInfo(RObj2D *obj)
	: Obj(obj), Selected(false), Pos(MaxCoord,MaxCoord), Ori(-1), Bound(0),
		Rects(0)
{
}


//-----------------------------------------------------------------------------
RGeoInfo::RGeoInfo(RGeoInfo *info)
{
	Obj=info->Obj;
	Selected=info->Selected;
	Pos=info->Pos;
	Bound=info->Bound;
	Ori=info->Ori;
	Rects=info->Rects;
	Rect=info->Rect;
}


//-----------------------------------------------------------------------------
void RGeoInfo::Clear(void)
{
	Selected=false;
	Pos.Set(MaxCoord,MaxCoord);
	Bound=0;
	Rects=0;	
	Ori=-1;
}


//-----------------------------------------------------------------------------
void RGeoInfo::SetOri(char i)
{
	Ori=i;
	Bound=Obj->GetPolygon(i);
	Rects=Obj->GetRects(i);
	Bound->Boundary(Rect);
}


//-----------------------------------------------------------------------------
RCoord RGeoInfo::GetArea(void)
{
	if(Obj)
		return(Obj->GetArea());
	return(0);
}


//-----------------------------------------------------------------------------
void RGeoInfo::Boundary(RRect &rect)
{
	Bound->Boundary(rect);
	rect.Pt1.X+=Pos.X;
	rect.Pt1.Y+=Pos.Y;
	rect.Pt2.X+=Pos.X;
	rect.Pt2.Y+=Pos.Y;
}


//-----------------------------------------------------------------------------
void RGeoInfo::Assign(const RPoint &pos,RGrid *grid)
{
	RRect **rect;
	unsigned int i;

	Pos=pos;
	for(i=Rects->NbPtr+1,rect=Rects->Tab;--i;rect++)
		grid->Assign(**rect,Pos,Obj->GetId());
}


//-----------------------------------------------------------------------------
bool RGeoInfo::Test(RPoint &pos,RPoint &limits,RGrid *grid)
{
	RRect Test;
	RPoint *start,*end;
	unsigned int nbpts;
	RDirection FromDir;
	RCoord X,Y;

	// Test % limits
	if((pos.X<0)||(pos.Y<0)) return(false);
	Test=Rect;
	Test+=pos;
	if(Test.Clip(limits)) return(false);	

	// Test it and go through the other
	start=Bound->GetBottomLeft();
	end=Bound->GetConX(start);
	FromDir=Left;
	X=start->X+pos.X;
	Y=start->Y+pos.Y;
	nbpts=Bound->NbPtr;

	// Test it and go through the other
	while(nbpts)
	{
		if(grid->IsOcc(X,Y)) return(false);

		// If end of an edge
		if((X==end->X+pos.X)&&(Y==end->Y+pos.Y))
		{
			start=end;
			nbpts--;			// Next point
			X=start->X+pos.X;
			Y=start->Y+pos.Y;
			if((FromDir==Left)||(FromDir==Right))
			{
				end=Bound->GetConY(start);
				if(start->Y<end->Y) FromDir=Down; else FromDir=Up;
			}
			else		// Go to left/right
			{
				end=Bound->GetConX(start);
				if(start->X<end->X) FromDir=Left; else FromDir=Right;
			}
		}
		else
			AdaptXY(X,Y,FromDir);
	}
	return(true);
}


//-----------------------------------------------------------------------------
void RGeoInfo::PushBottomLeft(RPoint &pos,RPoint &limits,RGrid *grid)
{
	RPoint TestPos;
	bool change=true;

	while(change)
	{
		change=false;

   	// Push Bottom
   	TestPos=pos;
  		TestPos.Y--;
   	while((TestPos.Y>=0)&&Test(TestPos,limits,grid))
   	{
			change=true;
   		pos=TestPos;
   		TestPos.Y--;
   	}

   	// Push Left
   	TestPos=pos;
  		TestPos.X--;
   	while((TestPos.X>=0)&&Test(TestPos,limits,grid))
   	{
			change=true;
   		pos=TestPos;
   		TestPos.X--;
	  	}	

	}
}


//-----------------------------------------------------------------------------
void RGeoInfo::PushCenter(RPoint &pos,RPoint &limits,RGrid *grid)
{
	bool PushLeft,PushBottom;
	RPoint TestPos;
	RPoint Center;
	bool change=true;

	Center.Set(limits.X/2,limits.Y/2);
	PushLeft=(pos.X-Center.X>0);
	PushBottom=(pos.Y-Center.Y>0);

	while(change)
	{
		change=false;

   	// Push Bottom/Up
   	TestPos=pos;
   	if(PushBottom) TestPos.Y--; else TestPos.Y++;
   	while((TestPos.Y!=Center.Y)&&Test(TestPos,limits,grid))
   	{
   		change=true;
   		pos=TestPos;
   		if(PushBottom) TestPos.Y--; else TestPos.Y++;
   	}

   	// Push Left/Right
   	TestPos=pos;
   	if(PushLeft) TestPos.X--; else TestPos.X++;
   	while((TestPos.X!=Center.X)&&Test(TestPos,limits,grid))
   	{
   		pos=TestPos;
   		change=true;
   		if(PushLeft) TestPos.X--; else TestPos.X++;
   	}	

	}

}


//-----------------------------------------------------------------------------
bool RGeoInfo::Overlap(RGeoInfo *info)
{
	RRect **rect1,**rect2;
	unsigned int i,j;
	RRect R1,R2;

	for(i=Rects->NbPtr+1,rect1=Rects->Tab;--i;rect1++)
	{
		R1=(**rect1);
		R1+=Pos;
		for(j=info->Rects->NbPtr+1,rect2=info->Rects->Tab;--j;rect2++)
		{
			R2=(**rect2);
			R2+=Pos;
			if(R1.Overlap(&R2))
				return(true);
    }
	}
	return(false);
}


//-----------------------------------------------------------------------------
RPoint& RGeoInfo::GetPos(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Pos;
	return(*pt);
}


//-----------------------------------------------------------------------------
RPoint& RGeoInfo::operator()(void)
{
	RPoint *Pt=RPoint::GetPoint();

	(*Pt)=(*(*Bound)());
	(*Pt)+=Pos;
	return(*Pt);
}


//-----------------------------------------------------------------------------
bool RGeoInfo::IsValid(void) const
{
	// Test Position
	if((Pos.X==MaxCoord)||(Pos.Y==MaxCoord))
		return(false);
	return(true);
}


//-----------------------------------------------------------------------------
RGeoInfo& RGeoInfo::operator=(const RGeoInfo &info)
{
	Pos=info.Pos;
	Bound=info.Bound;
	return(*this);
}


//-----------------------------------------------------------------------------
bool RGeoInfo::IsIn(RPoint pos)
{
	unsigned int i;
	RRect **rect;

	if(!IsValid()) return(false);
	pos-=Pos;
	for(i=Rects->NbPtr+1,rect=Rects->Tab;--i;rect++)
		if((*rect)->IsIn(pos)) return(true);
	return(false);
}


//-----------------------------------------------------------------------------
void RGeoInfo::Add(RPolygons &polys)
{
  RPolygon *p;

	p=new RPolygon(Bound);
	(*p)+=Pos;
	polys.InsertPtr(p);
}


//-----------------------------------------------------------------------------
RPolygon& RGeoInfo::GetPolygon(void)
{
	RPolygon *poly=RPolygon::GetPolygon();

	(*poly)=(*Bound);
	(*poly)+=Pos;
	return(*poly);
}



//-----------------------------------------------------------------------------
//
// RGeoInfos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeoInfos::RGeoInfos(unsigned int nb)
	: RContainer<RGeoInfo,unsigned int,false,false>(nb,1)
{
}


//-----------------------------------------------------------------------------
void RGeoInfos::Boundary(RRect &rect)
{

	RGeoInfo **ptr;
	unsigned int i;
	RRect tmp;

	rect.Pt1.X=rect.Pt1.Y=MaxCoord;
	rect.Pt2.X=rect.Pt2.Y=0;
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		(*ptr)->Boundary(tmp);
		if(tmp.Pt1.X<rect.Pt1.X) rect.Pt1.X=tmp.Pt1.X;
		if(tmp.Pt1.Y<rect.Pt1.Y) rect.Pt1.Y=tmp.Pt1.Y;
		if(tmp.Pt2.X>rect.Pt2.X) rect.Pt2.X=tmp.Pt2.X;
		if(tmp.Pt2.Y>rect.Pt2.Y) rect.Pt2.Y=tmp.Pt2.Y;
	}
}
