/*

	RInsth.hh

	Class representing an instance of a HGA - Implementation

	(C) 2001 by P. Francq.

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
// RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RThreadDataG(cInst *owner) throw(bad_alloc)
		: RThreadData<cInst,cChromo>(owner), Heuristic(0)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(void) throw(bad_alloc)
{
	RThreadData<cInst,cChromo>::Init();
	switch(Owner->GetHeuristic())
	{
		case FirstFit:
			Heuristic = new RFirstFitHeuristic<cGroup,cObj,cGroupData>(Owner->Random,Owner->Objs);
			break;
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RThreadDataG(void)
{
	if(Heuristic)
		delete Heuristic;
}



//-----------------------------------------------------------------------------
//
// RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RGA::RObjs<cObj>* objs,HeuristicType h,RDebug *debug) throw(bad_alloc)
		: RInst<cInst,cChromo,cFit,cThreadData>(popsize,debug),
		  Heuristic(h), Objs(objs)
{
	MaxGroups=Objs->NbPtr/2;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(cGroupData* gdata) throw(bad_alloc)
{
	cChromo **C;
	unsigned int i;

	RInst<cInst,cChromo,cFit,cThreadData>::Init();
	for(i=PopSize+1,C=Chromosomes;--i;C++)
		(static_cast<RGroups<cGroup,cObj,cGroupData>*>(*C))->Init(gdata);
	(static_cast<RGroups<cGroup,cObj,cGroupData>*>(BestChromosome))->Init(gdata);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RInstG(void)
{
}
