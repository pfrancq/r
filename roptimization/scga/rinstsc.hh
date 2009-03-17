/*

	R Project Library

	RInstSC.h

	Similarity-based Clustering Instance - Implementation

	Copyright 2002-2009 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
//
// RThreadDataSC<cInst,cChromo,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RThreadDataSC<cInst,cChromo,cThreadData,cGroup,cObj>::RThreadDataSC(cInst* owner)
		: RThreadDataG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>(owner),
	  ToDel(owner->Objs.GetNb()<4?4:owner->Objs.GetNb()/4), tmpObjs1(0),tmpObjs2(0), Tests(0),
	  Prom(Owner->Params), Sols(0), NbSols((Owner->Params->NbDivChromo*2)+2)
{
	RPromSol** s;
	size_t i;

	Tests=new cChromo*[NbSols];
	Sols=new RPromSol*[NbSols];
	for(i=NbSols+1,s=Sols;--i;s++)
		(*s)=Prom.NewSol();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RThreadDataSC<cInst,cChromo,cThreadData,cGroup,cObj>::Init(void)
{
	size_t i;

	RThreadDataG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Init();
	tmpObjs1=new cObj*[Owner->Objs.GetNb()];
	tmpObjs2=new cObj*[Owner->Objs.GetNb()];
	for(i=0;i<NbSols;i++)
	{
		Tests[i]=new cChromo(Owner,Owner->GetPopSize()+1+i);
		Tests[i]->Init(static_cast<cThreadData*>(this));
		(static_cast<RGroups<cGroup,cObj,cChromo>*>(Tests[i]))->Init();
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RThreadDataSC<cInst,cChromo,cThreadData,cGroup,cObj>::~RThreadDataSC(void)
{
	cChromo** C;
	size_t i;

	if(Tests)
	{
		for(i=NbSols+1,C=Tests;--i;C++)
			delete (*C);
		delete[] Tests;
	}
	delete[] Sols;
	delete[] tmpObjs1;
	delete[] tmpObjs2;
}



//-----------------------------------------------------------------------------
//
// RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::RInstSC(RCursor<cObj> objs,RParamsSC* p,RDebug *debug,bool inc)
		: RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>(p->PopSize,objs,"SCFirstFit","SCGA",debug),
		RPromKernelSC<cChromo>(p), Params(p), Sols(0), NoSocialObjs(objs.GetNb()),
		Incremental(inc)
#if BESTSOLSVERIFICATION
	  , BestSols(p->MaxGen,p->MaxGen/2)
#endif
{
	RPromSol** ptr;
	size_t i;

	// Change Freq
	SetMutationParams(5,8,1);

	// Init Solutions of the PROMETHEE part
	Sols=new RPromSol*[GetPopSize()+1];
	if(Sols)
	{
		for(i=GetPopSize()+2,ptr=Sols;--i;ptr++)
		{
			(*ptr)=NewSol();
		}
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::Init(void)
{
	// Init the GGA
	RInstG<cInst,cChromo,RFitnessSC,cThreadData,cGroup,cObj>::Init();

	// Init the Ratios
	double ratio;
	size_t i,j;

	RCursor<cObj> Cur1(Objs);
	RCursor<cObj> Cur2(Objs);
	for(Cur1.Start(),i=1;!Cur1.End();Cur1.Next(),i++)
	{
		// Add all the object with a greater agreement ratio than the minimum
		for(Cur2.Start(),j=i;--j;Cur2.Next())
		{
			ratio=GetAgreementRatio(Cur1()->GetElementId(),Cur2()->GetElementId());
			if(ratio>=Params->MinAgreement)
			{
				Cur1()->AddCloseObject(Cur2()->GetId(),ratio);
				Cur2()->AddCloseObject(Cur1()->GetId(),ratio);
			}
		}
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RGroupingHeuristic<cGroup,cObj,cChromo>* RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::CreateHeuristic(void)
{
	return(new RHeuristicSC<cGroup,cObj,cChromo>(Random,Objs,Debug));
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	cObj* RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::GetObj(size_t id) const
{
	R::RCursor<cObj> Cur(Objs);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetElementId()==id)
			return(Cur());
	return(0);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	bool RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::StopCondition(void)
{
	return(GetGen()==Params->MaxGen);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::WriteChromoInfo(cChromo* c)
{
	if(!Debug) return;
	Debug->PrintInfo("Id "+RString::Number(c->Id)+" (Fi="+RString::Number(c->Fi,"%1.5f")+",Fi+="+RString::Number(c->FiPlus,"%1.5f")+",Fi-="+RString::Number(c->FiMinus,"%1.5f")+
			" - J="+RString::Number(c->CritSimJ)+" - Agr.="+RString::Number(c->CritAgreement,"%1.5f")+" - Disagr.="+RString::Number(c->CritDisagreement,"%1.5f"));
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::PostEvaluate(void)
{
	size_t i;
	cChromo** C;
	cChromo* s;
	#if BESTSOLSVERIFICATION
		cChromo* b;
	#endif
	RPromSol** Res;
	RPromSol** ptr;
	double r;

	if(Debug)
		Debug->BeginFunc("PostEvaluate","RInstSC");
	ptr=Sols;
	AssignChromo(*ptr,BestChromosome);
	for(i=GetPopSize()+1,C=Chromosomes,ptr++;--i;C++,ptr++)
		AssignChromo(*ptr,*C);
	ComputePrometheeII();
	Res=GetSols();
	ptr=Res;

	// Look if the best chromosome ever is still the best (and have some groups)
	// -> If not, change the fitness of the best solution.
	if((*ptr)->GetId())
	{
		s=Chromosomes[(*ptr)->GetId()-1];
		(*s->Fitness)=static_cast<double>(GetGen())+1.1;
		#if BESTSOLSVERIFICATION
			BestSols.InsertPtr(b=new cChromo(this,BestSols.NbPtr));
			b->Init(thDatas[0]);
			(static_cast<RGroups<cGroup,cObj,cGroupData,cChromo>*>(b))->Init();
			(*b)=(*s);
		#endif
	}
	else
	{
		s=BestChromosome;
		// Verify that BestChromosome has at least one group
		// -> If not, exchange the first two solutions
		if(!BestChromosome->Used.GetNb())
		{
			RPromSol* old=(*ptr);
			(*ptr)=(*(ptr+1));
			(*(ptr+1))=old;
			s=Chromosomes[(*ptr)->GetId()-1];
		}

		if(s->Fitness->Value==0.0)
			(*s->Fitness)=1.1;
	}
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	if(Debug)
		WriteChromoInfo(s);
	ptr++;

	//  The second best has the fitness of 1
	if((*ptr)->GetId())
	{
		s=Chromosomes[(*ptr)->GetId()-1];
		(*s->Fitness)=1.0;
	}
	else
		s=BestChromosome;
	s->FiPlus=(*ptr)->GetFiPlus();
	s->FiMinus=(*ptr)->GetFiMinus();
	s->Fi=(*ptr)->GetFi();
	if(Debug)
		WriteChromoInfo(s);

	// Look for the rest
	for(i=GetPopSize(),ptr++;--i;ptr++)
	{
		r=((double)i)/((double)(GetPopSize()));
		if((*ptr)->GetId())
		{
			s=Chromosomes[(*ptr)->GetId()-1];
			(*s->Fitness)=r;
		}
		else
			s=BestChromosome;
		s->FiPlus=(*ptr)->GetFiPlus();
		s->FiMinus=(*ptr)->GetFiMinus();
		s->Fi=(*ptr)->GetFi();
		if(Debug)
			WriteChromoInfo(s);
	}

	// Delete the resulting array
	delete[] Res;

	if(Debug)
		Debug->EndFunc("PostEvaluate","RInstSC");
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::PostRun(void)
{
#if BESTSOLSVERIFICATION
	RPromSol* s;

	// Init Criterion and Solutions of the PROMETHEE part
	RPromSC::ClearSols();
	for(BestSols.Start();!BestSols.End();BestSols.Next())
	{
		s=NewSol();
		Assign(s,BestSols());
	}
	ComputePrometheeII();
	(*BestChromosome)=(*BestSols.Tab[GetBestSol()->GetId()]);
	emitBestSig();
#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::HandlerNotFound(const RNotification& /*notification*/)
{
//	std::cout<<" GCA '"<<notification.GetName()<<"' not treated (Gen="<<Gen<<")."<<std::endl;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RInstSC<cInst,cChromo,cThreadData,cGroup,cObj>::~RInstSC(void)
{
	if(Sols)
		delete[] Sols;
}
