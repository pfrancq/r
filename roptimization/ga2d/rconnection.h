/*

	R Project Library

	RConnection.h

	Connection for the 2D Placement - Header.

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
#ifndef RConnectionH
#define RConnectionH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga/rga.h>
using namespace RGA;
#include <rga2d/robj2d.h>
#include <rga2d/rgeoinfo.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RConnection class provides a representation for a connection between two
* objects.
* @author Pascal Francq
* @short Connection.
*/
class RConnection
{
public:
	/**
	* Weight of the connection.
	*/
	double Weight;

	/**
	* The connectors involved in the connections.
	*/
	RContainer<RObj2DConnector,unsigned int,false,false> Connect;

	/**
	* Construct a connection.
	* @param nb             Max number of connectors involved.
	* @param weight         Weight of the connection (Default is 1).
	*/
	RConnection(unsigned int nb,double weight=1.0);

	/**
	* This function compares two connections returns 0 if
	* there are the same.
	* @param c              Connection used for the comparaison.
	*/	
	int Compare(const RConnection* c) {return(this-c);}

	/**
	* Return the weight.
	*/
	double GetWeight(void) {return(Weight);}

	/**
	* Return the distance of the net for a given set of geometric information.
	* The "Prim's Algorithm for Minimum Spanning Trees" is used.
	* @param infos          Geometric informations.
	* @returns The "minimal" distance of the net.
	*/
	double GetMinDist(RGeoInfo** infos);

	// friend classes
	friend class RConnections;
	friend class RObj2DConnector;
	friend class RObj2D;
	friend class RProblem2D;
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
