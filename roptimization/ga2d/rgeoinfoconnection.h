/*

	R Project Library

	RGeoInfoConnection.h

	Connection for the 2D Placement (RGeoInfo Part) - Header.

	(c) 2000-2001 by P. Francq.

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
#ifndef RGeoInfoConnectionH
#define RGeoInfoConnectionH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
using namespace RStd;
#include <rga2d/rgeoinfo.h>
#include <rga2d/rconnection.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RGeoInfoConnectionPart class provides a representation for an edge of a
* connection.
* @author Pascal Francq
* @short Connection's Part for Geometric Information
*/
class RGeoInfoConnectionPart
{
public:
	RPoint PosCon1;
	RPoint PosCon2;

	RGeoInfoConnectionPart(const RPoint& pt1,const RPoint& pt2);
	int Compare(const RGeoInfoConnectionPart* p) {return(this-p);}
};


//-----------------------------------------------------------------------------
/**
* The RGeoInfoConnection class provides a representation for a connection
* between two geometric information.
* @author Pascal Francq
* @short Connection for Geometric Information.
*/
class RGeoInfoConnection : RContainer<RGeoInfoConnectionPart,unsigned int,true,false>
{
public:
	/**
	* The connection.
	*/
	RConnection* Con;

	/**
	* Construct a connection.
	*/
	RGeoInfoConnection(const RConnection* con);

	/**
	* This function compares two connections returns 0 if there are the same.
	* @param c              Connection used for the comparaison.
	*/
	int Compare(const RGeoInfoConnection* c) {return(this-c);}
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
