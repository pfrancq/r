/*

	RInsth.hh

	Class representing an instance of a HGA - Implementation

	Copyright 2001-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// class RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RThreadDataG(cInst *owner)
		: RThreadData<cInst,cChromo>(owner), Heuristic(0)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(void)
{
	RThreadData<cInst,cChromo>::Init();
	Heuristic=this->Owner->CreateHeuristic();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RThreadDataG(void)
{
	if(Heuristic)
		delete Heuristic;
}



//------------------------------------------------------------------------------
//
// class RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RObjs<cObj>* objs,HeuristicType h,const RString& name,RDebug *debug)
		: RInst<cInst,cChromo,cFit,cThreadData>(popsize,name,debug),
		  Heuristic(h), EmptyModifiedGroups(true), Objs(0)
{
	Objs=new RCursor<cObj>();
	Objs->Set(*objs);
	if(Objs->GetNb()<11)
		MaxGroups=10;
	else
		MaxGroups=Objs->GetNb()/4;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RContainer<cObj,false,true>* objs,HeuristicType h,const RString& name,RDebug *debug)
		: RInst<cInst,cChromo,cFit,cThreadData>(popsize,name,debug),
		  Heuristic(h), EmptyModifiedGroups(true), Objs(0)
{
	Objs=new RCursor<cObj>();
	Objs->Set(*objs);
	if(Objs->GetNb()<11)
		MaxGroups=10;
	else
		MaxGroups=Objs->GetNb()/4;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RInstG(unsigned int popsize,RContainer<cObj,true,true>* objs,HeuristicType h,const RString& name,RDebug *debug)
		: RInst<cInst,cChromo,cFit,cThreadData>(popsize,name,debug),
		  Heuristic(h), EmptyModifiedGroups(true), Objs(0)
{
	Objs=new RCursor<cObj>();
	Objs->Set(objs);
	if(Objs->GetNb()<11)
		MaxGroups=10;
	else
		MaxGroups=Objs->GetNb()/4;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Init(cGroupData* gdata)
{
	cChromo **C;
	unsigned int i;

	RInst<cInst,cChromo,cFit,cThreadData>::Init();
	for(i=this->PopSize+1,C=this->Chromosomes;--i;C++)
		(static_cast<RGroups<cGroup,cObj,cGroupData,cChromo>*>(*C))->Init(gdata);
	(static_cast<RGroups<cGroup,cObj,cGroupData,cChromo>*>(this->BestChromosome))->Init(gdata);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::CreateHeuristic(void)
{
	RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* h;

	switch(Heuristic)
	{
		case FirstFit:
			h = new RFirstFitHeuristic<cGroup,cObj,cGroupData,cChromo>(this->Random,Objs,this->Debug);
			break;
		default:
			h=0;
			break;
	}
	return(h);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::HandleSameChromosomes(void)
{
	cChromo **C,**C1,*p;
	unsigned int i,j;

	for(i=0,C=this->Chromosomes;i<this->PopSize-1;C++,i++)
	{
		for(j=i+1,C1=C+1;j<this->PopSize;C1++,j++)
		{
			if((*C)->SameGroupment(*C1))
			{
				if((*((*C)->Fitness))>(*((*C1)->Fitness)))
					p=(*C1);
				else
					p=(*C);
				if(this->RRand(100)<90)
				{
					p->Modify();
					this->emitInteractSig();
					p->Evaluate();
					this->emitInteractSig();
					p->ToEval=false;
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::RandomConstruct(void)
{
	RInst<cInst,cChromo,cFit,cThreadData>::RandomConstruct();
	HandleSameChromosomes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Crossover(void)
{
	RInst<cInst,cChromo,cFit,cThreadData>::Crossover();
	HandleSameChromosomes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Mutation(void)
{
	RInst<cInst,cChromo,cFit,cThreadData>::Mutation();
	HandleSameChromosomes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	void RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::Inversion(void)
{
	RInst<cInst,cChromo,cFit,cThreadData>::Inversion();
	HandleSameChromosomes();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>::~RInstG(void)
{
	if(Objs)
		delete Objs;
}
