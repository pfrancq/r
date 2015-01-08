/*

	R Project Library

	RInstSG.h

	Similarity-based Grouping Genetic Algorithm Instance - Implementation

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
// RThreadDataSG<cInst,cChromo,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RThreadDataSG<cInst,cChromo,cThreadData,cGroup,cObj>::RThreadDataSG(cInst* owner)
		: RThreadDataG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>(owner),
	  ToDel(owner->Objs.GetNb()<4?4:owner->Objs.GetNb()/4), tmpObjs1(0),tmpObjs2(0), Tests(0),
	  Prom(owner->Params), Sols(0), NbSols((owner->Params->NbDivChromo*2)+1)
{
	RPromSol** s;
	size_t i;

	Tests=new cChromo*[NbSols];
	Sols=new RPromSol*[NbSols+1];
	for(i=NbSols+2,s=Sols;--i;s++)
		(*s)=Prom.NewSol();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RThreadDataSG<cInst,cChromo,cThreadData,cGroup,cObj>::Init(void)
{
	size_t i;

	RThreadDataG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Init();

	tmpObjs1=new cObj*[Owner->Objs.GetNb()];
	tmpObjs2=new cObj*[Owner->Objs.GetNb()];
	for(i=0;i<NbSols;i++)
	{
		Tests[i]=new cChromo(Owner,Owner->GetPopSize()+1+i);
		(static_cast<RGroups<cGroup,cObj,cChromo>*>(Tests[i]))->Init();
		Tests[i]->Init(static_cast<cThreadData*>(this));
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RThreadDataSG<cInst,cChromo,cThreadData,cGroup,cObj>::~RThreadDataSG(void)
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
// RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::RInstSG(RCursor<cObj> objs,RParamsSG* p,RDebug *debug)
		: RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>(p->PopSize,objs,"SCFirstFit","SCGA",debug),
		RPromKernelSC<cChromo>(p), Params(p), Sols(0), NoSocialObjs(objs.GetNb())
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
	void RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::Init(void)
{
	// Init the GGA
	RInstG<cInst,cChromo,RFitnessSG,cThreadData,cGroup,cObj>::Init();

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
			ratio=GetAgreementRatio(Cur1(),Cur2());
			if(ratio>=Params->MinAgreement)
			{
				Params->AddCloseObject(Cur1()->GetId(),Cur2()->GetId(),ratio);
				Params->AddCloseObject(Cur2()->GetId(),Cur1()->GetId(),ratio);
			}
		}
	}
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RGroupingHeuristic<cGroup,cObj,cChromo>* RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::CreateHeuristic(void)
{
	return(new RHeuristicSG<cGroup,cObj,cChromo>(Random,Objs,Params,Debug));
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	cObj* RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::GetObj(size_t id) const
{
	R::RCursor<cObj> Cur(Objs);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->GetElementId()==id)
			return(Cur());
	return(0);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	bool RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::StopCondition(void)
{
	return(GetGen()==Params->MaxGen);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::WriteChromoInfo(cChromo* c)
{
	if(!Debug) return;
	Debug->PrintInfo("Id "+RString::Number(c->Id)+" (Fi="+RString::Number(c->Fi,"%1.5f")+",Fi+="+RString::Number(c->FiPlus,"%1.5f")+",Fi-="+RString::Number(c->FiMinus,"%1.5f")+
			" - J="+RString::Number(c->CritSimJ)+" - Agr.="+RString::Number(c->CritAgreement,"%1.5f")+" - Disagr.="+RString::Number(c->CritDisagreement,"%1.5f"));
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::PostEvaluate(void)
{
	size_t i;
	cChromo** C;
	cChromo* s;
	#if BESTSOLSVERIFICATION
		cChromo* b;
	#endif
	double r;

	if(Debug)
		Debug->BeginFunc("PostEvaluate","RInstSG");
	RPromSol** sol(Sols);
	AssignChromo(*sol,BestChromosome);
	for(i=GetPopSize()+1,C=Chromosomes,sol++;--i;C++,sol++)
		AssignChromo(*sol,*C);
	ComputePrometheeII();
	RContainer<RPromSol,false,false> Res(GetPopSize()+1);
	RPromKernel::CopySols(Res);
	RCursor<RPromSol> ptr(Res);
	ptr.Start();

	// Look if the best chromosome ever is still the best (and have some groups)
	// -> If not, change the fitness of the best solution.
	if(ptr()->GetId())
	{
		s=Chromosomes[ptr()->GetId()-1];
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
			RPromSol* old=Res[0];
			Res.InsertPtrAt(Res[1],0,false);
			Res.InsertPtrAt(old,1,false);
			s=Chromosomes[ptr()->GetId()-1];
		}

		if(s->Fitness->Value==0.0)
			(*s->Fitness)=1.1;
	}
	s->FiPlus=ptr()->GetFiPlus();
	s->FiMinus=ptr()->GetFiMinus();
	s->Fi=ptr()->GetFi();
	if(Debug)
		WriteChromoInfo(s);
	ptr.Next();

	//  The second best has the fitness of 1
	if(ptr()->GetId())
	{
		s=Chromosomes[ptr()->GetId()-1];
		(*s->Fitness)=1.0;
	}
	else
		s=BestChromosome;
	s->FiPlus=ptr()->GetFiPlus();
	s->FiMinus=ptr()->GetFiMinus();
	s->Fi=ptr()->GetFi();
	if(Debug)
		WriteChromoInfo(s);

	// Look for the rest
	for(i=GetPopSize(),ptr.Next();!ptr.End();ptr.Next(),--i)
	{
		r=((double)i)/((double)(GetPopSize()));
		if(ptr()->GetId())
		{
			s=Chromosomes[ptr()->GetId()-1];
			(*s->Fitness)=r;
		}
		else
			s=BestChromosome;
		s->FiPlus=ptr()->GetFiPlus();
		s->FiMinus=ptr()->GetFiMinus();
		s->Fi=ptr()->GetFi();
		if(Debug)
			WriteChromoInfo(s);
	}

	if(Debug)
		Debug->EndFunc("PostEvaluate","RInstSG");
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	void RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::PostRun(void)
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
	void RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::HandlerNotFound(const RNotification& /*notification*/)
{
//	std::cout<<" GCA '"<<notification.GetName()<<"' not treated (Gen="<<Gen<<")."<<std::endl;
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj>
	RInstSG<cInst,cChromo,cThreadData,cGroup,cObj>::~RInstSG(void)
{
	if(Sols)
		delete[] Sols;
}
