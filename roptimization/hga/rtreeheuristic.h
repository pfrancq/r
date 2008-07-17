/*

	R Project Library

	RTreeHeuristic.h

	Generic Heuristic for Tree - Header

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
#ifndef RTreeHeuristic_H
#define RTreeHeuristic_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <random.h>
#include <robjs.h>
#include <rdebug.h>
#include <rnodesga.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RTreeHeuristic class proposes a generic heuristic for a Hierarchical
 * Genetic Algorithm (HGA).
 * @author Pascal Francq
 * @short HGA Heuristic.
 */
template<class cNode,class cObj,class cNodes>
	class RTreeHeuristic
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
	* Number of objects to attach.
	*/
	size_t NbObjs;

	/**
	* Number of objects already attached.
	*/
	size_t NbObjsOk;

	/**
	* Nodes.
	*/
	cNodes* Nodes;

	/**
	* Order in which the objects are to be treated.
	*/
	cObj** Order;

	/**
	* Current object to place.
	*/
	cObj* CurObj;

	/**
	* Current node manipulated.
	*/
	cNode* CurNode;

	/**
	* Pointer to a "debugger" holding information about the GA when it is running.
	*/
	RDebug* Debug;

public:

	/**
	* Construct the tree heuristic.
	* @param n              Name of the heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	* @param debug          Debugger.
	*/
	RTreeHeuristic(const RString& n,RRandom* r,RCursor<cObj> objs,RDebug* debug=0);

	/**
	* Get the name of the heuristic.
	* @return Pointer to a C String.
	*/
	const RString GetName(void) const {return(Name);}
	
	/**
	* Initialize the heuristic.
	* @param nodes          Pointer to the nodes.
	*/
	virtual void Init(cNodes* nodes);

	/**
	* Select the next object to place.
	* The CurObj must pointed to object to place.
	*/
	virtual void SelectNextObject(void);

	/**
	* Return the current object to attach.
	*/
	cObj* GetNextObject(void) {return(CurObj);}

	/**
	* Return the current node.
	*/
	cNode* GetNode(void) {return(CurNode);}

	/**
	* Find a node for the next object.
	*/
	virtual cNode* FindNode(void)=0;

	/**
	* Put the next object into a node.
	*/
	void PutNextObject(void);

	/**
	* Run the heuristic.
	* @param nodes          Pointer to the nodes.
	*/
	void Run(cNodes* nodes);

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
	long RRand(long max) {return(Random->Value(max));}

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
	virtual ~RTreeHeuristic(void);
};


//------------------------------------------------------------------------------
// Definitions of templates
#include <rtreeheuristic.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
