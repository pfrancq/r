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
// class RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RThreadDataG(cInst *owner) throw(bad_alloc)
		: RGA::RThreadData<cInst,cChromo>(owner), Heuristic(0)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(void) throw(bad_alloc)
{
	RGA::RThreadData<cInst,cChromo>::Init();
	Heuristic=Owner->CreateHeuristic();
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
// class RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RGA::RObjs<cObj>* objs,HeuristicType h,RGA::RDebug *debug) throw(bad_alloc)
		: RGA::RInst<cInst,cChromo,cFit,cThreadData>(popsize,debug),
		  Heuristic(h), Objs(0)
{
	Objs=new RStd::RCursor<cObj,unsigned int>();
	Objs->Set(objs);
	MaxGroups=Objs->GetNb()/2;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RStd::RContainer<cObj,unsigned int,false,true>* objs,HeuristicType h,RGA::RDebug *debug) throw(bad_alloc)
		: RGA::RInst<cInst,cChromo,cFit,cThreadData>(popsize,debug),
		  Heuristic(h), Objs(0)
{
	Objs=new RStd::RCursor<cObj,unsigned int>();
	Objs->Set(objs);
	MaxGroups=Objs->GetNb()/2;
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RStd::RContainer<cObj,unsigned int,true,true>* objs,HeuristicType h,RGA::RDebug *debug) throw(bad_alloc)
		: RGA::RInst<cInst,cChromo,cFit,cThreadData>(popsize,debug),
		  Heuristic(h), Objs(0)
{
	Objs=new RStd::RCursor<cObj,unsigned int>();
	Objs->Set(objs);
	MaxGroups=Objs->GetNb()/2;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(cGroupData* gdata) throw(bad_alloc)
{
	cChromo **C;
	unsigned int i;

	RGA::RInst<cInst,cChromo,cFit,cThreadData>::Init();
	for(i=PopSize+1,C=Chromosomes;--i;C++)
		(static_cast<RGroups<cGroup,cObj,cGroupData,cChromo>*>(*C))->Init(gdata);
	(static_cast<RGroups<cGroup,cObj,cGroupData,cChromo>*>(BestChromosome))->Init(gdata);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::CreateHeuristic(void) throw(bad_alloc)
{
	RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* h;

	switch(Heuristic)
	{
		case FirstFit:
			h = new RFirstFitHeuristic<cGroup,cObj,cGroupData,cChromo>(Random,Objs);
			break;
		default:
			h=0;
			break;
	}
	return(h);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::HandleSameChromosomes(void) throw(RGA::eGA)
{
	cChromo **C,**C1,*p;
	unsigned int i,j;

	for(i=0,C=Chromosomes;i<PopSize-1;C++,i++)
	{
		for(j=i+1,C1=C+1;j<PopSize;C1++,j++)
		{
			if((*C)->SameGroupment(*C1))
			{
				if((*((*C)->Fitness))>(*((*C1)->Fitness)))
					p=(*C1);
				else
					p=(*C);
				if(RRand(100)<90)
				{
					if(!p->Modify())
						throw eGAModify(Gen,p->Id);
					emitInteractSig();
					p->Evaluate();
					emitInteractSig();
					p->ToEval=false;
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RandomConstruct(void) throw(RGA::eGA)
{
	RGA::RInst<cInst,cChromo,cFit,cThreadData>::RandomConstruct();
	HandleSameChromosomes();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Crossover(void) throw(RGA::eGA)
{
	RGA::RInst<cInst,cChromo,cFit,cThreadData>::Crossover();
	HandleSameChromosomes();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Mutation(void) throw(RGA::eGA)
{
	RGA::RInst<cInst,cChromo,cFit,cThreadData>::Mutation();
	HandleSameChromosomes();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Inversion(void) throw(RGA::eGA)
{
	RGA::RInst<cInst,cChromo,cFit,cThreadData>::Inversion();
	HandleSameChromosomes();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RInstG(void)
{
}
