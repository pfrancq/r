/*

	R Project Library

	RFreePolygon.h

	Polygon representing a free space - Header.

	Copyright 2000-2003 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RFreePolygon_H
#define RFreePolygon_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rpoint.h>
#include <rpolygon.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward declaration
class RGeoInfo;


//------------------------------------------------------------------------------
/**
* The RFreePolygon class provides a representation of a "free" polygon on the
* grid.
* @author Pascal Francq
* @short Free Polygon class.
*/
class RFreePolygon : public RPolygon
{
	/**
	* The position of the polygon.
	*/
	RPoint Pos;

	/**
	* Area of the polygon.
	*/
	tCoord Area;

public:

	/**
	* Construct a free polygon.
	*/
	RFreePolygon(const RPolygon& poly);

	/**
	* Construct a free polygon from another one.
	*/
	RFreePolygon(const RFreePolygon& poly);

	/**
	* The equal operator.
	*/
	bool operator==(const RFreePolygon& poly) const {return(RPolygon::operator==(poly));}

	/**
	* The non-equal operator.
	*/
	bool operator!=(const RFreePolygon& poly) const {return(RPolygon::operator!=(poly));}

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly           Polygon used for the comparaison.
	*/
	int Compare(const RFreePolygon* poly) const {return((*this)!=(*poly));}

	/**
	* This function compares two polygons and returns 0 if there have the same
	* number of points and at the same positions. This function is used for the
	* class RContainer.
	* @param poly           Polygon used for the comparaison.
	*/
	int Compare(const RFreePolygon& poly) const {return((*this)!=poly);}

	/**
	* This function test if a geometric information can be contained in the free
	* polygon.
	* @param info           The geometric information to compare.
	* @param pos            Position where the geometric information can eventualy
	*                       be inserted.
	* @return The function returns 0 if the polygon can't be contained, >0 if
	* the polygon can be contained. The less is the space wasted, the
	* greather is the return value.
	*/
	int CanContain(const RGeoInfo* info,RPoint& pos) const;

	/**
	* Return the position of the free polygon.
	*/
	RPoint GetPos(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
