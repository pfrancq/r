/*

	R Project Library

	RGroupingKMeans.h

	Generic Heuristic for Grouping - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		David Wartel (dwartel@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RGroupingKMeans_H
#define RGroupingKMeans_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
#include <rstd/random.h>
#include <rga/robjs.h>
#include <rgga/rgroups.h>
#include <rstd/rcursor.h>


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
	RContainer<cObj,unsigned int,false,true>* Objs;

	/**
	* Groups.
	*/
	cGroups* Groups;

	/**
	* Current object to place.
	*/
	cObj* CurObj;

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
	RContainer<cObj,unsigned int,false,false>* protos;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<cGroup,unsigned int,false,false>* grpstemp2;

	/**
	* Container of subprofiles considered as prototypes,
	* needed to calculate the error between two iterations
	*/
	RContainer<cObj,unsigned int,false,false>* protoserror;

	/**
	* Temporary container of groupment, needed to run tests
	*/
	RContainer<cGroup,unsigned int,false,false>* grpstemp;

	/**
	* Container of the final groupment
	*/
	RContainer<cGroup,unsigned int,false,false>* grpsfinal;

public:

	/**
	* Construct the grouping heuristic.
	* @param n              Name of the heuristic.
	* @param r              The random genrator to use.
	* @param objs           Pointer to the objects.
	*/
	RGroupingKMeans(RContainer<cObj,unsigned int,false,true>* Objs);

	/**
	* Get the name of the heuristic.
	* @return Pointer to a C String.
	*/
	const char* GetName(void) const {return(Name());}

	/**
	* Set the maximum number of iterations for kmeans.
	*/
	void SetIterNumber(unsigned int i) {IterNumber=i;}

	/**
	* Set the number of tests.
	*/
	void SetNbTests(unsigned int i) {NbTests=i;}

	/**
	* Set the number of Groups.
	*/
	void SetGroupsNumber(unsigned int i) {GroupsNumber=i;}

	/**
	*  Set the number of SubSamples.
	*/
	void SetSubSamplesNumber(unsigned int i) {NbSubSamples=i;}

	/**
	*  Set theSubSamples rate.
	*/
	void SetSubSamplesRate(unsigned int i) {SubSamplesRate=i;}

	/**
	*  Set VerifyKMeansMaxIters limt.
	*/
	void SetVerifyKMeansMaxIters(unsigned int i) {VerifyKMeansMaxIters=i;}


	/**
	* Set the Refined parameter.
	*/
	void SetInitial(Initial i) {initial=i;}

	/**
	* Set the value of Epsilon.
	*/
	void SetEpsilon(double d) {Epsilon=d;}

	/**
	* Initialize the heuristic.
	* @param groups         Pointer to the groups.
	*/
	void Init(void);

	/**
	* return true if the subprofile is a valid proto ;
	*/
	bool IsValidProto(RContainer<cObj,unsigned int,false,false>* prototypes,cObj* s) throw(bad_alloc);

	/**
	* Verify wether the initializing kmeans is ok.
	*/
	bool VerifyKMeansMod(void);

	/**
	* calulates the distortion of a grouping.
	*/
	double Distortion(RContainer<cGroup,unsigned int,false,false>* grps);

	/**
	*  calculates the variance of a group.
	*/
	double GroupVariance(cGroup* grp);

	/**
	* Re-Allocation step.
	*/
	void ReAllocate(RContainer<cObj,unsigned int,false,true>* dataset);

	/**
	* Re-Centering step.
	*/
	void ReCenter(void);

	/**
	* Calculates the cost function.
	*/
	double CostFunction(RContainer<cGroup,unsigned int,false,false>* grps);

	/**
	* Calculates the distance between two objects.
	*/
	double Distance(cObj *s1, cObj *proto);

	/**
	* Calculates the error between two iterations of K-Means.
	*/
	int CalcError(void);

	/**
	* Execute the K-Means.
	* @param dataset         set of all objects to group.
	* @param nbtests         number of tests.
	*/
	void Execute(RContainer<cObj, unsigned int, false, true>* dataset, unsigned int nbtests);

	/**
	* Couts many informations on K-Means configuration.
	*/
	void DisplayInfos(void);

	/**
	* Chooses subprofiles among the dataset to initialize the K-Means
	* @param dataset         set of all objects to group.
	* @param nbsubprofiles   number of objects to choose.
	*/
	void RandomInitObjects(RContainer<cObj,unsigned int,false,true>* dataset, unsigned int nbobjects);

	/**
	* Function to evaluates the number of groups.
	*/
	void EvaluateGroupsNumber(void);

	/**
	*  Finds the group containing a given object.
	*/
	cGroup* FindGroup(RContainer<cGroup,unsigned int,false,false>* grps, cObj* s);

	/**
	* Calculates the similarity between two objects.
	*/
	double Similarity(cObj *s1, cObj *s2);


	/**
	* Gets the final grouping, result of the K-Means procedure.
	*/
	RContainer<cGroup,unsigned int,false,false>* GetGrouping(void){return(grpsfinal);}

	/**
	* init the protos to the refined points.
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
	* @param groups          Pointer to the groups.
	*/
	void Run(void);

	/**
	* Destruct the grouping heuristic.
	*/
	virtual ~RGroupingKMeans(void);
};


//------------------------------------------------------------------------------
// Definitions of templates
#include <rgga/rgroupingkmeans.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
