/*

	R Project Library

	RGeoInfos.cpp

	Set of geometric informations - Implementation.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rgeoinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RGeoInfos
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfos::RGeoInfos(RProblem2D* prob,bool create)
	: RContainer<RGeoInfo,true,false>(prob->Objs.GetNb()+3),
	  Problem(prob), Cons(&prob->Cons,this), RealNb(prob->Objs.GetNb()), Selected(0)
{
	if(create)
	{
		RGeoInfo* l;

		// Create geometric information
		RCursor<RObj2D> obj(prob->Objs);
		for(obj.Start();!obj.End();obj.Next())
			InsertPtr(new RGeoInfo(obj()));
		InsertPtr(l=new RGeoInfo(&prob->Problem));
		l->SetOri(0);
		l->Pos.Set(-prob->Translation.X,-prob->Translation.Y);
	}
	Selected=new bool[GetNb()];
}


//------------------------------------------------------------------------------
void RGeoInfos::GetSetInfos(RObj2DContainer* cont,RGrid* /*grid*/,bool* selected)
{
//	RGeoInfo* best;
	RRect bound,r;
	RGeoInfo* i1;
	RGeoInfo* i2;

	cont->ClearInfo();
	Boundary(bound);
	bound-=bound.Pt1;
	bound.Pt2.X/=4;
	bound.Pt2.Y/=4;
	Cons.GetBestsConnected(i1,i2,bound,selected);
	if(!i1)
	{
		cont->Complete();
		return;
	}
	selected[i1->GetObj()->GetId()]=true;
	cont->Add(this,i1);
	if(!i2)
	{
		cont->Complete();
		return;
	}
	selected[i2->GetObj()->GetId()]=true;
	cont->Add(this,i2);
	i1->Boundary(bound);
	i2->Boundary(r);
	if(r.Pt1.X<bound.Pt1.X) bound.Pt1.X=r.Pt1.X;
	if(r.Pt1.Y<bound.Pt1.Y) bound.Pt1.Y=r.Pt1.Y;
	if(r.Pt2.X>bound.Pt2.X) bound.Pt2.X=r.Pt2.X;
	if(r.Pt2.Y>bound.Pt2.Y) bound.Pt2.Y=r.Pt2.Y;
	RCursor<RGeoInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if((!selected[Cur()->GetObj()->GetId()])&&Cur()->GetPolygon().Contained(&bound))
		{
			// Add it
			cont->Add(this,Cur());
			selected[Cur()->GetObj()->GetId()]=true;
		}
	}
	cont->Complete();
}


//------------------------------------------------------------------------------
void RGeoInfos::Boundary(RRect& rect)
{
	unsigned int i;
	RRect tmp;

	rect.Pt1.X=rect.Pt1.Y=MaxCoord;
	rect.Pt2.X=rect.Pt2.Y=0;
	RCursor<RGeoInfo> Cur(*this);
	for(i=RealNb+1,Cur.Start();--i;Cur.Next())
	{
		Cur()->Boundary(tmp);
		if(tmp.Pt1.X<rect.Pt1.X) rect.Pt1.X=tmp.Pt1.X;
		if(tmp.Pt1.Y<rect.Pt1.Y) rect.Pt1.Y=tmp.Pt1.Y;
		if(tmp.Pt2.X>rect.Pt2.X) rect.Pt2.X=tmp.Pt2.X;
		if(tmp.Pt2.Y>rect.Pt2.Y) rect.Pt2.Y=tmp.Pt2.Y;
	}
}


//------------------------------------------------------------------------------
void RGeoInfos::ClearInfos(void)
{
	RCursor<RGeoInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ClearInfo();
	Cons.UnComplete();
}


//------------------------------------------------------------------------------
RGeoInfos& RGeoInfos::operator=(const RGeoInfos& infos)
{
	RContainer<RGeoInfo,true,false>::operator=(infos);
	Problem=infos.Problem;
	Cons=infos.Cons;
	Cons.Infos=this;
	RealNb=infos.RealNb;
	memcpy(Selected,infos.Selected,GetNb()*sizeof(bool));
	return(*this);
}


//------------------------------------------------------------------------------
RGeoInfos::~RGeoInfos(void)
{
	delete[] Selected;
}
