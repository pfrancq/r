/*

	Rainbow Library Project

	RChromo2D.h

	Chromosome for 2D placement GA - Header

	(C) 1999-2000 by P. Francq.

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



//---------------------------------------------------------------------------
#ifndef RChromo2DH
#define RChromo2DH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* This class represents a chromosome for the 2D placement GA.
* @author Pascal Francq
* @short 2D GA chromosome.
*/
template<class cInst,class cChromo,class cFit,class cThreadData,class cInfo>
	class RChromo2D : public RChromo<cInst,cChromo,cFit,cThreadData>
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

public:

	/**
	* Geometric information of the objects.
	*/
	cInfo **Infos;

	/**
	* Which objects were selected for crossover.
	*/
	bool *Selected;

	/**
	* Point representing the limits for the placement.
	*/
	RPoint Limits;

	/**
	* Construct the chromosome.
	* @param inst			Pointer to the instance.
	* @param id				Identificator of the chromosome.
	*/
	RChromo2D(cInst *inst,unsigned int id) throw(bad_alloc);

	/**
	* This function initialises some important data, in particular Infos and
	* Selected.
	* @param thData		Pointer to the "thread-dependent" data of the chromosome.
	*/
	virtual void Init(cThreadData *thData) throw(bad_alloc);

	/**
	* This inline function clears all the geometric information. All the
	* positions of the two grids are put to NoObject, and each information is
	* place to (MaxCoord,MaxCoord) which is considered as an invalid position.
	*/
	inline void ClearInfos(void);

	/**
	* This inline function clears all the temporary variables needed for the
	* crossover.
	*/
	inline void thVarsClear(void);

	/**
	* This function constructs a set of objects that are not already selected for
	* a crossover.
	*
	*	First, an unselected object is chosen by random. After that, the function
	* try to add the objects that are placed near the selected object.
	* @param obj				Container object for the selected objects.
	* @param selected		Array of booleans to hold if an object is already
											selected.
	* @return						The function returns true if an object was constructed.
	*/
	bool GetSetOfObjs(RObj2DContainer *obj,bool *selected);

	/**
	* This function add all the unselected objects to thObjs and calculate the
	* corresponding number.
	* @param	objs			The array of pointers to objects to be filled.
	*	@param	nbobjs		The total number of objects in the array.
	*/
	void FillObjs(RObj2D **objs,unsigned int &nbobjs);

	/**
	* The random construction uses the heuristic to place all the objects.
	* @return		The function returns true if a solution has been constructed.
	*/
	virtual bool RandomConstruct(void);

	/**
	* Make the crossover for the chromosome. A set of objects is taken from the
	* first parent. Then, the heuristic is used to place this object and the
	* objects that aren't selected.
	* @param parent1	First parent used.
	* @param parent2	Second parent used (Actually, no information is taken
	*									from this parent).
	* @return 				The function returns true if the crossover has been done.
	*/
	virtual bool Crossover(cChromo *parent1,cChromo *parent2);

	/**
	* The mutation simply calls the heuristic with all the objects.
	* @return		The function returns true if the mutation has been done.
	*/
	virtual bool Mutation(void);

	/**
	* This function verify the validity of the chromosome, in particular that
	*	no polygons are overlaped.
	* @return		The function returns true if the chromosome is valid.
	*/
	virtual bool Verify(void);

	/**
	* The assignment operator.
	*/
	virtual RChromo2D& operator=(const RChromo2D &chromo);

	/**
	* Gives the object at position (X,Y).
	* @param X		X Position.
	* @param Y		Y Position.
	* @return			The function returns a pointer to the objects or 0.
	*/
	RObj2D* GetObj(RCoord X,RCoord Y);

	/**
	* Gives the geometric information of the object at position (X,Y).
	* @param X		X Position.
	* @param Y		Y Position.
	* @return			The function returns a pointer to the information or 0.
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


}//------- End of namespace RGA ---------------------------------------------


//---------------------------------------------------------------------------
#endif
