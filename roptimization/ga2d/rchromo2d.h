/*

	R Project Library

	RChromo2D.h

	Chromosome for 2D placement GA - Header

	(C) 1999-2001 by P. Francq.

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
#ifndef RChromo2DH
#define RChromo2DH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rga2d.h>
#include <rga2d/rgeoinfos.h>
using namespace RGA2D;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represents a chromosome for the 2D placement GA.
* @author Pascal Francq
* @short 2D GA chromosome.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	class RChromo2D : public RChromo<cInst,cChromo,cFit,cThreadData>, public RGeoInfos
{
protected:

	/**
	* The actual limits of the solution represented by the chromosome.
	*/
	RPoint ActLimits;

	/**
	* Heuristic used for the placement.
	*/
	RPlacementHeuristic *Heuristic;

	/**
	* A Grid representing the current placement.
	*/
	RGrid *Grid;

public:

	/**
	* Objects to place.
	*/
	RObj2D **Objs;

	/**
	* Number of objects to place.
	*/
	unsigned int NbObjs;

protected:

	unsigned int **OccupiedY;

	/**
	* Array to hold objects id. This is a "thread-dependent" data.
	*/
	unsigned int *thOrder;

	/**
	* Array to hold objects id. This is a "thread-dependent" data.
	*/
	unsigned int *thOrder2;

	/**
	* Temporary objects used in crossover and mutation. This is a
	* "thread-dependent" data.
	*/
	RObj2D **thObjs;

	/**
	* Number of temporary objects.
	*/
	unsigned int thNbObjs;

	/**
	* Temporary object container used for the crossover. This is a
	* "thread-dependent" data.
	*/
	RObj2DContainer *thObj1;

	/**
	* Temporary object container used for the crossover. This is a
	* "thread-dependent" data.
	*/
	RObj2DContainer *thObj2;

	/**
	* Temporary geometric informations. This is a "thread-dependent" data.
	*/
	RGeoInfos *thInfos;

public:

	/**
	* Point representing the limits for the placement.
	*/
	RPoint Limits;

	/**
	* Construct the chromosome.
	* @param inst           Pointer to the instance.
	* @param id             Identificator of the chromosome.
	*/
	RChromo2D(cInst *inst,unsigned int id) throw(bad_alloc);

	/**
	* This function initialises some important data, in particular Infos and
	* Selected.
	* @param thData         Pointer to the "thread-dependent" data of the chromosome.
	*/
	virtual void Init(cThreadData *thData) throw(bad_alloc);

	/**
	* The random construction uses the heuristic to place all the objects.
	* @return The function returns true if a solution has been constructed.
	*/
	virtual bool RandomConstruct(void);

	/**
	* Make the crossover for the chromosome. A set of objects is taken from the
	* first parent. Then, the heuristic is used to place this object and the
	* objects that aren't selected.
	* @param parent1        First parent used.
	* @param parent2        Second parent used.
	* @return The function returns true if the crossover has been done.
	*/
	virtual bool Crossover(cChromo *parent1,cChromo *parent2);

	/**
	* The mutation simply calls the heuristic with all the objects.
	* @return The function returns true if the mutation has been done.
	*/
	virtual bool Mutation(void);

	/**
	* This function verify the validity of the chromosome, in particular that
	* no polygons are overlaped.
	* @return The function returns true if the chromosome is valid.
	*/
	virtual bool Verify(void);

	/**
	* The assignment operator.
	*/
	virtual RChromo2D& operator=(const RChromo2D &chromo);

	/**
	* Gives the object at position (X,Y).
	* @param X              X Position.
	* @param Y              Y Position.
	* @return The function returns a pointer to the objects or 0.
	*/
	RObj2D* GetObj(RCoord X,RCoord Y);

	/**
	* Gives the geometric information of the object at position (X,Y).
	* @param X              X Position.
	* @param Y              Y Position.
	* @return The function returns a pointer to the information or 0.
	*/
	cInfo* GetInfo(RCoord X,RCoord Y);

	/**
	* Return the actual limits of the chromosome.
	*/
	RPoint& GetActLimits(void);

	/**
	* Return the number of levels.
	*/
	inline unsigned int GetNbLevels(void) { return(NbLevels); }

	/**
	* Return the ith level of the chromosome.
	*/
	RPoint& GetLevel(unsigned int i);

	/**
	* Destruct the chromosome.
	*/
	virtual ~RChromo2D(void);
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
