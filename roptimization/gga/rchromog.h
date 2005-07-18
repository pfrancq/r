/*

	RChromoG.h

	Class representing a chromosome of a GGA - Header

	Copyright 2001-2003 by the Universit�Libre de Bruxelles.

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
#ifndef RChromoG_H
#define RChromoG_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rchromo.h>
#include <rgga.h>
#include <rgroups.h>
#include <rgroupingheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RChromoG class provides a representation for a chromosome for the GGA.
* @author Pascal Francq
* @short GGA Chromosome.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	class RChromoG : public RChromo<cInst,cChromo,cFit,cThreadData>, public RGroups<cGroup,cObj,cGroupData,cChromo>
{
protected:

	/**
	* Heuristic used for the groupment.
	*/
	RGroupingHeuristic<cGroup,cObj,cGroupData,cChromo>* Heuristic;

public:

	/**
	* Construct the chromosome.
	* @param inst           The instance.
	* @param id             Identificator of the chromosome.
	*/
	RChromoG(cInst* inst,unsigned int id) throw(std::bad_alloc);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data of the
	*                       chromosome.
	*/
	virtual void Init(cThreadData *thData) throw(std::bad_alloc);

	/**
	* Clear all the information of the chromosome.
	*/
	void Clear(void);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual void RandomConstruct(void) throw(eGA);

private:

	/**
	* This method simply copies nb2 groups of parent2 beginning at pos2 and
	* verifies if some objects are in one of the nb1 groups of parent1
	* beginning at pos1.
	*
	* In regards of the parameter of the instance, the groups of parent2
	* containing objects of parent1 are copied (and the common objects not) or
	* not.
	*/
	void CopyGroups(cChromo* parent1,cChromo* parent2,unsigned int pos1,unsigned int nb1,unsigned int pos2,unsigned int nb2);

public:

	/**
	* Do a crossover by using the chromosome as child. The crossover
	* implemented is the BPX (bin packing crossover). The groups of parent1
	* containing objects that are inserted from parent2 are removed. Also, if
	* a group of parent1 is not compatible (IsCompatible) with one of the
	* groups inseeted from parent2 it is also removed.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	*/
	virtual void Crossover(cChromo* parent1,cChromo* parent2) throw(eGA);

	/**
	* Do a mutation of the chromosome, by choosing randomly groups and
	* destroy them.
	*/
	virtual void Mutation(void) throw(eGA);

	/**
	* Do a inversion of the chromosome, by exchanging two groups in list
	* representing all the used one.
	*/
	virtual void Inversion(void) throw(eGA);

	/**
	* Perform a local optimisation. This function is called by the crossover
	* and the mutation operators just before the use of the heuristic to find
	* a group for the objects not yet assigned.
	*/
	virtual void LocalOptimisation(void) throw(eGA) {}

	/**
	* Perform an optimisation. This function is called at the end of the
	* crossover and the mutation operators.
	*/
	virtual void Optimisation(void) throw(eGA) {}

	/**
	* Modify a given chromosome when it is identical to another one. By
	* default, the mutation operator of the chromosome is called.
	* destroy them.
	*/
	virtual void Modify(void) throw(eGA);

	/**
	* Verify the validity of the chromosome.
	*/
	virtual void Verify(void) throw(eGA);

	/**
	* The assigment operator.
	* @param chromo         The chromosome used as source.
	*/
	RChromoG& operator=(const RChromoG& chromo);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RChromoG(void);

	friend class RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj,cGroupData>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rchromog.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
