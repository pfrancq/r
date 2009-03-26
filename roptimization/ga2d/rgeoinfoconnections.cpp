/*

	R Project Library

	RGeoInfoConnections.cpp

	Connections for the 2D Placement (Geometric Information Part) - Implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
#include <rpromkernel.h>
#include <rgeoinfoconnections.h>
#include <rgeoinfos.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RGeoInfoConnections
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGeoInfoConnections::RGeoInfoConnections(RConnections* c,RGeoInfos* i)
	: RContainer<RGeoInfoConnection,true,false>(c->GetNb(),c->GetIncNb()),
	  Cons(c), Infos(i)
{
	RCursor<RConnection> Cur(*Cons);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		InsertPtr(new RGeoInfoConnection(Cur()));
	}
}


//------------------------------------------------------------------------------
double RGeoInfoConnections::GetDistances(RGeoInfo* info,const RPoint& pos)
{
	double sum=0.0;

	RCursor<RGeoInfoConnection> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		sum+=Cur()->GetDist(Infos,info,pos);
	return(sum);
}


//------------------------------------------------------------------------------
double RGeoInfoConnections::GetDistances(RGeoInfo* info)
{
	double sum=0.0;

	RCursor<RGeoInfoConnection> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		sum+=Cur()->GetDist(Infos,info);
	return(sum);
}


//------------------------------------------------------------------------------
void RGeoInfoConnections::GetBestsConnected(RGeoInfo* (&i1),RGeoInfo* (&i2),const RRect& bound,const bool* selected)
{
	RPromKernel Prom("PlacementCenter",Infos->GetNb(),2);
	RPromCriterion *weight,*dist;
	RPromSol *sol,**sols,**best;
	RGeoInfo **treat;
	size_t Nb=0;
	double w,d;
	RRect r1,r2;
	bool bFound;

	// Init Part
	i1=i2=0;
	Prom.AddCriterion(weight=new RPromLinearCriterion(RPromCriterion::Maximize,Cons->WeightParams,"Weight"));
	Prom.AddCriterion(dist=new RPromLinearCriterion(RPromCriterion::Minimize,Cons->DistParams,"Distance"));
	treat=new RGeoInfo*[Infos->GetMaxPos()+1];

	// Go through each info
	RCursor<RGeoInfo> info(*Infos);
	for(info.Start();!info.End();info.Next())
	{
		// If selected -> go to next
		if(selected[info()->GetObj()->GetId()])
			continue;

		// Calculate distance and weight
		w=Cons->GetCon(Infos,info());
		d=GetDistances(info());
		if((w>0.0)&&(d>0.0))
		{
			// If both are not null -> Create a PROMETHEE solution
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
		Nb=Infos->RealNb;
		Infos->GetTab(treat);
		Cons->Random->RandOrder<RGeoInfo*>(treat,Nb);
		RGeoInfo** info=treat;
		while(selected[(*info)->GetObj()->GetId()])
			info++;
		i1=(*(info++));

		// Go through the others and find the best one that can go in bound
		i1->Boundary(r1);                   // Get The boundary rectangle
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			(*info)->Boundary(r2);
			if(r1.X1<r2.X1) r2.X1=r1.X1;
			if(r1.Y1<r2.Y1) r2.Y1=r1.Y1;
			if(r1.X2>r2.X2) r2.X2=r1.X2;
			if(r1.Y2>r2.Y2) r2.Y2=r1.Y2;
			if((r2.GetWidth()<=bound.GetWidth())&&(r2.GetHeight()<=bound.GetHeight())&&(!selected[(*info)->GetObj()->GetId()]))
			{
					bFound=true;
					i2=(*info);
			}
			info++;
		}
	}
	else
	{
		Prom.ComputePrometheeII();           // Compute Prom�h�
		best=sols=Prom.GetSols();            // Get the solutions
		i1=treat[(*(best++))->GetId()];      // The first one is the best

		// Go through the others and find the best one that can go in bound
		i1->Boundary(r1);                   // Get The boundary rectangle
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			treat[(*best)->GetId()]->Boundary(r2);      // Get The boundary rectangle
			if(r1.X1<r2.X1) r2.X1=r1.X1;
			if(r1.Y1<r2.Y1) r2.Y1=r1.Y1;
			if(r1.X2>r2.X2) r2.X2=r1.X2;
			if(r1.Y2>r2.Y2) r2.Y2=r1.Y2;
			if((r2.GetWidth()<=bound.GetWidth())&&(r2.GetHeight()<=bound.GetHeight()))
			{
					bFound=true;
					i2=treat[(*(best++))->GetId()];
			}
			best++;
		}
		delete[] sols;
	}

	// Clear
	delete[] treat;
}


//------------------------------------------------------------------------------
RGeoInfoConnections& RGeoInfoConnections::operator=(const RGeoInfoConnections& cons)
{
	RContainer<RGeoInfoConnection,true,false>::operator=(cons);
	Cons=cons.Cons;
	Infos=cons.Infos;
	return(*this);
}


//------------------------------------------------------------------------------
void RGeoInfoConnections::UnComplete(void)
{
	RCursor<RGeoInfoConnection> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->UnComplete();
}
