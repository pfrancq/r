/*

  rconnections.h

  Connections for the 2D Placement - Header.

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
#ifndef RConnectionsH
#define RConnectionsH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rga.h"
#include "robj2d.h"
#include "rgeoinfo.h"
#include "rconnection.h"
using namespace RGA;



//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
/**
* The RConnections class provides a representation for the connections of the problem.
* @short Connections.
* @author Pascal Francq
*/
class RConnections : public RContainer<RConnection,unsigned int,true,false>
{			
public:

	/**
	* Construct the connections.
	* @param m		Maximal number of objects connected.
	*/
	RConnections(unsigned int m);

	/**
	* Initialize all the necessary structures when all conections are inserted.
	*/	
	void Init(void);
		
	/**
	* Calculate the sum of the weight of the connected objects, that are already
	* placed, to a given one representing by a geometric information.
	* @param infos			The geometric information of all the objects.
	* @param nb				Number of objects.
	* @param cur			Geometric information representing the given object for
	*						wich calculate the distance.	
	*/
	double GetCon(RGeoInfo **infos,unsigned int nb,RGeoInfo *cur);
	
	/**
	* Return the object that is not placed and that have the most connected
	* weight with the objects already placed.
	* @return				The geometric information of the object to place.
	* @param infos			The geometric information of all the objects.
	* @param nb				Number of objects.
	*/
	RGeoInfo* GetMostConnected(RGeoInfo **infos,unsigned int nb);
		
	/**
	* Calculate the Manhanttan distance from one given object representing by a
	* geometric information to all other that are connected with it.
	*	@param infos		The geometric information of all the objects.
	* @param nb				Number of objects.
	* @param cur			Geometric information representing the given object for
	*						wich calculate the distance.	
	* @param pos			The position of the object.
	*/
	double GetDistance(RGeoInfo **infos,unsigned int nb,RGeoInfo *cur,RPoint& pos);
	
	/**
	* Add a connection.
	* @param from		The "from" object.
	* @param to			The "to" object.
	* @param weight		The weight of the connection.
	*/
	void AddConnection(RObj2DConnector* from,RObj2DConnector* to,double weight=1.0);
	
	/**
	* Destruct the connections.
	*/
	~RConnections(void);
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
