/*

	R Project Library

	RObj2D.hh

	Object for 2D placement GA - Implementation

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rgeoinfos.h>
#include <rga2d/rconnection.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObj2DConnector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const RPoint pos)
	: Owner(owner), Id(id), Name(itoa(id)), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Pos[0]=pos;
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const RString& name,const RPoint pos)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0]=pos;
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const char* name,const RPoint pos)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0]=pos;
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const unsigned int x,unsigned y)
	: Owner(owner), Id(id), Name(itoa(id)), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const RString& name,const unsigned int x,unsigned y)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const unsigned int x,unsigned y)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const unsigned int nb)
	: Owner(owner), Id(id), Name(name), NbPos(nb), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	for(unsigned int i=0;i<NbPos;i++)
		Poss[i]=new RPoint[8];
}


//------------------------------------------------------------------------------
RPoint& RObj2DConnector::GetPos(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Pos[0];
	return(*pt);
}


//------------------------------------------------------------------------------
RPoint& RObj2DConnector::GetPos(unsigned int i,char o)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Poss[i][o];
	return(*pt);
}


//------------------------------------------------------------------------------
void RObj2DConnector::AddConnection(RConnection* con)
{
	Connections.InsertPtr(con);
}


//------------------------------------------------------------------------------
double RObj2DConnector::GetMinDist(RObj2DConnector* c,RGeoInfos* infos,RPoint& pt1,RPoint& pt2)
{
	double min=HUGE_VAL,d;
	RGeoInfo *g1,*g2;
	RGeoInfoConnector *c1,*c2;
	unsigned int i,j;
	RPoint p1,p2;

	g1=infos->GetPtr<unsigned int>(Owner->GetId());
	c1=g1->Connectors.GetPtr<unsigned int>(Id);
	g2=infos->GetPtr<unsigned int>(c->Owner->GetId());
	c2=g2->Connectors.GetPtr<unsigned int>(c->Id);
	for(i=0;i<c1->NbPos;i++)
	{
		p1=c1->Pos[i]+g1->GetPos();
		for(j=0;j<c2->NbPos;j++)
		{
			p2=c2->Pos[j]+g2->GetPos();
			d=p1.ManhattanDist(p2);
			if(d<min)
			{
				min=d;
				pt1=p1;
				pt2=p2;
			}
		}
	}
	return(min);
}


//------------------------------------------------------------------------------
RObj2DConnector::~RObj2DConnector(void)
{
	delete[] Pos;
	for(unsigned int i=0;i<NbPos;i++)
		delete Poss[i];
	delete[] Poss;
}



//------------------------------------------------------------------------------
//
// RObj2D
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2D::RObj2D(unsigned int id,bool deformable)
	: Id(id), Name(itoa(id)), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
RObj2D::RObj2D(unsigned int id,const RString& name,bool deformable)
	: Id(id), Name(name), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
RObj2D::RObj2D(unsigned int id,const char* name,bool deformable)
	: Id(id), Name(name), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
void RObj2D::Init(void)
{
	char i,j;
	RPolygon *p;
	RRects *r;

	CalcPolygons();
	for(i=NbPossOri+1,p=Polygons,r=Rects,j=0;--i;p++,r++,j++)
		p->RectDecomposition(r);
	Area=Rects[0].Area();
}


//------------------------------------------------------------------------------
void RObj2D::CalcPolygons(void)
{
	char i,idx;
	ROrientation *o;
	RPolygon *ptr;
	bool bNormal,bRota90;
	RPoint Min;

	if(!NbPossOri) return;

	// If Polygon is a rectangle
	if(Polygon.NbPtr==4)
	{
		// Test if a square -> 1 orientation only
		if(Polygon.Tab[2]->X==Polygon.Tab[2]->Y)
		{
			NbPossOri=1;
			Polygons[0]=Polygon;
			for(Connectors.Start();!Connectors.End();Connectors.Next())
			{
				for(unsigned int j=0;j<Connectors()->NbPos;j++)
				{
					Connectors()->Poss[j][0]=Connectors()->Pos[j]-Min;
				}
			}
			return;
		}

		// It is Rectangle -> Max 2 orientation only
		bNormal=bRota90=false;
		for(i=NbPossOri+1,o=PossOri;--i;o++)
		{
			if((!bNormal)&&(((*o)==Normal)||((*o)==NormalX)||((*o)==NormalY)||((*o)==NormalYX)))
			{
				bNormal=true;
				if(bRota90) idx=1; else idx=0;
				Polygons[idx]=Polygon;
				for(Connectors.Start();!Connectors.End();Connectors.Next())
				{
					for(unsigned int j=0;j<Connectors()->NbPos;j++)
					{
						Connectors()->Poss[j][idx]=Connectors()->Pos[j];
						Connectors()->Poss[j][idx]-=Min;
					}
				}
			}
			if((!bRota90)&&(((*o)==Rota90)||((*o)==Rota90X)||((*o)==Rota90Y)||((*o)==Rota90YX)))
			{
				bRota90=true;
				if(bNormal) idx=1; else idx=0;
				Polygons[idx]=Polygon;
				Polygons[idx].ChangeOrientation(Rota90,Min);
				for(Connectors.Start();!Connectors.End();Connectors.Next())
				{
					for(unsigned int j=0;j<Connectors()->NbPos;j++)
					{
						Connectors()->Poss[j][idx]=Connectors()->Pos[j];
						Connectors()->Poss[j][idx].ChangeOrientation(Rota90);
						Connectors()->Poss[j][idx]-=Min;
					}
				}
			}
		}
		if(bRota90||bNormal) NbPossOri=1;
		if(bRota90&&bNormal) NbPossOri=2;
		return;
	}

	// Other Polygon
	for(i=0,ptr=Polygons,o=PossOri;i<NbPossOri;ptr++,o++,i++)
	{
		(*ptr)=Polygon;
		ptr->ChangeOrientation(*o,Min);
		for(Connectors.Start();!Connectors.End();Connectors.Next())
		{
			for(unsigned int j=0;j<Connectors()->NbPos;j++)
			{
				Connectors()->Poss[j][i]=Connectors()->Pos[j];
				Connectors()->Poss[j][i].ChangeOrientation(*o);
				Connectors()->Poss[j][i]-=Min;
			}
		}
	}
}


//------------------------------------------------------------------------------
void RObj2D::SetOri(ROrientation o)
{
	char i;
	ROrientation *ptr;

	for(i=NbPossOri+1,ptr=PossOri;--i;ptr++)
		if((*ptr)==o) return;
	PossOri[NbPossOri++]=o;
}


//------------------------------------------------------------------------------
bool RObj2D::IsOriSet(ROrientation o)
{
	char i;
	ROrientation *ptr;

	for(i=NbPossOri+1,ptr=PossOri;--i;ptr++)
		if((*ptr)==o) return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPolygon* RObj2D::GetPolygon(char i)
{
	if(i<NbPossOri) return(&Polygons[i]);
	return(0);
}


//------------------------------------------------------------------------------
RRects* RObj2D::GetRects(char i)
{
	if(i<NbPossOri) return(&Rects[i]);
	return(0);
}


//------------------------------------------------------------------------------
RObj2D& RObj2D::operator=(const RObj2D &obj)
{
	char i;
	RPolygon *ptr;
	const RPolygon *ptr2;

	Id=obj.Id;
	NbPossOri=obj.NbPossOri;
	Deformable=obj.Deformable;
	memcpy(PossOri,obj.PossOri,sizeof(ROrientation)*8);
	Polygon=obj.Polygon;
	for(i=NbPossOri+1,ptr=Polygons,ptr2=obj.Polygons;--i;ptr++,ptr2++)
		(*ptr)=(*ptr2);
	return(*this);
}


//------------------------------------------------------------------------------
void RObj2D::AddConnector(unsigned int id,unsigned x,unsigned y)
{
	Connectors.InsertPtr(new RObj2DConnector(this,id,x,y));
}


//------------------------------------------------------------------------------
void RObj2D::CopyConnectors(RObj2D* o)
{
	RObj2DConnector *s,*d;

	Connectors.Clear(o->Connectors.MaxPtr,o->Connectors.IncPtr);
	for(o->Connectors.Start();!o->Connectors.End();o->Connectors.Next())
	{
		s=o->Connectors();
		d=new RObj2DConnector(this,Connectors.NbPtr,s->Name,s->NbPos);
		for(unsigned int i=0;i<s->NbPos;i++)
			d->Pos[i]=s->Pos[i];
		Connectors.InsertPtr(d);
	}
}


//------------------------------------------------------------------------------
RObj2D::~RObj2D(void)
{
}
