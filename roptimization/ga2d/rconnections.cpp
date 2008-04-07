/*

	R Project Library

	RConnections.cpp

	Connections for the 2D Placement - Implementation.

	Copyright 2000-2008 by the Université Libre de Bruxelles.

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
#include <random.h>
#include <rconnections.h>
#include <rgeoinfos.h>
#include <rpromkernel.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RConnections
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RConnections::RConnections(void)
	: RContainer<RConnection,true,false>(50,25), Random(0)
{
}


//------------------------------------------------------------------------------
void RConnections::Init(void)
{
	RCursor<RObj2DConnector> con;

	// Go through all connections
	RCursor<RConnection> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// Go through each connector
		con.Set(Cur()->Connect);
		for(con.Start();!con.End();con.Next())
		{
			con()->Connections.InsertPtr(Cur());
		}
	}
}


//------------------------------------------------------------------------------
void RConnections::SetParams(RParam* dist,RParam* weight,RRandom* r)
{
	DistParams=dist;
	WeightParams=weight;
	Random=r;
}


//------------------------------------------------------------------------------
double RConnections::GetCon(RGeoInfos* infos,RGeoInfo* cur)
{
	double sum(0.0);
	RCursor<RObj2DConnector> con2;
	RCursor<RConnection> tab;
	bool cont;
	RGeoInfo *tmp;

	// Go through all connectors
	RCursor<RObj2DConnector> con(cur->GetObj()->Connectors);
	for(con.Start();!con.End();con.Next())
	{
		// Go through each connection
		tab.Set(con()->Connections);
		for(tab.Start();!tab.End();tab.Next())
		{
			// Go through each connector and see if the geometric information is placed
			con2.Set(tab()->Connect);
			for(con2.Start();!con2.End();con2.Next())
			{
				tmp=infos->GetPtr(con2()->Owner->Id);
				if(tmp)
					cont=!tmp->IsValid();
			}
			if(!cont)
			{
				// At least one valid geometric information found
				sum+=tab()->Weight;
			}
		}
	}
	return(sum);
}


//------------------------------------------------------------------------------
RGeoInfo* RConnections::GetBestConnected(RGeoInfos* infos,unsigned int nb,bool* selected,RRect& bound)
{
	RPromKernel Prom("PlacementCenter",nb,2);
	RPromCriterion *weight,*dist;
	RPromSol* sol;
	RPromSol** sols=0;
	RPromSol** best=0;
	RGeoInfo **treat,*b;
	unsigned int Nb=0;
	double w,d;
	RRect r1,r2;
	bool bFound;
	tCoord X1,Y1,X2,Y2;
	RPoint Pt;
	bool bProm;

	// Init Part
	Prom.AddCriterion(weight=new RPromLinearCriterion(RPromCriterion::Maximize,WeightParams,"Weight"));
	Prom.AddCriterion(dist=new RPromLinearCriterion(RPromCriterion::Minimize,DistParams,"Distance"));
	treat=new RGeoInfo*[nb];

	// Go through each info
	RCursor<RGeoInfo> info(*infos);
	for(info.Start();!info.End();info.Next())
	{
		// If selected -> go to next
		if(selected[info()->GetObj()->GetId()])
			continue;

		// Calculate distance and weight
		w=GetCon(infos,info());
		d=GetDistances(*infos,info());
		if((w>0.0)&&(d>0.0))
		{
			// If both are not null -> Create a Prom�h� solution
			sol=Prom.NewSol();
			Prom.Assign(sol,weight,w);
			Prom.Assign(sol,dist,d);
			treat[sol->GetId()]=info();
			Nb++;        // Increment number of solution
		}
	}

	if(!Nb)
	{
		// Choose a random not selected object
		Nb=nb;
		memcpy(treat,infos,nb*sizeof(RGeoInfo*));
		Random->RandOrder<RGeoInfo*>(treat,nb);
		RGeoInfo** info=treat;
		while(selected[(*info)->GetObj()->GetId()])
			info++;
		b=(*info);
		bProm=false;
	}
	else
	{
		Prom.ComputePrometheeII();         // Compute PROMETHEE
		sols=best=Prom.GetSols();          // Get the solutions
		b=treat[(*(best++))->GetId()];     // The first one is the best
		bProm=true;

	}
		b->Boundary(r1);                   // Get The boundary rectangle

		// Go through the others and find the best one that can go in bound
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			treat[(*best)->GetId()]->Boundary(r2);      // Get The boundary rectangle
			if(r1.X1<r2.X1) X1=r1.X1; else X1=r2.X1;
			if(r1.Y1<r2.Y1) Y1=r1.Y1; else Y1=r2.Y1;
			if(r1.X2>r2.X2) X2=r1.X2; else X2=r2.X2;
			if(r1.Y2>r2.Y2) Y2=r1.Y2; else Y2=r2.Y2;
			if(((X2-X1+1)<bound.GetWidth())&&((Y2-Y1+1)<bound.GetHeight()))
			{
				bFound=true;
//				X2=bound.Width()-(X2-X1);
//				Y2=bound.Height()-(Y2-Y1);
//				Pt.Set(X1-(X2/2),Y1-(Y2/2));
//				bound+=Pt;
				r1.Set(X1,Y1,X2,Y2);
			}
			best++;
		}
		if(bProm)
			delete[] sols;

		if(!bFound)
		{
			// Nothing found -> Central Rectangle
			Pt=b->GetPos();
			Pt.X+=r1.GetWidth()/2-bound.GetWidth()/2;
			Pt.Y+=r1.GetHeight()/2-bound.GetHeight()/2;
			bound+=Pt;
		}
//	}
	delete[] treat;
	bound=r1;
	return(b);
}


//------------------------------------------------------------------------------
RGeoInfo* RConnections::GetMostConnected(RGeoInfos* infos,unsigned int nb,RGeoInfo** order,unsigned int nbok)
{
	RGeoInfo **ptr=&order[nbok],*best;
	unsigned int bestpos,i=nbok+1;
	double bestw,w;

	best=(*(ptr++));
	bestw=GetCon(infos,best);
	bestpos=nbok;
	while(i<nb)
	{
		w=GetCon(infos,*ptr);
		if(w>bestw)
		{
			bestw=w;
			best=*ptr;
			bestpos=i;
		}
		i++;
		ptr++;
	}
	order[bestpos]=order[nbok];
	return(best);
}


//------------------------------------------------------------------------------
//double RConnections::GetDistances(RGeoInfo** /*infos*/)
//{
//	RConnection* c;
//	RGeoInfo *f,*t;
//	RGeoInfoConnector *cf,*ct;
//	double sum=0.0;

