/*

	R Project Library

	RGeoInfoConnections.h

	Connections for the 2D Placement (Geometric Information Part) - Header.

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
#ifndef RGeoInfoConnectionsH
#define RGeoInfoConnectionsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rgeoinfoconnection.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RConnections class provides a representation for the connections of the
* problem.
* @short Connections.
* @author Pascal Francq
*/
class RGeoInfoConnections : public RContainer<RGeoInfoConnection,unsigned int,true,false>
{
};


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
