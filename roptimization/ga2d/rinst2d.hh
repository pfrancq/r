/*

	R Project Library

	RInst2D.cpp

	Instance for 2D placement GA - Inline Implementation

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
//
// class RThreadData2D<cInst,cChromo>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo>
	RThreadData2D<cInst,cChromo>::RThreadData2D(cInst *owner)
		: RThreadData<cInst,cChromo>(owner), Heuristic(0), Selected(0),
		  Kernel("PlacementCenter",Owner->Problem->GetNbObjs(),2)
{
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo>
	void RThreadData2D<cInst,cChromo>::Init(void)
{
	RThreadData<cInst,cChromo>::Init();

	// Initialize the arrays
	if(Owner->Problem->GetNbObjs())
	{
		Selected=new bool[Owner->Problem->GetNbObjs()];
	}

	// Initialize the heuristic
	RString Heur(Owner->GetHeuristic());
	if(Heur=="Bottom-Left")
		Heuristic=new RPlacementBottomLeft(Owner->Problem->GetNbObjs()+2,Owner->ComputeFreePolygons,Owner->UseFreePolygons,Owner->Random,Owner->HeurDist,Owner->HeurArea,Owner->AllOrientations);
	if(Heur=="Edge")
		Heuristic=new RPlacementEdge(Owner->Problem->GetNbObjs()+2,Owner->ComputeFreePolygons,Owner->UseFreePolygons,Owner->Random,Owner->HeurDist,Owner->HeurArea,Owner->AllOrientations);
	if(Heur=="Center")
		Heuristic=new RPlacementCenter(Owner->Problem->GetNbObjs()+2,Owner->ComputeFreePolygons,Owner->UseFreePolygons,Owner->Random,Owner->HeurDist,Owner->HeurArea,Owner->AllOrientations);

	// Initialize the PROMETHEE Kernel
	Kernel.AddCriterion(new RPromLinearCriterion(RPromCriterion::Maximize,Owner->SelectWeight,"Weight"));
	Kernel.AddCriterion(new RPromLinearCriterion(RPromCriterion::Minimize,Owner->SelectDist,"Distance"));
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo>
	RThreadData2D<cInst,cChromo>::~RThreadData2D(void)
{
	if(Heuristic) delete Heuristic;
	if(Selected) delete[] Selected;
}



//------------------------------------------------------------------------------
//
// class RInst2D<cInst,cChromo,cFit>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RInst2D(size_t popsize,RProblem2D* prob,const RString& h,const RString& name,
				RParamStruct* heurdist,RParamStruct* heurarea,RParamStruct* selectdist,RParamStruct* selectweight,
				RDebug *debug)
			: RInst<cInst,cChromo,cFit,cThreadData>(popsize,name,debug), Problem(prob),
			  bLocalOpti(true), Heuristic(h),
			  ComputeFreePolygons(false), UseFreePolygons(false), AllOrientations(false),
			  HeurDist(heurdist), HeurArea(heurarea), SelectDist(selectdist), SelectWeight(selectweight)
{
	// Verify that the identifiers are continuous starting from 0
	size_t id(0);
	RCursor<RObj2D> Objs(Problem->GetObjs());
	for(Objs.Start();!Objs.End();Objs.Next(),id++)
		if(Objs()->GetId()!=id)
			throw RGAException("RInst2D::RInst2D(size_t,RProblem2D*,const RString&,const R::RString&,R::RDebug*) : Identifiers must be continuous and starting from zero");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::Init(void)
{
	RInst<cInst,cChromo,cFit,cThreadData>::Init();
}
