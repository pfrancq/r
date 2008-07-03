/*

	R Project Library

	RGroupingKMeans.h

	k-Means Algorithm - Header

	Copyright 2003-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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
#ifndef RGroupingKMeans_H
#define RGroupingKMeans_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <random.h>
#include <rcursor.h>
#include <rdebug.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RGroupingKMeans class provides the implementation of a basic k-Means
* grouping algorithm.
* @author Pascal Francq
* @short k-Means Algorithm.
*/
template<class cGroup,class cObj,class cGroups>
	class RGroupingKMeans
{
protected:

	/**
	 * Name of the k-Means
	 */
	RString Name;

	/**
	 * Debugger.
	 */
	RDebug* Debug;

	/**
	* Random number generator to use.
	*/
	RRandom* Rand;

	/**
	* Objects to be grouped.
	*/
	 RCursor<cObj> Objs;

	/**
	* Groups.
	*/
	cGroups* Groups;

	/**
	* random table of objects;
	*/
	cObj** RandObjects;

	/**
	* Prototypes of the groups. The order of the prototypes is identical as the
	* corresponding group.
	*/
	RContainer<cObj,false,false> Protos;

	/**
	 * Number of Iterations.
	 */
	unsigned int NbIterations;

public:

	/**
	* Construct the k-Means.
	* @param n               Name of the k-Means.
	* @param r               Random number generator to use.
	* @param objs            Cursor over the objects to group.
	* @param debug           Debugger.
	*/
	RGroupingKMeans(const RString& n,RRandom* r,RCursor<cObj> objs,RDebug* debug=0);

	/**
	* Get the name of the heuristic.
	*/
	RString GetName(void) const {return(Name);}

	/**
	 * Get the number of interations runned.
	 */
	unsigned int GetNbIterations(void) const {return(NbIterations);}

protected:

	/**
	 * Compute the prototype of a given group.
	 * @param group          Group.
	 * @return Pointer to the object which is the prototype.
	 */
	cObj* ComputePrototype(cGroup* group);

	/**
	 * Compute the sum of similarities between a given object and the other
	 * objects of its group.
	 * @param group          Group.
	 * @param obj            Object.
	 */
	double ComputeSumSim(cGroup* group,cObj* obj);

	/**
	* Randmoly creates an initialization for the k-Means
	* @param nbclusters      Number of groups to create.
	*/
	void InitRandom(size_t nbclusters);

	/**
	* Re-Allocation step where the objects are put in the group of the most
	* similar prototype.
	*/
	void ReAllocate(void);

	/**
	* Computing the new prototypes.
	* @return Number of different prototypes.
	*/
	unsigned int CalcNewProtosNb(void);

public:

	/**
	* Couts many informations on K-Means configuration.
	*/
	void DisplayInfos(void);

	/**
	* Calculates the similarity between two objects. It is suppose that if the
	* similarity is between -1 and 1 (same objects).
	* @param obj1            First object.
	* @param obj2            Second object.
	*/
	virtual double Similarity(cObj* obj1, cObj* obj2)=0;

	/**
	* Run the heuristic.
	* @param groups          Group to initialize.
	* @param itermax         Maximal number of iterations.
	* @param nbclusters      Number of groups to create.
	*/
	void Run(cGroups* groups,unsigned int itermax,size_t nbclusters);

	/**
	* Destruct the k-Means.
	*/
	virtual ~RGroupingKMeans(void);
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgroupingkmeans.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
