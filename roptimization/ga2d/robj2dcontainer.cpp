/*

	R Project Library

	RGeoInfo.cpp

	Geometric information - Implementation.

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
#include <rlayout.h>
#include <robj2dconfig.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RObj2DContainer
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DContainer::RObj2DContainer(const size_t id,RLayout* layout)
	: RGeoInfo(), RContainer<RGeoInfo,false,false>(layout->GetProblem()->GetNbObjs()),
	  Origin(RPoint::Null), Layout(layout)
{
	Obj=new RObj2D(id,"Container "+RString::Number(id));
}


//------------------------------------------------------------------------------
void RObj2DContainer::ClearInfo(void)
{
	RGeoInfo::ClearInfo();
	RContainer<RGeoInfo,false,false>::Clear();
	Origin=RPoint::Null;
}


//------------------------------------------------------------------------------
void RObj2DContainer::Add(RGeoInfo* info)
{
	// Get element in the current layout corresponding to the one added.
	RGeoInfo* ptr((*Layout)[info->GetObj()->GetId()]);

	// Copy the geometric information
	RObj2DConfig* Config(info->GetConfig());
	if(!Config)
		mThrowRException("Geometric information of object '"+RString::Number(info->GetObj()->GetId())+"' has no configuration selected");
	ptr->SetConfig(info->GetConfig()->GetOrientation());
	ptr->Assign(RPoint::Null,0,cNoRef);

	// Compute the left-bottom point of the aggregator
	RPoint Pos(info->GetPos());
	if(Pos.X<Origin.X)
		Origin.X=Pos.X;
	if(Pos.Y<Origin.Y)
		Origin.Y=Pos.Y;

	// Add it to the aggregator
	ptr->SetContainer(this);
	RContainer<RGeoInfo,false,false>::InsertPtr(info);
}


//------------------------------------------------------------------------------
void RObj2DContainer::Complete(void)
{
	RContainer<RRect,false,false> rects(RContainer<RGeoInfo,false,false>::GetNb()*2);

	// Go through all elements to make the translation, and recreate also its connector
	RCursor<RGeoInfo> info(*this);
	for(info.Start();!info.End();info.Next())
	{
		// Add the rectangles of each element
		RCursor<RRect> Rect(info()->GetConfig()->GetRects());
		for(Rect.Start();!Rect.End();Rect.Next())
		{
			RRect* rect=new RRect(*Rect());
			(*rect)+=info()->GetPos()-Origin;
			rects.InsertPtr(rect);
		}

		// Re-create the connectors
		RCursor<RObj2DConfigConnector> Cur(info()->GetConfig()->GetConnectors());
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			RObj2DConfigConnector* Con(new RObj2DConfigConnector(Cur()->GetConnector()));
			Obj->GetDefaultConfig()->Add(Con);
			RCursor<RObj2DConfigPin> Cur2(*Cur());
			for(Cur2.Start();!Cur2.End();Cur2.Next())
				Con->InsertPtr(new RObj2DConfigPin(Cur2()->GetPin(),Cur2()->GetRect()));
		}
	}

	// Calculate the union
	Obj->GetDefaultConfig()->Set(rects);
	Obj->CreateOri(oNormal);
}


//------------------------------------------------------------------------------
void RObj2DContainer::Assign(const RPoint& pos,RGrid* grid,size_t order)
{
	RCursor<RGeoInfo> info(*this);
	for(info.Start();!info.End();info.Next())
	{
		// Get element in the current layout corresponding to the one added.
		RGeoInfo* ptr((*Layout)[info()->GetObj()->GetId()]);

		if(pos==RPoint::Null)
			ptr->Assign(RPoint::Null,grid,order);
		else
			ptr->Assign(info()->GetPos()+pos-Origin,grid,order);
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
	RCursor<RGeoInfo> Info(*this);
	for(Info.Start();!Info.End();Info.Next())
		Info()->SetContainer(0);
	delete Obj;
}
