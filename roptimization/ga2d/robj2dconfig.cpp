/*

	R Project Library

	RObj2DConfig.hh

	Configuration of an object for 2D placement GA - Implementation

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).

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
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <robj2dconfig.h>
#include <robj2d.h>
#include <rlayout.h>
#include <rgeoinfo.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObj2DConfigPin
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DConfigPin::RObj2DConfigPin(RObj2DPin* pin,const RRect& rect)
	: Pin(pin), Rect(rect), Pos(rect.GetPt1())
{
	RPoint Middle(rect.GetWidth()/2,rect.GetHeight()/2);
	Pos+=Middle;
}


//------------------------------------------------------------------------------
int RObj2DConfigPin::Compare(const RObj2DConfigPin& pin) const
{
	return(Pin->Compare(*pin.Pin));
}


//------------------------------------------------------------------------------
int RObj2DConfigPin::Compare(const size_t pin) const
{
	return(Pin->Compare(pin));
}



//------------------------------------------------------------------------------
//
// class RObj2DConfigConnector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DConfigConnector::RObj2DConfigConnector(RObj2DConnector* con)
	 : RContainer<RObj2DConfigPin,true,true>(4), Connector(con)
{
}


//------------------------------------------------------------------------------
int RObj2DConfigConnector::Compare(const RObj2DConfigConnector& con) const
{
	return(CompareIds(Connector->GetId(),con.Connector->GetId()));
}


//------------------------------------------------------------------------------
int RObj2DConfigConnector::Compare(const size_t con) const
{
	return(CompareIds(Connector->GetId(),con));
}


//------------------------------------------------------------------------------
int RObj2DConfigConnector::Compare(const RString& name) const
{
	return(Connector->Compare(name));
}


//------------------------------------------------------------------------------
double RObj2DConfigConnector::GetMinDist(RObj2DConfigConnector* c,RLayout* layout,RObj2DConfigPin* &pin1,RObj2DConfigPin* &pin2)
{
	tCoord min(cMaxCoord);
	RGeoInfo *Info1,*Info2;
	if(Connector->GetObj())
		Info1=(*layout)[Connector->GetObj()->GetId()];
	else
		Info1=0;
	if(c->Connector->GetObj())
		Info2=(*layout)[c->Connector->GetObj()->GetId()];
	else Info2=0;

	RCursor<RObj2DConfigPin> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RPoint p1(Cur()->GetPos());
		if(Info1)
			p1+=Info1->GetPos();
		RCursor<RObj2DConfigPin> Cur2(*c);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
		{
			RPoint p2(Cur()->GetPos());
			if(Info2)
				p2+=Info2->GetPos();
			tCoord d(p1.ManhattanDist(p2));
			if(d<min)
			{
				min=d;
				pin1=Cur();
				pin2=Cur2();
			}
		}
	}
	return(min);
}



//------------------------------------------------------------------------------
//
// RObj2DConfig
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DConfig::RObj2DConfig(RObj2D* obj,tOrientation ori)
	: Obj(obj), Orientation(ori), Connectors(10)
{
}


//------------------------------------------------------------------------------
int RObj2DConfig::Compare(const RObj2DConfig& obj) const
{
	if(Obj->GetId()==obj.Obj->GetId())
		return(Orientation-obj.Orientation);
	return(Obj->GetId()-obj.Obj->GetId());
}


//------------------------------------------------------------------------------
int RObj2DConfig::Compare(const tOrientation ori) const
{
	return(Orientation-ori);
}


//------------------------------------------------------------------------------
void RObj2DConfig::Set(const RPolygon& poly)
{
	if(Polygon.GetNbVertices())
		mThrowRException("Polygon is already set");
	Polygon=poly;
	Polygon.RectDecomposition(Rects);
	Polygon.Boundary(Rect);
	Area=Rects.Area();
}


//------------------------------------------------------------------------------
void RObj2DConfig::Set(RContainer<RRect,false,false>& rects)
{
	tCoord X1(cMaxCoord),Y1(cMaxCoord),X2(cMinCoord),Y2(cMinCoord);
	RCursor<RRect> rect(rects);
	for(rect.Start();!rect.End();rect.Next())
	{
		Rects.InsertPtr(rect());
		if(rect()->GetX1()<X1) X1=rect()->GetX1();
		if(rect()->GetY1()<Y1) Y1=rect()->GetY1();
		if(rect()->GetX2()>X2) X2=rect()->GetX2();
		if(rect()->GetY2()>Y2) Y2=rect()->GetY2();
	}
	Rect.Set(X1,Y1,X2,Y2);
	Area=Rects.Area();
	rects.Clear();
}


//------------------------------------------------------------------------------
RCursor<RObj2DConfigConnector> RObj2DConfig::GetConnectors(void) const
{
	return(RCursor<RObj2DConfigConnector>(Connectors));
}


//------------------------------------------------------------------------------
RCursor<RRect> RObj2DConfig::GetRects(void) const
{
	return(RCursor<RRect>(Rects));
}


//------------------------------------------------------------------------------
void RObj2DConfig::Add(RObj2DConfigConnector* con)
{
	Connectors.InsertPtr(con);
}


//------------------------------------------------------------------------------
RObj2DConfigConnector* RObj2DConfig::GetConnector(RObj2DConnector* con) const
{
	return(Connectors.GetPtr(con->GetId()));
}


//------------------------------------------------------------------------------
RObj2DConfigConnector* RObj2DConfig::GetConnector(const RString& con) const
{
	return(Connectors.GetPtr(con,false));
}
