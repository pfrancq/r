/*

	R Project Library

	SGGA.h

	Similarity-based Grouping Genetic Algorithm - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef SGGA_H
#define SGGA_H


//-----------------------------------------------------------------------------
// includes files for R Project
#include <rcontainer.h>
#include <rcursor.h>
#include <rparam.h>
#include <rgga.h>
#include <rfitness.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RParamsSG;
template<class cGroup,class cObj,class cGroups> class RGroupSG;
template<class cGroup,class cObj,class cGroups> class RHeuristicSG;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RThreadDataSG;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RChromoSG;
template<class cInst,class cChromo,class cThreadData,class cGroup,class cObj> class RInstSG;


//-----------------------------------------------------------------------------
/**
* The RSCFitness provides a representation for a fitness function for the
* similarity-based grouping genetic algorithm.
* @author Pascal Francq
* @short Similarity-based Grouping Genetic Algorithm Fitness.
*/
class RFitnessSG : public RFitness<double,true>
{
public:

	/**
	* Constructor of the fitness function used.
	*/
	RFitnessSG(void) : RFitness<double,true>() {}

	/**
	* Assignment operator with a fitness f.
	*/
	RFitnessSG& operator=(const RFitnessSG &f)
	{
		RFitness<double,true>::operator=(f);
		return(*this);
	}

	/**
	* Assignment operator with a double value.
	*/
	RFitnessSG& operator=(const double val)
	{
		RFitness<double,true>::operator=(val);
		return(*this);
	}
};


//-----------------------------------------------------------------------------
/*
* The RCloseObjSG class provides a representation for an object that is close
 * from another one. For example, the two objects have a high agreement ratio.
 * @author Pascal Francq.
 * @short Similarity-based Grouping Genetic Algorithm Closed Object.
 */
class RCloseObjSG
{
   /**
   * Identifier of the object.
   */
   size_t Id;

   /*
   * Ratio.
   */
   double Ratio;

   /**
   * Constructor.
   * @param id         Identifier.
   * @param ratio      Ratio.
   */
   RCloseObjSG(size_t id,double ratio);

   /**
   * Static function used to order the object by ratio.
   * @param a              Pointer to the first object.
   * @param b              Pointer to the second object.
   */
   static int sortOrder(const void* a,const void* b);

public:

   /**
    * Compare two ratios.
    * @param ratio          Ratio to compare with.
    * @return a number used for sorting purposes.
    */
   int Compare(const RCloseObjSG& ratio) const;

   friend class RParamsSG;
};


//-----------------------------------------------------------------------------
/**
 * The RObjRefSG provides a representation for reference to an object to group.
 * It remembers the closest object and if the object should be considered as
 * social (it prefer to be grouped with other objects rather than to remain
 * alone).
 * @author Pascal Francq.
 * @short Similarity-based Grouping Genetic Algorithm Object Reference.
 */
class RObjRefSG
{
   /**
    * Closest Objects.
    */
   RContainer<RCloseObjSG,true,false> CloseObjs;

   /**
   * Must the close objects be re-ordered.
   */
   bool MustReOrder;

   /**
   * Can the element be grouped in a group alone?
   */
   bool Social;

   /**
    * Constructor.
    * @param nb             Initial size of the container.
    */
   RObjRefSG(size_t nb);

   /**
   * See if the element is social or not.
   */
   inline bool IsSocial(void) const {return(Social);}

public:

   /**
    * Compare two object references.
    * @param obj            Object to compare with.
    * @return always -1.
    */
   int Compare(const RObjRefSG& obj) const;

   friend class RParamsSG;
};


//-----------------------------------------------------------------------------
/**
* The RParamsSG represents all the parameter used in the similarity-based
* grouping genetic algorithm.
* @short Similarity-based Grouping Genetic Algorithm Parameters.
*/
class RParamsSG
{
public:

	/**
	 * All closed objects.
	 */
	RContainer<RObjRefSG,true,false> Objs;

	/**
	* Size of the population of the GA.
	*/
	size_t PopSize;

	/**
	* Maximal number of generation to run.
	*/
	size_t MaxGen;

	/**
	* Is the GA in step mode?
	*/
	bool Step;

	/**
	* Number of generation for each step.
	*/
	size_t StepGen;

	/**
	* Minimum threshold for similarity between the objects.
	*/
	double MinSimLevel;

	/**
	* Minimum threshold for the agreement ratio.
	*/
	double MinAgreement;

	/**
	* Minimum threshold for the disagreement ratio.
	*/
	double MinDisagreement;

	/**
	* Maximal Number of k-Means.
	*/
	size_t MaxKMeans;

	/**
	* Convergence ratio between two k-Means iterations.
	*/
	double Convergence;

	/**
	* Number of derived chromosomes must be constructed for the optimization.
	*/
	size_t NbDivChromo;

	/**
	* Parameter for the criterion "Similarity".
	*/
	RParam* ParamsSim;

	/**
	* Parameter for the criterion "Agreement".
	*/
	RParam* ParamsAgreement;

	/**
	* Parameter for the criterion "Disagreement".
	*/
	RParam* ParamsDisagreement;

	/**
	 * Perform a local optimization.
	 */
	bool LocalOptimisation;

	/**
	 * Perform an optimization.
	 */
	bool Optimisation;

	/**
	* Incremental mode. The GCA constructs the chromosomes from existing
	* solutions.
	*/
	bool Incremental;

	/**
	 * Minimum number of objects per groups.
	 */
	size_t NbMinObjs;

	/**
	 * Maximum number of objects per group.
	 */
	size_t NbMaxObjs;

	/**
	 * All the pairs of objects of a group must respect the minimum of similarity constraint.
	 */
	bool AllMinSim;

	/**
	* Constructor.
	*/
	RParamsSG(void);

   /**
    * Allocate the object references.
    * @param highid         Highest identifier.
    * @param nb             Supposed number of closest objects.
    *
    */
   void InitObjs(size_t highid,size_t nb);

   /**
    * Look if a given object is social.
    * @param id             Identifier of the object.
    * @return a boolean
    */
   inline bool IsSocial(size_t id)
   {
       return(Objs[id]->Social);
   }

   /**
    * Set if a given object is social or not.
    * @param id             Identifier of the object.
    * @param social         Social object ?
    */
   inline void SetSocial(size_t id,bool social)
   {
      Objs[id]->Social=social;
   }

	/**
	 * Add a close object to an object.
	 * @param id             Identifier of the object.
     * @param closeid        Identifier of a close object.
	 * @param ratio          Ratio representing the closeness.
	 */
	void AddCloseObject(size_t id,size_t closeid,double ratio);

	/**
	 * Find the best group for the object based on the current clustering and
	 * the ratio.
	 * @param id             Identifier of the object.
	 * @param groups         Clustering.
	 * @param grp            Variable that will hold the pointer to the group
	 *                       (or 0 if no group was found).
	 */
	template<class cGroup,class cGroups>
		void FindBestGroup(size_t id,cGroups* groups,cGroup* &grp)
	{
      RObjRefSG* Obj(Objs[id]);
		if(Obj->MustReOrder)
		{
			Obj->MustReOrder=false;
			Obj->CloseObjs.ReOrder(RCloseObjSG::sortOrder);
		}
		RCursor<RCloseObjSG> Best(Obj->CloseObjs);
		for(Best.Start(),grp=0;(!Best.End())&&(!grp);Best.Next())
			grp=groups->GetGroup(Best()->Id);
	}
};


}//------- End of namespace R --------------------------------------------------


//-----------------------------------------------------------------------------
#endif
