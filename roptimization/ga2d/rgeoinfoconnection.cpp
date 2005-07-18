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
#include <rgeoinfoconnection.h>
#include <rgeoinfos.h>
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
	: RContainer<RGeoInfoConnectionPart,true,false>(con->Connect.GetNb()),
		Con(con), Complete(false), Dist(0)
{
}


//------------------------------------------------------------------------------
RGeoInfoConnection::RGeoInfoConnection(const RGeoInfoConnection& con)
	: RContainer<RGeoInfoConnectionPart,true,false>(con),
	  Con(con.Con), Complete(con.Complete), Dist(con.Dist)
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
	unsigned int count(0);

	// Go through each connectors
	RCursor<RObj2DConnector> con(Con->Connect);
	for(con.Start();!con.End();con.Next())
	{
		if(infos->GetPtr<unsigned int>(con()->Owner->GetId())->IsValid())
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
	RGeoInfo *g1,*g2;
	unsigned int id1,id2;
	RVertex *v1,*v2;
	RPoint pt1,pt2;
	RGraph T(Con->Connect.GetNb()),S(Con->Connect.GetNb());
	RContainer<RGeoInfoConnectionPart,true,false> Local(Con->Connect.GetNb());
	RGeoInfoConnectionPart::sSearch s;

	// Init
	Clear();
	Complete=true;

	// Go through each connectors but the last one
	RAssert(Con->Connect.GetNb());
	RCursor<RObj2DConnector> con(Con->Connect);
	RCursor<RObj2DConnector> con2(Con->Connect);
	for(con.Start();!con.End();con.Next())
	{
		id1=con()->Owner->GetId();
		g1=infos->GetPtr<unsigned int>(id1);
		if(g1->IsValid())
		{
			v1=T.GetVertex(id1);
			for(con2.GoTo(con.GetPos()+1);!con2.End();con.Next())
			{
				id2=con2()->Owner->GetId();
				g2=infos->GetPtr<unsigned int>(id2);
				if(g2->IsValid())
				{
					v2=T.GetVertex(id2);
					T.CreateEdge(v1,v2,con()->GetMinDist(con2(),infos,pt1,pt2));
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
	RCursor<REdge> e(S.Edges);
	for(e.Start();!e.End();e.Next())
	{
		Dist+=e()->Weight;
		s.id1=e()->v1->Id;
		s.id2=e()->v2->Id;
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
	RObj2D* owner;

	if(!info) return(false);
	owner=info->Obj;
	RCursor<RObj2DConnector> con(Con->Connect);
	for(con.Start();!con.End();con.Next())
	{
		if(con()->Owner==owner)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::IsIn(const RGeoInfoConnector* con)
{
	unsigned int id;
	RObj2D* owner;

	if(!con) return(false);
	id=con->Con->Id;
	owner=con->Owner->Obj;
	RCursor<RObj2DConnector> cons(Con->Connect);
	for(cons.Start();!cons.End();cons.Next())
	{
		if((cons()->Id==id)&&(cons()->Owner==owner))
			return(true);
	}
	return(false);
}
