/*

	R Project Library

	RPoint.cpp

	Point - Implementation.

	Copyright 1999-2011 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rstd.h>
#include <rpoints.h>
#include <rpolygon.h>
#include <rpolygons.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPoints
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPoints::RPoints(void)
	: RContainer<RPoint,true,false>(10,5)
{
}


//------------------------------------------------------------------------------
RPoints::RPoints(const size_t max)
	: RContainer<RPoint,true,false>(max,5)
{
}


//------------------------------------------------------------------------------
RPoints::RPoints(const RPoints& points)
	: RContainer<RPoint,true,false>(points.GetMaxNb(),points.GetIncNb())
{
	RCursor<RPoint> pts(points);
	for(pts.Start();!pts.End();pts.Next())
		InsertPtr(new RPoint(*pts()));
}


//------------------------------------------------------------------------------
RPoint RPoints::FindBottom(const RPoint& pt,const RPolygons& polys) const
{
	RPoint *Activ;
	size_t i;
	tCoord X,Y;
	tCoord AY;

	if(!GetNb()) return(RPoint::Null);
	X=pt.X;
	Y=pt.Y;
	Activ=0;
	RCursor<RPoint> point(*this);
	point.Start();
	i=GetNb()+1;
	// Find first point on bottom
	while((!Activ)&&(--i))
	{
		if((point()->X==X)&&(point()->Y==Y-1)) return(*point());
		if((point()->X==X)&&(polys.Edge(*point(),pt))&&(point()->Y<Y))
			Activ=point();
		point.Next();
	}
	if(!Activ) return(RPoint::Null);
	// Find up most point on bottom
	AY=Activ->Y;
	for(;--i;point.Next())
	{
		if((point()->X==X)&&(point()->Y==Y-1)) return(*point());
		if((point()->X==X)&&(polys.Edge(*point(),pt))&&(point()->Y>AY)&&(point()->Y<Y))
		{
			Activ=point();
			AY=Activ->Y;
    }
	}
	return(*Activ);
}


//------------------------------------------------------------------------------
RPoint RPoints::FindLeft(const RPoint& pt,const RPolygons& polys) const
{
	RPoint *Activ;
	size_t i;
	tCoord X,Y;
	tCoord AX;

	if(!GetNb()) return(RPoint::Null);
	X=pt.X;
	Y=pt.Y;
	Activ=0;
	RCursor<RPoint> point(*this);
	point.Start();
	i=GetNb()+1;
	// Find first point on left
	while((!Activ)&&(--i))
	{
		if((point()->Y==Y)&&(point()->X==X-1)) return(*point());
		if((point()->Y==Y)&&(polys.Edge(*point(),pt))&&(point()->X<X))
			Activ=point();
		point.Next();
	}
	if(!Activ) return(RPoint::Null);
	// Find right most point on left
	AX=Activ->X;
	for(;--i;point.Next())
	{
		if((point()->Y==Y)&&(point()->X==X-1)) return(*point());
		if((point()->Y==Y)&&(polys.Edge(*point(),pt))&&(point()->X>AX)&&(point()->X<X))
		{
			Activ=point();
			AX=Activ->X;
		}
	}
	return(*Activ);
}


//------------------------------------------------------------------------------
RPoint RPoints::FindRight(const RPoint& pt,const RPolygons& polys) const
{
	if(!GetNb()) return(RPoint::Null);

	RPoint* Activ(0);
	size_t i;
	tCoord X(pt.X), Y(pt.Y);
	RCursor<RPoint> point(*this);
	point.Start();
	i=GetNb()+1;
	// Find first point on right
	while((!Activ)&&(--i))
	{
		if((point()->Y==Y)&&(point()->X==X+1)) return(*point());
		if((point()->Y==Y)&&(polys.Edge(*point(),pt))&&(point()->X>X))
			Activ=point();
		point.Next();
	}
	if(!Activ) return(RPoint::Null);

	// Find left most point on right
	tCoord AX(Activ->X);
	for(;--i;point.Next())
	{
		if((point()->Y==Y)&&(point()->X==X+1)) return(*point());
		if((point()->Y==Y)&&(polys.Edge(*point(),pt))&&(point()->X<AX)&&(point()->X>X))
		{
			Activ=point();
			AX=Activ->X;
		}
	}
	return(*Activ);
}


//------------------------------------------------------------------------------
RPoint RPoints::FindUp(const RPoint& pt,const RPolygons& polys) const
{
	if(!GetNb()) return(RPoint::Null);

	RPoint* Activ(0);
	size_t i;
	tCoord X(pt.X),Y(pt.Y);
	RCursor<RPoint> point(*this);
	point.Start();
	i=GetNb()+1;
	// Find first point on up
	while((!Activ)&&(--i))
	{
		if((point()->X==X)&&(point()->Y==Y+1)) return(*point());
		if((point()->X==X)&&(polys.Edge(*point(),pt))&&(point()->Y>Y))
			Activ=point();
		point.Next();
	}
	if(!Activ) return(RPoint::Null);

	// Find bottom most point on up
	tCoord AY(Activ->Y);
	for(;--i;point.Next())
	{
		if((point()->X==X)&&(point()->Y==Y+1)) return(*point());
		if((point()->X==X)&&(polys.Edge(*point(),pt))&&(point()->Y<AY)&&(point()->Y>Y))
		{
			Activ=point();
			AY=Activ->Y;
		}
	}
	return(*Activ);
}


//------------------------------------------------------------------------------
RPoint RPoints::FindBottomLeft(void) const
{
	RPoint *Activ;
	size_t i;
	tCoord X,Y;

	if(!GetNb()) return(RPoint::Null);
	RCursor<RPoint> point(*this);
	point.Start();
	Activ=point();
	X=Activ->X;
	Y=Activ->Y;
	for(i=GetNb(),point.Next();--i;point.Next())
		if((point()->Y<Y)||((point()->Y==Y)&&(point()->X<X)))
		{
			Activ=point();
			X=Activ->X;
			Y=Activ->Y;
		}
	return(*Activ);
}


//------------------------------------------------------------------------------
bool RPoints::DuplicatePoints(void) const
{
	size_t i;

	RCursor<RPoint> Cur1(*this);
	RCursor<RPoint> Cur2(*this);
	for(i=0,Cur1.Start();i<GetNb()-1;Cur1.Next(),i++)
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.End())
			if((*Cur1())==(*Cur2()))
				return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPoints& RPoints::operator=(const RPoints& points)
{
	RContainer<RPoint,true,false>::operator=(points);
	return(*this);
}
