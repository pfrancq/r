/*

	R Project Library

	RGeoInfoConnection.cpp

	Connections for the 2D Placement (Geometric Information Part) - Implementation.

	(c) 2000-2003 by P. Francq.

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
#include <rga2d/rgeoinfoconnection.h>
#include <rga2d/rgeoinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RGeoInfoConnectionPart
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoConnectionPart::RGeoInfoConnectionPart(const RPoint& pt1,unsigned int id1,const RPoint& pt2,unsigned int id2)
	: Id1(id1), PosCon1(pt1), Id2(id2), PosCon2(pt2)
{
}


//------------------------------------------------------------------------------
RGeoInfoConnectionPart::RGeoInfoConnectionPart(const RGeoInfoConnectionPart* p)
	: Id1(p->Id1), PosCon1(p->PosCon1), Id2(p->Id2), PosCon2(p->PosCon2)
{
}



//------------------------------------------------------------------------------
//
// class RGeoInfoConnection
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoConnection::RGeoInfoConnection(RConnection* con)
	: RContainer<RGeoInfoConnectionPart,unsigned int,true,false>(con->Connect.NbPtr,con->Connect.NbPtr/2),
		Con(con), Complete(false), Dist(0)
{
}


//------------------------------------------------------------------------------
RGeoInfoConnection::RGeoInfoConnection(const RGeoInfoConnection& con)
	: RContainer<RGeoInfoConnectionPart,unsigned int,true,false>(con),
	  Con(con.Con), Complete(con.Complete), Dist(con.Dist)
{
}


//------------------------------------------------------------------------------
RGeoInfoConnection::RGeoInfoConnection(const RGeoInfoConnection* con)
	: RContainer<RGeoInfoConnectionPart,unsigned int,true,false>(con),
	  Con(con->Con), Complete(con->Complete), Dist(con->Dist)
{
}


//------------------------------------------------------------------------------
void RGeoInfoConnection::UnComplete(void)
{
	Complete=false;
	Dist=0.0;
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::MinObjPlaced(RGeoInfos* infos)
{
	unsigned int i,count;
	RObj2DConnector **con;

	count=0;
	// Go through each connectors
	for(i=Con->Connect.NbPtr+1,con=Con->Connect.Tab;--i;con++)
	{
		if(infos->GetPtr<unsigned int>((*con)->Owner->GetId())->IsValid())
		{
			count++;
		}
		if(count==2)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
void RGeoInfoConnection::ComputeMinDist(RGeoInfos* infos)
{
	RObj2DConnector **con,**con2;
	RGeoInfo *g1,*g2;
	unsigned int i,j;
	unsigned int id1,id2;
	RVertex *v1,*v2;
	REdge **e;
	RPoint pt1,pt2;
	RGraph T(Con->Connect.NbPtr),S(Con->Connect.NbPtr);
	RContainer<RGeoInfoConnectionPart,unsigned int,true,false> Local(Con->Connect.NbPtr,Con->Connect.NbPtr/2);
	RGeoInfoConnectionPart::sSearch s;

	// Init
	Clear();
	Complete=true;

	// Go through each connectors but the last one
	RAssert(Con->Connect.NbPtr);
	for(i=Con->Connect.NbPtr,con=Con->Connect.Tab;--i;con++)
	{
		id1=(*con)->Owner->GetId();
		g1=infos->GetPtr<unsigned int>(id1);
		if(g1->IsValid())
		{
			v1=T.GetVertex(id1);
			for(j=i+1,con2=con+1;--j;con2++)
			{
				id2=(*con2)->Owner->GetId();
				g2=infos->GetPtr<unsigned int>(id2);
				if(g2->IsValid())
				{
					v2=T.GetVertex(id2);
					T.CreateEdge(v1,v2,(*con)->GetMinDist(*con2,infos,pt1,pt2));
					Local.InsertPtr(new RGeoInfoConnectionPart(pt1,id1,pt2,id2));
				}
				else
					Complete=false;
			}
		}
		else
			Complete=false;
	}

	// Compute Minimum Spanning tree
	Dist=0.0;
	T.MinSpanningTree(&S);
	for(i=S.Edges.NbPtr+1,e=S.Edges.Tab;--i;e++)
	{
		Dist+=(*e)->Weight;
		s.id1=(*e)->v1->Id;
		s.id2=(*e)->v2->Id;
		InsertPtr(new RGeoInfoConnectionPart(Local.GetPtr<RGeoInfoConnectionPart::sSearch>(s)));
	}
}


//------------------------------------------------------------------------------
double RGeoInfoConnection::GetDist(RGeoInfos* infos,RGeoInfo* cur,const RPoint& pos)
{
	// Make the current geometric information temporary valid
	cur->Pos=pos;

	// Verify if this connection is affected.
	// if yes -> Compute the distances again.
	if((!Complete)&&IsIn(cur)&&MinObjPlaced(infos))
	{
		ComputeMinDist(infos);
	}

	// Make it invalid again
	cur->Pos.Set(MaxCoord,MaxCoord);

	// Return the distance
	return(Dist);
}


//------------------------------------------------------------------------------
double RGeoInfoConnection::GetDist(RGeoInfos* infos,RGeoInfo* cur)
{
	if(IsIn(cur))
	{
		// Verify if this connection is affected.
		// if yes -> Compute the distances again.
		if((!Complete)&&MinObjPlaced(infos))
		{
			ComputeMinDist(infos);
		}
		// Return the distance
		return(Dist);
	}
	return(0.0);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::IsIn(const RGeoInfo* info)
{
	RObj2DConnector **con;
	RObj2D* owner;
	unsigned int i;

	if(!info) return(false);
	owner=info->Obj;
	for(i=Con->Connect.NbPtr+1,con=Con->Connect.Tab;--i;con++)
	{
		if((*con)->Owner==owner)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::IsIn(const RGeoInfoConnector* con)
{
	RObj2DConnector **cons;
	unsigned int id;
	unsigned int i;
	RObj2D* owner;

	if(!con) return(false);
	id=con->Con->Id;
	owner=con->Owner->Obj;
	for(i=Con->Connect.NbPtr+1,cons=Con->Connect.Tab;--i;cons++)
	{
		if(((*cons)->Id==id)&&((*cons)->Owner==owner))
			return(true);
	}
	return(false);
}
