/*

	R Project Library

	RChromo.h

	Chromosomes of Genetic Algorithms - Header

	Copyright 1998-2008 by the Université Libre de Bruxelles.

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
#ifndef RChromo_H
#define RChromo_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* \ingroup GA
* Chromosome for the generic GA.
* @author Pascal Francq
* @short Generic chromosome.
*/
template<class cInst,class cChromo,class cFit,class cThreadData>
	class RChromo
{
public:

	/**
	* Pointer to the instance.
	*/
	cInst* Instance;

	/**
	* Identificator of the chromosome.
	*/
	size_t Id;

	/**
	* Pointer to the fitness of the chromosome.
	*/
	cFit* Fitness;

	/**
	* Variable to indicate if the chromosome has to be evaluated again,
	* for example after a crossover or a mutation.
	*/
	bool ToEval;

	/** Construct the chromosome.
	* @param inst           Pointer to the instance.
	* @param id             Identificator of the chromosome.
	*/
	RChromo(cInst* inst,size_t id);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data of the chromosome.
	*/
	virtual void Init(cThreadData* thData);

	/**
	* Construct a valid solution.
	*/
	virtual void RandomConstruct(void) {}

	/**
	* Evaluate the fitness of the chromosome.
	*/
	virtual void Evaluate(void) {}

	/**
	* Do a crossover by using the chromosome as child.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	*/
	virtual void Crossover(cChromo* parent1,cChromo* parent2);

	/**
	* Do a mutation with the chromosome.
	*/
	virtual void Mutation(void) {}

	/**
	* Do an inversion with the chromosome.
	*/
	virtual void Inversion(void) {}

	/**
	* Perform an optimisation. This function is called every time a chromosome
	* is modified by the crossover, inversion, or the mutation operator.
	*/
	virtual void Optimisation(void)  {}

	/**
	* Print information on screen.
	*/
	virtual void Print(void) {}

	/**
	* Verify the validity of the chromosome.
	*/
	virtual void Verify(void) {}

	/**
	* The assignment operator.
	*/
	virtual RChromo& operator=(const RChromo &chromo);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RChromo(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rchromo.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
