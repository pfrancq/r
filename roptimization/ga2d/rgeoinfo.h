/*

	Rainbow Library Project

	RGeoInfo.h

	Geometric information - Header

	(C) 1999-2000 by P. Francq.

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
#ifndef RGeoInfoH
#define RGeoInfoH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rrect.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;
#include <rga/robj2d.h>
#include <rga/rgrid.h>


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RObj2D;
class RGrid;
class RGeoInfo;



//-----------------------------------------------------------------------------
/**
* \ingroup 2DGA
* The RGeoInfoConnector class provides a connector of a geometric information
* concerning the placement of an object.
* @author Pascal Francq
* @short Connector of a Geometric Information.
*/
class RGeoInfoConnector
{
	/**
	* Pointer to the real connector.
	*/
	RObj2DConnector* Con;
	
	/**
	* Owner of the connector.
	*/
	RGeoInfo* Owner;
	
	/**
	* Position of the connector in the geometric information.
	*/
	RPoint Pos;
	
public:

	/**
	* Constructor of the connector.
	* @param con		"Real" Connector of this one.
	* @param owner		Geometric Information of the connector.
	* @param pos		Position of the connector.
	*/	
	RGeoInfoConnector(RObj2DConnector *con,RGeoInfo* owner,const RPoint &pos);
	
	RPoint& GetPos(void);
};



//-----------------------------------------------------------------------------
/**
* \ingroup 2DGA
* The RGeoInfo class provides a geometric information concerning the placement
* of an object.
* @author Pascal Francq
* @short Geometric Information.
*/
class RGeoInfo
{
	/**
	* Pointer to the the object.
	*/
	RObj2D *Obj;

	/**
	* Indicate if the object was selected for the crossover.
	*/
	bool Selected;

	/**
	* Position of the object.
	*/
	RPoint Pos;    			

	/**
	* Index of the orientation used for this info.
	*/
	char Ori;

	/**
	* Polygon representing the object.
	*/
	RPolygon *Bound;			

	/**
	* Rectangles Decomposition of the polygon.
	*/
	RRects *Rects;				

	/**
	* Rectangle holding the polygon.
	*/
	RRect Rect;

public:

	/**
	* Construct a geometric information.
	*/
	RGeoInfo(RObj2D *obj);

	/**
	* Construct a geometric information from another one.
	*/
	RGeoInfo(RGeoInfo *info);

	/**
	* Clears the geometric information.
	*/
	void Clear(void);

	/**
	* Return the index of the orientation.
	*/
	inline char GetOri(void) {return(Ori);}

	/**
	* Set to ith Orienation of the object.
	*/
	void SetOri(char i);

	/**
	* Area used by the polygon.
	*/
	RCoord GetArea(void);

	/**
	* Calculate the boundary rectangle of the polygon according to the position.
	* @param rect 	The rectangle that will hold the result.
	*/
	void Boundary(RRect &rect);

	/**
	* Return true if the geometric information is a valid one.
	*/	
	bool IsValid(void) const;

	/**
	* Return true if the object represented by the geometric information is
	* selected.
	*/
	bool IsSelect(void) const {return(Selected);}

	/**
	* Set that the object represented by the geometric information is selected.
	*/
	void SetSelect(void) {Selected=true;}

	/**
	* Return the object.
	*/
	RObj2D* GetObj(void) const {return(Obj);}

	/**
	* Assign the geometric information to the position and update the grids with the
	* identicator of the object.
	* @param pos				Position to place.
	* @param OccX				The grid with X as entry.
	* @param OccY				The grid with Y as entry.
	*/
	void Assign(const RPoint &pos,RGrid *grid);

	/**
	* Compare function use for the RContainer class. Compare only the adress of the
	* pointers.
	* @param info			The pointer used for the comparaison.
	*/
	int Compare(RGeoInfo *info) { return(info!=this); }

	/**
	* Test if the object can be placed at a specific position in regards of the
	* occupation.
	* @param pos				The position to test.
	* @param limits    	The limits for the placement.
	* @param grid				Grid.
	*/
	bool Test(RPoint &pos,RPoint &limits,RGrid *grid);

	/**
	* Calculate the position where the object represented can be placed, if it
	* is pushed to the bottom and to the left.
	* @param pos				The initial position, and then the final one.
	* @param limits			The limits for the placement.
	* @param grid				Grid.
	*/
	void PushBottomLeft(RPoint &pos,RPoint &limits,RGrid *grid);

	/**
	* Calculate the position where the object represented can be placed, if it
	* is pushed in direction of the center.
	* @param pos				The initial position, and then the final one.
	* @param limits	    The limits for the placement.
	* @param grid				Grid.
	*/
	void PushCenter(RPoint &pos,RPoint &limits,RGrid *grid);
	
	/**
	* This function returns true if the two objects represented by this geometric information
	* overlap (Rects have to be calculated.*/
	bool Overlap(RGeoInfo *info);

	/**
	* Return the Width of the object represented.
	*/
	inline RCoord Width(void) {return(Rect.Width());}

	/**
	* Return the Height of the object represented.
	*/
	inline RCoord Height(void) {return(Rect.Height());}

	/**
	* Return the number of points contained in the polygon.
	*/
	inline unsigned int NbPoints(void) {return(Bound->NbPtr);}

	/**
	* Return the position of the geometric information.
	*/
	RPoint& GetPos(void);

	/**
	* Make a translation of the point.
	* @param pt 	The point representing the vector used.
	*/
	RGeoInfo& operator+=(const RPoint &pt) {Pos+=pt;return(*this);}

	/**
	* Make a translation of the point.
	* @param pt 	The point representing the vector used.
	*/
	RGeoInfo& operator-=(const RPoint &pt)
	{
		Pos-=pt;
		return(*this);
	}

	/**
	* Start the iterator to go trough the points.
	*/
	inline void Start(void) {Bound->Start();}

	/**
	* Test if the end of the points is reached.
	*/
	inline bool End(void) {return(Bound->End());}

	/**
	* Goto the next point, if the end is reached, go to the beginning.
	*/
	inline void Next(void) {Bound->Next();}

	/**
	* Return the current point.
	*/
	RPoint& operator()(void);

	/**
	* Assignment Operator.
	*/
	RGeoInfo& operator=(const RGeoInfo &info);

	/**
	* Return true if a point is in the polygon.
	*/
	bool IsIn(RPoint pos);

	/**
	* Add the polygon representing the object in the container of polygons.
	*/
	void Add(RPolygons &polys);

	/**
	* Return the polygon representing the object placed.
	*/
	RPolygon& GetPolygon(void);

	/**
	* Return the polygon representing the object to place.
	*/
	RPolygon* GetBound(void) {return(Bound);}
};


//-----------------------------------------------------------------------------
/**
* This class implements a container of geometric information.
* @author Pascal Francq
* @short Container of geometric information.
*/
class RGeoInfos : public RStd::RContainer<RGeoInfo,unsigned int,false,false>
{
public:

	/**
	* Construct the container.
	* @param nb		Number of geometric information that will contained.
	*/
	RGeoInfos(unsigned int nb);

	/**
	* Calculate the boundary rectangle of all the geometric information.
	* @param rect		The rectangle that will be hold the result.
	*/
	void Boundary(RRect &rect);
};


}  //-------- End of namespace RGA --------------------------------------------


//-----------------------------------------------------------------------------
#endif
