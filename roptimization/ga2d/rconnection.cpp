/*

	R Project Library

	RCons.cpp

	Connections for the 2D Placement - Implementation.

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
#include <rmath/random.h>
using namespace RMath;
#include <rga2d/rconnection.h>
using namespace RGA2D;



//-----------------------------------------------------------------------------
//
// class RConnection
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RConnection::RConnection(RObj2DConnector* from,RObj2DConnector* to,double weight)
	: From(from), To(to), Weight(weight), Connect(10,5)
{
}


//-----------------------------------------------------------------------------
RGA2D::RConnection::RConnection(unsigned int nb,double weight=1.0)
	: From(0), To(0), Weight(weight), Connect(nb,nb/2)
{
}


//-----------------------------------------------------------------------------
double RGA2D::RConnection::GetMinDist(RGeoInfo** infos)
{
	RObj2DConnector **V,*u,**W;
	unsigned int nbW;

	// Init V
	V=new RObj2DConnector*[Connect.NbPtr-1];
	memcpy(V,&Connect.Tab[1],sizeof(RObj2DConnector*)*(Connect.NbPtr-1));
	W=new RObj2DConnector*[Connect.NbPtr];
	nbW=1;
	u=W[0]=Connect.Tab[0];

	//
	return(0.0);
}
