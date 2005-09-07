/*

	R Project Library

	RGeoInfo.cpp

	Geometric information - Implementation.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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
#include <rgeoinfo.h>
#include <rga.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RGeoInfoConnector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoConnector::RGeoInfoConnector(RObj2DConnector* con,RGeoInfo* owner)
	: Con(con), Owner(owner)
{
	Pos=new RPoint[con->NbPos];
	NbPos=con->NbPos;
}


//------------------------------------------------------------------------------
RGeoInfoConnector::RGeoInfoConnector(RObj2DConnector* con,RGeoInfo* owner,const RPoint& pos)
	: Con(con), Owner(owner)
{
	Pos=new RPoint[con->NbPos];
	Pos[0]=pos;
	NbPos=0;
}


//------------------------------------------------------------------------------
RGeoInfoConnector::RGeoInfoConnector(RGeoInfoConnector* con,RGeoInfo* owner)
	: Con(con->Con), Owner(owner)
{
	Pos=new RPoint[con->NbPos];
	NbPos=con->NbPos;
	for(unsigned int i=0;i<NbPos;i++)
		Pos[i]=con->Pos[i];
}


//------------------------------------------------------------------------------
RPoint RGeoInfoConnector::GetPos(void) const
{
	return( RPoint(Pos[0])+=Owner->GetPos());
}



//------------------------------------------------------------------------------
//
// class RRelPointCursor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RRelPointCursor::RRelPointCursor(void)
	: RCursor<RPoint>(), Base()
{
}


//------------------------------------------------------------------------------
RRelPointCursor::RRelPointCursor(RGeoInfo& info)
	: RCursor<RPoint>(*info.Bound), Base(info.Pos)
{
}


//------------------------------------------------------------------------------
void RRelPointCursor::Set(RGeoInfo& info)
{
	RCursor<RPoint>::Set(*info.Bound);
	Base=info.Pos;
}


//------------------------------------------------------------------------------
RPoint RRelPointCursor::operator()(void) const
{
	RPoint Pt(*(*(dynamic_cast<const RCursor<RPoint>*>(this)))());
	Pt+=Base;
	return(Pt);
}



//------------------------------------------------------------------------------
//
// Class RGeoInfo
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfo::RGeoInfo(void)
	: Obj(0), Selected(false), Pos(MaxCoord,MaxCoord), Ori(-1), Bound(0),
		Rects(0), Order(NoObject), Connectors(10,5)
{
}


//------------------------------------------------------------------------------
RGeoInfo::RGeoInfo(RPolygon* poly)
	: Obj(0), Selected(false), Pos(MaxCoord,MaxCoord), Ori(-1), Bound(poly),
		Rects(0), Order(NoObject), Connectors(10,5)
{
}


//------------------------------------------------------------------------------
RGeoInfo::RGeoInfo(RObj2D* obj)
	: Obj(obj), Selected(false), Pos(MaxCoord,MaxCoord), Ori(-1), Bound(0),
		Rects(0), Order(NoObject), Connectors(10,5)
{
	RCursor<RObj2DConnector> Cur(obj->Connectors);
	for(Cur.Start();!Cur.End();Cur.Next())
		Connectors.InsertPtr(new RGeoInfoConnector(Cur(),this));
}


//------------------------------------------------------------------------------
RGeoInfo::RGeoInfo(const RGeoInfo& info)
	: Connectors(info.Connectors.GetNb(),info.Connectors.GetIncNb())
{
	Obj=info.Obj;
	Selected=info.Selected;
	Pos=info.Pos;
	Bound=info.Bound;
	Ori=info.Ori;
	Rects=info.Rects;
	Rect=info.Rect;
	Selected=info.Selected;
	Order=info.Order;
	RCursor<RGeoInfoConnector> Cur(info.Connectors);
	for(Cur.Start();!Cur.End();Cur.Next())
		Connectors.InsertPtr(new RGeoInfoConnector(Cur(),this));
}


//------------------------------------------------------------------------------
void RGeoInfo::ClearInfo(void)
{
	Selected=false;
	Pos.Set(MaxCoord,MaxCoord);
	Bound=0;
	Rects=0;
	Ori=-1;
	Order=NoObject;
}


//------------------------------------------------------------------------------
void RGeoInfo::SetOri(char i)
{
	Ori=i;
	Bound=Obj->GetPolygon(i);
	Rects=Obj->GetRects(i);
	Bound->Boundary(Rect);
	RCursor<RGeoInfoConnector> Cur(Connectors);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		for(unsigned j=0;j<Cur()->Con->NbPos;j++)
		{
			Cur()->Pos[j]=Cur()->Con->GetPos(j,i);
		}
	}
}


//------------------------------------------------------------------------------
RCoord RGeoInfo::GetArea(void) const
{
	if(Obj)
		return(Obj->GetArea());
	return(0);
}


//------------------------------------------------------------------------------
void RGeoInfo::Boundary(RRect& rect)
{
	Bound->Boundary(rect);
	rect.Pt1.X+=Pos.X;
	rect.Pt1.Y+=Pos.Y;
	rect.Pt2.X+=Pos.X;
	rect.Pt2.Y+=Pos.Y;
}


//------------------------------------------------------------------------------
void RGeoInfo::Assign(const RPoint& pos,RGrid* grid)
{
	Pos=pos;
	RCursor<RRect> rect(*Rects);
	for(rect.Start();rect.End();rect.End())
		grid->Assign(*rect(),Pos,Obj->GetId());
}


//------------------------------------------------------------------------------
bool RGeoInfo::Test(RPoint& pos,RGrid* grid)
{
	RPoint start,end;
	unsigned int nbpts;
	RDirection FromDir;
	RCoord X,Y;

	// Test it and go through the other
	start=Bound->GetBottomLeft();
	end=Bound->GetConX(&start);
	FromDir=Left;
	X=start.X+pos.X;
	Y=start.Y+pos.Y;
	nbpts=Bound->GetNb();

	// Test it and go through the other
	while(nbpts)
	{
		if(grid->IsOcc(X,Y))
			return(false);

		// If end of an edge
		if((X==end.X+pos.X)&&(Y==end.Y+pos.Y))
		{
			start=end;
			nbpts--;        // Next point
			X=start.X+pos.X;
			Y=start.Y+pos.Y;
			if((FromDir==Left)||(FromDir==Right))
			{
				end=Bound->GetConY(&start);
				if(start.Y<end.Y) FromDir=Down; else FromDir=Up;
			}
			else		// Go to left/right
			{
				end=Bound->GetConX(&start);
				if(start.X<end.X) FromDir=Left; else FromDir=Right;
			}
		}
		else
			AdaptXY(X,Y,FromDir);
	}
	return(true);
}


//------------------------------------------------------------------------------
void RGeoInfo::PushBottomLeft(RPoint& pos,RPoint& limits,RGrid* grid)
{
	RPoint TestPos;
	bool change=true;

	while(change)
	{
		change=false;

		// Push Bottom
		TestPos=pos;
		TestPos.Y--;
		while(IsValid(TestPos,limits)&&Test(TestPos,grid))
		{
			change=true;
			pos=TestPos;
			TestPos.Y--;
		}

		// Push Left
		TestPos=pos;
		TestPos.X--;
		while(IsValid(TestPos,limits)&&Test(TestPos,grid))
		{
			change=true;
			pos=TestPos;
			TestPos.X--;
		}
	}
}


//------------------------------------------------------------------------------
void RGeoInfo::PushCenter(RPoint& pos,RPoint& limits,RGrid* grid)
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
		while(IsValid(TestPos,limits)&&(TestPos.Y!=Center.Y)&&Test(TestPos,grid))
		{
			change=true;
			pos=TestPos;
			if(PushBottom) TestPos.Y--; else TestPos.Y++;
		}

		// Push Left/Right
		TestPos=pos;
		if(PushLeft) TestPos.X--; else TestPos.X++;
		while(IsValid(TestPos,limits)&&(TestPos.X!=Center.X)&&Test(TestPos,grid))
		{
			pos=TestPos;
			change=true;
			if(PushLeft) TestPos.X--; else TestPos.X++;
		}
	}
}


//------------------------------------------------------------------------------
bool RGeoInfo::Overlap(RGeoInfo* info)
{
	RRect R1,R2;

	RCursor<RRect> rect(*Rects);
	RCursor<RRect> rect2(*info->Rects);
	for(rect.Start();rect.End();rect.End())
	{
		R1=(*rect());
		R1+=Pos;
		for(rect2.Start();rect2.End();rect2.End())
		{
			R2=(*rect2());
			R2+=info->Pos;
			if(R1.Overlap(&R2))
				return(true);
    	}
	}
	return(false);
}


//------------------------------------------------------------------------------
RPoint RGeoInfo::GetPos(void) const
{
	return(RPoint(Pos));
}


//------------------------------------------------------------------------------
bool RGeoInfo::IsValid(void) const
{
	// Test Position
	if((Pos.X==MaxCoord)||(Pos.Y==MaxCoord))
		return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool RGeoInfo::IsValid(const RPoint& pos,const RPoint& limits) const
{
	if(pos.X<0) return(false);
	if(pos.Y<0) return(false);
	if(pos.X+Rect.Width()>limits.X) return(false);
	if(pos.Y+Rect.Height()>limits.Y) return(false);
	return(true);
}


//------------------------------------------------------------------------------
RGeoInfo& RGeoInfo::operator=(const RGeoInfo& info)
{
	Pos=info.Pos;
	Bound=info.Bound;
	Selected=info.Selected;
	Obj=info.Obj;
	Rects=info.Rects;
	Rect=info.Rect;
	Order=info.Order;
	Connectors.Clear();
	RCursor<RGeoInfoConnector> tab(info.Connectors);
		Connectors.InsertPtr(new RGeoInfoConnector(tab(),this));
	return(*this);
}


//------------------------------------------------------------------------------
bool RGeoInfo::IsIn(RPoint pos) const
{
	if(!IsValid()) return(false);
	pos-=Pos;
	RCursor<RRect> rect(*Rects);
	for(rect.Start();!rect.End();rect.Next())
		if(rect()->IsIn(pos)) return(true);
	return(false);
}


//------------------------------------------------------------------------------
RGeoInfoConnector* RGeoInfo::GetConnector(const RPoint& pos)
{
	RPoint p;
	unsigned int j;

	RCursor<RGeoInfoConnector> tab(Connectors);
	for(tab.Start();!tab.End();tab.Next())
	{
		for(j=0;j<tab()->NbPos;j++)
		{
			p=tab()->Pos[j]+Pos;
			if((pos.X>=p.X-1)&&(pos.X<=p.X+1)&&(pos.Y>=p.Y-1)&&(pos.Y<=p.Y+1))
				return(tab());
		}
	}
	return(0);
}


//------------------------------------------------------------------------------
void RGeoInfo::Add(RPolygons& polys)
{
	RPolygon *p;

	p=new RPolygon(*Bound);
	(*p)+=Pos;
	polys.InsertPtr(p);
}


//------------------------------------------------------------------------------
RPolygon RGeoInfo::GetPolygon(void)
{
	RPolygon poly;

	poly=(*Bound);
	poly+=Pos;
	return(poly);
}


//------------------------------------------------------------------------------
RCursor<RObj2DConnector> RGeoInfo::GetObjConnectors(void) const
{
	return RCursor<RObj2DConnector>(Obj->Connectors);
}


//------------------------------------------------------------------------------
/*void RGeoInfo::StartCon(void)
{
	bool Ok=false;

	Obj->Connectors.Start();
	while((!Obj->Connectors.End())&&(!Ok))
	{
		Obj->Connectors()->Connections.Start();
		if(Obj->Connectors()->Connections.End())
			Obj->Connectors.Next();
		else
			Ok=true;
	}
}


//------------------------------------------------------------------------------
bool RGeoInfo::EndCon(void)
{
	return(Obj->Connectors.End());
}


//------------------------------------------------------------------------------
void RGeoInfo::NextCon(void)
{
	Obj->Connectors()->Connections.Next();
	if(Obj->Connectors()->Connections.End())
	{
		bool Ok=false;
		Obj->Connectors.Next();
		while((!Obj->Connectors.End())&&(!Ok))
		{
			Obj->Connectors()->Connections.Start();
			if(Obj->Connectors()->Connections.End())
				Obj->Connectors.Next();
				else
				Ok=true;
		}
	}
}


//------------------------------------------------------------------------------
RConnection* RGeoInfo::GetCurrentCon(void)
{
	return(Obj->Connectors()->Connections());
}*/


//------------------------------------------------------------------------------
RGeoInfo::~RGeoInfo(void)
{
}
