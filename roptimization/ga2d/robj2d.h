/*

	Rainbow Library Project

	RObj2D.h

	Object for 2D placement GA - Header

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



//---------------------------------------------------------------------------
#ifndef RObj2DH
#define RObj2DH


//---------------------------------------------------------------------------
// include files for Rainbow
#include <rgeometry/rpoint.h>
#include <rgeometry/rrect.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Forward class declaration
class RGeoInfo;


//---------------------------------------------------------------------------
/**
* This constance represent a non-identificator. It is used, for example, to
* specify that a variable containing an identificator of an object. is
* referencing no object.
*/
const unsigned int NoObject=UINT_MAX;


//---------------------------------------------------------------------------
/**
* This basic class represent an object to place by using the 2D placement GA.
* @author Pascal Francq
* @short 2D Object.
*/
class RObj2D
{
public:

	/**
	* Identificator of the object.
	*/
	unsigned int Id;

	/**
	* Polygon that define the object.
	*/
  RPolygon Polygon;

	/**
	* Represent the area of the object.
	*/
	RCoord Area;

	/**
	* Number of possible Orientations.
	*/
	char NbPossOri;

	/**
	* Different Orientations accepted.
	*/
	ROrientation PossOri[8];

	/**
	* Polygons for the possible orientations.
	*/
	RPolygon Polygons[8];

	/**
	* Rectangular decompositions for the possible orientatoins.
	*/
	RRects Rects[8];

	/**
	* Specify if the object is deformable or rigid.
	*/
	bool Deformable;

	/**
	* Construct an 2D object.
	* @param id						The identificator of the object.
	* @param deformable   Specify if the object is deformable.
	*/
	RObj2D(unsigned int id,bool deformable);

	/**
	* Initialize the object when all information are entered. In particular,
	* it calculates the different polygon based on the possible orientations,
	* the rectangular decompositions and the area of the object.*/
	void Init(void);

	/**
	* Calculate all the polygons based on the possible orientations of the object.
	*	@return The function returns true if the calculation has be done without errors.
	*/
	void CalcPolygons(void);

	/**
	* Set a specific orientation as possible.
	*/
	void SetOri(ROrientation o);

	/**
	* Return true if the orientation is possible.
	*/
	bool IsOriSet(ROrientation o);

	/**
	* Return the identificator of the object.
	*/
	inline unsigned int GetId(void) {return(Id);}

	/**
	* Return the area of the object.
	*/
	inline RCoord GetArea(void) {return(Area);}

	/**
	* Return a pointer to the polygon representing the ith orientation.
	*/
	RPolygon* GetPolygon(char i);

	/**
	* Return a pointer to the rectangular decomposition of the ith orientation.
	*/
	RRects* GetRects(char i);

	/**
	* The assignment operator.
	*/
	RObj2D& operator=(const RObj2D &obj);
};


//---------------------------------------------------------------------------
/**
* This class represent an object containg other objects to place. It is use,
* for example, in the 2D placement GA crossover.
* @author Pascal Francq
* @short Container of objects.
*/
class RObj2DContainer : public RObj2D
{
public:

	/**
	* Identificators of the objects contained.
	*/
	unsigned int *Ids;					

	/**
	* Pointer to all geometric infos of the objects.
	*/
	RGeoInfo** Infos;						

	/**
	* Maximum number of objects contained.
	*/
	unsigned int NbMax;					

	/**
	* Number of objects contained.
	*/
	unsigned int Nb;						

	/**
	* Polygons of the objects.
	*/
	RPolygons SPolygons;				

	/**
	* Point the most left-bottom.
	*/
	RCoord MinX,MinY;						

 	/**
	* Construct the container of objects.
	* @param id				The identificator of the object.
	* @param max			Maxmimum number of objects that can be contained.
	*/
	RObj2DContainer(unsigned int id,unsigned int max);

	/**
	* Clear the container.
	*/
	void Clear(void);

	/**
	* Add an object and his information to the container.
	* @param obj		The object to add.
	* @param info		The geometric information of the object.
	*/
	void AddObj(RObj2D *obj,RGeoInfo *info);

	/**
	* Does some calculation after each objects were added to the container.
	*/
	void EndObjs(void);

	/**
	* Assign this container to a position and replace it in the grid with all the
	* identificators of the objects contained.
	* @param pos		Position.
	* @param infos  The geometric information of the objects.
	* @param OccY		Grid with X as entry.
	* @param OccY		Grid with Y as entry.
	*/
	void Assign(RPoint &pos,RGeoInfo **infos,unsigned int **OccX,unsigned int **OccY);

	/**
	* Destruct the container.
	*/
	~RObj2DContainer(void);
};


}//------- End of namespace RGA ---------------------------------------------


//---------------------------------------------------------------------------
#endif
