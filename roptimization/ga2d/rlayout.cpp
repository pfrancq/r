/*

	R Project Library

	RLayout.cpp

	Layout - Implementation.

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
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
#include <rlayout.h>
#include <rpromkernel.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RLayout
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLayout::RLayout(RProblem2D* prob,bool create)
	: RContainer<RGeoInfo,true,false>(prob->GetNbObjs()+3),
	  Problem(prob), Board(prob), PlacedConnections(prob->GetNbConnections()), NbAggregators(0)
{
	if(create)
	{
		// Create the board
		Board.SetConfig(oNormal);
		Board.Assign(RPoint(0,0),0,cNoRef);

		// Create the geometric information and verify that the objects have the
		// "correct" identifiers.
		RCursor<RObj2D> obj(prob->GetObjs());
		size_t id;
		for(obj.Start(),id=0;!obj.End();obj.Next(),id++)
		{
			if(id!=obj()->GetId())
				mThrowRException("The identifiers of the objects must follow [0,"+RString::Number(Problem->GetNbObjs()-1)+"]");
			InsertPtr(new RGeoInfo(obj()));
		}

		// Create the connections
		RCursor<RConnection> Cur(prob->GetConnections());
		for(Cur.Start();!Cur.End();Cur.Next())
			PlacedConnections.InsertPtr(new RGeoInfoConnection(this,Cur()));
	}
	else
		std::cout<<"No creation"<<std::endl;
}


//------------------------------------------------------------------------------
void RLayout::Boundary(RRect& rect)
{
	RRect tmp;
	tCoord X1(0),Y1(0),X2(cMaxCoord),Y2(cMaxCoord);
	RCursor<RGeoInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->Boundary(tmp);
		if(tmp.GetX1()<X1) X1=tmp.GetX1();
		if(tmp.GetY1()<Y1) Y1=tmp.GetY1();
		if(tmp.GetX2()>X2) X2=tmp.GetX2();
		if(tmp.GetY2()>Y2) Y2=tmp.GetY2();
	}

	rect.Set(X1,Y1,X2,Y2);
}


//------------------------------------------------------------------------------
void RLayout::ClearInfos(void)
{
	if(NbAggregators)
	{
		std::cout<<"Not normal"<<std::endl;
		NbAggregators=0;
	}
	RCursor<RGeoInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->ClearInfo();
	RCursor<RGeoInfoConnection> Cur2(PlacedConnections);
	for(Cur2.Start();!Cur2.End();Cur2.Next())
		Cur2()->UnComplete();
}


//------------------------------------------------------------------------------
void RLayout::Copy(const RLayout& infos,RGrid* grid)
{
	// Basic copy
	ClearInfos();
	Problem=infos.Problem;

	// Reconstruct the geometric information
	RCursor<RGeoInfo> info(infos);
	for(info.Start();!info.End();info.Next())
	{
		RGeoInfo* ptr((*this)[info()->GetObj()->GetId()]);
		ptr->SetConfig(info()->GetConfig()->GetOrientation());
		ptr->Assign(info()->GetPos(),grid,info()->GetOrder());
		info()->SetContainer(0);
	}

	// Reconstruct the connections
	PlacedConnections.Clear();
	RCursor<RGeoInfoConnection> Cur(infos.PlacedConnections);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RGeoInfoConnection* Con(new RGeoInfoConnection(this,Cur()->GetConnection()));
		Con->ComputeMinDist(this);
		PlacedConnections.InsertPtr(Con);
	}
}


//------------------------------------------------------------------------------
RGeoInfo* RLayout::operator[](size_t id)
{
	if(id==ProblemId)
		return(&Board);
	return(RContainer<RGeoInfo,true,false>::operator[](id));
}


//------------------------------------------------------------------------------
const RGeoInfo* RLayout::operator[](size_t id) const
{
	if(id==ProblemId)
		return(&Board);
	return(RContainer<RGeoInfo,true,false>::operator[](id));
}


//------------------------------------------------------------------------------
RObj2DContainer* RLayout::GetNewAggregator(void)
{
	if(NbAggregators==2)
		mThrowRException("Maximum two aggregates per RLayout");
	RObj2DContainer* container(new RObj2DContainer(Problem->GetNbObjs()+NbAggregators,this));
	InsertPtrAt(container,Problem->GetNbObjs()+NbAggregators);
	NbAggregators++;
	return(container);
}


//------------------------------------------------------------------------------
void RLayout::DestroyAggregators(void)
{
	for(;NbAggregators;NbAggregators--)
		DeletePtrAt(Problem->GetNbObjs());
}


//------------------------------------------------------------------------------
tCoord RLayout::ComputeDist(RGeoInfo* info,bool weight,const RPoint& pos)
{
	// Initialize
	tCoord sum(0.0);
	RCursor<RGeoInfoConnection> Cur(PlacedConnections);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(weight)
			sum+=Cur()->GetDist(this,info,pos)*Cur()->GetConnection()->GetWeight();
		else
			sum+=Cur()->GetDist(this,info,pos);
	}
	return(sum);
}


//------------------------------------------------------------------------------
double RLayout::ComputeWeight(RGeoInfo* info)
{
	double weight(0.0);
	bool OnePlaced;   // At least one placed

	RGeoInfo *tmp;

	// Go through all connectors
	RCursor<RObj2DConnector> con(info->GetObj()->GetConnectors());
	for(con.Start();!con.End();con.Next())
	{
		// Go through each connection
		RCursor<RConnection> tab(con()->GetConnections());
		for(tab.Start();!tab.End();tab.Next())
		{
			// Go through each connector and see if the geometric information is placed
			RCursor<RObj2DConnector> con2(tab()->GetConnectors());
			for(con2.Start(),OnePlaced=false;(!con2.End())&&(!OnePlaced);con2.Next())
			{
				if(con()==con2()) continue;
				RObj2D* Obj(con2()->GetObj());
				if(!Obj)
				{
					continue;
					OnePlaced=true;
				}
				tmp=(*this)[Obj->GetId()];
				if(tmp&&tmp->IsValid())
					OnePlaced=true;
			}
			if(OnePlaced)
			{
				// At least one valid geometric information found
				weight+=tab()->GetWeight();
			}
		}
	}
	return(weight);
}


//------------------------------------------------------------------------------
void RLayout::GetBestsConnected(RGeoInfo* (&i1),RGeoInfo* (&i2),const RRect& bound,bool* selected,RPromKernel* kernel,RRandom& random,bool weight)
{
	RPromCriterion *cWeight,*cDist;
	RPromSol *sol;
	RGeoInfo **treat;
	size_t Nb=0;
	double w,d;
	RRect r1,r2;
	bool bFound;

	// Init Part
	i1=i2=0;
	kernel->ClearSols();
	cWeight=kernel->GetCriterion("Weight");
	cDist=kernel->GetCriterion("Distance");
	treat=new RGeoInfo*[Problem->GetNbObjs()+1];

	// Go through each info
	RCursor<RGeoInfo> info(*this);
	for(info.Start();!info.End();info.Next())
	{
		// If selected -> go to next
		if(selected[info()->GetObj()->GetId()])
			continue;

		// Calculate distance and weight
		d=ComputeDist(info(),weight,RPoint::Null);
		w=ComputeWeight(info());
		if((w>0.0)&&(d>0.0))
		{
			// If both are not null -> Create a PROMETHEE solution
			sol=kernel->NewSol();
			kernel->Assign(sol,cWeight,w);
			kernel->Assign(sol,cDist,d);
			treat[sol->GetId()]=info();
			Nb++;        // Increment number of solutions
		}
	}


	if(!Nb)
	{
		// Choose a random not selected object
		GetTab(treat);
		random.RandOrder<RGeoInfo*>(treat,Problem->GetNbObjs());
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
			tCoord X1(r2.GetX1()), Y1(r2.GetY1()), X2(r2.GetX2()), Y2(r2.GetY2());
			if(r1.GetX1()<X1) X1=r1.GetX1();
			if(r1.GetY1()<Y1) Y1=r1.GetY1();
			if(r1.GetX2()>X2) X2=r1.GetX2();
			if(r1.GetY2()>Y2) Y2=r1.GetY2();
			r2.Set(X1,Y1,X2,Y2);
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
		kernel->ComputePrometheeII();           // Compute PROMETHEE
		RCursor<RPromSol> Best(kernel->GetSols());                // Get the solutions
		Best.Start();
		i1=treat[Best()->GetId()];        // The first one is the best
		Best.Next();

		// Go through the others and find the best one that can go in bound
		i1->Boundary(r1);                      // Get The boundary rectangle
		bFound=false;
		while((--Nb)&&(!bFound))
		{
			treat[Best()->GetId()]->Boundary(r2);      // Get The boundary rectangle
			tCoord X1(r2.GetX1()), Y1(r2.GetY1()), X2(r2.GetX2()), Y2(r2.GetY2());
			if(r1.GetX1()<X1) X1=r1.GetX1();
			if(r1.GetY1()<Y1) Y1=r1.GetY1();
			if(r1.GetX2()>X2) X2=r1.GetX2();
			if(r1.GetY2()>Y2) Y2=r1.GetY2();
			r2.Set(X1,Y1,X2,Y2);
			if((r2.GetWidth()<=bound.GetWidth())&&(r2.GetHeight()<=bound.GetHeight()))
			{
					bFound=true;
					i2=treat[Best()->GetId()];
					Best.Next();
			}
			Best.Next();
		}
	}

	// Clear
	delete[] treat;
}


//------------------------------------------------------------------------------
RGeoInfo* RLayout::GetMostConnected(RGeoInfo** order,size_t nbobjs,size_t nbok,bool weight)
{
	RGeoInfo **ptr=&order[nbok],*best;
	size_t bestpos,i=nbok+1;
	double bestw,w;

	best=(*(ptr++));
	bestw=ComputeWeight(best);
	bestpos=nbok;
	while(i<nbobjs)
	{
		w=ComputeWeight(*ptr);
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
void RLayout::FillAggregator(RObj2DContainer* cont,bool* selected,RPromKernel* kernel,RRandom& random,bool weight)
{
	RRect bound,r;
	RGeoInfo* i1;
	RGeoInfo* i2;

	// Clear the aggregator and attach it to the layout
	cont->ClearInfo();

	// Define the objects
	Boundary(bound);
	bound-=bound.GetPt1();
	bound.Set(bound.GetX1(),bound.GetY1(),bound.GetX2()/4,bound.GetY2()/4);
	GetBestsConnected(i1,i2,bound,selected,kernel,random,weight);
	if(!i1)
	{
		cont->Complete();
		return;
	}
	selected[i1->GetObj()->GetId()]=true;
	cont->Add(i1);
	if(!i2)
	{
		cont->Complete();
		return;
	}
	selected[i2->GetObj()->GetId()]=true;
	cont->Add(i2);
	i1->Boundary(bound);
	i2->Boundary(r);
	tCoord X1(bound.GetX1()), Y1(bound.GetY1()), X2(bound.GetX2()), Y2(bound.GetY2());
	if(r.GetX1()<X1) X1=r.GetX1();
	if(r.GetY1()<Y1) Y1=r.GetY1();
	if(r.GetX2()>X2) X2=r.GetX2();
	if(r.GetY2()>Y2) Y2=r.GetY2();
	bound.Set(X1,Y1,X2,Y2);
	RCursor<RGeoInfo> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if((!selected[Cur()->GetObj()->GetId()])&&Cur()->GetPlacedPolygon().Contained(bound))
		{
			// Add it
			cont->Add(Cur());
			selected[Cur()->GetObj()->GetId()]=true;
		}
	}
	cont->Complete();
}


//------------------------------------------------------------------------------
tCoord RLayout::ComputeConnections(bool weight)
{
	tCoord Distances(0.0);
	RCursor<RGeoInfoConnection> Cur(PlacedConnections);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->UnComplete();
		if(weight)
			Distances+=Cur()->ComputeMinDist(this)*Cur()->GetConnection()->GetWeight();
		else
			Distances+=Cur()->ComputeMinDist(this);
	}
	return(Distances);
}


//------------------------------------------------------------------------------
RRect RLayout::ComputeBoundary(void)
{
	RPolygons Sol;
	RCursor<RGeoInfo> info(GetInfos());
	for(info.Start();!info.End();info.Next())
		Sol.InsertPtr(new RPolygon(info()->GetPlacedPolygon()));
	RPolygon Union;
	Sol.Union(Union);
	RRect Result;
	Union.ExternalBoundary(Result);
	return(Result);
}


//------------------------------------------------------------------------------
RLayout::~RLayout(void)
{
}
