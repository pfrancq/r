/*

	R Project Library

	Polygons.cpp

	Polygons - Implentation.

	Copyright 1999-2004 by the Universit�Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <iostream>


//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/rpolygons.h>
#include <rmath/rpoints.h>
#include <rmath/rrect.h>
#include <rmath/rline.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RPolygons
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPolygons::RPolygons(void)
	: RContainer<RPolygon,true,false>(20,10)
{
}


//------------------------------------------------------------------------------
bool RPolygons::Edge(const RPoint* pt) const
{
	RPolygon** poly;
	unsigned int i;

	RReturnValIfFail(pt,false);
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		if((*poly)->Edge(pt))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygons::Edge(const RPoint* pt,const RPolygon* poly) const
{
	RPolygon** ptr;
	unsigned int i;

	RReturnValIfFail(pt&&poly,false);
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
		if(((*ptr)!=poly)&&((*ptr)->Edge(pt)))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygons::Edge(const RPoint* pt1,const RPoint* pt2) const
{
	RPolygon** poly;
	unsigned int i;

	RReturnValIfFail(pt1&&pt2,false);
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		if((*poly)->Edge(pt1,pt2))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
void RPolygons::PutPoints(RPoints* points) const
{
	RPolygon** poly;
	RPoint** point;
	RPoint tmp;
	unsigned int i,j;

	RReturnIfFail(points);
	points->Clear();
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		(*poly)->AddPoints(points);
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		for(j=(*poly)->NbPtr+1,point=(*poly)->Tab;--j;point++)
		{
			tmp.X=(*point)->X;
			tmp.Y=(*point)->Y-1;
			if(!points->IsIn<RPoint>(tmp))
				if(Edge(&tmp,*poly))
					points->InsertPtr(new RPoint(tmp));
			tmp.Y=(*point)->Y+1;
			if(!points->IsIn<RPoint>(tmp))
				if(Edge(&tmp,*poly))
					points->InsertPtr(new RPoint(tmp));
			tmp.X=(*point)->X+1;
			tmp.Y=(*point)->Y;
			if(!points->IsIn<RPoint>(tmp))
				if(Edge(&tmp,*poly))
					points->InsertPtr(new RPoint(tmp));
				tmp.X=(*point)->X-1;
			if(!points->IsIn<RPoint>(tmp))
				if(Edge(&tmp,*poly))
					points->InsertPtr(new RPoint(tmp));
		}
}


//------------------------------------------------------------------------------
void RPolygons::Union(RPolygon* upoly) const
{
	RPoint *next,*first,*ins,*last;
	RDirection FromDir;
	RPoints pts(100);

	// Init Part
	RReturnIfFail(upoly);
	upoly->Clear();
	PutPoints(&pts);

	// Find the most (left,bottom) point -> curpt,curpoly -> next pt on the right
	last=first=next=pts.FindBottomLeft();
	RAssert(last);
	ins=new RPoint(next);
	upoly->InsertPtr(ins);
	next=pts.FindRight(next,this);
	RAssert(next);
	FromDir=Left;

	// While nextpt!=firspt
	while((*next)!=(*first))
	{
		ins=new RPoint(next);
		upoly->InsertPtr(ins);
		last=next;

		switch(FromDir)
		{

			case Left :
				if((next=pts.FindBottom(last,this)))
				{
					FromDir=Up;
					break;
				}
				if((next=pts.FindRight(last,this)))
				{
					upoly->DeletePtr(ins);
					break;
				}
				if((next=pts.FindUp(last,this)))
				{
					FromDir=Down;
					break;
				}
				FromDir=NoDirection;
				break;

			case Right :
				if((next=pts.FindUp(last,this)))
				{
					FromDir=Down;
					break;
				}
				if((next=pts.FindLeft(last,this)))
				{
					upoly->DeletePtr(ins);
					break;
				}
				if((next=pts.FindBottom(last,this)))
				{
					FromDir=Up;
					break;
				}
				FromDir=NoDirection;
				break;

			case Up :
				if((next=pts.FindLeft(last,this)))
				{
					FromDir=Right;
					break;
				}
				if((next=pts.FindBottom(last,this)))
				{
					upoly->DeletePtr(ins);
					break;
				}
				if((next=pts.FindRight(last,this)))
				{
					FromDir=Left;
					break;
				}
				FromDir=NoDirection;
				break;

			case Down :
				if((next=pts.FindRight(last,this)))
				{
					FromDir=Left;
					break;
				}
				if((next=pts.FindUp(last,this)))
				{
					upoly->DeletePtr(ins);
					break;
				}
				if((next=pts.FindLeft(last,this)))
				{
					FromDir=Right;
					break;
				}
				FromDir=NoDirection;
				break;

			case NoDirection:
				RAssertMsg("Direction can't be undefined");
				break;

			default:
				RAssertMsg("Not a valid Direction in this context");
				break;
		}
		RAssert(next);
	}
}


//------------------------------------------------------------------------------
bool RPolygons::DuplicatePoints(void) const
{
	RPoints tmp(500);

	PutPoints(&tmp);
	return(tmp.DuplicatePoints());
}


//------------------------------------------------------------------------------
bool RPolygons::IsIn(const RCoord X,const RCoord Y) const
{
	RPolygon **tab;
	unsigned int i;

	for(i=NbPtr+1,tab=Tab;--i;tab++)
		if((*tab)->IsIn(X,Y))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygons::IsIn(const RPoint& pt) const
{
	RPolygon **tab;
	unsigned int i;

	for(i=NbPtr+1,tab=Tab;--i;tab++)
		if((*tab)->IsIn(pt))
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
	f<<NbPtr<<endl;
	for(Start();!End();Next())
		(*this)()->Save(f);
}


//------------------------------------------------------------------------------
RCursor<RPolygon> RPolygons::GetPolygonsCursor(void)
{
	RCursor<RPolygon> cur(this);
	return(cur);
}
