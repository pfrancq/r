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
		: Debug(debug), Random(0), tmpChrom1(0), tmpChrom2(0),Receivers(10,5),bRandomConstruct(false),
			Chromosomes(0),PopSize(popsize),Gen(0),AgeBest(0),AgeBestPop(0)
{
//	EmitSig(sigGAInit);
	#ifdef RGADEBUG
		if(Debug) Debug->BeginApp();
		if(Debug) Debug->BeginFunc("RInst","RInst");
	#endif
	MaxBestPopAge=5;
	MaxBestAge=10;
	FreqInversion=AgeNextInversion=10;
	AgeNextMutation=MaxBestPopAge;
	AgeNextBestMutation=MaxBestAge;
	BestInPop=BestChromosome=0;
	Random = new RRandomGood(0);
	Random->Reset(12345);
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
		tmpChrom1=new cChromo*[PopSize];
		tmpChrom2=new cChromo*[PopSize];
		for(i=0,C=Chromosomes;i<PopSize;C++,i++)
			(*C)=new cChromo(static_cast<cInst*>(this),i);
		BestInPop=Chromosomes[0];
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
	unsigned int i;
	cChromo **C;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("RandomConstruct","RInst");
	#endif
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->RandomConstruct();
		emitInteractSig();
		(*C)->ToEval=true;
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("RandomConstruct","RInst");
	#endif
	bRandomConstruct=true;
	emitInteractSig();
	Verify();
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreaData>
	void RGA::RInst<cInst,cChromo,cFit,cThreaData>::Evaluate(void) throw(eGA)
{
	unsigned int i;
	cChromo **C;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Evaluate","RInst");
	#endif
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		if((*C)->ToEval)
		{
			(*C)->Evaluate();
			(*C)->ToEval=false;
			emitInteractSig();
		}
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Evaluate","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreaData>
	void RGA::RInst<cInst,cChromo,cFit,cThreaData>::AnalysePop(void) throw(eGA)
{
	unsigned int i;
	cChromo **C;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("AnalysePop","RInst");
	#endif
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		if((*((*C)->Fitness))>(*(BestInPop->Fitness)))
		{
			BestInPop=(*C);
			AgeBestPop=0;
			AgeNextMutation=MaxBestPopAge;
		}
	}
	if((*(BestInPop->Fitness))>(*(BestChromosome->Fitness)))
	{
		(*BestChromosome)=(*BestInPop);
		AgeBest=0;
		emitBestSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("AnalysePop","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	int RGA::RInst<cInst,cChromo,cFit,cThreadData>::sort_function_cChromosome(const void* a,const void* b)
{
	cFit* af=(*(static_cast<cChromo**>(a)))->Fitness;
	cFit* bf=(*(static_cast<cChromo**>(b)))->Fitness;

	if((*af)==(*bf)) return(0);
	if((*af)>(*bf))
		return(-1);
	else
		return(1);
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Crossover(void) throw(eGA)
{
	unsigned int i,idx1,idx2;
	cChromo **C1,**C2,*C3,*s1,*s2;
	static char Tmp[200];
	unsigned int NbToTreat,NbTreated;

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Crossover","RInst");
	#endif
	// Determine the childs (end of tmpChrom) and the parents (begin of tmpChrom)
	if(PopSize<4)
	{
		#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Crossover","RInst");
		#endif
		return;
	}

	// Apply Tournement - tmpChrom1 -> tmpChrom2
	NbToTreat=PopSize;
	NbTreated=PopSize-1;
	memcpy(tmpChrom1,Chromosomes,sizeof(cChromo*)*PopSize);
	memset(tmpChrom2,0,sizeof(cChromo*)*PopSize);
	while(NbToTreat>1)
	{
		if(NbToTreat>2)
		{
			idx1=RRand(NbToTreat);
			idx2=idx1+RRand(NbToTreat-2)+1;
			if(idx2>NbToTreat-1)
				idx2-=NbToTreat-1;
		}
		else
		{
			idx1=0;
			idx2=1;
		}
		RAssert(idx1!=idx2);
		s1=tmpChrom1[idx1];
		s2=tmpChrom1[idx2];
		if((*s1->Fitness)<(*s2->Fitness))
		{
			tmpChrom2[NbTreated--]=s2;
			memcpy(&tmpChrom1[idx2],&tmpChrom1[idx2+1],((--NbToTreat)-idx2)*sizeof(cChromo*));
		}
		else
		{
			tmpChrom2[NbTreated--]=s1;
			memcpy(&tmpChrom1[idx1],&tmpChrom1[idx1+1],((--NbToTreat)-idx1)*sizeof(cChromo*));
		}
	}
	tmpChrom2[0]=tmpChrom1[0];
	emitInteractSig();

	// Make the crossovers
	for(C1=&tmpChrom2[PopSize-1],C2=tmpChrom2,i=NbCross+1;--i;C1--,C2++) // To change if crossover not possible
	{
		// First Crossover
		C3=(*(C2++));
		#ifdef RGADEBUG
			if(Debug)
			{
				sprintf(Tmp,"Parent %u + Parent %u -> Child %u",(*C2)->Id,C3->Id,(*C1)->Id);
				Debug->PrintInfo(Tmp);
			}
		#endif
		(*C1)->Crossover(*C2,C3);
		(*C1)->Optimisation();
		(*C1)->ToEval=true;
		#ifdef RGADEBUG
			(*C1)->Verify();
		#endif
		emitInteractSig();

		// Second Crossover
		C1--;
		#ifdef RGADEBUG
			if(Debug)
			{
				sprintf(Tmp,"Parent %u + Parent %u -> Child %u",C3->Id,(*C2)->Id,(*C1)->Id);
				Debug->PrintInfo(Tmp);
			}
		#endif
		(*C1)->Crossover(C3,*C2);
		(*C1)->Optimisation();
		(*C1)->ToEval=true;
		#ifdef RGADEBUG
		(*C1)->Verify();
		#endif
		emitInteractSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Crossover","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Mutation(void) throw(eGA)
{
	unsigned int i;
	cFit *WorstFitness;
	cChromo **C,*p;

	#ifdef RGADEBUG
		static char Tmp[200];
		if(Debug) Debug->BeginFunc("Mutation","RInst");
	#endif
	if(AgeBestPop==AgeNextMutation)
	{
		AgeNextMutation+=MaxBestPopAge;
		C=Chromosomes;
		p=(*C);
		WorstFitness= (*(C++))->Fitness;
		for(i=PopSize;--i;C++)
		{
			if((*((*C)->Fitness))<(*WorstFitness))
			{
				WorstFitness=(*C)->Fitness;
				p=(*C);
			}
		}
		if(p->Id!=BestInPop->Id)
		{
			i=p->Id;
			(*p)=(*BestInPop);
			p->Id=i;
		}
		#ifdef RGADEBUG
			sprintf(Tmp,"Normal Mutation (BestInPop) -> Chromosome %u",p->Id);
			if(Debug) Debug->PrintInfo(Tmp);
		#endif
		emitInteractSig();
		p->Mutation();
		p->Optimisation();
		p->ToEval=true;
		#ifdef RGADEBUG
			p->Verify();
		#endif
		emitInteractSig();
	}
	if(AgeBest==AgeNextBestMutation)
	{
		AgeNextBestMutation+=MaxBestPopAge;
		C=Chromosomes;
		p=(*C);
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
		p->Optimisation();
		p->ToEval=true;
		#ifdef RGADEBUG
			p->Verify();
		#endif
		emitInteractSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Mutation","RInst");
	#endif
}


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RGA::RInst<cInst,cChromo,cFit,cThreadData>::Inversion(void) throw(eGA)
{
	cChromo *p;

	#ifdef RGADEBUG
		static char Tmp[200];
		if(Debug) Debug->BeginFunc("Inversion","RInst");
	#endif
	if(!(AgeNextInversion--))
	{
		AgeNextInversion=FreqInversion;
		p=Chromosomes[RRand(PopSize)];
		#ifdef RGADEBUG
			sprintf(Tmp,"Inversion of Chromosome %u",p->Id);
			if(Debug) Debug->PrintInfo(Tmp);
		#endif
		emitInteractSig();
		p->Inversion();
		p->Optimisation();
		p->ToEval=true;
		#ifdef RGADEBUG
			p->Verify();
		#endif
		emitInteractSig();
	}
	#ifdef RGADEBUG
		if(Debug) Debug->EndFunc("Inversion","RInst");
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
	emitInteractSig();
	Mutation();
	emitInteractSig();
	Inversion();
	emitInteractSig();
	Evaluate();
	emitInteractSig();
	PostEvaluate();
	emitInteractSig();
	AnalysePop();
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
	if(!bRandomConstruct)
	{
		RandomConstruct();
		emitInteractSig();
		Evaluate();
		emitInteractSig();
		PostEvaluate();
		emitInteractSig();
		AnalysePop();
		emitBestSig();
		emitGenSig();
	}
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

	#ifdef RGADEBUG
		if(Debug) Debug->BeginFunc("Verify","RInst");
	#endif
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->Verify();
		emitInteractSig();
	}
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
	if(tmpChrom1) delete[] tmpChrom1;
	if(tmpChrom2) delete[] tmpChrom2;
	delete RRandom::RandomGen;
	#ifdef RGADEBUG
		if(Debug) Debug->EndApp();
	#endif
	if(thDatas)
	{
		delete thDatas[0];
		delete[] thDatas;
	}
	if(Random)
		delete Random;
}
