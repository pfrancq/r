/*

	R Project Library

	RInst.hh

	Instance of Genetic Algorithms - Inline Implementation

	(C) 1998-2001 by P. Francq.

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
// RThreadData<cInst,cChromo>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo>
	RGA::RThreadData<cInst,cChromo>::RThreadData(cInst *owner) throw(bad_alloc)
		: Owner(owner)
{
}



//-----------------------------------------------------------------------------
//
// RInst<cInst,cChromo,cFit>
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	RGA::RInst<cInst,cChromo,cFit,cThreadData>::RInst(unsigned int popsize,RDebug *debug) throw(bad_alloc)
		: Debug(debug), Random(0), Parents(0),Childs(0),tmpChrom(0),Receivers(10,5),bRandomConstruct(false),
			Chromosomes(0),PopSize(popsize),Gen(0),AgeBest(0)
{
//	EmitSig(sigGAInit);
	#ifdef RGADEBUG
		if(Debug) Debug->BeginApp("HGA","Pascal Francq");
		if(Debug) Debug->BeginFunc("RInst","RInst");
	#endif
	MaxBestPopAge=5;
	MaxBestAge=10;
	AgeNextMutation=MaxBestPopAge;
	AgeNextBestMutation=MaxBestAge;
	BestChromosome=0;
	Random = new RRandomGood(12345);
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("RInst","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Init(void) throw(bad_alloc)
{
	cChromo **C;
	unsigned int i;

	if(PopSize)
	{
		Chromosomes=new cChromo*[PopSize];
		memset(Chromosomes,0,PopSize*sizeof(cChromo*));
		NbCross=PopSize/4;
		Childs=new cChromo*[2*NbCross];
		Parents=new cChromo*[2*NbCross];
		tmpChrom=new cChromo*[2*PopSize];
		for(i=0,C=Chromosomes;i<PopSize;C++,i++)
			(*C)=new cChromo(static_cast<cInst*>(this),i);
	}
	BestChromosome=new cChromo(static_cast<cInst*>(this),PopSize);
	thDatas=new cThreadData*[1];
	thDatas[0]=new cThreadData(static_cast<cInst*>(this));	
	thDatas[0]->Init();
	for(i=0,C=Chromosomes;i<PopSize;C++,i++)
		(*C)->Init(thDatas[0]);
	BestChromosome->Init(thDatas[0]);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::RandomConstruct(void) throw(eGA)
{
	cChromo **C=Chromosomes,*p;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("RandomConstruct","RInst");
	#endif
	p=(*(C++));
	p->RandomConstruct();
	emitInteractSig();
	p->Evaluate();
	p->ToEval=false;
	BestInPop=p;
	for(unsigned int i=PopSize;--i;C++)
	{
		p=(*C);
		if(!p->RandomConstruct())
			throw eGARandomConstruct();
		emitInteractSig();
		p->Evaluate();
		emitInteractSig();
		p->ToEval=false;
		if((*(p->Fitness))>(*(BestInPop->Fitness))) BestInPop=p;
	}
	(*BestChromosome)=(*BestInPop);
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("RandomConstruct","RInst");
	#endif
	emitInteractSig();
	Verify();
	bRandomConstruct=true;
	emitBestSig();
	emitGenSig();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreaData>
	inline void RGA::RInst<cInst,cChromo,cFit,cThreaData>::Evaluate(void) throw(eGA)
{
	cChromo **C=Chromosomes,*p,*tmp;
	unsigned int i;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Evaluate","RInst");
	#endif
	p=(*(C++));
	if(p->ToEval)
	{
		p->Evaluate();
		emitInteractSig();
		p->ToEval=false;
	}
	tmp=p;
	for(i=PopSize;--i;C++)
	{
		p=*C;
		if(p->ToEval)
		{
			p->Evaluate();
			emitInteractSig();
			p->ToEval=false;
		}
		if((*(p->Fitness))>(*(tmp->Fitness))) tmp=p;
	}
	if((*(tmp->Fitness))>(*(BestInPop->Fitness)))
	{
		BestInPop=tmp;
		AgeBestPop=0;
		AgeNextMutation=MaxBestPopAge;
	}
	if((*(BestInPop->Fitness))>(*(BestChromosome->Fitness)))
	{
		(*BestChromosome)=(*BestInPop);
		AgeBest=0;
		emitBestSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Evaluate","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	int RGA::RInst<cInst,cChromo,cFit,cThreadData>::sort_function_cChromosome( const void *a, const void *b)
{
	cFit *af=(*(static_cast<cChromo**>(a)))->Fitness;
	cFit *bf=(*(static_cast<cChromo**>(b)))->Fitness;

	if((*af)==(*bf)) return(0);
	if((*af)>(*bf))
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	inline void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Crossover(void) throw(eGA)
{
	unsigned i;
	cChromo **C1,**C2,*C3;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Crossover","RInst");
	#endif
	// Determine the childs (end of tmpChrom) and the parents (begin of tmpChrom)
	if(PopSize<4) return;
	memcpy(tmpChrom,Chromosomes,sizeof(cChromo*)*PopSize);
	qsort(static_cast<void*>(tmpChrom),PopSize,sizeof(cChromo*),sort_function_cChromosome);
	emitInteractSig();

	// Make the crossovers
//	Random->RandOrder<cChromo*>(tmpChrom,2*NbCross);
	for(C1=&tmpChrom[PopSize-1],C2=tmpChrom,i=NbCross+1;--i;C1--,C2++) // A changer au cas où crossover pas possible
	{
		C3=(*(C2++));
		#ifdef RGADEBUG
			static char Tmp[200];
			sprintf(Tmp,"Parent %u + Parent %u -> Child %u",(*C2)->Id,C3->Id,(*C1)->Id);
			if(Debug) Debug->PrintInfo(Tmp);
		#endif
		if(!((*(C1--))->Crossover(*C2,C3)))
			throw eGACrossover(Gen,(*C2)->Id,C3->Id,(*C1)->Id);
		emitInteractSig();
		#ifdef RGADEBUG
			sprintf(Tmp,"Parent %u + Parent %u -> Child %u",C3->Id,(*C2)->Id,(*C1)->Id);
			if(Debug) Debug->PrintInfo(Tmp);
		#endif
		if(!((*C1)->Crossover(C3,*C2)))
			throw eGACrossover(Gen,C3->Id,(*C2)->Id,(*C1)->Id);
		emitInteractSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Crossover","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	inline void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Mutation(void) throw(eGA)
{
	unsigned int i;
	cFit *WorstFitness=new cFit();
	cChromo **C,*p;

	#ifdef RGADEBUG
		static char Tmp[200];
		if(Debug) Debug->BeginFunc("Mutation","RInst");
	#endif
	if(AgeBestPop==AgeNextMutation)
	{
		AgeNextMutation+=MaxBestPopAge;
		C=Chromosomes;
		WorstFitness= (*(C++))->Fitness;
		for(i=PopSize;--i;C++)
		{
			if((*((*C)->Fitness))<(*WorstFitness))
			{
				WorstFitness=(*C)->Fitness;
				p=(*C);
			}
		}
		i=p->Id;
		(*p)=(*BestInPop);
		p->Id=i;
		#ifdef RGADEBUG
			sprintf(Tmp,"Normal Mutation (BestInPop) -> Chromosome %u",p->Id);
			if(Debug) Debug->PrintInfo(Tmp);
		#endif
		emitInteractSig();
		p->Mutation();
		emitInteractSig();
	}
	if(AgeBest==AgeNextBestMutation)
	{
		AgeNextBestMutation+=MaxBestPopAge;
		C=Chromosomes;
		WorstFitness= (*(C++))->Fitness;
		for(i=PopSize;--i;C++)
		{
			if((*((*C)->Fitness))<(*WorstFitness))
			{
				WorstFitness=(*C)->Fitness;
				p=(*C);
			}
		}
		i=p->Id;
		(*p)=(*BestChromosome);
		p->Id=i;
		#ifdef RGADEBUG
			sprintf(Tmp,"Strong Mutation (BestInPop) -> Chromosome %u",p->Id);
			if(Debug) Debug->PrintInfo(Tmp);
		#endif
		emitInteractSig();
		p->Mutation();
		emitInteractSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Mutation","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Generation(void) throw(eGA)
{
	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Generation","RInst");
	#endif
	Gen++;
	AgeBest++;
	AgeBestPop++;
	#ifdef RGADEBUG
		static char tmp[200];
		sprintf(tmp,"Gen=%u  ;  AgeBest=%u  ;  AgeBestPop=%u",Gen,AgeBest,AgeBestPop);
		if(Debug) Debug->PrintComment(tmp);
	#endif
	Crossover();
	Mutation();
	Verify();
	Evaluate();
	emitGenSig();
	emitInteractSig();
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Generation","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Run(void) throw(eGA)
{
	#ifdef RGADEBUG
		if(Debug&&(!ExternBreak)) Debug->BeginFunc("Run","RInst");
	#endif
	if(!bRandomConstruct) RandomConstruct();
	ExternBreak = false;
	while((!StopCondition())&&(!ExternBreak))
	{
		Generation();
		DisplayInfos();
	}
	PostRun();
	#ifdef RGADEBUG
		if(Debug&&(!ExternBreak)) Debug->EndFunc("Run","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Verify(void) throw(eGA)
{
	unsigned int i;
	cChromo **C;
	bool b=false;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Verify","RInst");
	#endif
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		if(!((*C)->Verify()))
		{
			b=true;
		}
		emitInteractSig();
	}
	if(b)
		throw eGAVerify();
	#ifdef RGADEBUG
		if(Debug)
		{
			Debug->PrintComment("Instance Ok");
			Debug->EndFunc("Verify","RInst");
		}
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	inline void RGA::RInst<cInst,cChromo,cFit,cThreadData>::AddReceiver(RGASignalsReceiver<cInst,cChromo,cFit> *rec)
{
	Receivers.InsertPtr(rec);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	inline void RGA::RInst<cInst,cChromo,cFit,cThreadData>::DelReceiver(RGASignalsReceiver<cInst,cChromo,cFit> *rec)
{
	Receivers.DeletePtr(rec);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::emitGenSig(void)
{
	unsigned int i;
	RGASignalsReceiver<cInst,cChromo,cFit> **r;
	RGASignalsReceiver<cInst,cChromo,cFit>::GenSig s(Gen,AgeBest,Chromosomes,BestChromosome);

	for(i=Receivers.NbPtr+1,r=Receivers.Tab;--i;r++)
		(*r)->receiveGenSig(&s);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::emitInteractSig(void)
{
	unsigned int i;
	RGASignalsReceiver<cInst,cChromo,cFit> **r;
	RGASignalsReceiver<cInst,cChromo,cFit>::InteractSig s;

	for(i=Receivers.NbPtr+1,r=Receivers.Tab;--i;r++)
		(*r)->receiveInteractSig(&s);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::emitBestSig(void)
{
	unsigned int i;
	RGASignalsReceiver<cInst,cChromo,cFit> **r;
	RGASignalsReceiver<cInst,cChromo,cFit>::BestSig s(BestChromosome);

	for(i=Receivers.NbPtr+1,r=Receivers.Tab;--i;r++)
		(*r)->receiveBestSig(&s);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	RGA::RInst<cInst,cChromo,cFit,cThreadData>::~RInst(void)
{
	cChromo **C;
	unsigned int i;

	if(Chromosomes)
	{
		for(i=PopSize+1,C=Chromosomes;--i;C++)
			if(*C) delete (*C);
		delete[] Chromosomes;
	}
	if(BestChromosome) delete BestChromosome;
	if(Childs) delete[] Childs;
	if(Parents) delete[] Parents;
	if(tmpChrom) delete[] tmpChrom;
	delete RRandom::RandomGen;
	#ifdef RGADEBUG
		if(Debug) Debug->EndApp("HGA","Pascal Francq");
	#endif
	if(thDatas)
	{
		delete thDatas[0];
		delete[] thDatas;
	}

	if(Random)
		delete Random;
}
