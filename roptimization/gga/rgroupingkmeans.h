/*

	R Project Library

	RGroupingKMeans.h

	Generic Heuristic for Grouping - Header

	Copyright 1998-2003 by the Universit�Libre de Bruxelles.

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
#include <rdate.h>
#include <rstring.h>
#include <rcontainer.h>
#include <random.h>
#include <robjs.h>
#include <rgroups.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RGroupingKMeans class provides an abstract class for grouping
* heuristics.
* @author Pascal Francq
* @short Generic Grouping Heuristic class.
*/
template<class cGroup,class cObj,class cGroupData,class cGroups>
	class RGroupingKMeans
{

public:

	/**
	* enum of initial conditions
	*/
	enum Initial {Refined,Random};

protected:

	/**
	* init method chosen
	*/
	Initial initial;

	/**
	* The random number generator to use.
	*/
	RRandom* Rand;

	/**
	* the number of subsamples
	*/
	unsigned int NbSubSamples;

	/**
	* rate of subprofiles in each subsamples
	*/
	unsigned int SubSamplesRate;

	/**
	* maximum number of KMeans verification in refineing procedure
	*/
	 unsigned int VerifyKMeansMaxIters;

	/**
	* Objects to be grouped.
	*/
	RContainer<cObj,false,true>* Objs;

	/**
	* Groups.
	*/
	cGroups* Groups;

	/**
	* Current object to place.
	*/
	cObj* CurObj;

	/**
	* random table of objects;
	*/
	cObj **RandObjects;

	/**
	* Current group manipulated.
	*/
	cGroup* CurGroup;

	/**
	* value for decimals error
	*/
	double Epsilon;

	/**
	* Number of groups.
	*/
	unsigned int GroupsNumber;

	/**
	* Number of iterations
	*/
	unsigned int IterNumber;

	/**
	* Number of tests
	*/
	unsigned int NbTests;

	/**
	* Maximum number of groups
	*/
	unsigned int MaxNbGroups;

	/**
	* Minimum number of groups
	*/
	unsigned int MinNbGroups;

	/**
	* Container of cObj considered as prototypes
	*/
	RContainer<cObj,false,false>* protos;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<cGroup,false,false>* grpstemp2;

	/**
	* Container of subprofiles considered as prototypes,
	* needed to calculate the error between two iterations
	*/
	RContainer<cObj,false,false>* protoserror;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<cGroup,false,false>* grpstemp;

	/**
	* Container of the final groupment
	*/
	RContainer<cGroup,false,false>* grpsfinal;

public:

	/**
	* Construct the grouping heuristic.
	* @param Objs           Pointer to the objects.
	*/
	RGroupingKMeans(RContainer<cObj,false,true>* Objs);

	/**
	* Get the name of the heuristic.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(this->Name());}

	/**
	* Set the maximum number of iterations for kmeans.
	* @param i              Number of iterations.
	*/
	void SetIterNumber(unsigned int i) {IterNumber=i;}

	/**
	* Set the number of tests.
	* @param i              Number of tests.
	*/
	void SetNbTests(unsigned int i) {NbTests=i;}

	/**
	* Set the number of Groups.
	* @param i              Number of groups.
	*/
	void SetGroupsNumber(unsigned int i) {GroupsNumber=i;}

	/**
	* Set the number of sub-samples.
	* @param i              Number of sub-samples.
	*/
	void SetSubSamplesNumber(unsigned int i) {NbSubSamples=i;}

	/**
	* Set the sub-samples rate.
	* @param i              Sub-samples rate.
	*/
	void SetSubSamplesRate(unsigned int i) {SubSamplesRate=i;}

	/**
	* Set VerifyKMeansMaxIters limit.
	* @param i              Maximum number of k-Means iterations.
	*/
	void SetVerifyKMeansMaxIters(unsigned int i) {VerifyKMeansMaxIters=i;}

	/**
	* Set the refined parameter.
	* @param i              Refined parameter.
	*/
	void SetInitial(Initial i) {initial=i;}

	/**
	* Set the value of Epsilon.
	* @param d              Value of epsilon.
	*/
	void SetEpsilon(double d) {Epsilon=d;}

	/**
	* Initialize the heuristic.
	*/
	void Init(void);

	/**
	* Test if an object is a valid prototype.
	* @param prototypes     List of prototypes.
	* @param obj            Object to test.
	* @return bool.
	*/
	bool IsValidProto(RContainer<cObj,false,false>* prototypes,cObj* obj) throw(std::bad_alloc);

	/**
	* Verify wether the initializing kmeans is ok.
	*/
	bool VerifyKMeansMod(void) throw(RException);

	/**
	* Calulates the distortion of a grouping.
	* @param grps           Groups.
	*/
	double Distortion(RContainer<cGroup,false,false>* grps);

	/**
	* Calculates the variance of a group.
	* @param grp            Group.
	*/
	double GroupVariance(cGroup* grp);

	/**
	* Re-Allocation step.
	* @param dataset        Dataset to re-allocate.
	*/
	void ReAllocate(RContainer<cObj,false,true>* dataset);

	/**
	* Re-Centering step.
	*/
	void ReCenter(void);

	/**
	* Calculates the cost function.
	* @param grps           Groups.
	*/
	double CostFunction(RContainer<cGroup,false,false>* grps);

	/**
	* Calculate the distance between an object and a prototype.
	* @param obj            Object.
	* @param proto          Prototype.
	*/
	double Distance(cObj* obj, cObj* proto);

	/**
	* Calculates the error between two iterations of K-Means.
	*/
	int CalcError(void);

	/**
	* Execute the K-Means.
	* @param dataset         Set of all objects to group.
	* @param nbtests         Number of tests.
	*/
	void Execute(RContainer<cObj,false,true>* dataset, unsigned int nbtests);

	/**
	* Couts many informations on K-Means configuration.
	*/
	void DisplayInfos(void);

	/**
	* Chooses subprofiles among the dataset to initialize the K-Means
	* @param dataset         Set of all objects to group.
	* @param nbobjects       Number of objects to choose.
	*/
	void RandomInitObjects(RContainer<cObj,false,true>* dataset, unsigned int nbobjects) throw(RException);

	/**
	* Function to evaluates the number of groups.
	*/
	void EvaluateGroupsNumber(void);

	/**
	* Finds the group containing a given object.
	* @param grps           List of all groups.
	* @param obj            Object.
	*/
	cGroup* FindGroup(RContainer<cGroup,false,false>* grps, cObj* obj);

	/**
	* Calculates the similarity between two objects.
	* @param obj1           First object.
	* @param obj2           Second object.
	*/
	double Similarity(cObj* obj1, cObj* obj2);

	/**
	* Gets the final grouping, result of the K-Means procedure.
	*/
	RContainer<cGroup,false,false>* GetGrouping(void){return(grpsfinal);}

	/**
	* Init the protos to the refined points.
	* @param nbsub           number of subsamples to get.
	* @param level           level  (in %) of the full dataset for the subsamples
	*/
	void RefiningCenters(int nbsub, int level);


	/**
	* Find a group for the next object.
	*/
	virtual cGroup* FindGroup(void) throw(eGA) {return(0);};

	/**
	* Run the heuristic.
	*/
	void Run(void);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~RGroupingKMeans(void);
};


//------------------------------------------------------------------------------
// Definitions of templates
#include <rgroupingkmeans.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
