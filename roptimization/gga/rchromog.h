/*

	RChromoG.h

	Class representing a chromosome of a GGA - Header

	(C) 2001 by P. Francq.

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
#ifndef RChromoGH
#define RChromoGH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rga/rchromo.h>
#include <rgga/rgga.h>
#include <rgga/rgroups.h>
#include <rgga/rgroupingheuristic.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RChromoG class provides a representation for a chromosome for the GGA.
* @author Pascal Francq
* @short GGA Chromosome.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj,class cGroupData>
	class RChromoG : public RGA::RChromo<cInst,cChromo,cFit,cThreadData>, public RGroups<cGroup,cObj,cGroupData,cChromo>
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
	RChromoG(cInst* inst,unsigned int id) throw(bad_alloc);

	/**
	* Initialisation of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data of the
	*                       chromosome.
	*/
	virtual void Init(cThreadData *thData) throw(bad_alloc);

	/**
	* Clear all the information of the chromosome.
	*/
	void Clear(void);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual bool RandomConstruct(void);

	/**
	* Do a crossover by using the chromosome as child. The crossover
	* implemented is the BPX (bin packing crossover). The groups of parent1
	* containing objects that are inserted from parent2 are removed. Also, if
	* a group of parent1 is not compatible (IsCompatible) with one of the
	* groups inseeted from parent2 it is also removed.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	* @return The function must return true if the crossover has been done.
	*/
	virtual bool Crossover(cChromo* parent1,cChromo* parent2);

	/**
	* Do a mutation of the chromosome, by choosing randomly groups and
	* destroy them.
	*/
	virtual bool Mutation(void);

	/**
	* Do a inversion of the chromosome, by exchanging two groups in list
	* representing all the used one.
	*/
	virtual bool Inversion(void);

	/**
	* Perform a local optimisation. This function is called by the crossover
	* and the mutation operators just before the use of the heuristic to find
	* a group for the objects not yet assigned.
	*/
	virtual void LocalOptimisation(void) {}

	/**
	* Modify a given chromosome when it is identical to another one. By
	* default, the mutation operator of the chromosome is called.
	* destroy them.
	*/
	virtual bool Modify(void);

	/**
	* Verify the validity of the chromosome.
	* @return True if the chromosome is a valid one, false else.
	*/
	virtual bool Verify(void);

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


//-----------------------------------------------------------------------------
// inline implementation
#include <rgga/rchromog.hh>


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
