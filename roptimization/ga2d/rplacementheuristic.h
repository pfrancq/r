/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Header

	Copyright 1998-2003 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#ifndef RPlacementHeuristicH
#define RPlacementHeuristicH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rmath/rpoint.h>
#include <rmath/rrect.h>
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rgeoinfos.h>
#include <rga2d/rgrid.h>
#include <rga2d/rfreepolygons.h>
#include <rga2d/rconnections.h>
#include <rga2d/rproblem2d.h>
#include <rpromethee/rpromcriterion.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
*/
class RPlacementHeuristicException : public eGA
{
public:
	RPlacementHeuristicException(const RString& msg) : eGA(msg) {}
	RPlacementHeuristicException(const char* msg) : eGA(msg) {}
};


//------------------------------------------------------------------------------
/**
* The RPlacementHeuristic class provides an abstract class for placement
* heuristics.
* @author Pascal Francq
* @short Generic Placement Heuristic class.
*/
class RPlacementHeuristic
{
	struct ObjectPos
	{
		RPoint Pos;
		char Ori;
	};
	
protected:

	/**
	* The random number genrator to use
	*/
	RRandom* Random;

	/**
	* The limits for the placement.
	*/
	RPoint Limits;

	/**
	* The grid used for the placement.
	*/
	RGrid* Grid;

	/**
	* The geometric information of the objects.
	*/
	RGeoInfos* Infos;
	
	/**
	* The connections of the objects.	
	*/
	RConnections* Connections;
	
	/**
	* Total number of objects to place.
	*/
	unsigned int NbObjs;

	/**
	* Number of objects already placed.
	*/
	unsigned int NbObjsOk;

	/**
	* The bound rectangle of the resulting placement.
	*/
	RRect Result;

	/**
	* The sum of the connections distances.
	*/
	double Distances;

	/**
	* Order in which the objects are to be treated.
	*/
	RGeoInfo** Order;

	/**
	* Geometric information of the current object to place.
	*/
	RGeoInfo* CurInfo;

	/**
	* Free Polygons resulting of the placement.
	*/
	RFreePolygons Free;

	/**
	* Calculations of free polygons.
	*/
	bool CalcFree;

	/**
	* Use of free polygons.
	*/
	bool UseFree;
	
	/**
	* All possible orientation must be tested.
	*/
	bool AllOri;

	/**
	* Maximal number of solutions that can be evaluated.
	*/
	unsigned int MaxPromSol;

	/**
	* Number of solutions to be evaluated.
	*/
	unsigned int NbPromSol;

	/**
	* Array representing different solutions.
	*/
	ObjectPos* Sols;

	/**
	* Prom�th�e Kernel
	*/
	RPromKernel* Prom;

	/**
	* Pointer to the area criteria.
	*/
	RPromCriterion* area;

	/**
	* Pointer to the distance criteria.
	*/
	RPromCriterion* dist;

	/**
	* Prom�th�e Parameters for the distance.
	*/
	RPromCriterionParams DistParams;
	
	/**
	* Prom�th�e Parameters for the area.
	*/
	RPromCriterionParams AreaParams;
	
public:

	/**
	* Construct the placement heuristic.
	* @param maxobjs        The maximum objects to placed fore this object.
	* @param calc           Must free polygons be calculated.
	* @param use            Must free polygons be used.
	* @param r              The random genrator to use.
	* @param ori            Must all orientation be tested.
	*/
	RPlacementHeuristic(unsigned int maxobjs,bool calc,bool use,RRandom* r,bool ori=false);

	/**
	* Initialize the heuristic.
	* @param prob           The problem.
	* @param infos          Pointer to the geometric information.
	* @param grid           Pointer to the grid.
	*/
	virtual void Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid);	

	/**
	* Set the parameters for the "area" criterion.
	* @param p              The indifference's threshold.
	* @param q              The preference's threshold.
	* @param w              The weight.
	*/
	void SetAreaParams(double p,double q,double w);

	/**
	* Set the parameters for the "area" criterion.
	* @param params         The parameters.
	*/
	void SetAreaParams(const RPromCriterionParams& params);

	/**
	* Set the parameters for the "dist" criterion.
	* @param p              The indifference's threshold.
	* @param q              The preference's threshold.
	* @param w              The weight.
	*/
	void SetDistParams(double p,double q,double w);

	/**
	* Set the parameters for the "area" criterion.
	* @param params         The parameters.
	*/
	void SetDistParams(const RPromCriterionParams& params);

	/**
	* Select the next object to place.
	* The CurInfo must pointed to the geometric information representing the
	* object to place.
	*/
	virtual void SelectNextObject(void) throw(RPlacementHeuristicException);

	/**
	* Place the next object.
	*/
	RGeoInfo* NextObject(void) throw(RPlacementHeuristicException);

	/**
	* Calculate the position to place the next object for a specific geometric
	* information. The function have to register the valid positions with the
	* 'AddValidPosition' method.
	*/
	virtual void NextObjectOri(void) throw(RPlacementHeuristicException)=0;

	/**
	* Place the current object to a specific position. This function is called
	* by the NextObject method.<BR>
	* This function is responsible to update Result.
	* @param pos            The position where to place it.
	*/
	virtual void Place(RPoint& pos) throw(RPlacementHeuristicException)=0;

	/**
	* Run the heuristic.
	* @param prob           The problem.
	* @param infos          Pointer to the geometric information.
	* @param grid           Pointer to the grid.
	*/
	void Run(RProblem2D* prob,RGeoInfos* infos,RGrid* grid) throw(RPlacementHeuristicException);

	/**
	* Add a valid position fot the current geometric information to place.
	* @param pos            Position to be a valid.
	*/
	void AddValidPosition(RPoint& pos);

	/**
	* Do some operations after the run.
	*/
	virtual void PostRun(RPoint &);

	/**
	* Return the bound rectangle containing all the objects.
	*/
	RRect& GetResult(void);

	/**
	* Return the sum of the distances of all the connections.
	*/
	double GetDistances(void) {return(Distances);}

	/**
	* Return true if all the objects are placed.
	*/
	inline bool IsEnd(void) {return(NbObjsOk==NbObjs);}

	/**
	* Return the number of objects placed.
	*/
	inline unsigned int GetNbObjsOk(void) {return(NbObjsOk);}

	/**
	* Return a pointer to all the free polygons.
	*/
	inline RFreePolygons* GetFreePolygons(void) {return(&Free);}

	/**
	* Create a problem file for a specific configuration.
	* @seek                 Actual "seek" value for the random number
	*                       generator.
	*/
	void CreateProblem(void);
	
	/**
	* Destruct the placement heuristic.
	*/
	virtual ~RPlacementHeuristic(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
