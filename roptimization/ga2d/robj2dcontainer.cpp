/*

	R Project Library

	RGeoInfo.cpp

	Geometric information - Implementation.

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

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
#include <rga2d/robj2dcontainer.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rgeoinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RObj2DContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DContainer::RObj2DContainer(const unsigned int id,const char* name,const unsigned int nb)
	: RObj2D(id,name,false), RContainer<RGeoInfo,false,false>(nb,nb/2)
{
}


//------------------------------------------------------------------------------
void RObj2DContainer::ClearInfo(void)
{
	unsigned int i;

	Clear();
	Connectors.Clear();
	NbPossOri=0;
	for(i=0;i<8;i++)
	{
		Polygons[i].Clear();
		Rects[i].Clear();
	}
	Translation.Set(MaxCoord,MaxCoord);
}


//------------------------------------------------------------------------------
void RObj2DContainer::Add(RGeoInfos* infos,RGeoInfo* info)
{
	RGeoInfo* ptr;

	ptr=infos->GetPtr<unsigned int>(info->GetObj()->GetId());
	InsertPtr(ptr);
	if(ptr->Pos.X<Translation.X)
		Translation.X=ptr->Pos.X;
	if(ptr->Pos.Y<Translation.Y)
		Translation.Y=ptr->Pos.Y;
}


//------------------------------------------------------------------------------
void RObj2DContainer::Complete(void)
{
	RPolygons Polys;
	RPolygon* p;
	RGeoInfo** info;
	RGeoInfoConnector** tab;
	RObj2DConnector* con;
	unsigned int i,j,k;

	// Go through all geometric information to make the translation
	// Add it to the polygons
	for(i=NbPtr+1,info=Tab;--i;info++)
	{
		p=new RPolygon((*info)->Bound);
		(*p)+=(*info)->Pos-Translation;
		Polys.InsertPtr(p);
		for(j=(*info)->Connectors.NbPtr+1,tab=(*info)->Connectors.Tab;--j;tab++)
		{
			con=new RObj2DConnector(this,(*tab)->Con->Id,(*tab)->Con->Name,(*tab)->Con->NbPos);
			for(k=0;k<con->NbPos;k++)
			{
				con->Pos[k]=con->Pos[k]+(*info)->Pos-Translation;
			}
			Connectors.InsertPtr(con);
		}
	}

	// Calculate the union
	Polys.Union(&Polygon);
	SetOri(Normal);
	Init();
}


//------------------------------------------------------------------------------
void RObj2DContainer::Assign(RGeoInfos* infos,const RPoint& pos,RGrid* grid,const unsigned int order)
{
	RGeoInfo **info,*ptr;
	unsigned int i,j,k;
	RGeoInfoConnector **tab,*con;

	for(i=NbPtr+1,info=Tab;--i;info++)
	{
		ptr=infos->GetPtr<unsigned int>((*info)->GetObj()->GetId());
		ptr->Bound=(*info)->Bound;
		ptr->Selected=true;
		ptr->Obj=(*info)->Obj;
		ptr->Rects=(*info)->Rects;
		ptr->Rect=(*info)->Rect;
		ptr->Order=order;
		ptr->Connectors.Clear();
		for(j=(*info)->Connectors.NbPtr+1,tab=(*info)->Connectors.Tab;--j;tab++)
		{
			con=new RGeoInfoConnector((*tab),ptr);
			for(k=0;k<con->NbPos;k++)
			{
				con->Pos[k]=con->Pos[k]+(*info)->Pos-Translation;
			}
			ptr->Connectors.InsertPtr(con);
		}
		ptr->Assign((*info)->Pos-Translation+pos,grid);
	}
}


//------------------------------------------------------------------------------
bool RObj2DContainer::IsIn(unsigned int id)
{
	RGeoInfo **info;
	unsigned int i;

	for(i=NbPtr+1,info=Tab;--i;info++)
	{
		if((*info)->GetObj()->GetId()==id)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
RObj2DContainer::~RObj2DContainer(void)
{
}
