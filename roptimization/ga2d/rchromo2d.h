/*

	R Project Library

	RChromo2D.h

	Chromosome for 2D placement GA - Header

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RChromo2D_H
#define RChromo2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rchromo.h>
#include <rga2d.h>
#include <rlayout.h>
#include <rplacementheuristic.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a chromosome for the 2D placement GA.
* @short 2D GA chromosome.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	class RChromo2D : public RChromo<cInst,cChromo,cFit,cThreadData>, public RLayout
{
protected:
	using RChromo<cInst,cChromo,cFit,cThreadData>::Id;
	using RChromo<cInst,cChromo,cFit,cThreadData>::Instance;

	/**
	* The actual limits of the solution represented by the chromosome.
	*/
	RSize ActLimits;

	/**
	* Heuristic used for the placement.
	*/
	RPlacementHeuristic* Heuristic;

	/**
	* A Grid representing the current placement.
	*/
	RGrid* Grid;

protected:

	/**
	* Temporary array to remember which objects are selected.
	*/
	bool* Selected;

	/**
	 * PROMETHEE kernel.
	 */
	RPromKernel* Kernel;

public:

	/**
	* Construct the chromosome.
	* @param inst           Pointer to the instance.
	* @param id             Identifier of the chromosome.
	*/
	RChromo2D(cInst* inst,size_t id);

	/**
	* This function initializes some important data, in particular Infos and
	* Selected.
	* @param thData         Pointer to the "thread-dependent" data of the chromosome.
	*/
	virtual void Init(cThreadData* thData);

	/**
	* The random construction uses the heuristic to place all the objects.
	*/
	virtual void RandomConstruct(void);

	/**
	* Make the crossover for the chromosome. A set of objects is taken from the
	* first parent. Then, the heuristic is used to place this object and the
	* objects that aren't selected.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	*/
	virtual void Crossover(cChromo* parent1,cChromo* parent2);

	/**
	* The mutation simply calls the heuristic with all the objects.
	*/
	virtual void Mutation(void);

	/**
	* This function verify the validity of the chromosome, in particular that
	* no polygons are overlapped.
	*/
	virtual void Verify(void);

	/**
	* The method copies a chromosome into another.
	* @param chromo          Chromosome.
	*/
	virtual void Copy(const cChromo& chromo);

	/**
	* Gives the object at position (X,Y).
	* @param X              X Position.
	* @param Y              Y Position.
	* @return The function returns a pointer to the objects or 0.
	*/
	RObj2D* GetObj(tCoord X,tCoord Y);

	/**
	* Gives the geometric information of the object at position (X,Y).
	* @param X              X Position.
	* @param Y              Y Position.
	* @return The function returns a pointer to the information or 0.
	*/
	cInfo* GetInfo(tCoord X,tCoord Y);

	/**
	* Return the actual limits of the chromosome.
	*/
	RPoint& GetActLimits(void);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RChromo2D(void);
};


//------------------------------------------------------------------------------
// Template implementation
#include <rchromo2d.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
