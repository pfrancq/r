/*

  RGeoInfo.cpp

  Geometric information - Implementation.

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
#include "rgeoinfo.h"
using namespace RGA;


//---------------------------------------------------------------------------
//
// Class "RGeoInfo"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RGeoInfo::RGeoInfo(void)
	: Id(NoObject),Pos(MaxCoord,MaxCoord),Bound(), Rects()
{
}

/*
//---------------------------------------------------------------------------
RGeoInfo::RGeoInfo(RPolygon *poly)
	: Pos(),Bound(poly->NbPtr), Rects()
{
}  */


//---------------------------------------------------------------------------
void RGeoInfo::AdaptPos(void)
{
	RPoint **point;
	unsigned int i;

	for(i=Bound.NbPtr+1,point=Bound.Tab;--i;point++)
	{
		(*point)->X+=Pos.X;
		(*point)->Y+=Pos.Y;
	}	
}


//---------------------------------------------------------------------------
double RGeoInfo::Area(void)
{
  RRect **rect;	
	double area;
	unsigned int i;

	for(i=Rects.NbPtr+1,rect=Rects.Tab,area=0.0;--i;rect++) area+=(*rect)->Area();
	return(area);
}


//---------------------------------------------------------------------------
void RGeoInfo::Boundary(RRect &rect)
{
  Bound.Boundary(rect);
  rect.Pt1.X+=Pos.X;
  rect.Pt1.Y+=Pos.Y;
  rect.Pt2.X+=Pos.X;
  rect.Pt2.Y+=Pos.Y;
}


//---------------------------------------------------------------------------
void RGeoInfo::AssignBound(unsigned int id,const RPolygon& poly)
{
	Id=id;
	Bound=poly;
	Bound.RectDecomposition(&Rects);
}


//---------------------------------------------------------------------------
// Verify if each point in the bound rectangle is in the polygon
void RGeoInfo::Assign(unsigned int **OccX,unsigned int **OccY,RCoord x,RCoord y)
{
  RRect **rect;
	RCoord BeginX,BeginY,j,k;
	unsigned int i,**ptr1,*ptr2;

  Pos.X=x;
  Pos.Y=y;
	for(i=Rects.NbPtr+1,rect=Rects.Tab;--i;rect++)
	{
		BeginX=(*rect)->Pt1.X+Pos.X;
		BeginY=(*rect)->Pt1.Y+Pos.Y;
		// OccX
		for(j=(*rect)->Width()+1,ptr1=&OccX[BeginX];--j;ptr1++)
			for(k=(*rect)->Height()+1,ptr2=&((*ptr1)[BeginY]);--k;ptr2++)
				(*ptr2)=Id;
		// OccY
		for(j=(*rect)->Height()+1,ptr1=&OccY[BeginY];--j;ptr1++)
			for(k=(*rect)->Width()+1,ptr2=&((*ptr1)[BeginX]);--k;ptr2++)
				(*ptr2)=Id;
	}
}


//---------------------------------------------------------------------------
bool RGeoInfo::Overlap(RGeoInfo *info)
{
	RRect **rect1,**rect2;
	unsigned int i,j;
	RRect R1,R2;

	for(i=Rects.NbPtr+1,rect1=Rects.Tab;--i;rect1++)
	{
		R1=(**rect1);
		R1.Translation(Pos.X,Pos.Y);
		for(j=info->Rects.NbPtr+1,rect2=info->Rects.Tab;--j;rect2++)
		{
			R2=(**rect2);
			R2.Translation(info->Pos.X,info->Pos.Y);	
			if(R1.Overlap(&R2))
				return(true);
    }
	}
	return(false);
}


//---------------------------------------------------------------------------
RGeoInfo& RGeoInfo::operator=(const RGeoInfo &info)
{
	Pos=info.Pos;
  Bound=info.Bound;
	return(*this);
}



//---------------------------------------------------------------------------
//
// RGeoInfos
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RGeoInfos::RGeoInfos(unsigned int nb)
	: RContainer<RGeoInfo,unsigned int,false,false>(nb,1)
{
}


//---------------------------------------------------------------------------
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