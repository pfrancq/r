/*

	R Project Library

	RInst.hh

	Instance of Genetic Algorithms - Inline Implementation

	Copyright 1998-2005 by the Université Libre de Bruxelles.

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
// RThreadData<cInst,cChromo>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo>
	RThreadData<cInst,cChromo>::RThreadData(cInst *owner)
		: Owner(owner)
{
}



//------------------------------------------------------------------------------
//
// RInst<cInst,cChromo,cFit>
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	RInst<cInst,cChromo,cFit,cThreadData>::RInst(unsigned int popsize,RDebug *debug)
		: Debug(debug), Random(0), tmpChrom1(0), tmpChrom2(0),Receivers(10,5),bRandomConstruct(false),
		  VerifyGA(false), Chromosomes(0),PopSize(popsize),Gen(0),AgeBest(0),AgeBestPop(0)
{
//	EmitSig(sigGAInit);
	if(Debug)
		Debug->BeginApp();
	if(Debug)
		Debug->BeginFunc("RInst","RInst");
	MaxBestPopAge=5;
	MaxBestAge=10;
	FreqInversion=AgeNextInversion=10;
	AgeNextMutation=MaxBestPopAge;
	AgeNextBestMutation=MaxBestAge;
	BestInPop=BestChromosome=0;
	Random = new RRandomGood(0);
	Random->Reset(12345);
	if(Debug)
		Debug->EndFunc("RInst","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Init(void)
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


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::RandomConstruct(void)
{
	unsigned int i;
	cChromo **C;

	if(Debug)
		Debug->BeginFunc("RandomConstruct","RInst");
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->RandomConstruct();
		emitInteractSig();
		(*C)->ToEval=true;
	}
	if(Debug)
		Debug->EndFunc("RandomConstruct","RInst");
	bRandomConstruct=true;
	emitInteractSig();
	if(VerifyGA)
		Verify();
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreaData>
	void RInst<cInst,cChromo,cFit,cThreaData>::Evaluate(void)
{
	unsigned int i;
	cChromo **C;

	if(Debug)
		Debug->BeginFunc("Evaluate","RInst");
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		if((*C)->ToEval)
		{
			(*C)->Evaluate();
			(*C)->ToEval=false;
			emitInteractSig();
		}
	}
	if(Debug)
		Debug->EndFunc("Evaluate","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreaData>
	void RInst<cInst,cChromo,cFit,cThreaData>::AnalysePop(void)
{
	unsigned int i;
	cChromo **C;

	if(Debug)
		Debug->BeginFunc("AnalysePop","RInst");
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		if((*((*C)->Fitness))>(*(BestInPop->Fitness)))
		{
				if(Debug)
				{
					static char tmp[200];
					sprintf(tmp,"Chromosome %u best of population",(*C)->Id);
					Debug->PrintInfo(tmp);
				}
			BestInPop=(*C);
			AgeBestPop=0;
			AgeNextMutation=MaxBestPopAge;
		}
	}
	if((*(BestInPop->Fitness))>(*(BestChromosome->Fitness)))
	{
			if(Debug)
			{
				static char tmp[200];
				sprintf(tmp,"Chromosome %u best ever",BestInPop->Id);
				Debug->PrintInfo(tmp);
			}
		(*BestChromosome)=(*BestInPop);
		AgeBest=0;
		emitBestSig();
	}
	if(Debug)
		Debug->EndFunc("AnalysePop","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	int RInst<cInst,cChromo,cFit,cThreadData>::sort_function_cChromosome(const void* a,const void* b)
{
	cFit* af=(*(static_cast<cChromo**>(a)))->Fitness;
	cFit* bf=(*(static_cast<cChromo**>(b)))->Fitness;

	if((*af)==(*bf)) return(0);
	if((*af)>(*bf))
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Crossover(void)
{
	unsigned int i,idx1,idx2;
	cChromo **C1,**C2,*C3,*s1,*s2;
	unsigned int NbToTreat,NbTreated;

	if(Debug)
		Debug->BeginFunc("Crossover","RInst");

	// Determine the childs (end of tmpChrom) and the parents (begin of tmpChrom)
	if(PopSize<4)
	{
		if(Debug)
			Debug->EndFunc("Crossover","RInst");
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
		if((*s1->Fitness)>(*s2->Fitness))
		{
			tmpChrom2[NbTreated--]=s2;
			memmove(&tmpChrom1[idx2],&tmpChrom1[idx2+1],((--NbToTreat)-idx2)*sizeof(cChromo*));
		}
		else
		{
			tmpChrom2[NbTreated--]=s1;
			memmove(&tmpChrom1[idx1],&tmpChrom1[idx1+1],((--NbToTreat)-idx1)*sizeof(cChromo*));
		}
	}
	tmpChrom2[0]=tmpChrom1[0];
	emitInteractSig();

	// Make the crossovers
	for(C1=&tmpChrom2[PopSize-1],C2=tmpChrom2,i=NbCross+1;--i;C1--,C2++) // To change if crossover not possible
	{
		// First Crossover
		C3=(*(C2++));
		if(Debug)
		{
			static char Tmp[200];
			sprintf(Tmp,"Parent %u + Parent %u -> Child %u",(*C2)->Id,C3->Id,(*C1)->Id);
			Debug->PrintInfo(Tmp);
		}
		(*C1)->Crossover(*C2,C3);
		(*C1)->ToEval=true;
		if(VerifyGA)
			(*C1)->Verify();
		emitInteractSig();

		// Second Crossover
		C1--;
		if(Debug)
		{
			static char Tmp[200];
			sprintf(Tmp,"Parent %u + Parent %u -> Child %u",C3->Id,(*C2)->Id,(*C1)->Id);
			Debug->PrintInfo(Tmp);
		}
		(*C1)->Crossover(C3,*C2);
		(*C1)->ToEval=true;
		if(VerifyGA)
			(*C1)->Verify();
		emitInteractSig();
	}
	if(Debug)
		Debug->EndFunc("Crossover","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Mutation(void)
{
	unsigned int i;
	cFit *WorstFitness;
	cChromo **C,*p;


	if(Debug)
		Debug->BeginFunc("Mutation","RInst");
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
		if(Debug)
		{
			static char Tmp[200];
			sprintf(Tmp,"Normal Mutation (BestInPop) -> Chromosome %u",p->Id);
			Debug->PrintInfo(Tmp);
		}
		emitInteractSig();
		p->Mutation();
		p->ToEval=true;
		if(VerifyGA)
			p->Verify();
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
		if(Debug)
		{
			static char Tmp[200];
			sprintf(Tmp,"Strong Mutation (BestInPop) -> Chromosome %u",p->Id);
			Debug->PrintInfo(Tmp);
		}
		emitInteractSig();
		p->Mutation();
		p->ToEval=true;
		if(VerifyGA)
			p->Verify();
		emitInteractSig();
	}
	if(Debug)
		Debug->EndFunc("Mutation","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Inversion(void)
{
	cChromo *p;


	if(Debug)
		Debug->BeginFunc("Inversion","RInst");
	if(!(AgeNextInversion--))
	{
		AgeNextInversion=FreqInversion;
		p=Chromosomes[RRand(PopSize)];
		if(Debug)
		{
			static char Tmp[200];
			sprintf(Tmp,"Inversion of Chromosome %u",p->Id);
			Debug->PrintInfo(Tmp);
		}
		emitInteractSig();
		p->Inversion();
		p->ToEval=true;
		if(VerifyGA)
			p->Verify();
		emitInteractSig();
	}
	if(Debug)
		Debug->EndFunc("Inversion","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Generation(void)
{
	if(Debug)
		Debug->BeginFunc("Generation","RInst");
	Gen++;
	AgeBest++;
	AgeBestPop++;
	if(Debug)
	{
		static char tmp[200];
		sprintf(tmp,"Gen=%u  ;  AgeBest=%u  ;  AgeBestPop=%u",Gen,AgeBest,AgeBestPop);
		Debug->PrintComment(tmp);
	}
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
	if(Debug)
		Debug->EndFunc("Generation","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Run(void)
{
	if(Debug&&(!ExternBreak))
		Debug->BeginFunc("Run","RInst");
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
	if(Debug&&(!ExternBreak))
		Debug->EndFunc("Run","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Verify(void)
{
	unsigned int i;
	cChromo **C;

	if(Debug)
		Debug->BeginFunc("Verify","RInst");
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		(*C)->Verify();
		emitInteractSig();
	}
	if(Debug)
	{
		Debug->PrintComment("Instance Ok");
		Debug->EndFunc("Verify","RInst");
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	inline void RInst<cInst,cChromo,cFit,cThreadData>::AddReceiver(RGASignalsReceiver<cInst,cChromo,cFit> *rec)
{
	Receivers.InsertPtr(rec);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	inline void RInst<cInst,cChromo,cFit,cThreadData>::DelReceiver(RGASignalsReceiver<cInst,cChromo,cFit> *rec)
{
	Receivers.DeletePtr(rec);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::emitGenSig(void)
{
	typename RGASignalsReceiver<cInst,cChromo,cFit>::GenSig s(Gen,AgeBest,Chromosomes,BestChromosome);
	RCursor<RGASignalsReceiver<cInst,cChromo,cFit> > r(Receivers);
	for(r.Start();!r.End();r.Next())
		r()->receiveGenSig(&s);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::emitInteractSig(void)
{
	typename RGASignalsReceiver<cInst,cChromo,cFit>::InteractSig s;
	RCursor<RGASignalsReceiver<cInst,cChromo,cFit> > r(Receivers);
	for(r.Start();!r.End();r.Next())
		r()->receiveInteractSig(&s);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::emitBestSig(void)
{
	typename RGASignalsReceiver<cInst,cChromo,cFit>::BestSig s(BestChromosome);

	RCursor<RGASignalsReceiver<cInst,cChromo,cFit> > r(Receivers);
	for(r.Start();!r.End();r.Next())
		r()->receiveBestSig(&s);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	RInst<cInst,cChromo,cFit,cThreadData>::~RInst(void)
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
	if(Debug)
		Debug->EndApp();
	if(thDatas)
	{
		delete thDatas[0];
		delete[] thDatas;
	}
	if(Random)
		delete Random;
}
