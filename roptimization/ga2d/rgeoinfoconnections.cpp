/*

	R Project Library

	RGeoInfoConnections.cpp

	Connections for the 2D Placement (Geometric Information Part) - Implementation.

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
#include <rpromethee/rpromkernel.h>
using namespace RPromethee;
#include <rga2d/rgeoinfoconnections.h>
#include <rga2d/rgeoinfos.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
//
// class RGeoInfoConnections
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RGeoInfoConnections::RGeoInfoConnections(RConnections* c,RGeoInfos* i)
	: RContainer<RGeoInfoConnection,unsigned int,true,false>(c->NbPtr,c->IncPtr),
	  Cons(c), Infos(i)
{
	for(Cons->Start();!Cons->End();Cons->Next())
	{
		InsertPtr(new RGeoInfoConnection((*Cons)()));
	}
}


//-----------------------------------------------------------------------------
double RGA2D::RGeoInfoConnections::GetDistances(RGeoInfo* info,const RPoint& pos)
{
	double sum=0.0;
	unsigned int i;
	RGeoInfoConnection** cur;

	for(i=NbPtr+1,cur=Tab;--i;cur++)
	{
		sum+=(*cur)->GetDist(Infos,info,pos);
	}
	return(sum);
}


//-----------------------------------------------------------------------------
double RGA2D::RGeoInfoConnections::GetDistances(RGeoInfo* info)
{
	double sum=0.0;
	unsigned int i;
	RGeoInfoConnection** cur;

	for(i=NbPtr+1,cur=Tab;--i;cur++)
	{
		sum+=(*cur)->GetDist(Infos,info);
	}
	return(sum);
}


//-----------------------------------------------------------------------------
void RGA2D::RGeoInfoConnections::GetBestsConnected(RGeoInfo* (&i1),RGeoInfo* (&i2),const RRect& bound,const bool* selected)
{
	RPromKernel Prom("PlacementCenter",Infos->NbPtr,2);
	RPromCriterion *weight,*dist;
	RPromSol *sol,**sols,**best;
	RGeoInfo **info,**treat;
	unsigned int i,Nb=0;
	double w,d;
	RRect r1,r2;
	bool bFound;

	// Init Part
	i1=i2=0;
	weight=Prom.NewCriterion(Maximize,Cons->WeightParams);
	dist=Prom.NewCriterion(Minimize,Cons->DistParams);
	treat=new RGeoInfo*[Infos->NbPtr];

	// Go through each info
	for(i=Infos->RealNb+1,info=Infos->Tab;--i;info++)
	{
		// If selected -> go to next
		if(selected[(*info)->GetObj()->GetId()])
			continue;

		// Calculate distance and weight
		w=Cons->GetCon(Infos,*info);
		d=GetDistances(*info);
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
		Nb=Infos->RealNb;
		memcpy(treat,Infos->Tab,Nb*sizeof(RGeoInfo*));
		Cons->Random->RandOrder<RGeoInfo*>(treat,Nb);
		info=treat;
		while(selected[(*info)->GetObj()->GetId()])
			info++;
		i1=(*(info++));

		// Go through the others and find the best one that can go in bound
		i1->Boundary(r1);                   // Get The boundary rectangle
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			(*info)->Boundary(r2);
			if(r1.Pt1.X<r2.Pt1.X) r2.Pt1.X=r1.Pt1.X;
			if(r1.Pt1.Y<r2.Pt1.Y) r2.Pt1.Y=r1.Pt1.Y;
			if(r1.Pt2.X>r2.Pt2.X) r2.Pt2.X=r1.Pt2.X;
			if(r1.Pt2.Y>r2.Pt2.Y) r2.Pt2.Y=r1.Pt2.Y;
			if((r2.Width()<=bound.Width())&&(r2.Height()<=bound.Height())&&(!selected[(*info)->GetObj()->GetId()]))
			{
					bFound=true;
					i2=(*info);
			}
			info++;
		}
	}
	else
	{
		Prom.ComputePrometheeII();           // Compute Prométhée
		best=sols=Prom.GetSols();            // Get the solutions
		i1=treat[(*(best++))->GetId()];      // The first one is the best

		// Go through the others and find the best one that can go in bound
		i1->Boundary(r1);                   // Get The boundary rectangle
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			treat[(*best)->GetId()]->Boundary(r2);      // Get The boundary rectangle
			if(r1.Pt1.X<r2.Pt1.X) r2.Pt1.X=r1.Pt1.X;
			if(r1.Pt1.Y<r2.Pt1.Y) r2.Pt1.Y=r1.Pt1.Y;
			if(r1.Pt2.X>r2.Pt2.X) r2.Pt2.X=r1.Pt2.X;
			if(r1.Pt2.Y>r2.Pt2.Y) r2.Pt2.Y=r1.Pt2.Y;
			if((r2.Width()<=bound.Width())&&(r2.Height()<=bound.Height()))
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


//-----------------------------------------------------------------------------
RGeoInfoConnections& RGA2D::RGeoInfoConnections::operator=(const RGeoInfoConnections& cons) throw(bad_alloc)
{
	RContainer<RGeoInfoConnection,unsigned int,true,false>::operator=(cons);
	Cons=cons.Cons;
	Infos=cons.Infos;
	return(*this);
}


//-----------------------------------------------------------------------------
void RGA2D::RGeoInfoConnections::UnComplete(void)
{
	unsigned int i;
	RGeoInfoConnection** cur;

	for(i=NbPtr+1,cur=Tab;--i;cur++)
	{
		(*cur)->UnComplete();
	}
}