//	for(Start();!End();Next())
//	{
//		c=(*this)();
//		f=infos[c->From->Owner->GetId()];
//		t=infos[c->To->Owner->GetId()];
//		if((!t->IsValid())||(!f->IsValid())) continue;
//		cf=f->Connectors.GetPtr<unsigned int>(c->From->GetId());
//		ct=t->Connectors.GetPtr<unsigned int>(c->To->GetId());
//		sum+=cf->GetPos().ManhattanDist(ct->GetPos())*c->Weight;
//	}
//	return(sum);
//}


//------------------------------------------------------------------------------
double RConnections::GetDistances(RGeoInfos&/*infos*/,RGeoInfo* /*info*/)
{
//	RConnection* c;
//	RGeoInfo *f,*t;
//	RGeoInfoConnector *cf,*ct;
	double sum=0.0;

//	for(Start();!End();Next())
//	{
//		c=(*this)();
//		f=infos[c->From->Owner->GetId()];
//		t=infos[c->To->Owner->GetId()];
//		if((!t->IsValid())||(!f->IsValid())) continue;
//		if((t!=info)&&(f!=info)) continue;
//		cf=f->Connectors.GetPtr<unsigned int>(c->From->GetId());
//		ct=t->Connectors.GetPtr<unsigned int>(c->To->GetId());
//		sum+=cf->GetPos().ManhattanDist(ct->GetPos())*c->Weight;
//	}
	return(sum);
}


//------------------------------------------------------------------------------
RConnections::~RConnections(void)
{
}
