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

*/



//---------------------------------------------------------------------------
//
// RInst
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> RInst<cInst,cChromo,cFit>::RInst(unsigned P) throw(bad_alloc)
{
  cChromo **C;
  unsigned i;

  #ifdef DEBUG
    DebugOutput.Init(DebugOutputFileName);
    DebugOutput.BeginApp("HGA","Pascal Francq");
    DebugOutput.BeginFunc("RInst","RInst");
  #endif
  PopSize=P;
  AgeBestPop=AgeBest=Gen=0;
  AgeNextMutation=MaxBestPopAge;
  AgeNextBestMutation=MaxBestAge;
  Chromosomes=new cChromo*[PopSize];
  memset(Chromosomes,0,PopSize*sizeof(cChromo*));
  NbCross=PopSize/4;
  Childs=new cChromo*[2*NbCross];
  Parents=new cChromo*[2*NbCross];
  tmpChrom=new cChromo*[2*PopSize];
  BestChromosome=NULL;
  RandomGen = new RRandomGood(12345);
  for(i=0,C=Chromosomes;i<PopSize;C++,i++)
    (*C)=new cChromo(((cInst*)this),i);
  BestChromosome=new cChromo(((cInst*)this),PopSize);
  #ifdef DEBUG
    DebugOutput.EndFunc("RInst","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> bool RInst<cInst,cChromo,cFit>::RandomConstruct(void)
{
  cChromo **C=Chromosomes,*p;

  #ifdef DEBUG
    DebugOutput.BeginFunc("RandomConstruct","RInst");
  #endif
  p=(*(C++));
  p->RandomConstruct();
  p->Evaluate();
  p->ToEval=false;
  BestInPop=p;
  for(unsigned i=PopSize;--i;C++)
  {
    p=(*C);
    p->RandomConstruct();
    p->Evaluate();
    p->ToEval=false;
    if((*(p->Fitness))>(*(BestInPop->Fitness))) BestInPop=p;
  }
  (*BestChromosome)=(*BestInPop);
  #ifdef DEBUG
    DebugOutput.EndFunc("RandomConstruct","RInst");
  #endif
  return(true);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> inline void RInst<cInst,cChromo,cFit>::Evaluate(void)
{
  cChromo **C=Chromosomes,*p,*tmp;

  #ifdef DEBUG
    DebugOutput.BeginFunc("Evaluate","RInst");
  #endif
  p=(*(C++));
  p->Evaluate();
  p->ToEval=false;
  tmp=p;
  for(unsigned i=PopSize;--i;C++)
  {
    p=*C;
    if(p->ToEval)
    {
      p->Evaluate();
      p->ToEval=false;
    }
    if((*(p->Fitness))>(*(BestInPop->Fitness))) tmp=p;
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
  }
  #ifdef DEBUG
    DebugOutput.EndFunc("Evaluate","RInst");
  #endif
}


//--------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> int RInst<cInst,cChromo,cFit>::sort_function_cChromosome( const void *a, const void *b)
{
  cFit *af=(*((cChromo**)a))->Fitness;
  cFit *bf=(*((cChromo**)b))->Fitness;

  if((*af)==(*bf)) return(0);
  if((*af)>(*bf))
    return(-1);
  else
    return(1);
}


//---------------------------------------------------------------------------
// The bests chromosomes are crossovered
template<class cInst,class cChromo,class cFit> inline void RInst<cInst,cChromo,cFit>::Crossover(void)
{
  unsigned i;
  cChromo **C1,**C2,*C3;

  #ifdef DEBUG
    DebugOutput.BeginFunc("Crossover","RInst");
  #endif
  // Determine the childs (end of tmpChrom) and the parents (begin of tmpChrom)
  if(PopSize<4) return;
  memcpy(tmpChrom,Chromosomes,sizeof(cChromo*)*PopSize);
  qsort((void*)tmpChrom,PopSize,sizeof(cChromo*),sort_function_cChromosome);

  // Make the crossovers
  randorder<cChromo*>(tmpChrom,2*NbCross);
  for(C1=&tmpChrom[PopSize-1],C2=tmpChrom,i=NbCross+1;--i;C1--,C2++) // A changer au cas où crossover pas possible
  {
    C3=(*(C2++));
    #ifdef DEBUG
      static char Tmp[200];
      sprintf(Tmp,"Parent %u + Parent %u -> Child %u",(*C2)->Id,C3->Id,(*C1)->Id);
      DebugOutput.Print(Tmp);
    #endif
    (*(C1--))->Crossover(*C2,C3);
    #ifdef DEBUG
      sprintf(Tmp,"Parent %u + Parent %u -> Child %u",C3->Id,(*C2)->Id,(*C1)->Id);
      DebugOutput.Print(Tmp);
    #endif
    (*C1)->Crossover(C3,*C2);
  }
  #ifdef DEBUG
    DebugOutput.EndFunc("Crossover","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> inline void RInst<cInst,cChromo,cFit>::Mutation(void)
{
  unsigned i;
  cFit *WorstFitness=BestChromosome->NewFitness();
  cChromo **C,*p;

  #ifdef DEBUG
    static char Tmp[200];
    DebugOutput.BeginFunc("Mutation","RInst");
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
    #ifdef DEBUG
      sprintf(Tmp,"Normal Mutation (BestInPop) -> Chromosome %u",p->Id);
      DebugOutput.Print(Tmp);
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
    #ifdef DEBUG
      sprintf(Tmp,"Strong Mutation (BestInPop) -> Chromosome %u",p->Id);
      DebugOutput.Print(Tmp);
    #endif
    p->Mutation();
  }
  #ifdef DEBUG
    DebugOutput.EndFunc("Mutation","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> void RInst<cInst,cChromo,cFit>::Generation(void)
{
  #ifdef DEBUG
    DebugOutput.BeginFunc("Generation","RInst");
  #endif
  Gen++;
  AgeBest++;
  AgeBestPop++;
  #ifdef DEBUG
    static char tmp[200];
    sprintf(tmp,"Gen=%u  ;  AgeBest=%u  ;  AgeBestPop=%u",Gen,AgeBest,AgeBestPop);
    DebugOutput.Print(tmp);
  #endif
  Crossover();
  Mutation();
  Evaluate();
  #ifdef DEBUG
    DebugOutput.EndFunc("Generation","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> void RInst<cInst,cChromo,cFit>::Run(void)
{
  #ifdef DEBUG
    DebugOutput.BeginFunc("Run","RInst");
  #endif
    if(!Gen) RandomConstruct();
  ExternBreak = false;
    while((!StopCondition())&&(!ExternBreak))
  {
    Generation();
    DisplayInfos();
  }
  PostRun();
  #ifdef DEBUG
    DebugOutput.EndFunc("Run","RInst");
  #endif
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> bool RInst<cInst,cChromo,cFit>::Verify(void)
{
  unsigned i;
  cChromo **C;
  bool Ok=true;

  #ifdef DEBUG
    DebugOutput.BeginFunc("Verify","RInst");
  #endif
  for(i=PopSize+1,C=Chromosomes;--i;C++)
    if(!((*C)->Verify()))
      Ok=false;
  #ifdef DEBUG
    if(!Ok) DebugOutput.Print("Instance not Ok");
    DebugOutput.EndFunc("Verify","RInst");
  #endif
  return(Ok);
}


//---------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit> RInst<cInst,cChromo,cFit>::~RInst(void)
{
  cChromo **C;
  unsigned i;

  for(i=PopSize+1,C=Chromosomes;--i;C++)
    if(*C) delete (*C);
  delete[] Chromosomes;
  if(BestChromosome) delete BestChromosome;
  delete[] Childs;
  delete[] Parents;
  delete[] tmpChrom;
  delete RandomGen;
  #ifdef DEBUG
    DebugOutput.EndApp("HGA","Pascal Francq");
    DebugOutput.Done();
  #endif
}