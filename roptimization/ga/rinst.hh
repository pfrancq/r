/*

  RInst.hh

  Instance of Genetic Algorithms - Inline Implementation

  (C) 1998-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
//
// RInst<cInst,cChromo,cFit>
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	RInst<cInst,cChromo,cFit>::RInst(unsigned int popsize) throw(bad_alloc)
		: Chromosomes(NULL),PopSize(popsize),Gen(0),AgeBest(0),Parents(NULL),Childs(NULL),tmpChrom(NULL)
{
  cChromo **C;
  unsigned int i;
	
	EmitSig(sigGAInit);
  #ifdef RGADEBUG
    if(Debug) Debug->BeginApp("HGA","Pascal Francq");
		if(Debug) Debug->BeginFunc("RInst","RInst");
  #endif
  MaxBestPopAge=0;
  AgeNextMutation=MaxBestPopAge;
  AgeNextBestMutation=MaxBestAge;
  BestChromosome=NULL;
  RandomGen = new RRandomGood(12345);
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
  #ifdef RGADEBUG
    if(Debug) Debug->EndFunc("RInst","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	void RInst<cInst,cChromo,cFit>::Init(void) throw(bad_alloc)
{
  cChromo **C;
  unsigned int i;
	
  for(i=0,C=Chromosomes;i<PopSize;C++,i++)
 	  (*C)->Init();
	BestChromosome->Init();
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	bool RInst<cInst,cChromo,cFit>::RandomConstruct(void)
{
  cChromo **C=Chromosomes,*p;

  #ifdef RGADEBUG
    if(Debug) Debug->BeginFunc("RandomConstruct","RInst");
  #endif
  p=(*(C++));
  p->RandomConstruct();
  p->Evaluate();
  p->ToEval=false;
  BestInPop=p;
  for(unsigned int i=PopSize;--i;C++)
  {
    p=(*C);
    p->RandomConstruct();
    p->Evaluate();
    p->ToEval=false;
    if((*(p->Fitness))>(*(BestInPop->Fitness))) BestInPop=p;
  }
  (*BestChromosome)=(*BestInPop);
  #ifdef RGADEBUG
    if(Debug) Debug->EndFunc("RandomConstruct","RInst");
  #endif
  return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	inline void RInst<cInst,cChromo,cFit>::Evaluate(void)
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
  	p->ToEval=false;
	}
	tmp=p;
  for(i=PopSize;--i;C++)
  {
    p=*C;
    if(p->ToEval)
    {
      p->Evaluate();
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
		EmitSig(sigNewBest);
  }
  #ifdef RGADEBUG
    if(Debug) Debug->EndFunc("Evaluate","RInst");
  #endif
}


//--------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	int RInst<cInst,cChromo,cFit>::sort_function_cChromosome( const void *a, const void *b)
{
  cFit *af=(*(static_cast<cChromo**>(a)))->Fitness;
  cFit *bf=(*(static_cast<cChromo**>(b)))->Fitness;

		

  if((*af)==(*bf)) return(0);
  if((*af)>(*bf))
    return(-1);
  else
    return(1);
}


//---------------------------------------------------------------------------
// The bests chromosomes are crossovered
template<class cInst,class cChromo,class cFit>
	inline void RInst<cInst,cChromo,cFit>::Crossover(void)
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

  // Make the crossovers
  randorder<cChromo*>(tmpChrom,2*NbCross);
  for(C1=&tmpChrom[PopSize-1],C2=tmpChrom,i=NbCross+1;--i;C1--,C2++) // A changer au cas où crossover pas possible
  {
    C3=(*(C2++));
    #ifdef RGADEBUG
      static char Tmp[200];
      sprintf(Tmp,"Parent %u + Parent %u -> Child %u",(*C2)->Id,C3->Id,(*C1)->Id);
	    if(Debug) Debug->PrintInfo(Tmp);
    #endif
    (*(C1--))->Crossover(*C2,C3);
    #ifdef RGADEBUG
      sprintf(Tmp,"Parent %u + Parent %u -> Child %u",C3->Id,(*C2)->Id,(*C1)->Id);
	    if(Debug) Debug->PrintInfo(Tmp);
    #endif
    (*C1)->Crossover(C3,*C2);
  }
  #ifdef RGADEBUG
    if(Debug) Debug->EndFunc("Crossover","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	inline void RInst<cInst,cChromo,cFit>::Mutation(void)
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
      if((*((*C)->Fitness))<(*WorstFitness))
      {
        WorstFitness=(*C)->Fitness;
        p=(*C);
      }
    (*p)=(*BestInPop);
    #ifdef RGADEBUG
      sprintf(Tmp,"Normal Mutation (BestInPop) -> Chromosome %u",p->Id);
	    if(Debug) Debug->PrintInfo(Tmp);
    #endif
    p->Mutation();
  }
  if(AgeBest==AgeNextBestMutation)
  {
    AgeNextBestMutation+=MaxBestPopAge;
    C=Chromosomes;
    WorstFitness= (*(C++))->Fitness;
    for(i=PopSize;--i;C++)
      if((*((*C)->Fitness))<(*WorstFitness))
      {
        WorstFitness=(*C)->Fitness;
        p=(*C);
      }
    (*p)=(*BestChromosome);
    #ifdef RGADEBUG
      sprintf(Tmp,"Strong Mutation (BestInPop) -> Chromosome %u",p->Id);
	    if(Debug) Debug->PrintInfo(Tmp);
    #endif
    p->Mutation();
  }
  #ifdef RGADEBUG
    if(Debug) Debug->EndFunc("Mutation","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	void RInst<cInst,cChromo,cFit>::Generation(void)
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
  #ifdef RGADEBUG
    if(Debug) Debug->EndFunc("Generation","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	void RInst<cInst,cChromo,cFit>::Run(void)
{
  #ifdef RGADEBUG
    if(Debug&&(!ExternBreak)) Debug->BeginFunc("Run","RInst");
  #endif
	EmitSig(sigGARun);
  if(!Gen) RandomConstruct();
	EmitSig(sigInteract);
  ExternBreak = false;
  while((!StopCondition())&&(!ExternBreak))
  {
    Generation();
    DisplayInfos();
		EmitSig(sigGAGen);
		EmitSig(sigInteract);
  }
  PostRun();
  #ifdef RGADEBUG
    if(Debug&&(!ExternBreak)) Debug->EndFunc("Run","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	bool RInst<cInst,cChromo,cFit>::Verify(void)
{
  unsigned int i;
  cChromo **C;
  bool Ok=true;

  #ifdef RGADEBUG
    if(Debug) Debug->BeginFunc("Verify","RInst");
  #endif
  for(i=PopSize+1,C=Chromosomes;--i;C++)
    if(!((*C)->Verify()))
      Ok=false;
  #ifdef RGADEBUG
		if(Debug)
		{
			if(Ok)
				Debug->PrintComment("Instance Ok");
			else
				Debug->PrintComment("!!! Instance not Ok !!!");
		}
    if(Debug) Debug->EndFunc("Verify","RInst");
  #endif
  return(Ok);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit>
	RInst<cInst,cChromo,cFit>::~RInst(void)
{
  cChromo **C;
  unsigned int i;

	EmitSig(sigGADone);
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
  delete RandomGen;
  #ifdef RGADEBUG
    if(Debug) Debug->EndApp("HGA","Pascal Francq");
  #endif
}