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
#ifndef RChromogH
#define RChromogH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rchromo.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
template<class cInst,class cChromo,class cFit,class cThreadData,class cGroup,class cObj>
	class RChromoG : public RGA::RChromo<cInst,cChromo,cFit,cThreadData>
{
	/**
	* The Objects to group.
	*/
	cObj** Objects;

	/**
	* The groups of the chromosomes.
	*/
	cGroup** Groups;

	/**
	* Total number of groups used.
	*/
	unsigned int NbGroups;

	/**
	* Maximal number of groups to allocate.
	*/
	unsigned int MaxGroups;

	/**
	*  Array of idenfiticators of groups.
	*/
	unsigned int* GroupsList;

	/**
	* Number of groups in the list.
	*/
	unsigned int NbGroupsList;

	/**
	* Assignment of the objects.
	*/
	unsigned int* ObjectsAss;

	/**
	* Array of identificator of objects.
	*/
	unsigned int* ObjectsList;

	/**
	* Number of objects in the list.
	*/
	unsigned int NbObjectsList;

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
	* Reserve a Node.
	* @returns A pointer to the node reserved.
	*/
	cGroup* ReserveGroup(void);

	/**
	* Release a node.
	* @param node           Identificator of the node.
	*/
	void ReleaseGroup(const unsigned int group);

	/**
	* Insert an object to a node.
	* @param to             Pointer of the destination node.
	* @param obj            Identificator of the object to insert.
	*/
	void InsertObj(cGroup* to,const unsigned int obj);

	/**
	* Delete an object from a node.
	* @param from           Pointer of the node.
	* @param obj            Identificator of the object to insert.
	*/
	void DeleteObj(cGroup* from,const unsigned int obj);

	/**
	* Delete all the objects attached to a given node.
	* @param node           Pointer to the node.
	*/
	void DeleteObjs(cGroup* node);

	/**
	* Move all the objects of a node to another.
	* @param to             Pointer of the destination node.
	* @param from           Pointer of the origin node.
	*/
	void MoveObjs(cGroup* to,cGroup* from);

	/**
	* Construct a valid solution.
	* @return The function must retrun true if a solution has been constructed.
	*/
	virtual bool RandomConstruct(void);

	/**
	* Do a crossover by using the chromosome as child.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	* @return The function must return true if the crossover has been done.
	*/
	virtual bool Crossover(cChromo* parent1,cChromo* parent2);

	/**
	* Do a mutation with the chromosome.
	* @return               The function must return true if the mutation has been done.
	*/
	virtual bool Mutation(void);

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
	
	friend class RInstG<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
	friend class RGroup<cInst,cChromo,cFit,cThreadData,cGroup,cObj>;
};


}//------- End of namespace RGGA ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
