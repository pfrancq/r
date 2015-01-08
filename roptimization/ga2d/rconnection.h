/*

	R Project Library

	RConnection.h

	Connection for the 2D Placement - Header.

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RConnection_H
#define RConnection_H


//------------------------------------------------------------------------------
// include files for R Project
#include <robj2d.h>
#include <rgeoinfo.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RConnection class provides a representation for a connection between
* objects.
* @author Pascal Francq
* @short Connection.
*/
class RConnection : private RContainer<RObj2DConnector,false,false>
{
	/**
	 * Name of the connection.
	 */
	RString Name;

	/**
	* Weight of the connection.
	*/
	double Weight;

public:

	/**
	* Construct a connection.
	* @param name           Name of the connection.
	* @param weight         Weight of the connection (Default is 1).
	*/
	RConnection(const RString& name,double weight=1.0);

	/**
	* This function compares two connections returns 0 if
	* there are the same.
	* @param c              Connection used for the comparison.
	*/
	int Compare(const RConnection& c) const;

	/**
	 * Add a connector to the connection.
	 * @param con            Connector.
	 */
	void Insert(RObj2DConnector* con);

	/**
	 * @return the number of connectors.
	 */
	inline size_t GetNb(void) const {return(RContainer<RObj2DConnector,false,false>::GetNb());}

	/**
	 * @return a cursor over the connectors.
	 */
	RCursor<RObj2DConnector> GetConnectors(void) const;

	/**
	* @return the weight.
	*/
	double GetWeight(void) const {return(Weight);}

	/**
	 * @return the name.
	 */
	RString GetName(void) const {return(Name);}

	// friend classes
	friend class RConnections;
	friend class RObj2DConnector;
	friend class RObj2D;
	friend class RProblem2D;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
