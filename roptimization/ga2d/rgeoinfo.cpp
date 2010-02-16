/*

	R Project Library

	RGeoInfo.cpp

	Geometric information - Implementation.

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
#include <rgeoinfo.h>
#include <rga.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RGeoInfoPin
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoPin::RGeoInfoPin(RObj2DConfigPin* pin,RGeoInfo* info)
	: Pin(pin), Info(info)
{
	if(!Pin)
		ThrowRException("Invalid pin");
}


//------------------------------------------------------------------------------
int RGeoInfoPin::Compare(const RGeoInfoPin& pin) const
{
	RPoint p1(Pin->GetPos());
	if(Info)
		p1+=Info->GetPos();
	RPoint p2(pin.Pin->GetPos());
	if(pin.Info)
		p2+=pin.Info->GetPos();
    tCoord X(p1.X-p2.X);
    if(fabs(X)<cEpsi)
    {
    	tCoord Y(p1.Y-p2.Y);
    	if(fabs(X)<cEpsi)
    		return(0);
    	else
    	{
    		if(Y>0)
    			return(-1);
    		else
    			return(1);
    	}
    }
    else
    {
    	if(X<0)
    		return(-1);
    	else
    		return(1);
    }
}


//------------------------------------------------------------------------------
RPoint RGeoInfoPin::GetPos(void) const
{
	RPoint Pos(Pin->GetPos());
	if(Info)
		Pos+=Info->GetPos();
	return(Pos);
}



//------------------------------------------------------------------------------
//
// class RGeoInfoCursor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoCursor::RGeoInfoCursor(void)
	: RCursor<RPoint>(), Base()
{
}


//------------------------------------------------------------------------------
RGeoInfoCursor::RGeoInfoCursor(RGeoInfo& info)
	: RCursor<RPoint>(), Base()
{
	Set(info);
}


//------------------------------------------------------------------------------
void RGeoInfoCursor::Set(RGeoInfo& info)
{
	if(!info.GetConfig())
		ThrowRException("The geometric information has no configuration specified");
	RCursor<RPoint>::Set(info.GetConfig()->GetPolygon());
	Base=info.GetPos();
}


//------------------------------------------------------------------------------
RPoint RGeoInfoCursor::operator()(void) const
{
	RPoint Pt(*RCursor<RPoint>::operator()());
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
	: Obj(0), Config(0), Pos(RPoint::Null), Container(0), Order(cNoRef)
{
}


//------------------------------------------------------------------------------
RGeoInfo::RGeoInfo(RObj2D* obj)
	: Obj(obj), Config(0), Pos(RPoint::Null), Container(0), Order(cNoRef)
{
	if(!obj)
		ThrowRException("Null object is not allowed");
}


//------------------------------------------------------------------------------
int RGeoInfo::Compare(const RGeoInfo&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
RGeoInfo::RGeoInfo(const RGeoInfo& info)
{
	Obj=info.Obj;
	Pos=info.Pos;
	Config=info.Config;
	Container=info.Container;
	Order=info.Order;
}


//------------------------------------------------------------------------------
void RGeoInfo::ClearInfo(void)
{
	Container=0;
	Pos=RPoint::Null;
	Config=0;
	Order=cNoRef;
}


//------------------------------------------------------------------------------
tCoord RGeoInfo::GetArea(void) const
{
	if(!Config)
		ThrowRException("No configuration defined");
	return(Config->GetArea());
}


//------------------------------------------------------------------------------
void RGeoInfo::Boundary(RRect& rect)
{
	Config->GetPolygon().Boundary(rect);
	rect+=Pos;
}


//------------------------------------------------------------------------------
void RGeoInfo::SetContainer(RObj2DContainer* container)
{
	Container=container;
}


//------------------------------------------------------------------------------
void RGeoInfo::SetConfig(tOrientation ori)
{
	Config=Obj->GetPtr(ori);
}


//------------------------------------------------------------------------------
void RGeoInfo::Assign(const RPoint& pos,RGrid* grid,size_t order)
{
	if(!Config)
		ThrowRException("No configuration defined");
	Pos=pos;
	if(order!=cNoRef)
		Order=order;
	if(!grid)
		return;
	RCursor<RRect> Rect(Config->GetRects());
	for(Rect.Start();!Rect.End();Rect.Next())
		grid->Assign(*Rect(),Pos,Obj->GetId());
}


//------------------------------------------------------------------------------
bool RGeoInfo::IsValid(const RPoint& pos,const RSize& limits) const
{
	if(pos.X<0) return(false);
	if(pos.Y<0) return(false);
	if(pos.X+Config->GetWidth()>limits.GetWidth()) return(false);
	if(pos.Y+Config->GetHeight()>limits.GetHeight()) return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool RGeoInfo::Test(RPoint& pos,RGrid* grid)
{
	if(!Config)
		ThrowRException("No configuration defined");

/*	RPoint start,end;
	size_t nbpts;
	tDirection FromDir;
	tCoord X,Y;
	const RPolygon& Bound(Config->GetPolygon());

	// Test it and go through the other
	start=Bound.GetBottomLeft();
	end=Bound.GetConX(start);
	FromDir=dLeft;
	X=start.X+pos.X;
	Y=start.Y+pos.Y;
	nbpts=Bound.GetNb();

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
			if((FromDir==dLeft)||(FromDir==dRight))
			{
				end=Bound.GetConY(start);
				if(start.Y<end.Y) FromDir=dDown; else FromDir=dUp;
			}
			else		// Go to left/right
			{
				end=Bound.GetConX(start);
				if(start.X<end.X) FromDir=dLeft; else FromDir=dRight;
			}
		}
		else
			AdaptXY(X,Y,FromDir);
	}
	return(true);*/

	RCursor<RRect> Rect(Config->GetRects());
	for(Rect.Start();!Rect.End();Rect.Next())
	{
		RRect rect(*Rect());
		rect+=pos;
		if(grid->IsOcc(rect))
			return(false);
	}
	return(true);
}


