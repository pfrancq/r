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
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** The RGeoInfo class provides a geometric information concerning the placement
	* of an object.
	* @author Pascal Francq
	* @short Geometric Information.
	*/
class RGeoInfo
{
public:
	/** Identificator of the object.*/
	unsigned int Id;
	/** Indicate if the object was selected for the crossover.*/
	bool Selected;
	/** Position of the object.*/
	RPoint Pos;    			
	/** Polygon representing the object.*/
  RPolygon Bound;			
	/** Rectangles Decomposition of the polygon.*/
	RRects Rects;				

	/** Construct a geometric information.*/
  RGeoInfo(void);

	/** Clears the geometric information.*/
	void Clear(void);

	/** This function adpat the rectangles according to the position of the polygon.*/
	void AdaptPos(void);

	/** Area used by the polygon (Rects have to be calculated).*/
	double Area(void);	

	/** Calculate the boundary rectangle of the polygon according to the position.
		* @param rect 	The rectangle that will hold the result.
		*/
  void Boundary(RRect &rect);

	/** Assign the geometric informartion with a polygon, and calculate the rectangular
		* decomposition.
		* @param id			The identificator of the object.
		* @param poly		The polygon used.
		*/
	void AssignBound(unsigned int id,const RPolygon& poly);

	/** Assign the geometric information to the position and update the grids with the
		* identicator of the object.
		*	@param OccX			The grid with X as entry.
		*	@param OcYX			The grid with Y as entry.
		* @param x				X position for the object.
		* @param y				Y position for the object.
		*/
  void Assign(unsigned int **OccX,unsigned int **OccY,RCoord x,RCoord y);	

	/** Compare function use for the RContainer class. Compare only the adress of the
		* pointers.
		* @param info			The pointer used for the comparaison.
		*/
	int Compare(RGeoInfo *info) { return(info!=this); }

	/** This function returns true if the two objects represented by this geometric information
		* overlap (Rects have to be calculated.*/
	bool Overlap(RGeoInfo *info);

	/** Assignment Operator.*/
	RGeoInfo& operator=(const RGeoInfo &info);
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
