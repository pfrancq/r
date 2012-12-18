/*

	R Project Library

	RChromoH.h

	Class representing a tree (chromosome) - Header

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RChromoH_H
#define RChromoH_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rchromo.h>
#include <rnodesga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RChromoH class proposes a generic chromosome for a Hierarchical Genetic
 * Algorithm (HGA).
 * @author Pascal Francq
 * @short HGA Chromosome.
 */
template<class cInst,class cChromo,class cFit,class cThreadData,class cNode,class cObj>
	class RChromoH : public RChromo<cInst,cChromo,cFit,cThreadData>, public RNodesGA<cNode,cObj,cChromo>
{
public:

	using RChromo<cInst,cChromo,cFit,cThreadData>::Instance;
	using RChromo<cInst,cChromo,cFit,cThreadData>::Id;
	using RNodesGA<cNode,cObj,cChromo>::Nodes;
	using RNodesGA<cNode,cObj,cChromo>::ReserveNode;
	using RNodesGA<cNode,cObj,cChromo>::ReleaseNode;
	using RNodesGA<cNode,cObj,cChromo>::InsertNode;
	using RNodesGA<cNode,cObj,cChromo>::GetNbNodes;
	using RNodesGA<cNode,cObj,cChromo>::BuildFile;
   using RNodesGA<cNode,cObj,cChromo>::Clear;
	using RNodesGA<cNode,cObj,cChromo>::CopyExceptBranch;

protected:

	/**
	* Heuristic Used.
	*/
	RTreeHeuristic<cNode,cObj,cChromo>* Heuristic;

	/**
	* Use for Nodes manipulation functions. Thread-depend data.
	*/
	cNode** thNodes1;

	/**
	* Use for Nodes manipulation functions. Thread-depend data.
	*/
	cNode** thNodes2;

	/**
	 * Use for objects manipulation functions. Thread-depend data.
	 */
	RNumContainer<size_t,true>* thObjects;

public:

	/**
	* Construct the chromosome.
	* @param inst           The instance.
	* @param id             Identifier of the chromosome.
	*/
	RChromoH(cInst* inst,size_t id);

	/**
	* Initialization of the chromosome.
	* @param thData         Pointer to the "thread-dependent" data of the
	*                       chromosome.
	*/
	virtual void Init(cThreadData* thData);

	/**
	* Clear all the information of the chromosome.
	*/
	virtual void Clear(void);

	/**
	* Clean up a chromosome by removing all the empty nodes.
	*/
	virtual void CleanUp(void);

	/**
	* Construct a valid solution.
	*/
	virtual void RandomConstruct(void);

	/**
	* Do a crossover by using the chromosome as child:
	* 1. Two nodes having the same attributes are chosen in each parent
	*    (crossing nodes).
	* 2. The first parent is copied except its nodes having objects attached to
	*    the node of the second parent and its crossing node.
	* 3. The node of the second parent is copied at the place of the node of
	*    the first parent.
	* 4. The objects which are not attached are placed using the heuristic.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	*/
	virtual void Crossover(cChromo* parent1,cChromo* parent2);

	/**
	* Do a mutation with the chromosome. Choose randomly a node and remove it
	* with all its subnodes.
	* @return               The function must return true if the mutation has been done.
	*/
	virtual void Mutation(void);

	/**
	* Print information on screen.
	*/
	virtual void Print(void);

	/**
	* Verify the validity of the chromosome.
	*/
	virtual void Verify(void);

	/**
	* The assignment operator.
	* @param chromo         The chromosome used as source.
	*/
	virtual void Copy(const cChromo& chromo);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RChromoH(void);
};


//------------------------------------------------------------------------------
// template implementation
#include <rchromoh.hh>


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif
