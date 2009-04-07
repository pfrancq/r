/*

	R Project Library

	RGeoInfo.cpp

	Geometric information - Implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
#include <robj2dcontainer.h>
#include <rgeoinfo.h>
#include <rgeoinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RObj2DContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DContainer::RObj2DContainer(const size_t id,const char* name,const size_t nb)
	: RObj2D(id,name,false), RContainer<RGeoInfo,false,false>(nb,nb/2)
{
}


//------------------------------------------------------------------------------
void RObj2DContainer::ClearInfo(void)
{
	size_t i;

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

	ptr=infos->GetPtr<size_t>(info->GetObj()->GetId());
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
	RObj2DConnector* con;
	size_t k;

	// Go through all geometric information to make the translation
	// Add it to the polygons
	RCursor<RGeoInfo> info(*this);
	for(info.Start();!info.End();info.Next())
	{
		p=new RPolygon(*info()->Bound);
		(*p)+=info()->Pos-Translation;
		Polys.InsertPtr(p);
		RCursor<RGeoInfoConnector> tab(info()->Connectors);
		for(tab.Start();!tab.End();tab.Next())
		{
			con=new RObj2DConnector(this,tab()->Con->Id,tab()->Con->Name,tab()->Con->NbPos);
			for(k=0;k<con->NbPos;k++)
			{
				con->Pos[k]=con->Pos[k]+info()->Pos-Translation;
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
void RObj2DContainer::Assign(RGeoInfos* infos,const RPoint& pos,RGrid* grid,const size_t order)
{
	RGeoInfo *ptr;
	size_t k;
	RGeoInfoConnector *con;

	RCursor<RGeoInfo> info(*this);
	for(info.Start();!info.End();info.Next())
	{
		ptr=infos->GetPtr<size_t>(info()->GetObj()->GetId());
		ptr->Bound=info()->Bound;
		ptr->Selected=true;
		ptr->Obj=info()->Obj;
		ptr->Rects=info()->Rects;
		ptr->Rect=info()->Rect;
		ptr->Order=order;
		ptr->Connectors.Clear();
		RCursor<RGeoInfoConnector> tab(info()->Connectors);
		for(tab.Start();!tab.End();tab.Next())
		{
			con=new RGeoInfoConnector(tab(),ptr);
			for(k=0;k<con->NbPos;k++)
			{
				con->Pos[k]=con->Pos[k]+info()->Pos-Translation;
			}
			ptr->Connectors.InsertPtr(con);
		}
		ptr->Assign(info()->Pos-Translation+pos,grid);
	}
}


//------------------------------------------------------------------------------
bool RObj2DContainer::IsIn(size_t id) const
{
	RCursor<RGeoInfo> info(*this);
	for(info.Start();!info.End();info.Next())
	{
		if(info()->GetObj()->GetId()==id)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
RObj2DContainer::~RObj2DContainer(void)
{
}
