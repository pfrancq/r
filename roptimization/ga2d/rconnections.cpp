/*

	R Project Library

	RConnections.cpp

	Connections for the 2D Placement - Implementation.

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
#include <rstd/random.h>
#include <rga2d/rconnections.h>
#include <rga2d/rgeoinfos.h>
#include <rpromethee/rpromkernel.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RConnections
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RConnections::RConnections(void)
	: RContainer<RConnection,unsigned int,true,false>(50,25), Random(0)
{
}


//------------------------------------------------------------------------------
void RConnections::Init(void)
{
	RConnection **tab;
	RObj2DConnector **con;
	unsigned int i,j;

	// Go through all connections
	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		// Go through each connector
		for(j=(*tab)->Connect.NbPtr+1,con=(*tab)->Connect.Tab;--j;con++)
		{
			(*con)->Connections.InsertPtr(*tab);
		}
	}
}


//------------------------------------------------------------------------------
void RConnections::SetParams(const RPromCriterionParams& dist,const RPromCriterionParams& weight,RRandom* r)
{
	DistParams=dist;
	WeightParams=weight;
	Random=r;
}


//------------------------------------------------------------------------------
double RConnections::GetCon(RGeoInfos* infos,RGeoInfo* cur)
{
	double sum=0.0;
	unsigned int i,j,k;
	RObj2DConnector **con,**con2;
	RConnection **tab;
	bool cont;
	RGeoInfo *tmp;

	// Go through all connectors
	for(i=cur->GetObj()->Connectors.NbPtr+1,con=cur->GetObj()->Connectors.Tab;--i;con++)
	{
		// Go through each connection
		for(j=(*con)->Connections.NbPtr+1,tab=(*con)->Connections.Tab;--j;tab++)
		{
			// Go through each connector and see if the geometric information is placed
			for(k=(*tab)->Connect.NbPtr+1,con2=(*tab)->Connect.Tab,cont=true;(--k)&&cont;con2++)
			{
				tmp=infos->GetPtr<unsigned int>((*con2)->Owner->Id);
				if(tmp)
					cont=!tmp->IsValid();
			}
			if(!cont)
			{
				// At least one valid geometric information found
				sum+=(*tab)->Weight;
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
	RGeoInfo **info,**treat,*b;
	unsigned int i,Nb=0;
	double w,d;
	RRect r1,r2;
	bool bFound;
	RCoord X1,Y1,X2,Y2;
	RPoint Pt;
	bool bProm;

	// Init Part
	weight=Prom.NewCriterion(Maximize,WeightParams);
	dist=Prom.NewCriterion(Minimize,DistParams);
	treat=new RGeoInfo*[nb];

	// Go through each info
	for(i=nb+1,info=infos->Tab;--i;info++)
	{
		// If selected -> go to next
		if(selected[(*info)->GetObj()->GetId()])
			continue;

		// Calculate distance and weight
		w=GetCon(infos,*info);
		d=GetDistances(infos->Tab,*info);
		if((w>0.0)&&(d>0.0))
		{
			// If both are not null -> Create a Prométhée solution
			sol=Prom.NewSol();
			Prom.Assign(sol,weight,w);
			Prom.Assign(sol,dist,d);
			treat[sol->GetId()]=(*info);
			Nb++;        // Increment number of solution
		}
	}

	if(!Nb)
	{
		// Choose a random not selected object
		Nb=nb;
		memcpy(treat,infos,nb*sizeof(RGeoInfo*));
		Random->RandOrder<RGeoInfo*>(treat,nb);
		info=treat;
		while(selected[(*info)->GetObj()->GetId()])
			info++;
		b=(*info);
		bProm=false;
	}
	else
	{
		Prom.ComputePrometheeII();         // Compute Prométhée
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
			if(r1.Pt1.X<r2.Pt1.X) X1=r1.Pt1.X; else X1=r2.Pt1.X;
			if(r1.Pt1.Y<r2.Pt1.Y) Y1=r1.Pt1.Y; else Y1=r2.Pt1.Y;
			if(r1.Pt2.X>r2.Pt2.X) X2=r1.Pt2.X; else X2=r2.Pt2.X;
			if(r1.Pt2.Y>r2.Pt2.Y) Y2=r1.Pt2.Y; else Y2=r2.Pt2.Y;
			if(((X2-X1+1)<bound.Width())&&((Y2-Y1+1)<bound.Height()))
			{
				bFound=true;
//				X2=bound.Width()-(X2-X1);
//				Y2=bound.Height()-(Y2-Y1);
//				Pt.Set(X1-(X2/2),Y1-(Y2/2));
//				bound+=Pt;
				r1.Pt1.Set(X1,Y1);
				r1.Pt2.Set(X2,Y2);
			}
			best++;
		}
		if(bProm)
			delete[] sols;

		if(!bFound)
		{
			// Nothing found -> Central Rectangle
			Pt=b->GetPos();
			Pt.X+=r1.Width()/2-bound.Width()/2;
			Pt.Y+=r1.Height()/2-bound.Height()/2;
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
double RConnections::GetDistances(RGeoInfo** /*infos*/,RGeoInfo* /*info*/)
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
