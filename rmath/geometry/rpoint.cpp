/*

  RPoint.cpp

  Point - Implementation.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
// include files for Rainbow
#include "rfunc.h"
using namespace RStd;
#include "rpoint.h"
#include "polygons.h"
using namespace RGeometry;



//---------------------------------------------------------------------------
//
// Class "RPoint"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPoint::RPoint(void)
{
  X=Y=0;
}


//---------------------------------------------------------------------------
RPoint::RPoint(RCoord x,RCoord y)
{
  X=x;
  Y=y;
}


//---------------------------------------------------------------------------
RPoint::RPoint(const RPoint& pt)
{
  X=pt.X;
  Y=pt.Y;
}


//---------------------------------------------------------------------------
RPoint::RPoint(RPoint *pt)
{
  X=pt->X;
  Y=pt->Y;
}


//---------------------------------------------------------------------------
RPoint* RPoint::GetPoint(void)
{
	return(GetTemporaryObject<RPoint,30>());
}



//---------------------------------------------------------------------------
//
// RPoints
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPoints::RPoints(void)
	: RContainer<RPoint,unsigned int,true,false>(10,5)
{
}


//---------------------------------------------------------------------------
RPoints::RPoints(unsigned int max)
	: RContainer<RPoint,unsigned int,true,false>(max,5)
{
}


//---------------------------------------------------------------------------
RPoints::RPoints(RPoints *points)
	: RContainer<RPoint,unsigned int,true,false>(points->MaxPtr,points->IncPtr)
{
	RPoint **pts;
	unsigned int i;

	for(i=points->NbPtr+1,pts=points->Tab;--i;pts++)
		InsertPtr(new RPoint(*pts));
}


//---------------------------------------------------------------------------
RPoint* RPoints::FindLeft(RPoint *pt,RPolygons *polys)
{
  RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AX;

	if(!NbPtr) return(NULL);
	X=pt->X;
	Y=pt->Y;
	Activ=NULL;
	point=Tab;
	i=NbPtr+1;
	// Find first point on left
	while((!Activ)&&(--i))
	{
		if(((*point)->Y==Y)&&((*point)->X==X-1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Vertex(*point,pt))&&((*point)->X<X))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(NULL);
	// Find right most point on left
	AX=Activ->X;
	for(;--i;point++)
	{
		if(((*point)->Y==Y)&&((*point)->X==X-1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Vertex(*point,pt))&&((*point)->X>AX)&&((*point)->X<X))
		{
			Activ=(*point);	
			AX=Activ->X;
    }
	}
  return(Activ);
}


//---------------------------------------------------------------------------
RPoint* RPoints::FindRight(RPoint *pt,RPolygons *polys)
{
  RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AX;

	if(!NbPtr) return(NULL);
	X=pt->X;
	Y=pt->Y;
	Activ=NULL;
	point=Tab;
	i=NbPtr+1;
	// Find first point on right
	while((!Activ)&&(--i))
	{
		if(((*point)->Y==Y)&&((*point)->X==X+1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Vertex(*point,pt))&&((*point)->X>X))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(NULL);
	// Find left most point on right
	AX=Activ->X;
	for(;--i;point++)
	{
		if(((*point)->Y==Y)&&((*point)->X==X+1)) return(*point);
		if(((*point)->Y==Y)&&(polys->Vertex(*point,pt))&&((*point)->X<AX)&&((*point)->X>X))
		{
			Activ=(*point);	
			AX=Activ->X;
    }
	}
  return(Activ);
}


//---------------------------------------------------------------------------
RPoint* RPoints::FindBottom(RPoint *pt,RPolygons *polys)
{
  RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AY;

	if(!NbPtr) return(NULL);
	X=pt->X;
	Y=pt->Y;
	Activ=NULL;
	point=Tab;
	i=NbPtr+1;
	// Find first point on bottom
	while((!Activ)&&(--i))
	{
		if(((*point)->X==X)&&((*point)->Y==Y-1)) return(*point);
		if(((*point)->X==X)&&(polys->Vertex(*point,pt))&&((*point)->Y<Y))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(NULL);
	// Find up most point on bottom
	AY=Activ->Y;
	for(;--i;point++)
	{
		if(((*point)->X==X)&&((*point)->Y==Y-1)) return(*point);
		if(((*point)->X==X)&&(polys->Vertex(*point,pt))&&((*point)->Y>AY)&&((*point)->Y<Y))
		{
			Activ=(*point);	
			AY=Activ->Y;
    }
	}
  return(Activ);
}


//---------------------------------------------------------------------------
RPoint* RPoints::FindUp(RPoint *pt,RPolygons *polys)
{
  RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;
	RCoord AY;

	if(!NbPtr) return(NULL);
	X=pt->X;
	Y=pt->Y;
	Activ=NULL;
	point=Tab;
	i=NbPtr+1;
	// Find first point on up
	while((!Activ)&&(--i))
	{
		if(((*point)->X==X)&&((*point)->Y==Y+1)) return(*point);
		if(((*point)->X==X)&&(polys->Vertex(*point,pt))&&((*point)->Y>Y))
			Activ=(*point);
		point++;
	}
	if(!Activ) return(NULL);
	// Find bottom most point on up
	AY=Activ->Y;
	for(;--i;point++)
	{
		if(((*point)->X==X)&&((*point)->Y==Y+1)) return(*point);
		if(((*point)->X==X)&&(polys->Vertex(*point,pt))&&((*point)->Y<AY)&&((*point)->Y>Y))
		{
			Activ=(*point);	
			AY=Activ->Y;
    }
	}
  return(Activ);
}


//---------------------------------------------------------------------------
RPoint* RPoints::FindBottomLeft(void)
{
  RPoint *Activ,**point;
	unsigned int i;
	RCoord X,Y;

	if(!NbPtr) return(NULL);
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


//---------------------------------------------------------------------------
bool RPoints::DuplicatePoints(void)
{
	unsigned int i,j;
	RPoint **point1,**point2;

	for(i=0,point1=Tab;i<NbPtr-1;point1++,i++)
		for(j=i+1,point2=&Tab[i+1];j<NbPtr;point2++,j++)
			if((**point1)==(**point2))
				return(true);
	return(false);
}


//---------------------------------------------------------------------------
RPoints& RPoints::operator=(const RPoints &points)
{
	RContainer<RPoint,unsigned int,true,false>::operator=(points);
	return(*this);
}