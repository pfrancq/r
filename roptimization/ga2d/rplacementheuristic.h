/*

	Rainbow Library Project

	RPlacementHeuristic.h

	Generic Heuristic for Placement - Header

	(C) 1998-2000 by P. Francq.

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



//---------------------------------------------------------------------------
#ifndef RPlacementHeuristicH
#define RPlacementHeuristicH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rrect.h>
using namespace RGeometry2D;
#include "robj2d.h"
#include "rgeoinfo.h"
#include "rgrid.h"


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
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
	* The objects to place.
	*/
	RObj2D **Objs;

	/**
	* The geometric information of the objects.
	*/
	RGeoInfo **Infos;

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

public:

	/**
	* Construct the placement heuristic.
	* @param maxobjs		The maximum objects to placed fore this object.
	*/
	RPlacementHeuristic(unsigned int maxobjs);
	
	/**
	* Initialize the heuristic.
	*/
	virtual void Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs);

	/**
	* Place the next object.	
	*/
	RGeoInfo* NextObject(void);

	/**
	* Place the next object (To override) for a specific information.
	* @return		The function returns true if the object can be placed with the
	*						current orientation.
	*/
	virtual bool NextObjectOri(void)=0;

	/**
	* Run the heuristic.
	*/
	void Run(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs);

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
	* Destruct the placement heuristic.
	*/
	virtual ~RPlacementHeuristic(void);
};


}//------- End of namespace RGA ---------------------------------------------


//---------------------------------------------------------------------------
#endif