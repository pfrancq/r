/*

	R Project Library

	RConnection.cpp

	Connection for the 2D Placement - Implementation.

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
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
// include files for R Project
#include <random.h>
#include <rconnection.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RConnection
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RConnection::RConnection(const RString& name,double weight)
	: RContainer<RObj2DConnector,false,false>(10), Name(name), Weight(weight)
{
}


//------------------------------------------------------------------------------
int RConnection::Compare(const RConnection&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
void RConnection::Insert(RObj2DConnector* con)
{
	InsertPtr(con);
	con->Connections.InsertPtr(this);
}


//------------------------------------------------------------------------------
RCursor<RObj2DConnector> RConnection::GetConnectors(void) const
{
	return(RCursor<RObj2DConnector>(*this));
}
