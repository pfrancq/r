/*

	R Project Library

	RConnections.cpp

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
#include <rga2d/rconnections.h>
using namespace RGA2D;
#include <rpromethee/rpromkernel.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
//
// class RCons
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RConnections::RConnections(void)
	: RContainer<RConnection,unsigned int,true,false>(50,25)
{
}


//-----------------------------------------------------------------------------	
void RGA2D::RConnections::Init(void)
{
	RConnection **tab;
	unsigned int i;
	
	// Go through all connections
	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		// Add Connection to "To"
		(*tab)->GetTo()->AddConnection(*tab);
		// Add Connection to "From"
		(*tab)->GetFrom()->AddConnection(*tab);
	}
}


//-----------------------------------------------------------------------------
void RGA2D::RConnections::SetParams(const RPromCriterionParams& dist,const RPromCriterionParams& weight)
{
	DistParams=dist;
	WeightParams=weight;	
}


//-----------------------------------------------------------------------------	
double RGA2D::RConnections::GetCon(RGeoInfo **infos,RGeoInfo *cur)
{
	double sum=0.0;
	RObj2DConnector *con;
	RGeoInfo *f,*t;
	RObj2D* o1;
	RObj2D *o2;
		
	for(cur->Connectors.Start();!cur->Connectors.End();cur->Connectors.Next())
	{
		con=cur->Connectors()->Con;
		for(con->Connections.Start();!con->Connections.End();con->Connections.Next())
		{
			o1=con->Connections()->From->Owner;
			o2=con->Connections()->To->Owner;
			f=infos[con->Connections()->From->Owner->GetId()];
			t=infos[con->Connections()->To->Owner->GetId()];
			if(((f==cur)&&t->IsValid())||((t==cur)&&f->IsValid()))
				sum+=con->Connections()->GetWeight();
		}
	}
	return(sum);
}


//-----------------------------------------------------------------------------	
RGeoInfo* RGA2D::RConnections::GetBestConnected(RGeoInfo **infos,unsigned int nb,bool* selected,RRect& bound)
{
	RPromKernel Prom("PlacementCenter",nb,2);
	RPromCriterion *weight,*dist;
  	RPromSol *sol,**sols,**best;	
	RGeoInfo **info,**treat,*b;
	unsigned int i,Nb=0;
	double w,d;
	RRect r1,r2;
	bool bFound;
	RCoord X1,Y1,X2,Y2;
	RPoint Pt;
	
	// Init Part
	weight=Prom.NewCriterion(Maximize,WeightParams);
	dist=Prom.NewCriterion(Minimize,DistParams);
	treat=new RGeoInfo*[nb];
	
	// Go through each info
	for(i=nb+1,info=infos;--i;info++)
	{
		// If selected -> go to next
		if(selected[(*info)->GetObj()->GetId()])
			continue;
		
		// Calculate distance and weight
		w=GetCon(infos,*info);
		d=GetDistances(infos,*info);		
		if((w>0.0)&&(d>0.0))
		{
			// If both are not null -> Create a Prométhée solution
			sol=Prom.NewSol();
			Prom.Assign(sol,weight,w);
			Prom.Assign(sol,dist,d);
			treat[sol->GetId()]=(*info);
			Nb++;	// Increment number of solution
		}
	}
	
	if(!Nb)
	{
		// Choose a random not selected object
		memcpy(treat,infos,nb*sizeof(RGeoInfo*));
		RRandom::randorder<RGeoInfo*>(treat,nb);
		info=treat;		
		while(selected[(*info)->GetObj()->GetId()])
			info++;
		b=(*info);
	}
	else
	{
		Prom.ComputePrometheeII();		// Compute Prométhée		
		best=sols=Prom.GetSols();		// Get the solutions
		b=treat[(*(best++))->GetId()];	// The first one is the best
		b->Boundary(r1);				// Get The boundary rectangle
		
		// Go through the others and find the best one that can go in bound
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			treat[(*best)->GetId()]->Boundary(r2);		// Get The boundary rectangle
			if(r1.Pt1.X<r2.Pt1.X) X1=r1.Pt1.X; else X1=r2.Pt1.X;
			if(r1.Pt1.Y<r2.Pt1.Y) Y1=r1.Pt1.Y; else Y1=r2.Pt1.Y;
			if(r1.Pt2.X>r2.Pt2.X) X2=r1.Pt2.X; else X2=r2.Pt2.X;
			if(r1.Pt2.Y>r2.Pt2.Y) Y2=r1.Pt2.Y; else Y2=r2.Pt2.Y;
			if(((X2-X1)<bound.Width())&&((Y2-Y1)<bound.Height()))
			{
				bFound=true;
				X2=bound.Width()-(X2-X1);
				Y2=bound.Height()-(Y2-Y1);
				Pt.Set(X1-(X2/2),Y1-(Y2/2));
				bound+=Pt;
			}
			best++;
		}
		delete[] sols;
		
		if(!bFound)
		{
			// Nothing found -> Central Rectangle
			Pt=b->GetPos();
			Pt.X+=r1.Width()/2-bound.Width()/2;
			Pt.Y+=r1.Height()/2-bound.Height()/2;
			bound+=Pt;
		}
	}
	delete[] treat;
	return(b);
}


//-----------------------------------------------------------------------------	
RGeoInfo* RGA2D::RConnections::GetMostConnected(RGeoInfo **infos,unsigned int nb,unsigned int* order,unsigned int nbok)
{
	unsigned int *ptr=&order[nbok],best,bestpos,i=nbok+1;
	double bestw,w;	
	
	best=(*(ptr++));
	bestw=GetCon(infos,infos[best]);
	bestpos=nbok;
	while(i<nb)
	{
		w=GetCon(infos,infos[*ptr]);
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
	return(infos[best]);
}


//-----------------------------------------------------------------------------	
double RGA2D::RConnections::GetDistances(RGeoInfo** infos)
{
	RConnection* c;
	RGeoInfo *f,*t;
	RGeoInfoConnector *cf,*ct;
	double sum=0.0;
	
	for(Start();!End();Next())
	{
		c=(*this)();
		f=infos[c->From->Owner->GetId()];
		t=infos[c->To->Owner->GetId()];		
		if((!t->IsValid())||(!f->IsValid())) continue;
		cf=f->Connectors.GetPtr<unsigned int>(c->From->GetId());
		ct=t->Connectors.GetPtr<unsigned int>(c->To->GetId());		
		sum+=cf->GetPos().ManhattanDist(ct->GetPos())*c->Weight;
	}
	return(sum);
}


//-----------------------------------------------------------------------------	
double RGA2D::RConnections::GetDistances(RGeoInfo** infos,RGeoInfo* info)
{
	RConnection* c;
	RGeoInfo *f,*t;
	RGeoInfoConnector *cf,*ct;
	double sum=0.0;
	
	for(Start();!End();Next())
	{
		c=(*this)();
		f=infos[c->From->Owner->GetId()];
		t=infos[c->To->Owner->GetId()];		
		if((!t->IsValid())||(!f->IsValid())) continue;
		if((t!=info)&&(f!=info)) continue;
		cf=f->Connectors.GetPtr<unsigned int>(c->From->GetId());
		ct=t->Connectors.GetPtr<unsigned int>(c->To->GetId());		
		sum+=cf->GetPos().ManhattanDist(ct->GetPos())*c->Weight;
	}
	return(sum);
}


//-----------------------------------------------------------------------------	
void RGA2D::RConnections::AddConnection(RObj2DConnector* from,RObj2DConnector* to,double weight)
{
	InsertPtr(new RConnection(from,to,weight));
}


//-----------------------------------------------------------------------------	
RGA2D::RConnections::~RConnections(void)
{
}
