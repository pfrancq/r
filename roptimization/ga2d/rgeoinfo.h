/*

  RGeoInfo.h

  Geometric information - Header

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
#ifndef RGeoInfoH
#define RGeoInfoH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rcontainer.h"
using namespace RStd;
#include "rpoint.h"
#include "rrect.h"
#include "rpolygon.h"
using namespace RGeometry2D;
#include "robj2d.h"



//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Forward class declaration
class RObj2D;


//---------------------------------------------------------------------------
/** The RGeoInfo class provides a geometric information concerning the placement
	* of an object.
	* @author Pascal Francq
	* @short Geometric Information.
	*/
class RGeoInfo
{
public:
	/** Pointer to the the object.*/
	RObj2D *Obj;
	/** Indicate if the object was selected for the crossover.*/
	bool Selected;
	/** Position of the object.*/
	RPoint Pos;    			
	/** Index of the orientation used for this info.*/
	char Ori;
	/** Polygon representing the object.*/
  RPolygon *Bound;			
	/** Rectangles Decomposition of the polygon.*/
	RRects *Rects;				
	/** Rectangle holding the polygon.*/
	RRect Rect;

	/** Construct a geometric information.*/
  RGeoInfo(RObj2D *obj);

	/** Clears the geometric information.*/
	void Clear(void);

	/** Return the index of the orientation.*/
	inline char GetOri(void) {return(Ori);}

	/** Set to ith Orienation of the object.*/
	void SetOri(char i);

	/** Area used by the polygon.*/
	RCoord GetArea(void);

	/** Calculate the boundary rectangle of the polygon according to the position.
		* @param rect 	The rectangle that will hold the result.
		*/
  void Boundary(RRect &rect);

	/** Return true if the geometric information is a valid one.*/		
	bool IsValid(void);

	/** Return the object.*/
	RObj2D* GetObj(void) {return(Obj);}

	/** Assign the geometric information to the position and update the grids with the
		* identicator of the object.
		* @param pos			Position to place.
		*	@param OccX			The grid with X as entry.
		*	@param OccY			The grid with Y as entry.
		*/
  void Assign(const RPoint &pos,unsigned int **OccX,unsigned int **OccY);	

	/** Compare function use for the RContainer class. Compare only the adress of the
		* pointers.
		* @param info			The pointer used for the comparaison.
		*/
	int Compare(RGeoInfo *info) { return(info!=this); }

	/** Test if the object represented can be "pushed" to the left of a given
		* position.
		* @param test				The position.
		* @param limits	    The limits for the placement.
		* @param OccX				Grid with X as entry.
		* @return 	The function returns 0 if the OBJECT can be pushed, 1 if it can
		*  					be pushed but with clipping it or 2 if it can be pushed without
		*           to clip it.
		*/
	int TestLeft(RPoint test,RPoint &limits,unsigned int **OccX);

	/** Test if the object represented can be "pushed" to the bottom of a given
		* position.
		* @param test				The position.
		* @param limits	    The limits for the placement.
		* @param OccY				Grid with Y as entry.
		* @return 	The function returns 0 if the OBJECT can be pushed, 1 if it can
		*  					be pushed but with clipping it or 2 if it can be pushed without
		*           to clip it.
		*/
	int TestBottom(RPoint test,RPoint &limits,unsigned int **OccY);

	/** Calculate the position where the object represented can be placed, if it
		* is pushed to the bottom and to the left.
		* @param pos				The initial position, and then the final one.
		* @param limits	    The limits for the placement.
		* @param OccX				Grid with X as entry.
		* @param OccY				Grid with Y as entry.
		*/
	void PushBottomLeft(RPoint &pos,RPoint &limits,unsigned int **OccX,unsigned int **OccY);

	/** Test if the obejct can be placed at a specific position in regards of the
		* occupation.
		* @param pos				The position to test.
		* @param limits	    The limits for the placement.
		* @param OccX				Grid with X as entry.
		* @param OccY				Grid with Y as entry.
		*/
	bool Test(RPoint &pos,RPoint &limits,unsigned int **OccX,unsigned int **OccY);

	/** Calculate the position where the object represented can be placed, if it
		* is pushed in direction of the center.
		* @param pos				The initial position, and then the final one.
		* @param limits	    The limits for the placement.
		* @param OccX				Grid with X as entry.
		* @param OccY				Grid with Y as entry.
		*/
	void PushCenter(RPoint &pos,RPoint &limits,unsigned int **OccX,unsigned int **OccY);
	
	/** This function returns true if the two objects represented by this geometric information
		* overlap (Rects have to be calculated.*/
	bool Overlap(RGeoInfo *info);

	/** Return the Width of the object represented.*/
	inline RCoord Width(void) {return(Rect.Pt2.X+1);}

	/** Return the Height of the object represented.*/
	inline RCoord Height(void) {return(Rect.Pt2.Y+1);}

	/** Return the number of points contained in the polygon.*/
	inline unsigned int NbPoints(void) {return(Bound->NbPtr);}

	/** Start the iterator to go trough the points.*/
	inline void Start(void) {Bound->Start();}

	/** Test if the end of the points is reached.*/
	inline bool End(void) {return(Bound->End());}

	/** Goto the next point, if the end is reached, go to the beginning*/
	inline void Next(void) {Bound->Next();}

	/** Return the current point.*/
	RPoint& operator()(void);

	/** Assignment Operator.*/
	RGeoInfo& operator=(const RGeoInfo &info);

	/** Return true if a point is in the polygon.*/
	bool IsIn(RPoint pos);

	/** Add the polygon representing the object in the container of polygons.*/
	void Add(RPolygons &polys);
};


//---------------------------------------------------------------------------
/** This class implements a container of geometric information.
	* @author Pascal Francq
	* @short Container of geometric information.
	*/
class RGeoInfos : public RStd::RContainer<RGeoInfo,unsigned int,false,false>
{
public:
	/** Construct the container.
		* @param nb		Number of geometric information that will contained.
		*/
	RGeoInfos(unsigned int nb);

	/** Calculate the boundary rectangle of all the geometric information.
		* @param rect		The rectangle that will be hold the result.
		*/
	void Boundary(RRect &rect);
};


}  //-------- End of namespace RGA ------------------------------------------

//---------------------------------------------------------------------------
#endif
