/*

	R Project Library

	RGeoInfoConnection.cpp

	Connections for the 2D Placement (Geometric Information Part) - Implementation.

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
#include <redge.h>
#include <rvertex.h>
#include <rgeoinfoconnection.h>
#include <rlayout.h>
#include <robj2dconfig.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RGeoInfoConnection::Part
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RGeoInfoConnection::Part
{
public:
	size_t Obj1;
	RObj2DConfigPin* Pin1;
	size_t Obj2;
	RObj2DConfigPin* Pin2;

	Part(size_t obj1,size_t obj2) : Obj1(obj1), Pin1(0), Obj2(obj2), Pin2(0) {}
	int Compare(const Part& tmp) const
	{
		if(Obj1==tmp.Obj1)
			return(Obj2-tmp.Obj2);
		return(Obj1-tmp.Obj2);
	}
};


//------------------------------------------------------------------------------
//
// class RGeoInfoConnection
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoConnection::RGeoInfoConnection(RLayout* layout,RConnection* con)
	: RContainer<RGeoInfoPin,true,true>(con->GetNb()),
		Con(con), Layout(layout), Complete(false), Dist(0)
{
}


//------------------------------------------------------------------------------
void RGeoInfoConnection::UnComplete(void)
{
	Complete=false;
	Dist=0.0;
}


//------------------------------------------------------------------------------
int RGeoInfoConnection::Compare(const RGeoInfoConnection&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::MinObjPlaced(RLayout* layout)
{
	size_t count(0);

	// Go through each connectors
	RCursor<RObj2DConnector> con(Con->GetConnectors());
	for(con.Start();!con.End();con.Next())
	{
		RObj2D* Obj(con()->GetObj());
		if((!Obj)||((Obj)&&((*layout)[Obj->GetId()]->IsValid())))
			count++;
		if(count==2)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
tCoord RGeoInfoConnection::ComputeMinDist(RLayout* layout)
{
	Dist=0.0;

	if(Con->GetNb()<2)
		return(Dist);

	RGeoInfo *g1,*g2;
	RObj2DConfigConnector *Con1,*Con2;
	size_t id1,id2,i;
	RVertex *v1,*v2;
	RPoint pt1,pt2;
	RGraph<RVertex,REdge> Graph(Con->GetNb());  // The complete graph of all placed pins
	RGraph<RVertex,REdge> Tree(Con->GetNb());   // The minimum spanning tree
	RContainer<Part,true,false> Local(Con->GetNb());

	// Initialize
	Clear();
	Complete=true;  // Suppose it is completed

	// Construct the graph between all pairs of pins
	mAssert(Con->GetNb());
	RCursor<RObj2DConnector> con(Con->GetConnectors());
	RCursor<RObj2DConnector> con2(Con->GetConnectors());
	for(con.Start(),i=con.GetNb();--i;con.Next())
	{
		id1=con()->GetObj()->GetId();
		g1=(*layout)[id1];
		if(g1->IsValid())
			Con1=g1->GetConfig()->GetConnector(con());
		else
			Con1=0;
		if(Con1)
		{
			v1=Graph.GetVertex(id1);
			for(con2.GoTo(con.GetPos()+1);!con2.End();con2.Next())
			{
				id2=con2()->GetObj()->GetId();
				g2=(*layout)[id2];
				if(g2->IsValid())
					Con2=g2->GetConfig()->GetConnector(con2());
				else
					Con2=0;
				if(Con2)
				{
					v2=Graph.GetVertex(id2);
					Part* Net(new Part(id1,id2));
					Graph.CreateEdge(v1,v2,Con1->GetMinDist(Con2,layout,Net->Pin1,Net->Pin2));
					if((!Net->Pin1)||(!Net->Pin2))
						std::cout<<"Problem"<<std::endl;
					Local.InsertPtr(Net);
				}
				else
					Complete=false;
			}
		}
		else
			Complete=false;
	}

	// Compute Minimum Spanning tree
	Graph.MinSpanningTree(&Tree);
	RCursor<REdge> e(Tree.GetEdges());
	for(e.Start();!e.End();e.Next())
	{
		Dist+=e()->GetWeight();
		Part Search(e()->GetFrom()->GetId(),e()->GetTo()->GetId());
		Part* Net(Local.GetPtr(Search));
		g1=(*layout)[Net->Obj1];
		g2=(*layout)[Net->Obj2];
		InsertPtr(new RGeoInfoPin(Net->Pin1,g1));
		InsertPtr(new RGeoInfoPin(Net->Pin2,g2));
	}
	return(Dist);
}


//------------------------------------------------------------------------------
tCoord RGeoInfoConnection::GetDist(RLayout* layout,RGeoInfo* info,const RPoint& pos)
{
	// Is the geometric information concerned ?
	bool InfoIsIn=IsIn(info);

	// Make the current geometric information temporary valid
	if(pos!=RPoint::Null)
		info->Assign(pos,0,cNoRef);
	else
		if(!InfoIsIn)
			return(0.0);

	// Verify if this connection is affected.
	// if yes -> Compute the distances again.
	if((!Complete)&&InfoIsIn&&MinObjPlaced(layout))
	{
		ComputeMinDist(layout);
	}

	// Make it invalid again
	if(pos!=RPoint::Null)
		info->Assign(RPoint::Null,0,cNoRef);

	// Return the distance
	return(Dist);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::IsIn(const RGeoInfo* info)
{
	RObj2D* owner;

	if(!info) return(false);
	owner=info->GetObj();
	RCursor<RObj2DConnector> con(Con->GetConnectors());
	for(con.Start();!con.End();con.Next())
	{
		if(con()->GetObj()==owner)
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::IsIn(const RObj2DConfigConnector* con)
{
	if(!con) return(false);
	size_t id(con->GetConnector()->GetId());
	RObj2D* owner(con->GetConnector()->GetObj());
	RCursor<RObj2DConnector> cons(Con->GetConnectors());
	for(cons.Start();!cons.End();cons.Next())
	{
		if((cons()->GetId()==id)&&(cons()->GetObj()==owner))
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RGeoInfoConnection::IsIn(const RObj2DConfigPin* pin)
{
	if(!pin) return(false);
	size_t id(pin->GetPin()->GetConnector()->GetId());
	RObj2D* owner(pin->GetPin()->GetConnector()->GetObj());
	RCursor<RObj2DConnector> cons(Con->GetConnectors());
	for(cons.Start();!cons.End();cons.Next())
	{
		if((cons()->GetId()==id)&&(cons()->GetObj()==owner))
			return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
RCursor<RGeoInfoPin> RGeoInfoConnection::GetPins(void) const
{
	return(RCursor<RGeoInfoPin>(*this));
}
