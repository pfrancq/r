/*

	R Project Library

	RInst.hh

	Instance of Genetic Algorithms - Inline Implementation

	Copyright 1998-2007 by the Université Libre de Bruxelles.

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
	RInst<cInst,cChromo,cFit,cThreadData>::RInst(unsigned int popsize,const RString& name,RDebug* debug)
		: RObject(name), Debug(debug), Random(0), tmpChrom1(0), tmpChrom2(0),bRandomConstruct(false),		
		  VerifyGA(false), DoPostEvaluation(false),
		  Chromosomes(0), Ranked(0), PopSize(popsize), Gen(0), AgeBest(0), AgeBestPop(0)
{
	if(Debug)
		Debug->BeginApp(GetName());
	if(Debug)
		Debug->BeginFunc("RInst","RInst");
	FreqInversion=AgeNextInversion=10;
	NbMutations=1;
	FreqMutation=AgeNextMutation=5;
	FreqBestMutation=AgeNextBestMutation=10;
	BestInPop=BestChromosome=0;
	Random = new RRandomGood(0);
	Random->Reset(12345);
	if(Debug)
		Debug->EndFunc("RInst","RInst");
}

	
//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::SetMutationParams(unsigned int agemut,unsigned int agebestmut,unsigned int nbmut)
{
		if(nbmut>PopSize)
			throw RGAException("Number of mutations cannot be greather than the population size");
		NbMutations=nbmut;
		FreqMutation=agemut;
		FreqBestMutation=agebestmut;		
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
		Ranked=new cChromo*[PopSize];
		memset(Chromosomes,0,PopSize*sizeof(cChromo*));
		memset(Ranked,0,PopSize*sizeof(cChromo*));
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
	int RInst<cInst,cChromo,cFit,cThreadData>::sort_function_cChromosome(const void* a,const void* b)
{
	cFit* af=(*((cChromo**)a))->Fitness;
	cFit* bf=(*((cChromo**)b))->Fitness;
		
	if((*af)==(*bf)) return(0);
	if((*af)>(*bf))
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreaData>
	void RInst<cInst,cChromo,cFit,cThreaData>::AnalysePop(void)
{
	unsigned int i;
	cChromo **C;

	if(Debug)
		Debug->BeginFunc("AnalysePop","RInst");
	
	// Evaluate all the chromosomes if necessary
	PostNotification("RInst::Interact");
	for(i=PopSize+1,C=Chromosomes;--i;C++)
	{
		if((*C)->ToEval)
		{
			(*C)->Evaluate();
			(*C)->ToEval=false;
			DoPostEvaluation=true;
			PostNotification("RInst::Interact");
		}
	}
	PostNotification("RInst::Interact");
	
	// If necessary, do a post eveluation
	if(DoPostEvaluation)
	{
		PostEvaluate();
		DoPostEvaluation=true;
	};
	PostNotification("RInst::Interact");
	
	// Ranked the chromosomes and verify that the best of the population or the best ever have not changed.
	memcpy(Ranked,Chromosomes,sizeof(cChromo*)*PopSize);
	qsort(static_cast<void*>(Ranked),PopSize,sizeof(cChromo*),sort_function_cChromosome);
	cChromo* best=Ranked[0];
	if((*best->Fitness)>(*BestInPop->Fitness))
	{
		if(Debug)
		{
			static char tmp[200];
			sprintf(tmp,"Chromosome %u best of population",(*C)->Id);
			Debug->PrintInfo(tmp);
		}
		BestInPop=best;
		AgeBestPop=0;
		AgeNextMutation=FreqMutation;
	}
	if((*BestInPop->Fitness)>(*BestChromosome->Fitness))
	{
		if(Debug)
		{
			static char tmp[200];
			sprintf(tmp,"Chromosome %u best ever",BestInPop->Id);
			Debug->PrintInfo(tmp);
		}
		(*BestChromosome)=(*BestInPop);
		AgeBest=0;
		AgeNextBestMutation=FreqBestMutation;
		PostNotification("RInst::Best");
	}
	if(Debug)
		Debug->EndFunc("AnalysePop","RInst");
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
		PostNotification("RInst::Interact");
		(*C)->ToEval=true;
	}
	bRandomConstruct=true;
	PostNotification("RInst::Interact");
	if(VerifyGA)
		Verify();

	if(Debug)
		Debug->EndFunc("RandomConstruct","RInst");
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
	PostNotification("RInst::Interact");

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
		PostNotification("RInst::Interact");

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
		PostNotification("RInst::Interact");
	}
	if(Debug)
		Debug->EndFunc("Crossover","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Mutation(void)
{
	if(Debug)
		Debug->BeginFunc("Mutation","RInst");
	
	// Normal mutation?
	if(!AgeNextMutation)
	{
		AgeNextMutation=FreqMutation;
		cChromo** Mut=&Ranked[PopSize-1];
		for(unsigned int i=NbMutations+1;--i;Mut--)
		{
			cChromo* p=(*Mut);
			if(p->Id!=BestInPop->Id)
			{	
				unsigned int id=p->Id;
				(*p)=(*BestInPop);
				p->Id=id;
			}
			if(Debug)
			{
				static char Tmp[200];
				sprintf(Tmp,"Normal Mutation (BestInPop) -> Chromosome %u",p->Id);
				Debug->PrintInfo(Tmp);
			}
			PostNotification("RInst::Interact");
			p->Mutation();
			p->ToEval=true;
			if(VerifyGA)
				p->Verify();
			PostNotification("RInst::Interact");
		}
	}
	
	// Best mutation?
	if(!AgeNextBestMutation)
	{
		AgeNextBestMutation=FreqBestMutation;
		cChromo** Mut=&Ranked[PopSize-1];
		for(unsigned int i=NbMutations+1;--i;Mut--)
		{
			cChromo* p=(*Mut);
			unsigned int id=p->Id;
			(*p)=(*BestChromosome);
			p->Id=id;
			if(Debug)
			{
				static char Tmp[200];
				sprintf(Tmp,"Strong Mutation (BestInPop) -> Chromosome %u",p->Id);
				Debug->PrintInfo(Tmp);
			}
			PostNotification("RInst::Interact");
			p->Mutation();
			p->ToEval=true;
			if(VerifyGA)
				p->Verify();
			PostNotification("RInst::Interact");
		}
	}
	if(Debug)
		Debug->EndFunc("Mutation","RInst");
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::Inversion(void)
{
	if(Debug)
		Debug->BeginFunc("Inversion","RInst");
		
	// Do the inversion
	AgeNextInversion=FreqInversion;
	cChromo* p=Chromosomes[RRand(PopSize)];
	if(Debug)
	{
		static char Tmp[200];
		sprintf(Tmp,"Inversion of Chromosome %u",p->Id);
		Debug->PrintInfo(Tmp);
	}
	p->Inversion();
	p->ToEval=true;
	if(VerifyGA)
		p->Verify();

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
	PostNotification("RInst::Interact");	
	if(!((AgeNextMutation--)&&(AgeNextBestMutation--)))  // Decrease the number of generations between mutations and verify if a mutation must be done
	{
		// Evaluation is necessary since the crossover has been done
		AnalysePop();
		PostNotification("RInst::Interact");				
		Mutation();
	}
	PostNotification("RInst::Interact");	
	if(!(AgeNextInversion--)) // Decrease the number of generations between inversions and verify if a mutation must be done
	{
		// Evaluation is necessary since the crossover has been done (and perhaps Mutation)
		AnalysePop();
		PostNotification("RInst::Interact");	
		Inversion();
	}
	PostNotification("RInst::Interact");
	AnalysePop();
	PostNotification("RInst::Generation");
	PostNotification("RInst::Interact");
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
		PostNotification("RInst::Interact");
		AnalysePop();
		PostNotification("RInst::Generation");
		DisplayInfos();
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
		PostNotification("RInst::Interact");
	}
	if(Debug)
	{
		Debug->PrintComment("Instance Ok");
		Debug->EndFunc("Verify","RInst");
	}
}


//------------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData>
	void RInst<cInst,cChromo,cFit,cThreadData>::HandlerNotFound(const RNotification& notification)
{
	std::cout<<GetName()<<" : Message '"<<notification.GetName()<<"' not treated (Gen="<<Gen<<")."<<std::endl;
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
	if(Ranked) delete[] Ranked;
	if(BestChromosome) delete BestChromosome;
	if(tmpChrom1) delete[] tmpChrom1;
	if(tmpChrom2) delete[] tmpChrom2;
	if(Debug)
		Debug->EndApp(GetName());
	if(thDatas)
	{
		delete thDatas[0];
		delete[] thDatas;
	}
	if(Random)
		delete Random;
}
