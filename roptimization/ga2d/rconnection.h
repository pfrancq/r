/*

  rconnection.h

  Connection for the 2D Placement - Header.

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
#ifndef RConnectionH
#define RConnectionH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rga.h"
#include "robj2d.h"
#include "rgeoinfo.h"
using namespace RGA;



//-----------------------------------------------------------------------------
namespace RGA{
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
	RObj2DConnector* From;
	RObj2DConnector* To;
	double Weight;

public:
	/**
	* Construct a connection.
	* @param from 		"From" Connector.
	* @param to 		"To" Connector.	
	* @param weight		Weight of the connection (Default is 1).
	*/
	RConnection(RObj2DConnector* from,RObj2DConnector* to,double weight=1.0);
	
	/**
	* This function compares two connections returns 0 if
	* there are the same.
	* @param c		Connection used for the comparaison.
	*/	
	int Compare(const RConnection* c) {return(this-c);}
	
	/**
	* Return the "from" part.
	*/
	RObj2DConnector* GetFrom(void) {return(From);}
	
	/**
	* Return the "from" part.
	*/
	RObj2DConnector* GetTo(void) {return(To);}

	/**
	* Return the weight.
	*/	
	double GetWeight(void) {return(Weight);}
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
