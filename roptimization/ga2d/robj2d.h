/*

	Rainbow Library Project

  RObj2D.h

  Object for 2D placement GA - Header

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

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
#ifndef RObj2DH
#define RObj2DH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rgeometry/polygons.h"
using namespace RGeometry;
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This basic class represent an object to place by using the 2D placement GA.
	*	@author Pascal Francq
	* @short 2D Object.
	*/
class RObj2D
{
public:
	/** Id of the object.*/
	unsigned int Id;
	/** Number of possible Orientations.*/
	char NbPossOri;
	/** Different Orientations accepted.*/
  char PossOri[8];
	/** Polygon that define the object.*/
  RPolygon Polygon;
	/** Polygons corresponding to the possible orientations.*/
  RPolygon Polygons[8];
	/** Specify if the object is deformable or rigid.*/
  bool Deformable;

	/** Construct an 2D object.
		* @param id						The identificator of the object.
		* @param deformable   Specify if the object is deformable.
		*/
  RObj2D(unsigned int id,bool deformable);

	/** Calculate all the polygons based on the possible orientations of the object.
		*	@return The function returns true if the calculation has be done without errors.
		*/
	bool CalcPolygons(void);

	/** Set a specific orientation as possible.*/
	void SetOri(char ori);

	/** Return true if the orientation is possible.*/
	bool IsOriSet(char ori);

	/** The assignment operator.*/
  RObj2D& operator=(const RObj2D &obj);
};


//---------------------------------------------------------------------------
/** This class represent an object containg other objects to place. It is use,
	* for example, in the 2D placement GA crossover.
	* @author Pascal Francq
	* @short Container of objects.
	*/
class RObj2DContainer : public RObj2D
{
public:
	/** Ids of the objects contained.*/
	unsigned int *Ids;					
	/** Pointer to all geometric infos of the objects.*/
	RGeoInfo** Infos;						
	/** Maximum number of objects contained.*/
	unsigned int NbMax;					
	/** Number of objects contained.*/
	unsigned int Nb;						
	/** Polygons of the objects.*/
	RPolygons SPolygons;				
	/** Point the most left-bottom.*/
	RCoord MinX,MinY;						
	/** Point the most left-bottom.*/
  double Area;							

	/** Construct the container of objects.
		* @param id		The identificator of the object.
		* @param max	Maxmimum number of objects that can be contained.
		*/
	RObj2DContainer(unsigned int id,unsigned int max);

	/** Clear the container.*/
	void Clear(void);

	/** Add an object and his information to the container.
		* @param obj		The object to add.
		* @param info		The geometric information of the object.
		*/
	void AddObj(RObj2D *obj,RGeoInfo *info);

	/** Does some calculation after each objects were added to the container.*/
	void EndObjs(void);

	/** Assign this container to a position and replace it in the grid with all the
		* identificators of the objects contained.*/
	void Assign(RGeoInfo **infos,unsigned int **OccX,unsigned int **OccY,RCoord x,RCoord y);

	/** Destruct the container.*/
	~RObj2DContainer(void);
};


}//------- End of namespace RGA ---------------------------------------------



//---------------------------------------------------------------------------
#endif
