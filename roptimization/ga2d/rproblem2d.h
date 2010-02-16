/*

	R Project Library

	RProblem2D.h

	2D Placement Problem - Header.

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
#ifndef RProblem2D_H
#define RProblem2D_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rga2d.h>
#include <robj2d.h>
#include <robj2dconfig.h>
#include <rconnection.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Variables
const size_t ProblemId=cNoRef-1;


//------------------------------------------------------------------------------
/**
* The RProblem2D class provides a representation for a 2D placement problem.
* @author Pascal Francq
* @short 2D Placement Problem
*/
class RProblem2D : public RObj2D
{
	/**
	* Limits for the construction.
	*/
	RSize Limits;

	/**
	* Board (including the connectors).
	*/
	RRect Board;

	/**
	* Objects.
	*/
	RContainer<RObj2D,true,true> Objs;

	/**
	* Connections.
	*/
	RContainer<RConnection,true,false> Connections;

public:

	/**
	* Constructor of the problem.
	* @param name            Name of the problem.
	*/
	RProblem2D(const RString& name);

	/**
	* Determine the limits of the problem based of the shape.
	*/
	void DetermineBoard(void);

	/**
	 * Set the limits of the placement.
	 * @param limits         Limits.
	 */
	void SetLimits(const RSize& limits);

	/**
	 * @return the limits for the construction.
	 */
	RSize GetLimits(void) const {return(Limits);}

	/**
	 * @return the board for the construction.
	 */
	RRect GetBoard(void) const {return(Board);}

	/**
	 * Insert an object to the problem.
	 * @param obj            Object to add.
	 */
	void Insert(RObj2D* obj);

	/**
	 * @return the number of objects.
	 */
	size_t GetNbObjs(void) const {return(Objs.GetNb());}

	/**
	 * @return a pointer to a given object.
	 * @param id             Identifier of the object.
	 */
	RObj2D* GetObj(size_t id) const {return(Objs.GetPtr(id));}

	/**
	 * @return a pointer to a given object.
	 * @param name           Name of the object.
	 */
	RObj2D* GetObj(const RString& name) const {return(Objs.GetPtr(name,false));}

	/**
	 * @return a cursor over the objects.
	 */
	RCursor<RObj2D> GetObjs(void) const {return(RCursor<RObj2D>(Objs));}

	/**
	 * Insert a connection.
	 * @param con            Connection to insert.
	 */
	void Insert(RConnection* con);

	/**
	* @return the number of connections.
	*/
	size_t GetNbConnections(void) const {return(Connections.GetNb());}

	/**
	 * @return a pointer to a given connection.
	 * @param name           Name of the connection.
	 */
	RConnection* GetConnection(const RString& name) const {return(Connections.GetPtr(name,false));}

	/**
	 * @return the connections.
	 */
	const RCursor<RConnection> GetConnections(void) const {return(RCursor<RConnection>(Connections));}

	/**
	 * Insert a connector to the board.
	 * @param id             Identifier of the connector.
	 * @param name           Name of the connector.
	 * @return a pointer to connector created.
	 */
	RObj2DConnector* InsertConnector(size_t id,const RString& name);

	/**
	* Destructor of the problem.
	*/
	~RProblem2D(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
