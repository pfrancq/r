/*

	R Project Library

	RGroupingKMeans.h

	k-Means Algorithm - Header

	Copyright 2004-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2004 by David Wartel.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
public:

	/**
	* How to start the kMeans.
	*/
	enum tInitial
	{
		Incremental    /** Start from the existing clustering.*/,
		Refine         /** Use several sub-samples to initialize.*/,
		kMeansPlusPlus /** Use the kMeans++ method.*/,
		Random         /** Initialize randomly.*/
	};

protected:

	// Internal classes
	class Group : public R::RContainer<cObj,false,false>
	{
	public:

		size_t Id;

		Group(size_t id,size_t max) : R::RContainer<cObj,false,false>(max), Id(id) {}
		int Compare(const Group& tmp) const {return(Id-tmp.Id);}
	};

	class Centroid
	{
	public:

		cObj* Obj;       // Object being the centroid
		double AvgSim;   // Average similarity with the objects.

		Centroid(void) : Obj(0), AvgSim(-2.0) {}
		void SetObj(cObj* obj) {Obj=obj; AvgSim=-2.0;}
		int Compare(const Centroid&) const {return(-1);}
		int Compare(const cObj* obj) const {return(Obj!=obj);}
	};

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
	* Groups.
	*/
	cGroups* Groups;

	/**
	* Objects to group;
	*/
	cObj** Objs;

	/**
	 * Number of objects.
	 */
	size_t NbObjs;

	/**
	 * Objects used to build the clusters.
	 */
	cObj** ObjsUsed;

	/**
	 * Number of objects used to build the clusters.
	 */
	size_t NbObjsUsed;

	/**
	* Prototypes of the groups. The order of the prototypes is identical as the
	* corresponding group.
	*/
	RContainer<Centroid,true,false> Protos;

	/**
	 * Number of Iterations.
	 */
	size_t NbIterations;

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
	 * Get the number of iterations run.
	 */
	size_t GetNbIterations(void) const {return(NbIterations);}

protected:

	/**
	 * Compute the prototype of a given group.
	 * @param group          Group.
	 */
	void ComputePrototype(cGroup* group);

	/**
	 * Compute the sum of similarities between a given object and the other
	 * objects of its group.
	 * @param group          Group.
	 * @param obj            Object.
	 */
	double ComputeSumSim(cGroup* group,const cObj* obj);

	/**
	 * Creates the initial prototypes by randomly choosing them.
	 * @param nb              Number of groups to create.
	 */
	void InitRandom(size_t nb);

	/**
	 * Creates the initial prototypes by using a given number of samples
	 * containing a given percentage of the objects to group and running the
	 * kMeans. The best centroids of the sample are used.
	 * @param nb              Number of groups to create.
	 * @param max             Maximal number of iterations.
	 * @param nbsub           Number of samples to use.
	 * @param level           Percentage of objects to group for each sample.
	 */
	void InitRefine(size_t nb,size_t max,size_t nbsub=5,int level=5);

	/**
	 * Creates the initial prototypes by using the kMeans++ method.
	 * @param nb              Number of groups to create.
	 */
	void InitkMeansPlusPlus(size_t nb);

	/**
	* Re-Allocation step where the objects are put in the group of the most
	* similar prototype.
	*/
	void ReAllocate(void);

	/**
	* Computing the new prototypes.
	* @return Number of different prototypes.
	*/
	size_t CalcNewProtosNb(void);

public:

	/**
	* Compute the similarity between two objects. It is suppose that if the
	* similarity is between 0 and 1 (same objects).
	* @param obj1            First object.
	* @param obj2            Second object.
	*/
	virtual double Similarity(const cObj* obj1,const cObj* obj2)=0;

	/**
	* Compute the fitness of the given clustering (the higher is the fitness,
	* the best is the clustering.
	*
	* By default, it computes the ratio between the average intra-similarity
	* and the similarity between the centroids and the meta-centroid.
	*/
	virtual double Fitness(void);

	/**
	 * Perform a kMeans.
	 * @param max             Maximal number of iterations.
	 */
	void DokMeans(size_t max);

	/**
	* Run the heuristic.
	* @param groups          Group to initialize.
	* @param max             Maximal number of iterations.
	* @param nb              Number of groups to create.
	* @param start           How to the start the clustering.
	*/
	void Run(cGroups* groups,size_t max,size_t nb,tInitial start=Random);

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
