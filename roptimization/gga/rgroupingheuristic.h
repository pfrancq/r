/*

	R Project Library

	RGroupingHeuristic.h

	Generic Heuristic for Grouping - Header

	(C) 1998-2001 by P. Francq.

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
#ifndef RGroupingHeuristicH
#define RGroupingHeuristicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
using namespace RStd;
#include <rmath/random.h>
using namespace RMath;
#include <rga/robjs.h>
#include <rgga/rgroups.h>


//-----------------------------------------------------------------------------
namespace RGGA{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The RGroupingHeuristic class provides an abstract class for grouping
* heuristics.
* @author Pascal Francq
* @short Generic Grouping Heuristic class.
*/
template<class cGroup,class cObj,class cGroupData,class cGroups>
	class RGroupingHeuristic
{
protected:

	/**
	* Name of the heuristic.
	*/
	RStd::RString Name;

	/**
	* The random number generator to use.
	*/
	RMath::RRandom* Random;

	/**
	* Objects to be grouped.
	*/
	RStd::RCursor<cObj,unsigned int>* Objs;

	/**
	* Number of objects to be grouped.
	*/
	unsigned int NbObjs;

	/**
	* Number of objects already grouped.
	*/
	unsigned int NbObjsOk;

	/**
	* Groups.
	*/
	cGroups* Groups;
	
	/**
	* Order in which the objects are to be treated.
	*/
	cObj** Order;

	/**
	* Current object to place.
	*/
	cObj* CurObj;

	/**
	* Current group manipulated.
	*/
	cGroup* CurGroup;

public:

	/**
	* Construct the grouping heuristic.
	* @param n              Name of the heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	*/
	RGroupingHeuristic(const char* n,RRandom* r,RStd::RCursor<cObj,unsigned int>* objs);

	/**
	* Get the name of the heuristic.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}
	
	/**
	* Initialize the heuristic.
	* @param groups         Pointer to the groups.
	*/
	virtual void Init(cGroups* groups);

	/**
	* Select the next object to place.
	* The CurObj must pointed to object to place.
	*/
	virtual void SelectNextObject(void) throw(RGA::eGA);

	/**
	* Return the current object to place.
	*/
	cObj* GetNextObject(void) {return(CurObj);}

	/**
	* Return the current group.
	*/
	cGroup* GetGroup(void) {return(CurGroup);}

	/**
	* Find a group for the next object.
	*/
	virtual cGroup* FindGroup(void) throw(RGA::eGA)=0;

	/**
	* Put the next object into a group.
	*/
	void PutNextObject(void) throw(RGA::eGA);

	/**
	* Run the heuristic.
	* @param groups         Pointer to the groups.
	*/
	void Run(cGroups* groups) throw(RGA::eGA);

	/**
	* Do some operations after the run.
	*/
	virtual void PostRun(void) throw(RGA::eGA);

	/**
	* Return true if all the objects are grouped.
	*/
	inline bool IsEnd(void) {return(NbObjsOk==NbObjs);}

	/**
	* Return the number of objects grouped.
	*/
	inline unsigned int GetNbObjsOk(void) {return(NbObjsOk);}

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
	* Destruct the grouping heuristic.
	*/
	virtual ~RGroupingHeuristic(void);
};


//-----------------------------------------------------------------------------
// Definitions of templates
#include <rgga/rgroupingheuristic.hh>


}  //------- End of namespace RGGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