//------------------------------------------------------------------------------
void RGeoInfo::PushBottomLeft(RPoint& pos,const RSize& limits,RGrid* grid)
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
void RGeoInfo::PushCenter(RPoint& pos,const RSize& limits,RGrid* grid)
{
	bool PushLeft,PushBottom;
	RPoint TestPos;
	RPoint Center;
	bool change=true;

	Center.Set(limits.GetWidth()/2,limits.GetHeight()/2);
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
bool RGeoInfo::Overlap(RGeoInfo* info) const
{
	if((!Config)||(!info->Config))
		ThrowRException("No configuration defined");

	RRect R1,R2;

	RCursor<RRect> rect(Config->GetRects());
	RCursor<RRect> rect2(info->Config->GetRects());
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
RGeoInfo& RGeoInfo::operator=(const RGeoInfo& info)
{
	Pos=info.Pos;
	Config=info.Config;
	Container=info.Container;
	Obj=info.Obj;
	Order=info.Order;
	return(*this);
}


//------------------------------------------------------------------------------
bool RGeoInfo::IsIn(RPoint pos) const
{
	if(!Config)
		ThrowRException("No configuration defined");

	if(!IsValid()) return(false);
	pos-=Pos;
	RCursor<RRect> rect(Config->GetRects());
	for(rect.Start();!rect.End();rect.Next())
		if(rect()->IsIn(pos)) return(true);
	return(false);
}


//------------------------------------------------------------------------------
void RGeoInfo::Add(RPolygons& polys)
{
	if(!Config)
		return;
	RPolygon *p(new RPolygon(Config->GetPolygon()));
	(*p)+=Pos;
	polys.InsertPtr(p);
}


//------------------------------------------------------------------------------
RPolygon RGeoInfo::GetPlacedPolygon(void) const
{
	RPolygon poly(Config->GetPolygon());
	poly+=Pos;
	return(poly);
}


//------------------------------------------------------------------------------
RGeoInfo::~RGeoInfo(void)
{
}
