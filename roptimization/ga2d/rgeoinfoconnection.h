/*

	R Project Library

	RGeoInfoConnection.h

	Connection for the 2D Placement (RGeoInfo Part) - Header.

	Copyright 1998-2010 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RGeoInfoConnection_H
#define RGeoInfoConnection_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rgraph.h>
#include <rgeoinfo.h>
#include <rconnection.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RGeoInfoConnection class provides a representation for a connection
* placed.
* @author Pascal Francq
* @short Placed Connection.
*/
class RGeoInfoConnection : private RContainer<RGeoInfoPin,true,true>
{
	class Part;

	/**
	* The connection involved.
	*/
	RConnection* Con;

	/**
	 * Corresponding placement.
	 */
	RLayout* Layout;

	/**
	* Complete connection.
	*/
	bool Complete;

	/**
	* Actual total distance.
	*/
	tCoord Dist;

public:

	/**
	* Construct a connection.
	* @param layout         Layout.
	* @param con            Original connection.
	*/
	RGeoInfoConnection(RLayout* layout,RConnection* con);

	/**
	* Make this connection uncompleted.
	*/
	void UnComplete(void);

	/**
	* This function compares two connections returns 0 if there are the same.
	* @param c              Connection used for the comparison.
	*/
	int Compare(const RGeoInfoConnection& c) const;

	/**
	 * @return the corresponding connection.
	 */
	inline RConnection* GetConnection(void) const {return(Con);}

private:

	/**
	* Verify that at least two objects of the connection are placed.
	* @param layout          Layout.
	*/
	bool MinObjPlaced(RLayout* layout);

public:

	/**
	* Compute the distance of the connection for a given set of geometric
	* information. In practice, a graph is build and the minimum spanning tree
	* is computed. The method manages uncompleted connections (for which all
	* involved connectors are not placed).
	* @param layout          Layout.
	* @return The "minimal" distance of the connection.
	*/
	tCoord ComputeMinDist(RLayout* layout);

	/**
	* Compute the distance of the connection involving a given object.
	* Eventually, the object is considered to be placed at a given position.
	* @param layout          Layout.
	* @param info            The geometric information to place.
	* @param pos             Position where to placed the geometric information
	*                        (if RPoint::Null, considered it as already placed).
	* @return the computed distance.
	*/
	tCoord GetDist(RLayout* layout,RGeoInfo* info,const RPoint& pos=RPoint::Null);

	/**
	* Test if a a geometric information is in the connection.
	* @param info           Geometric information.
	* Return bool.
	*/
	bool IsIn(const RGeoInfo* info);

	/**
	* Test if a connector is in the connection.
	* @param con            Connector.
	* @return bool.
	*/
	bool IsIn(const RObj2DConfigConnector* con);

	/**
	* Test if a pin is in the connection.
	* @param pin             Pin.
	* @return bool.
	*/
	bool IsIn(const RObj2DConfigPin* pin);

	/**
	 * @return a cursor over the pins.
	 */
	RCursor<RGeoInfoPin> GetPins(void) const;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
