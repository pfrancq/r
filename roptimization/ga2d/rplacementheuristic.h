/*

	R Project Library

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Header

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
#ifndef RPlacementHeuristicH
#define RPlacementHeuristicH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rrect.h>
using namespace RGeometry2D;
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
#include <rga2d/rgeoinfos.h>
#include <rga2d/rgrid.h>
#include <rga2d/rfreepolygons.h>
#include <rga2d/rconnections.h>
#include <rga2d/rproblem2d.h>
using namespace RGA2D;
#include <rpromethee/rpromcriterion.h>
using namespace RPromethee;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPlacementHeuristic class provides an abstract class for placement
* heuristics.
* @author Pascal Francq
* @short Generic Placement Heuristic class.
*/
class RPlacementHeuristic
{
protected:

	/**
	* The limits for the placement.
	*/
	RPoint Limits;

	/**
	* The grid used for the placement.
	*/
	RGrid *Grid;

	/**
	* The geometric information of the objects.
	*/
	RGeoInfo **Infos;
	
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
	* Order in which the objects are to be treated.
	*/
	unsigned int *Order;

	/**
	* Geometric information of the current object to place.
	*/
	RGeoInfo *CurInfo;

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
	* All the bounding rectangles for the different orientation.
	*/
	RRect OriResult[8];
	
	/**
	* All the position for the different orientation.
	*/
	RPoint OriPos[8];

	/**
	* All the orientation.
	*/
	char Ori[8]	;
	
	/**
	* Number of orientations tested.
	*/
	char NbOri;
	
	/**
	* Prométhée Parameters for the distance.
	*/
	RPromCriterionParams DistParams;
	
	/**
	* Prométhée Parameters for the area.
	*/
	RPromCriterionParams AreaParams;						
	
public:

	/**
	* Construct the placement heuristic.
	* @param maxobjs		The maximum objects to placed fore this object.
	* @param calc			Must free polygons be calculated.
	* @param use			Must free polygons be used.
	* @param ori			Must all orientation be tested.
	*/
	RPlacementHeuristic(unsigned int maxobjs,bool calc,bool use,bool ori=false);
	
	/**
	* Initialize the heuristic.
	* @param prob			The problem.
	* @param infos			Pointer to the geometric information.
	* @param grid			Pointer to the grid.	
	*/
	virtual void Init(RProblem2D* prob,RGeoInfo** infos,RGrid* grid);
		
	/**
	* Initialize the heuristic.
	* @param prob			The problem.
	* @param infos			Pointer to the geometric information.
	* @param grid			Pointer to the grid.	
	*/
	virtual void Init(RProblem2D* prob,RGeoInfos* infos,RGrid* grid);	
	
	/**
	* Set the parameters for the "area" criterion.
	* @param p	The indifference's threshold.
	* @param q	The preference's threshold.
	* @param w	The weight.
	*/
	void SetAreaParams(double p,double q,double w);
	
	/**
	* Set the parameters for the "area" criterion.
	* @param params		The parameters..
	*/
	void SetAreaParams(const RPromCriterionParams& params);
	
	/**
	* Set the parameters for the "dist" criterion.
	* @param p	The indifference's threshold.
	* @param q	The preference's threshold.
	* @param w	The weight.
	*/
	void SetDistParams(double p,double q,double w);
	
	/**
	* Set the parameters for the "area" criterion.
	* @param params		The parameters..
	*/
	void SetDistParams(const RPromCriterionParams& params);

	/**
	* Select the next object to place.
	* The CurInfo must pointed to the geometric information representing the
	* object to place.
	*/
	virtual void SelectNextObject(void);
		
	/**
	* Place the next object.	
	*/
	RGeoInfo* NextObject(void);

	/**
	* Calculate the position to place the next object for a specific geometric
	* information.
	* @return	The function returns the position where the object can be
	*				placed with the current orientation. The position returned
	*				can be invalid if nothing was found.
	*/
	virtual RPoint& NextObjectOri(void)=0;

	/**
	* Place the current object to a specific position. This function is called
	* by the NextObject method.<BR>
	* This function is responsible to update Result.
	* @param pos	The position where to place it.
	*/
	virtual void Place(RPoint& pos)=0;
	
	/**
	* Run the heuristic.
	* @param prob			The problem.
	* @param infos			Pointer to the geometric information.
	* @param grid			Pointer to the grid.		
	*/
	void Run(RProblem2D* prob,RGeoInfo** infos,RGrid* grid);
	
	/**
	* Run the heuristic.
	* @param prob			The problem.
	* @param infos			Pointer to the geometric information.
	* @param grid			Pointer to the grid.		
	*/
	void Run(RProblem2D* prob,RGeoInfos* infos,RGrid* grid);

	/**
	*	Do some operations after the run.
	*/
	virtual void PostRun(RPoint &){}

	/**
	* Return the bound rectangle containing all the objects.
	*/
   	RRect& GetResult(void);

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
	* Destruct the placement heuristic.
	*/
	virtual ~RPlacementHeuristic(void);
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
