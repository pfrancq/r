/*

	Rainbow Library Project

	RPoint.cpp

	Point - Implementation.

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
// include files for Rainbow
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rpoints.h>
#include <rgeometry/rpolygon.h>
#include <rgeometry/rpolygons.h>
using namespace RGeometry2D;



//-----------------------------------------------------------------------------
//
// Class "RPoint"
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPoint::RPoint(void)
{
	X=Y=0;
}


//-----------------------------------------------------------------------------
RPoint::RPoint(const RCoord x,const RCoord y)
{
	X=x;
	Y=y;
}


//-----------------------------------------------------------------------------
RPoint::RPoint(const RPoint& pt)
{
	X=pt.X;
	Y=pt.Y;
}


//-----------------------------------------------------------------------------
RPoint::RPoint(const RPoint *pt)
{
	RReturnIfFail(pt);
	if(!pt) return;
	X=pt->X;
	Y=pt->Y;
}


//-----------------------------------------------------------------------------
RPoint* RPoint::GetPoint(void)
{
	return(GetTemporaryObject<RPoint,30>());
}



//-----------------------------------------------------------------------------
//
// Class "RPoints"
// Problem: can't remove this function to rpoints.cpp -> linker problem?
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPoint* RPoints::FindBottom(RPoint *pt,RPolygons *polys)
{
	RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AY;

	RReturnValIfFail(pt&&polys,0);
	if(!NbPtr) return(0);
	X=pt->X;
	Y=pt->Y;
	Activ=0;
	point=Tab;
	i=NbPtr+1;
	// Find first point on bottom
	while((!Activ)&&(--i))
	{
		if(((*point)->X==X)&&((*point)->Y==Y-1)) return(*point);
		if(((*point)->X==X)&&(polys->Edge(*point,pt))&&((*point)->Y<Y))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(0);
	// Find up most point on bottom
	AY=Activ->Y;
	for(;--i;point++)
	{
		if(((*point)->X==X)&&((*point)->Y==Y-1)) return(*point);
		if(((*point)->X==X)&&(polys->Edge(*point,pt))&&((*point)->Y>AY)&&((*point)->Y<Y))
		{
			Activ=(*point);	
			AY=Activ->Y;
    }
	}
	return(Activ);
}



//-----------------------------------------------------------------------------
//
// Global Functions and Operators
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPoint& RGeometry2D::operator+(const RPoint &arg1,const RPoint &arg2)
{
	RPoint *res=RPoint::GetPoint();
	(*res)=arg1;
	return((*res)+=arg2);
}


//-----------------------------------------------------------------------------
RPoint& RGeometry2D::operator-(const RPoint &arg1,const RPoint &arg2)
{
	RPoint *res=RPoint::GetPoint();
	(*res)=arg1;
	return((*res)-=arg2);
}
