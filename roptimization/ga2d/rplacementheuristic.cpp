/*

	Rainbow Library Project

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Implemenation

	(C) 1998-2000 by By P. Francq.

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rpromethee/rpromkernel.h>
using namespace RPromethee;
#include <rga/rplacementheuristic.h>
using namespace RGA;



//-----------------------------------------------------------------------------
//
//	RPlacementHeuristic
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPlacementHeuristic::RPlacementHeuristic(unsigned int maxobjs,bool calc,bool use,bool ori)
	: Free(), CalcFree(calc), UseFree(calc&&use), AllOri(ori)
{
	Order=new unsigned int[maxobjs];
}


//-----------------------------------------------------------------------------
void RPlacementHeuristic::Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs)
{
	// Assign
	Limits=limits;
	Grid=grid;
	Objs=objs;
	Infos=infos;
	NbObjs=nbobjs;

	// Init the data for a placement
	NbObjsOk=0;
	Grid->Clear();	
	Free.Clear();
	Result.Pt1.Set(0,0);
	Result.Pt2.Set(0,0);

	// Calculate an order
	for(unsigned int i=0;i<NbObjs;i++)
		Order[i]=i;
}


//----------------------------------------------------------------------------
void RPlacementHeuristic::SetDistParams(double p,double q,double w)
{
	DistP=p;
	DistQ=q;
	DistWeight=w;
}

	
//----------------------------------------------------------------------------
void RPlacementHeuristic::SetAreaParams(double p,double q,double w)
{
	AreaP=p;
	AreaQ=q;
	AreaWeight=w;
}


//-----------------------------------------------------------------------------
double RPlacementHeuristic::CalcDist(RPoint& pos)
{
	RPoint Center(CurInfo->Width()/2,CurInfo->Height()/2);
	static RPoint Attraction;

	if(!NbObjsOk)
	{
		return(0.0);
	}
	if(NbObjsOk==1)
	{
		RGeoInfo *f=Infos[Order[0]];
		Attraction=f->GetPos();
		Attraction.X+=f->Width()/2;
		Attraction.Y+=f->Height()/2;
	}
	return(Attraction.ManhattanDist(pos+Center));
}


//-----------------------------------------------------------------------------
RGeoInfo* RPlacementHeuristic::NextObject(void)
{
	RPoint pos;
	char nb;
	RObj2D* obj;	
	
	// Select the next object
	CurInfo=Infos[Order[NbObjsOk]];
	obj=CurInfo->GetObj();		
	
	// Choose best free polygon (if any).
	pos.Set(MaxCoord,MaxCoord);
	if(UseFree)
		pos=Free.CanPlace(CurInfo);

  	// Find the best position for current orientation if not already found.	
	if(!pos.IsValid())		
	{
   	if(AllOri&&(obj->NbPossOri>1))
   	{
   		RPromKernel Prom("Orientations",8,2);
   		RPromCriterion *area,*dist;
   		RPromSol *sol;
   		char o;

   		// Init Part
   		area=Prom.NewCriterion(Minimize,AreaP,AreaQ,AreaWeight);
   		dist=Prom.NewCriterion(Minimize,DistP,DistQ,DistWeight);		
			nb=0;
			   		
   		// Compute all orientations
   		for(o=0;o<obj->NbPossOri;o++)
   		{
	   		// Set an orientation & Find a position
   			CurInfo->SetOri(o);		
  				pos=NextObjectOri();	
   			if(pos.IsValid())
   			{
   				Ori[nb]=o;
   				OriPos[nb]=pos;
   				OriResult[nb++]=Result;
   				sol=Prom.NewSol();
   				Prom.Assign(sol,area,Result.Width()*Result.Height());
   				Prom.Assign(sol,dist,CalcDist(pos));
   			}
   		}
   		
   		// Run Prométhée
  			Prom.ComputePrometheeII();
  			nb=Prom.GetBestSolId();
			pos=OriPos[nb];	
			Result=OriResult[nb];
			CurInfo->SetOri(Ori[nb]);
   	}
   	else
   	{
   		// Set an orientation & Find a position
   		CurInfo->SetOri(0);
  			pos=NextObjectOri();
  			if(AllOri&&(NbObjsOk==1))	// Need to calc Attraction in CalcDist as static
  				(void)CalcDist(pos);		// Will change if connections
   	}
	}
			
	// Place it	
	if(!pos.IsValid())
		throw;
	Place(pos);		

	// Look for free polygons
	if(CalcFree)
		Grid->AddFreePolygons(CurInfo,&Free,Result);

	// Next Object
	NbObjsOk++;
	return(CurInfo);
}


//-----------------------------------------------------------------------------
void RPlacementHeuristic::Run(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs)
{
	Init(limits,grid,objs,infos,nbobjs);
	while(NbObjsOk<NbObjs)
		NextObject();
	PostRun(limits);
}


//-----------------------------------------------------------------------------
RRect& RPlacementHeuristic::GetResult(void)
{
	RRect *rect=RRect::GetRect();
	
	(*rect)=Result;
	return(*rect);
}


//-----------------------------------------------------------------------------
RPlacementHeuristic::~RPlacementHeuristic(void)
{
	if(Order) delete[] Order;
}
