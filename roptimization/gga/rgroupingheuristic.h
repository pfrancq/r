/*

	R Project Library

	RGroupingHeuristic.h

	Generic Heuristic for Grouping - Header

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RGroupingHeuristic_H
#define RGroupingHeuristic_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <random.h>
#include <robjs.h>
#include <rgroups.h>
#include <rdebug.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGroupingHeuristic class provides an abstract class for grouping
* heuristics.
* @author Pascal Francq
* @short Generic Grouping Heuristic class.
*/
template<class cGroup,class cObj,class cGroups>
	class RGroupingHeuristic
{
protected:

	/**
	* Name of the heuristic.
	*/
	RString Name;

	/**
	* The random number generator to use.
	*/
	RRandom* Random;

	/**
	* Objects to be grouped.
	*/
	RCursor<cObj> Objs;

	/**
	* Number of objects to be grouped.
	*/
	size_t NbObjs;

	/**
	* Number of objects already grouped.
	*/
	size_t NbObjsOk;

	/**
	* Groups.
	*/
	cGroups* Groups;

	/**
	* Order in which the objects are to be treated.
	*/
	cObj** Order;

	/**
	* Pointer to a "debugger" holding information about the GA when it is running.
	*/
	RDebug* Debug;

public:

	/**
	* Construct the grouping heuristic.
	* @param n              Name of the heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	RGroupingHeuristic(const RString& n,RRandom* r,RCursor<cObj> objs,RDebug* debug=0);

	/**
	* Get the name of the heuristic.
	* @return Pointer to a C String.
	*/
	const RString GetName(void) const {return(Name);}

	/**
	* Initialize the heuristic.
	* @param groups         Pointer to the groups.
	*/
	virtual void Init(cGroups* groups);

	/**
	 * Build the order in which the objects that are not assigned must be
	 * grouped. By default, the methods treats them in random order.
	 *
	 * The array Order must be filled with the pointer to the objects to treat
	 * in the correct order and the variable NbObjs must be set with the
	 * correct number of objects to treat.
	 */
	virtual void BuildOrder(void);

	/**
	* Find a group for the next object.
	* @param obj             Object.
	*/
	virtual cGroup* FindGroup(cObj* obj)=0;

	/**
	* Run the heuristic.
	* @param groups         Pointer to the groups.
	*/
	void Run(cGroups* groups);

	/**
	* Do some operations after the run.
	*/
	virtual void PostRun(void);

	/**
	* Return true if all the objects are grouped.
	*/
	inline bool IsEnd(void) {return(NbObjsOk==NbObjs);}

	/**
	* Return the number of objects grouped.
	*/
	inline size_t GetNbObjsOk(void) {return(NbObjsOk);}

	/**
	* Return a number in the interval [0,max[ using the current random generator.
	* @param max            Variable used to calculate the number.
	*/
	long RRand(long max) {return(Random->GetValue(max));}

	/**
	* Random the position of elements of a vector using the current random generator.
	* @param arr            A pointer to the array representing the vector.
	* @param size           The size of the vector.
	*/
	template<class T> inline void RandOrder(T* arr,size_t size)
		{Random->RandOrder<T>(arr,size);}

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~RGroupingHeuristic(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgroupingheuristic.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
