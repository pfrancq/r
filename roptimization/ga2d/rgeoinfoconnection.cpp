/*

	R Project Library

	RGeoInfoConnection.cpp

	Connections for the 2D Placement (Geometric Information Part) - Implementation.

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
// include files for R Project
#include <rga2d/rgeoinfoconnection.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
//
// class RGeoInfoConnectionPart
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeoInfoConnectionPart::RGeoInfoConnectionPart(const RPoint& pt1,const RPoint& pt2)
	: PosCon1(pt1), PosCon2(pt2)
{
}



//-----------------------------------------------------------------------------
//
// class RGeoInfoConnection
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeoInfoConnection::RGeoInfoConnection(const RConnection* con)
	: RContainer<RGeoInfoConnectionPart,unsigned int,true,false>(con->Connect.NbPtr,con->Connect.NbPtr/2)
{
}
