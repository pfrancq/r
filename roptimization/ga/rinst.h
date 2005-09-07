/*

	R Project Library

	RInst.h

	Instance of Genetic Algorithms - Header

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
#ifndef RInst_H
#define RInst_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>
#include <rdebug.h>
#include <rgasignals.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represent "thread-dependent" data for the GA.
* @author Pascal Francq
* @short GA "thread-dependent" Data.
*/
template<class cInst,class cChromo>
	class RThreadData
{
public:

	/**
	* Instance of the problem.
	*/
	cInst* Owner;

	/**
	* Construct the data.
	* @param owner          The instance of the problem.
	*/
	RThreadData(cInst *owner);

	/**
	* Initialise the data.
	*/
	virtual void Init(void) {}

	/**
	* Destruct the data.
	*/
	virtual ~RThreadData() {}
};


//------------------------------------------------------------------------------
/**
* Instance for the generic GA.
* @author Pascal Francq
* @short Generic GA instance.
*/
template<class cInst,class cChromo,class cFit,class cThreadData>
	class RInst
{
	/**
	* This function is used to make a sort of the chromosomes based on their fitness.
	* @param a              The first chromosome.
	* @param b              The second chromosome.
	* @return a-b
	*/
	static int sort_function_cChromosome(const void *a,const void *b);

public:

	/**
	* Pointer to a "debugger" holding information about the GA when it is running.
	*/
	RDebug* Debug;

	/**
	* Random number generator
	*/
	RRandom* Random;

private:

	/**
	* Temporary data for internal use.
	*/
	cChromo** tmpChrom1;

	/**
	* Temporary data for internal use.
	*/
	cChromo** tmpChrom2;

protected:

	/**
	* "Thread-dependent" data of the instance.
	*/
	cThreadData** thDatas;

	/**
	* Container of all objects that will be receive signals from the GA.
	*/
	RContainer<RGASignalsReceiver<cInst,cChromo,cFit>,false,false> Receivers;

	/**
	* This variable is true if the random construction where done.
	*/
	bool bRandomConstruct;

	/**
	* Set if the GA must verify the chromosomes during the different steps of
	* the run.
	*/
	bool VerifyGA;

public:

	/**
	* The chromosomes.
	*/
	cChromo** Chromosomes;

	/**
	* The best chromosome ever calculated.
	*/
	cChromo* BestChromosome;

	/**
	* The best chromosome in the population.
	*/
	cChromo* BestInPop;

	/**
	* Size of the population.
	*/
	unsigned int PopSize;

	/**
	* Number of generations.
	*/
	unsigned int Gen;

	/**
	* Age of the best chromosome.
	*/
	unsigned int AgeBest;

	/**
	* Number of crossovers to be done in a generation.
	*/
	unsigned int NbCross;

	/**
	* Number of generation left before next mutation.
	*/
	unsigned int AgeNextMutation;

	/**
	* Number of generation left before mutation of the best chromosome.
	*/
	unsigned int AgeNextBestMutation;

	/**
	* Number of generation left before next inversion.
	*/
	unsigned int AgeNextInversion;

	/**
	* Frequence of inversion.
	*/
	unsigned int FreqInversion;

	/**
	* Age of the best chromosome ever calculate.
	*/
	unsigned int AgeBestPop;

	/**
	* Age of the best chromosome before insertion of mutation of it in the
	* population.
	*/
	unsigned int MaxBestPopAge;

	/**
	* Age of the best chromosome ever calculate before insertion of mutation of it
	* in the population.
	*/
	unsigned int MaxBestAge;

	/**
	* Construct the instance.
	* @param popsize        The size of the population.
	* @param debug          Debugger.
	*/
	RInst(unsigned int popsize,RDebug* debug=0);

	/**
	* Set if the verifications must be done.
	* @param verify          Verify?
	*/
	inline void SetVerify(bool verify) {VerifyGA=verify;}

	/**
	* See if the verifications are made by the GA.
	*/
	inline bool GetVerify(void) const {return(VerifyGA);}

	/**
	* Initialisation of the instance.
	*/
	virtual void Init(void);

	/**
	* Random construction of the chromosomes.
	* @return The function returns true if all chromosomes are constructed.
	*/
	virtual void RandomConstruct(void);

	/**
	* This function determines if the GA must stop. It is called after
	* each generation. This function is a pure one and has to be implement.
	* @return When the function returns true, the GA is stop.
	*/
	virtual bool StopCondition(void)=0;

	/**
	* This function can be used to do a traitement after the GA stops.
	*/
	virtual void PostRun(void) {}

	/**
	* This function can be used to display some information. It is called after
	* each generation.
	*/
	virtual void DisplayInfos(void) {}

	/**
	* This function does the evaluation of the chromosomes when it is needed.
	*/
	virtual void Evaluate(void);

	/**
	* Do some post evluation traitment. It can be used to implement a
	* multi-criteria approach like PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void) {}

	/**
	* Analyse the population to find the best chromosome of the population and
	* to verify if the best chromosome ever calculated has to replaced.
	*/
	virtual void AnalysePop(void);

	/**
	* This function does the crossovers for a generation. Actually, the
	* tournament strategy is implemented.
	*/
	virtual void Crossover(void);

	/**
	* This function does the necessary mutations for a generation.
	*/
	virtual void Mutation(void);

	/**
	* This function does the necessary inversions for a generation.
	*/
	virtual void Inversion(void);

	/**
	* This function does a generation.
	*/
	void Generation(void);

	/**
	* This functions runs the GA.
	*/
	void Run(void);

	/**
	* This functions verifies all the chromosomes after each generation.
	*/
	virtual void Verify(void);

	/**
	* Add a receiver to the list of signals receivers.
	*/
	inline void AddReceiver(RGASignalsReceiver<cInst,cChromo,cFit> *rec);

	/**
	* Delete a receiver from the list of signals receivers.
	*/
	inline void DelReceiver(RGASignalsReceiver<cInst,cChromo,cFit> *rec);

	/**
	* Emit a Generation Signal.
	*/
	void emitGenSig(void);

	/**
	* Emit an Interact Signal.
	*/
	void emitInteractSig(void);

	/**
	* Emit a "Best Chromosome" Signal.
	*/
	void emitBestSig(void);

	/**
	* Return a number in the interval [0,max[ using the current random generator.
	* @param max            Variable used to calculate the number.
	*/
	long RRand(long max) {return(Random->Value(max));}

	/**
	* Random the position of elements of a vector using the current random generator.
	* @param arr            A pointer to the array representing the vector.
	* @param size           The size of the vector.
	*/
	template<class T> inline void RandOrder(T* arr,unsigned int size)
		{Random->RandOrder<T>(arr,size);}

	/**
	* Destruct the instance.
	*/
	virtual ~RInst(void);

	/**
	* The instance is a master.
	*/
	static const char instMaster=0;

	/**
	* The instance is a slave.
	*/
	static const char instSlave=1;

	/**
	* The instance can use multi-processors.
	*/
	static const char instMultiProcess=2;

	/**
	* The instance can use slaves on other computers.
	*/
	static const char instMultiComputers=4;

	/**
	* The echo of the slaves is received by the master.
	*/
	static const char instMultiEchos=8;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rinst.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
