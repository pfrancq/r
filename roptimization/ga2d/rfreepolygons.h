/*

  rfreepolygons.h

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
#ifndef RFREEPOLYGONS_H
#define RFREEPOLYGONS_H


//-----------------------------------------------------------------------------
// include files for Rainbow
#include <rstd/rcontainer.h>
using namespace RStd;
#include <rga/rfreepolygon.h>
using namespace RGA;


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward declaration
class RGeoInfo;


//-----------------------------------------------------------------------------
/**
* \ingroup RGA
* The RFreePolygons provides a representation for a container of polygons.
* @author Pascal Francq
* @short Container of free polygons.
*/
class RFreePolygons : public RContainer<RFreePolygon,unsigned int,true,false>
{
public:

	/**
	* Construct the container.
	*/
	RFreePolygons(void);

	/**
	* Construct the container from another one.
	*/
	RFreePolygons(RFreePolygons *cont);

	/**
	* Calculate a position for the given geometric information.
	* @param info	The object to place.
	* @return	The function returns a valid position the object could be
	*				placed.
	*/
	RPoint& CanPlace(RGeoInfo *info);

	/**
	* Derstruct the container.
	*/
	virtual ~RFreePolygons(void);
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
