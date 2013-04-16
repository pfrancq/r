/*

	R Project Library

	Polygons.cpp

	Polygons - Implementation.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <iostream>


//------------------------------------------------------------------------------
// include files for R Project
#include <rpolygons.h>
#include <rpoints.h>
#include <rrect.h>
#include <rline.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RPolygons
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPolygons::RPolygons(size_t nb)
	: RContainer<RPolygon,true,false>(nb)
{
}


//------------------------------------------------------------------------------
bool RPolygons::Edge(const RPoint& pt) const
{
	RCursor<RPolygon> poly(*this);
	for(poly.Start();!poly.End();poly.Next())
		if(poly()->Edge(pt))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygons::Edge(const RPoint& pt,const RPolygon& poly) const
{
	RCursor<RPolygon> polys(*this);
	for(polys.Start();!polys.End();polys.Next())
		if((polys()!=&poly)&&(polys()->Edge(pt)))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygons::Edge(const RPoint& pt1,const RPoint& pt2) const
{
	RCursor<RPolygon> poly(*this);
	for(poly.Start();!poly.End();poly.Next())
		if(poly()->Edge(pt1,pt2))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void RPolygons::PutPoints(RPoints& points) const
{
	RPoint tmp;

	points.Clear();
	RCursor<RPolygon> poly(*this);
	for(poly.Start();!poly.End();poly.Next())
		poly()->AddVertices(points);
	for(poly.Start();!poly.End();poly.Next())
	{
		RCursor<RPoint> point(poly()->GetVertices());
		for(point.Start();!point.End();point.Next())
		{
			tmp.X=point()->X;
			tmp.Y=point()->Y-1;
			if(!points.IsIn(tmp))
				if(Edge(tmp,*poly()))
					points.InsertPtr(new RPoint(tmp));
			tmp.Y=point()->Y+1;
			if(!points.IsIn(tmp))
				if(Edge(tmp,*poly()))
					points.InsertPtr(new RPoint(tmp));
			tmp.X=point()->X+1;
			tmp.Y=point()->Y;
			if(!points.IsIn(tmp))
				if(Edge(tmp,*poly()))
					points.InsertPtr(new RPoint(tmp));
				tmp.X=point()->X-1;
			if(!points.IsIn(tmp))
				if(Edge(tmp,*poly()))
					points.InsertPtr(new RPoint(tmp));
		}
	}
}


//------------------------------------------------------------------------------
void RPolygons::Union(RPolygon& upoly) const
{
	RPoint next,first,*ins,last;
	tDirection FromDir;
	RPoints pts(100);

	// Init Part
	upoly.Clear();
	PutPoints(pts);

	// Find the most (left,bottom) point -> curpt,curpoly -> next pt on the right
	last=first=next=pts.FindBottomLeft();
	ins=new RPoint(next);
	upoly.InsertVertex(ins);
	next=pts.FindRight(next,*this);
	FromDir=dLeft;

	// While nextpt!=firspt
	while(next!=first)
	{
		ins=new RPoint(next);
		upoly.InsertVertex(ins);
		last=next;

		switch(FromDir)
		{

			case dLeft :
				next=pts.FindBottom(last,*this);
				if(next.IsValid())
				{
					FromDir=dUp;
					break;
				}
				next=pts.FindRight(last,*this);
				if(next.IsValid())
				{
					upoly.DeleteVertex(ins);
					break;
				}
				next=pts.FindUp(last,*this);
				if(next.IsValid())
				{
					FromDir=dDown;
					break;
				}
				FromDir=dNoDirection;
				break;

			case dRight :
				next=pts.FindUp(last,*this);
				if(next.IsValid())
				{
					FromDir=dDown;
					break;
				}
				next=pts.FindLeft(last,*this);
				if(next.IsValid())
				{
					upoly.DeleteVertex(ins);
					break;
				}
				next=pts.FindBottom(last,*this);
				if(next.IsValid())
				{
					FromDir=dUp;
					break;
				}
				FromDir=dNoDirection;
				break;

			case dUp :
				next=pts.FindLeft(last,*this);
				if(next.IsValid())
				{
					FromDir=dRight;
					break;
				}
				next=pts.FindBottom(last,*this);
				if(next.IsValid())
				{
					upoly.DeleteVertex(ins);
					break;
				}
				next=pts.FindRight(last,*this);
				if(next.IsValid())
				{
					FromDir=dLeft;
					break;
				}
				FromDir=dNoDirection;
				break;

			case dDown :
				next=pts.FindRight(last,*this);
				if(next.IsValid())
				{
					FromDir=dLeft;
					break;
				}
				next=pts.FindUp(last,*this);
				if(next.IsValid())
				{
					upoly.DeleteVertex(ins);
					break;
				}
				next=pts.FindLeft(last,*this);
				if(next.IsValid())
				{
					FromDir=dRight;
					break;
				}
				FromDir=dNoDirection;
				break;

			case dNoDirection:
				RAssertMsg("Direction can't be undefined");
				break;

			default:
				RAssertMsg("Not a valid Direction in this context");
				break;
		}
		RAssert(next.IsValid());
	}
}


//------------------------------------------------------------------------------
bool RPolygons::DuplicatePoints(void) const
{
	RPoints tmp(500);

	PutPoints(tmp);
	return(tmp.DuplicatePoints());
}


//------------------------------------------------------------------------------
bool RPolygons::IsIn(const tCoord X,const tCoord Y) const
{
	RCursor<RPolygon> poly(*this);
	for(poly.Start();!poly.End();poly.Next())
		if(poly()->IsIn(X,Y))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygons::IsIn(const RPoint& pt) const
{
	RCursor<RPolygon> poly(*this);
	for(poly.Start();!poly.End();poly.Next())
		if(poly()->IsIn(pt))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPolygons& RPolygons::operator=(const RPolygons& poly)
{
	RContainer<RPolygon,true,false>::operator=(poly);
	return(*this);
}


//------------------------------------------------------------------------------
void RPolygons::Save(RTextFile& f)
{
	f<<GetNb()<<endl;
	RCursor<RPolygon> poly(*this);
	for(poly.Start();!poly.End();poly.Next())
		poly()->Save(f);
}


//------------------------------------------------------------------------------
RCursor<RPolygon> RPolygons::GetPolygonsCursor(void)
{
	return(RCursor<RPolygon>(*this));
}
