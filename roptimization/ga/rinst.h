/*

	R Project Library

	RInst.h

	Instance of Genetic Algorithms - Header

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
#ifndef RInst_H
#define RInst_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>
#include <rdebug.h>
#include <rcursor.h>
#include <robject.h>
#include <rnotification.h>


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
	class RInst : public RObject
{
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
	* This variable is true if the random construction where done.
	*/
	bool bRandomConstruct;

	/**
	* Set if the GA must verify the chromosomes during the different steps of
	* the run.
	*/
	bool VerifyGA;

	/**
	 * Define if the post evalutation must be done.
	 */
	bool DoPostEvaluation;
	
public:

	/**
	* The chromosomes.
	*/
	cChromo** Chromosomes;

	/**
	 * The chromosomes ranked by fitness.
	 */
	cChromo** Ranked;
	
	/**
	* The best chromosome ever calculated.
	*/
	cChromo* BestChromosome;

	/**
	* The best chromosome in the population.
	*/
	cChromo* BestInPop;

private:
	
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
	* Age of the best chromosome ever calculate.
	*/
	unsigned int AgeBestPop;

	/**
	* Number of crossovers to be done in a generation.
	*/
	unsigned int NbCross;

	/**
	* Frequence of mutation.
	*/
	unsigned int FreqMutation;

	/**
	* Frequence of mutation of the best chromosome.
	*/
	unsigned int FreqBestMutation;

	/**
	 * Number of mutations to do at each mutation operation.
	 */
	unsigned int NbMutations;
	
	/**
	* Frequence of inversion.
	*/
	unsigned int FreqInversion;
		
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

public:
	
	/**
	* Construct the instance.
	* @param popsize        The size of the population.
	* @param name           Name of the genetic algorithm.
	* @param debug          Debugger.
	*/
	RInst(unsigned int popsize,const RString& name,RDebug* debug=0);

	/**
	 * Return the name of the class.
	 */
	virtual RCString GetClassName(void) const {return("RInst");}

	/**
	 * Set the parameters of the mutation operator.
	 * @param agemut         Number of generations between mutations.
	 * @param agebestmut     Number of generations between mutations of the
	 *                       best chromosome.
	 * @param nbmut          Number of mutations to do at each mutation operation.
	 */
	void SetMutationParams(unsigned int agemut,unsigned int agebestmut,unsigned int nbmut);
	 
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
	 * Get the size of the population.
	 */
	inline unsigned int GetPopSize(void) const {return(PopSize);}
	
	/**
	 * Get the number of generations run.
	 */
	inline unsigned int GetGen(void) const {return(Gen);}

	/**
	* Get the age of the best chromosome.
	*/
	inline unsigned int GetAgeBest(void) const {return(AgeBest);}

	/**
	* Get the age of the best chromosome ever calculate.
	*/
	inline unsigned int GetAgeBestPop(void) const {return(AgeBestPop);}
	
	/**
	* Initialisation of the instance.
	*/
	virtual void Init(void);

	/**
	* This function is used to make a sort of the chromosomes based on their
	* fitness where the best chromosomes are at the top.
	* @param a              The first chromosome.
	* @param b              The second chromosome.
	* @return a-b
	*/
	static int sort_function_cChromosome(const void *a,const void *b);

	/**
	* Analyse the population to find the best chromosome of the population and
	* to verify if the best chromosome ever calculated has to replaced. When
	* needed, the chromosomes are evaluated.
	*/
	virtual void AnalysePop(void);
	
	/**
	* Do some post evluation traitment. It can be used to implement a
	* multi-criteria approach like PROMETHEE to classify the chromosomes.
	*/
	virtual void PostEvaluate(void) {}
		
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
	* This is the handler that is called when an object does not find any
	* handler for a sended notification. By default, it prints the name of the
	* message and the generation numbers.
	* @param notification    Notification.
	*/
	virtual void HandlerNotFound(const RNotification& notification);
	
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
