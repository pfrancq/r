/*

	R Project Library

	RCons.cpp

	Connections for the 2D Placement - Implementation.

	(c) 2000-2001 by P. Francq.

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
#include <rmath/random.h>
using namespace RMath;
#include <rga2d/rconnection.h>
using namespace RGA2D;
#include <rgraph/rgraph.h>
using namespace RGraphs;


//-----------------------------------------------------------------------------
//
// class LocalVertexRepresentation
//
//-----------------------------------------------------------------------------
class LocalVertexRepresentation
{
public:
	RPoint pt1;
	RPoint pt2;
	REdge* edge;

	LocalVertexRepresentation(const RPoint& p1,const RPoint& p2,REdge* e)
		: pt1(p1),pt2(p2),edge(e) {}
	int Compare(const LocalVertexRepresentation *v) {return(edge->Compare(v->edge));}
};



//-----------------------------------------------------------------------------
//
// class RConnection
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RConnection::RConnection(unsigned int nb,double weight)
	: Weight(weight), Connect(nb,nb/2)
{
}


//-----------------------------------------------------------------------------
double RGA2D::RConnection::GetMinDist(RGeoInfo** infos)
{
	RObj2DConnector **con,**con2;
	RGeoInfo *g1,*g2;
	unsigned int i,j;
	RVertex *v1,*v2;
	RGraph T(Connect.NbPtr),S(Connect.NbPtr);
	double sum=0.0;
	REdge **e;
	RPoint pt1,pt2;
	RContainer<LocalVertexRepresentation,unsigned int,true,false> Local(Connect.NbPtr,Connect.NbPtr/2);

	// Go through each connectors
	for(i=Connect.NbPtr+1,con=Connect.Tab;--i;con++)
	{
		g1=infos[(*con)->Owner->GetId()];
		if(g1->IsValid())
		{
			v1=T.GetVertex(g1->GetObj()->GetId());
			for(j=i+1,con2=con+1;--j;con++)
			{
				g2=infos[(*con)->Owner->GetId()];
				if(g2->IsValid())
				{
					v2=T.GetVertex(g1->GetObj()->GetId());
					Local.InsertPtr(new LocalVertexRepresentation(pt1,pt2,T.CreateEdge(v1,v2,(*con)->GetMinDist(*con2,infos,pt1,pt2))));
				}
			}
		}
	}
	T.MinSpanningTree(&S);
	for(i=S.Edges.NbPtr+1,e=S.Edges.Tab;--i;e++)
		sum+=(*e)->Weight;
	return(sum);
}
