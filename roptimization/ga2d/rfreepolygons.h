/*

	R Project Library

	RFreePolygons.h

	List of polygons representing a free space - Header.

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
#ifndef RFreePolygons_H
#define RFreePolygons_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rga/rfreepolygon.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward declaration
class RGeoInfo;


//------------------------------------------------------------------------------
/**
* The RFreePolygons provides a representation for a container of polygons.
* @author Pascal Francq
* @short Container of free polygons.
*/
class RFreePolygons : public RContainer<RFreePolygon,true,false>
{
public:

	/**
	* Construct the container.
	*/
	RFreePolygons(void);

	/**
	* Construct the container from another one.
	*/
	RFreePolygons(RFreePolygons* cont);

	/**
	* Calculate a position for the given geometric information.
	* @param info           The object to place.
	* @return The function returns a valid position the object could be
	* placed.
	*/
	RPoint CanPlace(RGeoInfo* info);

	/**
	* Derstruct the container.
	*/
	virtual ~RFreePolygons(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
