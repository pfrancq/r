/*

	R Project Library

	RPoint.cpp

	Point - Implementation.

	(C) 1999-2001 by P. Francq.

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
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoints.h>
#include <rgeometry/rpolygon.h>
#include <rgeometry/rpolygons.h>
using namespace RGeometry2D;



//-----------------------------------------------------------------------------
//
// RPoints
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeometry2D::RPoints::RPoints(void)
	: RContainer<RPoint,unsigned int,true,false>(10,5)
{
}


//-----------------------------------------------------------------------------
RGeometry2D::RPoints::RPoints(unsigned int max)
	: RContainer<RPoint,unsigned int,true,false>(max,5)
{
}


//-----------------------------------------------------------------------------
RGeometry2D::RPoints::RPoints(RPoints *points)
	: RContainer<RPoint,unsigned int,true,false>(points->MaxPtr,points->IncPtr)
{
	RPoint **pts;
	unsigned int i;

	RReturnIfFail(points);
	if(!points) return;
	for(i=points->NbPtr+1,pts=points->Tab;--i;pts++)
		InsertPtr(new RPoint(*pts));
}


//-----------------------------------------------------------------------------
RPoint* RGeometry2D::RPoints::FindLeft(RPoint *pt,RPolygons *polys)
{
	RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AX;

	RReturnValIfFail(pt&&polys,0);
	if(!NbPtr) return(0);
	X=pt->X;
	Y=pt->Y;
	Activ=0;
	point=Tab;
	i=NbPtr+1;
	// Find first point on left
	while((!Activ)&&(--i))
	{
		if(((*point)->Y==Y)&&((*point)->X==X-1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Edge(*point,pt))&&((*point)->X<X))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(0);
	// Find right most point on left
	AX=Activ->X;
	for(;--i;point++)
	{
		if(((*point)->Y==Y)&&((*point)->X==X-1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Edge(*point,pt))&&((*point)->X>AX)&&((*point)->X<X))
		{
			Activ=(*point);	
			AX=Activ->X;
		}
	}
	return(Activ);
}


//-----------------------------------------------------------------------------
RPoint* RGeometry2D::RPoints::FindRight(RPoint *pt,RPolygons *polys)
{
	RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AX;

	RReturnValIfFail(pt&&polys,0);
	if(!NbPtr) return(0);
	X=pt->X;
	Y=pt->Y;
	Activ=0;
	point=Tab;
	i=NbPtr+1;
	// Find first point on right
	while((!Activ)&&(--i))
	{
		if(((*point)->Y==Y)&&((*point)->X==X+1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Edge(*point,pt))&&((*point)->X>X))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(0);
	// Find left most point on right
	AX=Activ->X;
	for(;--i;point++)
	{
		if(((*point)->Y==Y)&&((*point)->X==X+1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Edge(*point,pt))&&((*point)->X<AX)&&((*point)->X>X))
		{
			Activ=(*point);	
			AX=Activ->X;
    }
	}
	return(Activ);
}


//-----------------------------------------------------------------------------
RPoint* RGeometry2D::RPoints::FindUp(RPoint *pt,RPolygons *polys)
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
	// Find first point on up
	while((!Activ)&&(--i))
	{
		if(((*point)->X==X)&&((*point)->Y==Y+1)) return(*point);
		if(((*point)->X==X)&&(polys->Edge(*point,pt))&&((*point)->Y>Y))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(0);
	// Find bottom most point on up
	AY=Activ->Y;
	for(;--i;point++)
	{
		if(((*point)->X==X)&&((*point)->Y==Y+1)) return(*point);
		if(((*point)->X==X)&&(polys->Edge(*point,pt))&&((*point)->Y<AY)&&((*point)->Y>Y))
		{
			Activ=(*point);	
			AY=Activ->Y;
    }
	}
	return(Activ);
}


//-----------------------------------------------------------------------------
RPoint* RGeometry2D::RPoints::FindBottomLeft(void)
{
	RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;

	if(!NbPtr) return(0);
	point=Tab;
	Activ=(*point);
	X=Activ->X;
	Y=Activ->Y;
	for(i=NbPtr,point++;--i;point++)
		if(((*point)->Y<Y)||(((*point)->Y==Y)&&((*point)->X<X)))
		{
			Activ=(*point);
			X=Activ->X;
			Y=Activ->Y;
		}
	return(Activ);
}


//-----------------------------------------------------------------------------
bool RGeometry2D::RPoints::DuplicatePoints(void)
{
	unsigned int i,j;
	RPoint **point1,**point2;

	for(i=0,point1=Tab;i<NbPtr-1;point1++,i++)
		for(j=i+1,point2=&Tab[i+1];j<NbPtr;point2++,j++)
			if((**point1)==(**point2))
				return(true);
	return(false);
}


//-----------------------------------------------------------------------------
RPoints& RGeometry2D::RPoints::operator=(const RPoints &points)
{
	RContainer<RPoint,unsigned int,true,false>::operator=(points);
	return(*this);
}
