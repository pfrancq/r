/*

  rfreepolygon.h

  Description - Header.

  (c) 2000 by P. Francq.

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



//-----------------------------------------------------------------------------
#ifndef RFREEPOLYGON_H
#define RFREEPOLYGON_H


//-----------------------------------------------------------------------------
#include <rgeometry/rpoint.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* \ingroup 2DGA
* The RFreePolygon class provides a representation of a "free" polygon on the grid.
* @author Pascal Francq
* @short Free Polygon class.
*/
class RFreePolygon : public RPolygon
{
public:

	/**
	* Construct a free polygon.
	*/	
	RFreePolygon(RPolygon& poly);

	/**
	* The equal operator.
	*/
 	bool operator==(const RFreePolygon &poly) {return(RPolygon::operator==(poly));}

	/**
	* The non-equal operator.
	*/
	bool operator!=(const RFreePolygon &poly) {return(RPolygon::operator!=(poly));}

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly		Polygon used for the comparaison.
	*/
	int Compare(const RFreePolygon* poly) {return((*this)!=(*poly));}

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly		Polygon used for the comparaison.
	*/
	int Compare(const RFreePolygon& poly) {return((*this)!=poly);}

};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
