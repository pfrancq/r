/*

	R Project Library

	RObj2D.hh

	Object for 2D placement GA - Implementation

	(C) 1999-2001 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rconnection.h>



//-----------------------------------------------------------------------------
//
// RObj2DConnector
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const RPoint pos)
	: Owner(owner), Id(id), Name(itoa(id)), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	Pos[0]=pos;
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const RString& name,const RPoint pos)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[1];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0]=pos;
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const char* name,const RPoint pos)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[1];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0]=pos;
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const unsigned int x,unsigned y)
	: Owner(owner), Id(id), Name(itoa(id)), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[1];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner,unsigned int id,const RString& name,const unsigned int x,unsigned y)
	: Owner(owner), Id(id), Name(name), Connections(10,5)
{
	Pos=new RPoint[1];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const unsigned int x,unsigned y)
	: Owner(owner), Id(id), Name(name), NbPos(1), Connections(10,5)
{
	Pos=new RPoint[1];
	Poss=new RPoint*[NbPos];
	Poss[0]=new RPoint[8];
	Pos[0].Set(x,y);
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DConnector::RObj2DConnector(RObj2D* owner, unsigned int id,const char* name,const unsigned int nb)
	: Owner(owner), Id(id), Name(name), NbPos(nb), Connections(10,5)
{
	Pos=new RPoint[NbPos];
	Poss=new RPoint*[NbPos];
	for(unsigned int i;i<NbPos;i++)
		Poss[i]=new RPoint[8];
}


//-----------------------------------------------------------------------------
RPoint& RGA2D::RObj2DConnector::GetPos(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Pos[0];
	return(*pt);
}


//-----------------------------------------------------------------------------
RPoint& RGA2D::RObj2DConnector::GetPos(unsigned int i,char o)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Poss[i][o];
	return(*pt);
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2DConnector::AddConnection(RConnection* con)
{
	Connections.InsertPtr(con);
}



//-----------------------------------------------------------------------------
//
// RObj2D
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RObj2D::RObj2D(unsigned int id,bool deformable)
	: Id(id), Name(itoa(id)), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//-----------------------------------------------------------------------------
RGA2D::RObj2D::RObj2D(unsigned int id,const RString& name,bool deformable)
	: Id(id), Name(name), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//-----------------------------------------------------------------------------
RGA2D::RObj2D::RObj2D(unsigned int id,const char* name,bool deformable)
	: Id(id), Name(name), Area(0), NbPossOri(0), Deformable(deformable), Connectors(5,5)
{
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2D::Init(void)
{
	char i;
	RPolygon *p;
	RRects *r;

	CalcPolygons();
	for(i=NbPossOri+1,p=Polygons,r=Rects;--i;p++,r++)
		p->RectDecomposition(r);
	Area=Rects[0].Area();
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2D::CalcPolygons(void)
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
				Connectors()->Poss[0]=Connectors()->Pos;
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
					Connectors()->Poss[idx]=Connectors()->Pos;			
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
						Connectors()->Poss[j][idx]=Connectors()->Pos;
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
 				Connectors()->Poss[j][i]=Connectors()->Pos;
 				Connectors()->Poss[j][i].ChangeOrientation(*o);
				Connectors()->Poss[j][i]-=Min;
			}
 		}		
	}
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2D::SetOri(ROrientation o)
{
	char i;
	ROrientation *ptr;

	for(i=NbPossOri+1,ptr=PossOri;--i;ptr++)
		if((*ptr)==o) return;
	PossOri[NbPossOri++]=o;
}


//-----------------------------------------------------------------------------
bool RGA2D::RObj2D::IsOriSet(ROrientation o)
{
	char i;
	ROrientation *ptr;

	for(i=NbPossOri+1,ptr=PossOri;--i;ptr++)
		if((*ptr)==o) return(true);
	return(false);
}


//-----------------------------------------------------------------------------
RPolygon* RGA2D::RObj2D::GetPolygon(char i)
{
	if(i<NbPossOri) return(&Polygons[i]);
	return(0);
}


//-----------------------------------------------------------------------------
RRects* RGA2D::RObj2D::GetRects(char i)
{
	if(i<NbPossOri) return(&Rects[i]);
	return(0);
}


//-----------------------------------------------------------------------------
RObj2D& RGA2D::RObj2D::operator=(const RObj2D &obj)
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


//-----------------------------------------------------------------------------
void RGA2D::RObj2D::AddConnector(unsigned int id,unsigned x,unsigned y)
{
	Connectors.InsertPtr(new RObj2DConnector(this,id,x,y));
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2D::CopyConnectors(RObj2D* o)
{
	RObj2DConnector *s,*d;

	Connectors.Clear(o->Connectors.MaxPtr,o->Connectors.IncPtr);
	for(o->Connectors.Start();!o->Connectors.End();o->Connectors.Next())
	{
		s=o->Connectors();
		d=new RObj2DConnector(this,Connectors.NbPtr,s->Name,s->NbPos);
		d->NbPos=s->NbPos;
		for(unsigned int i=0;i<s->NbPos;i++)
			d->Pos[i]=s->Pos[i];
		Connectors.InsertPtr(d);
	}
}



//-----------------------------------------------------------------------------
//
// RObj2DContainer
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RObj2DContainer::RObj2DContainer(unsigned int id,unsigned int max)
	: RObj2D(id,false),Ids(0),Infos(0),NbMax(max),Nb(0)
{
	NbPossOri=1;
	PossOri[0]=Normal;
	Ids=new unsigned int[NbMax];
	Infos=new RGeoInfo*[NbMax];
	memset(Ids,0,NbMax*sizeof(unsigned int));	
	memset(Infos,0,NbMax*sizeof(RGeoInfo*));	
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2DContainer::Clear(void)
{
	SPolygons.Clear();
	memset(Ids,0,NbMax*sizeof(unsigned int));	
	memset(Infos,0,NbMax*sizeof(RGeoInfo*));	
	Nb=0;
	MinX=MinY=MaxCoord;
	Area=0;
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2DContainer::AddObj(RObj2D *obj,RGeoInfo *info)
{
	RPoint **point,pt;
	unsigned int i,j;
	RCoord CalcX,CalcY;
	RPolygon ins,**poly;

	// Determine the most left-bottom edge
	CalcX=MinX;
	CalcY=MinY;
	for(info->Start();!info->End();info->Next())
	{
		pt=(*info)();
		if(pt.Y<MinY) MinY=pt.Y;
		if(pt.X<MinX) MinX=pt.X;
	}
	
	// Create new polygon and translate it
	ins=info->GetPolygon();
	pt.Set(-MinX,-MinY);
	ins+=pt;

	// translate already inserted polygons if new MinX,MinY
	if(Nb&&((CalcX!=MinX)||(CalcY!=MinY)))
	{
		CalcX-=MinX;
		CalcY-=MinY;
		for(i=SPolygons.NbPtr+1,poly=SPolygons.Tab;--i;poly++)
			for(j=(*poly)->NbPtr+1,point=(*poly)->Tab;--j;point++)
			{
				(*point)->X+=CalcX;
				(*point)->Y+=CalcY;
			}
	}

	// Put object in lists
	SPolygons.InsertPtr(new RPolygon(ins));
	Ids[Nb]=obj->Id;
	Infos[Nb]=info;
	Area+=info->GetArea();
	Nb++;
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2DContainer::EndObjs(void)
{
	SPolygons.Union(&Polygon);
	Polygons[0]=Polygon;
	Polygon.RectDecomposition(&Rects[0]);
}


//-----------------------------------------------------------------------------
void RGA2D::RObj2DContainer::Assign(RPoint& /*pos*/,RGeoInfo** /*infos*/,unsigned int** /*OccX*/,unsigned int** /*OccY*/)
{
/*	unsigned int i;
	unsigned int *ids;
	RGeoInfo **info,*cur;
	RPoint p;

	pos.X-=MinX;
	pos.Y-=MinY;	
	for(i=Nb+1,ids=Ids,info=Infos;--i;ids++,info++)
	{
		cur=infos[*ids];
		p=(*info)->Pos;
		p+=pos;
		cur->Assign(p,OccX,OccY);
		cur->Selected=true;
	}*/
}


//-----------------------------------------------------------------------------
RGA2D::RObj2DContainer::~RObj2DContainer(void)
{
	if(Ids) delete[] Ids;
	if(Infos) delete[] Infos;
}
