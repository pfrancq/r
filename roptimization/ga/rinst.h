/*

	R Project Library

	RInst.h

	Instance of Genetic Algorithms - Header

	(C) 1998-2000 by P. Francq.

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
#ifndef RInstH
#define RInstH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R project
#include <rga/rga.h>
using namespace RGA;


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* \ingroup GA
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
	cInst *Owner;

	/**
	* Construct the data.
	* @param owner		The instance of the problem.
	*/
	RThreadData(cInst *owner) throw(bad_alloc);

	/**
	* Initialise the data.
	*/
	virtual void Init(void) throw(bad_alloc) {}

	/**
	* Destruct the data.
	*/
	virtual ~RThreadData() {}
};


//-----------------------------------------------------------------------------
/**
* \ingroup GA
* Instance for the generic GA.
* @author Pascal Francq
* @short Generic instance.
*/
template<class cInst,class cChromo,class cFit,class cThreadData>
	class RInst
{
	/**
	* This function is used to make a sort of the chromosomes based on their fitness.
	* @param a	The first chromosome.
	* @param b	The second chromosome.
	* @return 	a-b
	*/
	static int sort_function_cChromosome( const void *a, const void *b);

	/**
	* Temporary data for internal use.
	*/
	cChromo **Parents;

	/**
	* Temporary data for internal use.
	*/
	cChromo **Childs;

	/**
	* Temporary data for internal use.
	*/
	cChromo **tmpChrom;

	/**
	* "Thread-dependent" data of the instance.
	*/
	cThreadData **thDatas;

	/**
	* Container of all objects that will be receive signals from the GA.
	*/
	RContainer<RGASignalsReceiver<cInst,cChromo,cFit>,unsigned int,false,false> Receivers;

	/**
	* This function is true if the randol construction where done.
	*/
	bool bRandomConstruct;

public:

	/**
	* The chromosomes.
	*/
	cChromo **Chromosomes;

	/**
	* The best chromosome ever calculated.
	*/
	cChromo *BestChromosome;

	/**
	* The best chromosome in the population.
	*/
	cChromo *BestInPop;

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
	* Age of the best chromosome ever calculate.
	*/
	unsigned int AgeBestPop;

	/**
	* Age of the best chromosome before insertion of mutation of it in the
	*	population.
	*/
	unsigned int MaxBestPopAge;

	/**
	* Age of the best chromosome ever calculate before insertion of mutation of it
	* in the population.*/
	unsigned int MaxBestAge;

	/**
	* Construct the instance.
	* @param popsize		The size of the population.
	*/
	RInst(unsigned int popsize) throw(bad_alloc);

	/**
	* Initialisation of the instance.
	*/
	virtual void Init(void) throw(bad_alloc);

	/**
	* Random construction of the chromosomes.
   * @return			The function returns true if all chromosomes are constructed.
	*/
	virtual void RandomConstruct(void) throw(eGA);

	/**
	* This function determines if the GA must stop. It is called after
	* each generation. This function is a pure one and has to be implement.
	* @return	When the function returns true, the GA is stop.
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
	inline void Evaluate(void) throw(eGA);

	/**
	* This function does the crossovers for a generation.
	*/
	inline void Crossover(void) throw(eGA);

	/**
	* This function does the necessary mutations for a generation.
	*/
	inline void Mutation(void) throw(eGA);

	/**
	* This function does a generation.
	*/
	void Generation(void) throw(eGA);

	/**
	* This functions runs the GA.
	*/
	void Run(void) throw(eGA);

	/**
	* This functions verifies all the chromosomes after each generation.
	*/
	virtual void Verify(void) throw(eGA);

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


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
