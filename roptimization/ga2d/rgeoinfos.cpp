/*

	R Project Library

	RGeoInfos.cpp

	Set of geometric informations - Implementation.

	(c) 2001 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rgeoinfos.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
//
// RGeoInfos
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RGeoInfos::RGeoInfos(RProblem2D* prob,bool create) throw(bad_alloc)
	: RContainer<RGeoInfo,unsigned int,true,false>(prob->Objs.NbPtr,prob->Objs.NbPtr/2), Problem(prob)
{
	if(create)
	{
		RObj2D** obj;	
		unsigned int i;
	
		// Create geometric information and Objs orientations
		for(i=prob->Objs.NbPtr+1,obj=Problem->Objs.Tab;--i;obj++)
			InsertPtr(new RGeoInfo(*obj));
	}
}


//-----------------------------------------------------------------------------
void RGA2D::RGeoInfos::GetSetInfos(RObj2DContainer* cont,RGrid* /*grid*/,bool* selected)
{
	RGeoInfo* best;
	RPolygon Envelop(4);
	RRect bound;
	
	cont->Clear();
	Boundary(bound);	
	bound.Pt2.X/=4;
	bound.Pt2.Y/=4;
	best=Problem->Cons.GetBestConnected(Tab,NbPtr,selected,bound);
	best->SetSelect();
	selected[best->GetObj()->GetId()]=true;
	Envelop.InsertPtr(new RPoint(bound.Pt1));
	Envelop.InsertPtr(new RPoint(bound.Pt2.X,bound.Pt1.Y));
	Envelop.InsertPtr(new RPoint(bound.Pt2));	
	Envelop.InsertPtr(new RPoint(bound.Pt1.X,bound.Pt2.Y));	
	for(Start();!End();Next())
	{
		best=(*this)();
		if((!selected[best->GetObj()->GetId()])&&Envelop.IsIn(best->GetPolygon()))
		{
			// Add it
			cont->AddObj(best->GetObj(),best);
			best->SetSelect();
			selected[best->GetObj()->GetId()]=true;
		}
	}
	cont->EndObjs();
}


//-----------------------------------------------------------------------------
void RGA2D::RGeoInfos::Boundary(RRect &rect)
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


//-----------------------------------------------------------------------------
void RGA2D::RGeoInfos::Clear(void)
{
	for(Start();!End();Next())
		(*this)()->Clear();
}
