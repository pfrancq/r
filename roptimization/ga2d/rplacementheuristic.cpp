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



//---------------------------------------------------------------------------
// include files for Rainbow
#include "rplacementheuristic.h"
using namespace RGA;



//---------------------------------------------------------------------------
//
//	RPlacementHeuristic
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPlacementHeuristic::RPlacementHeuristic(unsigned int maxobjs)
{
	Order=new unsigned int[maxobjs];
}


//---------------------------------------------------------------------------
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

	// Calculate an order
	for(unsigned int i=0;i<NbObjs;i++)
		Order[i]=i;
}


//---------------------------------------------------------------------------
RGeoInfo* RPlacementHeuristic::NextObject(void)
{
	// Select the next object
	CurInfo=Infos[Order[NbObjsOk]];

	// Set an orientation
	CurInfo->SetOri(0);

	// Place it
	NextObjectOri();

	// Next Object
	NbObjsOk++;
	return(CurInfo);
}


//---------------------------------------------------------------------------
void RPlacementHeuristic::Run(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs)
{
	Init(limits,grid,objs,infos,nbobjs);
	while(NbObjsOk<NbObjs)
		NextObject();
	PostRun(limits);
}


//---------------------------------------------------------------------------
RRect& RPlacementHeuristic::GetResult(void)
{
	RRect *rect=RRect::GetRect();
	
	(*rect)=Result;
	return(*rect);
}


//---------------------------------------------------------------------------
RPlacementHeuristic::~RPlacementHeuristic(void)
{
	if(Order) delete[] Order;
}