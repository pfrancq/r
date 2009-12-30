/*

	R Project Library

	RObj2D.hh

	Object for 2D placement GA - Implementation

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
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <robj2d.h>
#include <rstring.h>
#include <rgeoinfo.h>
#include <rgeoinfos.h>
#include <rconnection.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RObj2DConnector
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,size_t id,const RPoint pos)
	: Owner(owner), Id(id), Name(RString::Number(id)), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Pos[0]=pos;
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,size_t id,const RString& name,const RPoint pos)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0]=pos;
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,size_t id,const char* name,const RPoint pos)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0]=pos;
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,size_t id,tCoord x,tCoord y)
	: Owner(owner), Id(id), Name(RString::Number(id)), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner,size_t id,const RString& name,tCoord x,tCoord y)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner, size_t id,const char* name,tCoord x,tCoord y)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//------------------------------------------------------------------------------
RObj2DConnector::RObj2DConnector(RObj2D* owner, size_t id,const char* name,const size_t nb)
	: Owner(owner), Id(id), Name(name), NbPos(nb), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	for(size_t i=0;i<NbPos;i++)
		Poss[i]=new RPoint[8];
}


//------------------------------------------------------------------------------
RPoint RObj2DConnector::GetPos(void)
{
	return(Pos[0]);
}


//------------------------------------------------------------------------------
RPoint RObj2DConnector::GetPos(size_t i,char o)
{
	return(Poss[i][static_cast<size_t>(o)]);
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
	size_t i,j;
	RPoint p1,p2;

	g1=infos->GetPtr<size_t>(Owner->GetId());
	c1=g1->Connectors.GetPtr<size_t>(Id);
	g2=infos->GetPtr<size_t>(c->Owner->GetId());
	c2=g2->Connectors.GetPtr<size_t>(c->Id);
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
	for(size_t i=0;i<NbPos;i++)
		delete[] Poss[i];
	delete[] Poss;
}



//------------------------------------------------------------------------------
//
// RObj2D
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RObj2D::RObj2D(size_t id,bool deformable)
	: Id(id), Name(RString::Number(id)), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
RObj2D::RObj2D(size_t id,const RString& name,bool deformable)
	: Id(id), Name(name), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
RObj2D::RObj2D(size_t id,const char* name,bool deformable)
	: Id(id), Name(name), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//------------------------------------------------------------------------------
void RObj2D::Init(void)
{
	int i,j;
	RPolygon *p;
	RRects *r;

	CalcPolygons();
	for(i=NbPossOri+1,p=Polygons,r=Rects,j=0;--i;p++,r++,j++)
		p->RectDecomposition(*r);
	Area=Rects[0].Area();
}


//------------------------------------------------------------------------------
void RObj2D::CalcPolygons(void)
{
	int i,idx;
	ROrientation *o;
	RPolygon *ptr;
	bool bNormal,bRota90;
	RPoint Min;

	if(!NbPossOri) return;

	// If Polygon is a rectangle
	if(Polygon.GetNb()==4)
	{
		// Test if a square -> 1 orientation only
		if(Polygon[2]->X==Polygon[2]->Y)
		{
			NbPossOri=1;
			Polygons[0]=Polygon;
			RCursor<RObj2DConnector> Cur(Connectors);
			for(Cur.Start();!Cur.End();Cur.Next())
			{
				for(size_t j=0;j<Cur()->NbPos;j++)
				{
					Cur()->Poss[j][0]=Cur()->Pos[j]-Min;
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
				RCursor<RObj2DConnector> Cur(Connectors);
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					for(size_t j=0;j<Cur()->NbPos;j++)
					{
						Cur()->Poss[j][idx]=Cur()->Pos[j];
						Cur()->Poss[j][idx]-=Min;
					}
				}
			}
			if((!bRota90)&&(((*o)==Rota90)||((*o)==Rota90X)||((*o)==Rota90Y)||((*o)==Rota90YX)))
			{
				bRota90=true;
				if(bNormal) idx=1; else idx=0;
				Polygons[idx]=Polygon;
				Polygons[idx].ChangeOrientation(Rota90,Min);
				RCursor<RObj2DConnector> Cur(Connectors);
				for(Cur.Start();!Cur.End();Cur.Next())
				{
					for(size_t j=0;j<Cur()->NbPos;j++)
					{
						Cur()->Poss[j][idx]=Cur()->Pos[j];
						Cur()->Poss[j][idx].ChangeOrientation(Rota90);
						Cur()->Poss[j][idx]-=Min;
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
		RCursor<RObj2DConnector> Cur(Connectors);
		for(Cur.Start();!Cur.End();Cur.Next())
		{
			for(size_t j=0;j<Cur()->NbPos;j++)
			{
				Cur()->Poss[j][i]=Cur()->Pos[j];
				Cur()->Poss[j][i].ChangeOrientation(*o);
				Cur()->Poss[j][i]-=Min;
			}
		}
	}
}


//------------------------------------------------------------------------------
void RObj2D::SetOri(ROrientation o)
{
	int i;
	ROrientation *ptr;

	for(i=NbPossOri+1,ptr=PossOri;--i;ptr++)
		if((*ptr)==o) return;
	PossOri[NbPossOri++]=o;
}


//------------------------------------------------------------------------------
bool RObj2D::IsOriSet(ROrientation o)
{
	int i;
	ROrientation *ptr;

	for(i=NbPossOri+1,ptr=PossOri;--i;ptr++)
		if((*ptr)==o) return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPolygon* RObj2D::GetPolygon(int i)
{
	if(i<NbPossOri) return(&Polygons[i]);
	return(0);
}


//------------------------------------------------------------------------------
RRects* RObj2D::GetRects(int i)
{
	if(i<NbPossOri) return(&Rects[i]);
	return(0);
}


//------------------------------------------------------------------------------
RObj2D& RObj2D::operator=(const RObj2D &obj)
{
	int i;
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
void RObj2D::AddConnector(size_t id,unsigned x,unsigned y)
{
	Connectors.InsertPtr(new RObj2DConnector(this,id,x,y));
}


//------------------------------------------------------------------------------
void RObj2D::CopyConnectors(RObj2D* o)
{
	RObj2DConnector* d;

	Connectors.Clear(o->Connectors.GetMaxNb(),o->Connectors.GetIncNb());
	RCursor<RObj2DConnector> Cur(o->Connectors);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		d=new RObj2DConnector(this,Connectors.GetNb(),Cur()->Name,Cur()->NbPos);
		for(size_t i=0;i<Cur()->NbPos;i++)
			d->Pos[i]=Cur()->Pos[i];
		Connectors.InsertPtr(d);
	}
}


//------------------------------------------------------------------------------
RObj2D::~RObj2D(void)
{
}
