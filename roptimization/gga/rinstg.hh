/*

	R Project Library

	RInstG.hh

	Class representing an instance of a GGA - Implementation

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RThreadDataG(cInst *owner)
		: R::RThreadData<cInst,cChromo>(owner), Heuristic(0)
{
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Init(void)
{
	R::RThreadData<cInst,cChromo>::Init();
	Heuristic=Owner->CreateHeuristic();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RThreadDataG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RThreadDataG(void)
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
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RInstG(size_t popsize,R::RCursor<cObj> objs,const RString& h,const R::RString& name,R::RDebug* debug)
		: R::RInst<cInst,cChromo,cFit,cThreadData>(popsize,name,debug),
		  Heuristic(h), EmptyModifiedGroups(true), Objs(objs), DoLocalOptimisation(true)
{
	// Verify that the identifiers are continuous starting from 0
	size_t id(0);
	for(Objs.Start();!Objs.End();Objs.Next(),id++)
		if(Objs()->GetId()!=id)
			throw RGAException("RInstG::RInstG(size_t,R::RCursor<cObj>,const RString&,const R::RString&,R::RDebug*) : Identifiers must be continuous and starting from zero");

	if(Objs.GetNb()<11)
		MaxGroups=10;
	else
		MaxGroups=Objs.GetNb()/4;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Init(void)
{
	cChromo **C;
	size_t i;

	R::RInst<cInst,cChromo,cFit,cThreadData>::Init();
	for(i=GetPopSize()+1,C=Chromosomes;--i;C++)
		(static_cast<R::RGroups<cGroup,cObj,cChromo>*>(*C))->Init();
	(static_cast<R::RGroups<cGroup,cObj,cChromo>*>(BestChromosome))->Init();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::SetLocalOptimisation(bool localopti)
{
	DoLocalOptimisation=localopti;
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RGroupingHeuristic<cGroup,cObj,cChromo>* R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::CreateHeuristic(void)
{
	R::RGroupingHeuristic<cGroup,cObj,cChromo>* h(0);

	if(Heuristic=="FirstFit")
		h = new R::RFirstFitHeuristic<cGroup,cObj,cChromo>(Random,Objs,Debug);
	return(h);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::AnalysePop(void)
{
	cChromo **C,**C1,*p;
	size_t i,j;

	if(Debug)
		Debug->BeginFunc("AnalysePop","RInstG");

	// Call the default AnalysePop
	R::RInst<cInst,cChromo,cFit,cThreadData>::AnalysePop();

	// Look if same chromosomes and modify population to have only one copy of each
	for(i=0,C=Chromosomes;i<GetPopSize()-1;C++,i++)
	{
		for(j=i+1,C1=C+1;j<GetPopSize();C1++,j++)
		{
			if((*C)->SameGroupment(*C1))
			{
				if((*((*C)->Fitness))>(*((*C1)->Fitness)))
					p=(*C1);
				else
					p=(*C);
				if(RRand(100)<90)
				{
					p->Modify();
					PostNotification("RInst::Interact");
					p->Evaluate();
					PostNotification("RInst::Interact");
					p->ToEval=false;
				}
			}
		}
	}

	if(Debug)
		Debug->EndFunc("AnalysePop","RInstG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::RandomConstruct(void)
{
	if(Debug)
		Debug->BeginFunc("RandomConstruct","RInstG");

	R::RInst<cInst,cChromo,cFit,cThreadData>::RandomConstruct();

	if(Debug)
		Debug->EndFunc("RandomConstruct","RInstG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Crossover(void)
{
	if(Debug)
		Debug->BeginFunc("Crossover","RInstG");

	R::RInst<cInst,cChromo,cFit,cThreadData>::Crossover();

	if(Debug)
		Debug->EndFunc("Crossover","RInstG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Mutation(void)
{
	if(Debug)
		Debug->BeginFunc("Mutation","RInstG");

	R::RInst<cInst,cChromo,cFit,cThreadData>::Mutation();

	if(Debug)
		Debug->EndFunc("Mutation","RInstG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	void R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::Inversion(void)
{
	if(Debug)
		Debug->BeginFunc("Inversion","RInstG");

	R::RInst<cInst,cChromo,cFit,cThreadData>::Inversion();

	if(Debug)
		Debug->EndFunc("Inversion","RInstG");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	R::RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>::~RInstG(void)
{
}
