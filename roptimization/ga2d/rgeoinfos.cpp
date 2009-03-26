/*

	R Project Library

	RGeoInfos.cpp

	Set of geometric informations - Implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
	bound-=bound.GetPt1();
	bound.X2/=4;
	bound.Y2/=4;
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
	if(r.X1<bound.X1) bound.X1=r.X1;
	if(r.Y1<bound.Y1) bound.Y1=r.Y1;
	if(r.X2>bound.X2) bound.X2=r.X2;
	if(r.Y2>bound.Y2) bound.Y2=r.Y2;
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
	size_t i;
	RRect tmp;

	rect.Set(0,0,MaxCoord,MaxCoord);
	RCursor<RGeoInfo> Cur(*this);
	for(i=RealNb+1,Cur.Start();--i;Cur.Next())
	{
		Cur()->Boundary(tmp);
		if(tmp.X1<rect.X1) rect.X1=tmp.X1;
		if(tmp.Y1<rect.Y1) rect.Y1=tmp.Y1;
		if(tmp.X2>rect.X2) rect.X2=tmp.X2;
		if(tmp.Y2>rect.Y2) rect.Y2=tmp.Y2;
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
