/*

	R Project Library

	RInst2D.cpp

	Instance for 2D placement GA - Inline Implementation

	 (C) 1999-2001 by P. Francq.

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
//
// RThreadData2D<cInst,cChromo>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo>
	RGA2D::RThreadData2D<cInst,cChromo>::RThreadData2D(cInst *owner) throw(bad_alloc)
		: RThreadData<cInst,cChromo>(owner),NbObjs(0),Order(0),tmpObj1(0),
			tmpObj2(0), tmpInfos(0), Heuristic(0)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo>
	void RGA2D::RThreadData2D<cInst,cChromo>::Init(void) throw(bad_alloc)
{
	RThreadData<cInst,cChromo>::Init();
	NbObjs=Owner->NbObjs;
	if(NbObjs)
	{
		Order=new unsigned int[NbObjs];
		Order2=new unsigned int[NbObjs];
		tmpObjs=new RObj2D*[NbObjs];
		tmpObj1=new RObj2DContainer(NbObjs+1,"Temporary Object 1",NbObjs);
		tmpObj2=new RObj2DContainer(NbObjs+2,"Temporary Object 2",NbObjs);
		tmpInfos=new RGeoInfos(Owner->Problem,true);
	}
	switch(Owner->GetHeuristic())
	{
		case BottomLeft:
			Heuristic = new RPlacementBottomLeft(NbObjs+2,true,true,Owner->Random,true);
			break;
		case Edge:
			Heuristic = new RPlacementEdge(NbObjs+2,true,true,Owner->Random,true);
			break;
		case Center:
			Heuristic = new RPlacementCenter(NbObjs+2,true,true,Owner->Random,true);
			break;
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo>
	RGA2D::RThreadData2D<cInst,cChromo>::~RThreadData2D(void)
{
	if(Order) delete[] Order;
	if(tmpObjs)	delete[] tmpObjs;
	if(tmpObj1) delete tmpObj1;
	if(tmpObj2) delete tmpObj2;
	if(tmpInfos) delete tmpInfos;
	if(Heuristic) delete Heuristic;
}



//-----------------------------------------------------------------------------
//
// RInst2D<cInst,cChromo,cFit>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RGA2D::RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::
		RInst2D(unsigned int popsize,RProblem2D* prob,HeuristicType h,RDebug *debug) throw(bad_alloc)
			: RInst<cInst,cChromo,cFit,cThreadData>(popsize,debug), Problem(prob), Objs(prob->Objs.Tab),
			 NbObjs(prob->Objs.NbPtr), bLocalOpti(true), Heuristic(h), Limits(prob->Limits)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RGA2D::RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::Init(void) throw(bad_alloc)
{
	RInst<cInst,cChromo,cFit,cThreadData>::Init();
	BestChromosome->Objs=Objs;
	BestChromosome->NbObjs=NbObjs;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	RPoint& RGA2D::RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::GetLimits(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Limits;
	return(*pt);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RGA2D::RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::SetAreaParams(const RPromCriterionParams& params)
{
	thDatas[0]->Heuristic->SetAreaParams(params);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	void RGA2D::RInst2D<cInst,cChromo,cFit,cThreadData,cInfo>::SetDistParams(const RPromCriterionParams& params)
{
	thDatas[0]->Heuristic->SetDistParams(params);
}
