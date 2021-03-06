/*

	R Project Library

	RGroupSG.h

	Similarity-based Grouping Genetic Algorithm Group - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef RGroupSG_H
#define RGroupSG_H

//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rgroup.h>
#include <rquicksort.h>
class GCAInst;
class GCAObj;


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* The RGroupSG provides a representation for a group for the similarity-based
* grouping genetic algorithm.
* @short Similarity-based Grouping Genetic Algorithm Group.
*/
template<class cGroup,class cObj,class cGroups>
	class RGroupSG : public RGroup<cGroup,cObj,cGroups>
{
	using RGroup<cGroup,cObj,cGroups>::Id;
	using RGroup<cGroup,cObj,cGroups>::Owner;
	using RGroup<cGroup,cObj,cGroups>::NbSubObjects;

	/**
	 * Implementation of a specific quicksort algorithm to sort an array of
	 * objects by similarity with the centroid of the group.
	 */
	class OrderBySim : public RQuickSort<cObj>
	{
		/**
		 * Group corresponding to the elements to sort.
		 */
		cGroup* Group;

	public:

		/**
		 * Constructor.
		 * @param group      Corresponding group.
		 * @param objs       Array of objects.
		 * @param nb         Number of objects.
		 */
		OrderBySim(cGroup* group,cObj** objs,size_t nb);

		/**
		 * Compare two objects.
		 * @param obj1       First object.
		 * @param obj2       Second object.
		 * @return the value to sort the objects by descending similarities
		 * with the centroid.
		 */
		virtual int Compare(cObj* obj1,cObj* obj2);
	};

public:

	using RGroup<cGroup,cObj,cGroups>::GetObjs;
	using RGroup<cGroup,cObj,cGroups>::Delete;
	using RGroup<cGroup,cObj,cGroups>::Insert;

protected:

	/**
	* Average similarity between each object and the centroid.
	*/
	double AvgIntraSim;

	/**
	 * Average agreement ratio between the objects of the group.
	 */
	double AvgAgreement;

	/**
	 * Average disagreement ratio between the object of the group.
	 */
	double AvgDisagreement;

	/**
	* Centroid of the group.
	*/
	cObj* Centroid;

	/**
	 * Must the group be re-evaluated?
	 */
	bool ToEval;

	/**
	 * Maximum similarity with the last object called by CanInsert.
	 */
	double LastMaxSim;

public:

	/**
	* Construct the group.
	* @param grp            Group used as source.
	*/
	RGroupSG(RGroupSG* grp);

	/**
	* Construct the group.
	* @param owner          Owner of the group.
	* @param id             Identifier of the group.
	*/
	RGroupSG(cGroups* owner,const size_t id);

	/**
	* Clear the information container in a group.
	*/
	virtual void Clear(void);

	/**
	* Verify if an object has the same parent as another object of the group.
	* @param obj            Pointer to the object to insert.
	*/
	bool HasSameUser(const cObj* obj) const;

	/**
	* Look if an object can be insert in the group. In practice, the object
	* must have a minimum similarity and maximum disagreement with the objects
	* of the group. Moreover, two objects having the same parents cannot be
	* grouped together.
	* not allowed.
	* @param obj             Object to insert.
	*/
	virtual bool CanInsert(const cObj* obj);

	/**
	* Method call after an object was inserted in the group.
	* @param obj             Object to insert.
	*/
	virtual void PostInsert(const cObj* obj);

	/**
	* Method call after an object was deleted from the group.
	* @param obj             Object to delete.
	*/
	virtual void PostDelete(const cObj* obj);

	/**
	 * Get the maximum similarity with the last object called with CanInsert.
	 */
	inline double GetLastMaxSim(void) const {return(LastMaxSim);}

private:

	/**
	* Compute the sum of the similarities of a given object to all the others.
	* If the object is itself in the group, '1.0' is added to the sum.
	* @param obj             Object.
	*/
	double ComputeSumSim(cObj* obj);

	/**
	* Compute the centroid of the group, i.a. the object which is the most
	* similar to all the other objects.
	*/
	void ComputeCentroid(void);

public:

	/**
	* @return the centroid of the group.
	*/
	inline cObj* GetCentroid(void)
	{
		if(!Centroid)
			ComputeCentroid();
		return(Centroid);
	}

	/**
	 * Compute the sum of the average intra-similarity of the objects and the
	 * average of the agreement and disagreement.
	 * objects.
	 * @param dist           Sum of the average intra-similarity.
	 * @param agree          Sum of the average agreement ratios.
	 * @param disagree       Sum of the average disagreement ratios.
	 */
	void Evaluate(double& dist,double& agree,double& disagree);

	/**
	 * Set the centroid of the group. This method must be used with caution.
	 * @param obj            Object becoming the centroid.
	 */
	void SetCentroid(cObj* obj);

	/**
	* @return the average intra-similarity of the group.
	*/
	inline double GetAvgIntraSim(void)
	{
		if(!Centroid)
			ComputeCentroid();
		return(AvgIntraSim);
	}

	/**
	* Compute the similarity of a given object to the centroid of the group.
	* @param obj             Object.
	*/
	inline double ComputeRelSim(const cObj* obj) {return(Owner->Instance->GetSim(GetCentroid(),obj));}

	/**
	* Copy internal information from a given group, in particular information
	* that are computer (such as an average similarity inside the group).
	* @param grp            The group used as source.
	*/
	virtual void CopyInfos(const cGroup* grp);

private:

	bool Test(cObj** del,size_t& nbdel,cObj* obj1,cObj* obj2);

public:

	/**
	 * Try to optimize the group by exchanging a given number of objects no
	 * assigned with some of the group.
	 * @param objs           Objects no assigned.
	 * @param nbobjs         Number of objects (can be modified).
	 * @return true if the group could be optimized.
	 */
	bool DoOptimisation(cObj** objs,size_t& nbobjs);

	/**
	* Destruct the group.
	*/
	virtual ~RGroupSG(void);

	friend class OrderBySim;
	friend class RGroups<cGroup,cObj,cGroups>;
};


//------------------------------------------------------------------------------
// inline implementation
#include <rgroupsg.hh>


}  //------- End of namespace R ------------------------------------------------


//-----------------------------------------------------------------------------
#endif
