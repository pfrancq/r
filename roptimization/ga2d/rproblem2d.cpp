/*

	R Project Library

	RProblem2D.cpp

	2D Placement Problem - Implementation.

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <stdlib.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rproblem2d.h>
#include <rsize.h>
#include <rpoints.h>
#include <rvectorbool.h>
#include <robj2dconfig.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// class RProblem2D
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RProblem2D::RProblem2D(const RString& name)
	: RObj2D(ProblemId,name), Objs(100), Connections(50)
{
}


//------------------------------------------------------------------------------
void RProblem2D::DetermineBoard(void)
{
	// Suppose the board has the limits of the system
	tCoord X1(0), Y1(0), X2(Limits.GetWidth()), Y2(Limits.GetHeight());
	RRect Rect(0,0,Limits.GetWidth(),Limits.GetHeight());

	// Go through each external connectors to determine the limits
	RCursor<RObj2DConfigConnector> Con(GetDefaultConfig()->GetConnectors());
	for(Con.Start();!Con.End();Con.Next())
	{
		RCursor<RObj2DConfigPin> Pins(*Con());
		for(Pins.Start();!Pins.End();Pins.Next())
		{
			RRect r(Pins()->GetRect());

			// Verify that it is an external connector
			if(Rect.IsIn(r))
				ThrowRException("Pin '"+Pins()->GetPin()->GetName()+"' of external connector '"+Con()->GetConnector()->GetName()+"' is invalid");

			// Adapt the board
			if(r.GetX1()<X1) X1=r.GetX1();
			if(r.GetY1()<Y1) Y1=r.GetY1();
			if(r.GetX2()>X2) X2=r.GetX2();
			if(r.GetY2()>Y2) Y2=r.GetY2();
		}
	}

	Board.Set(X1,Y1,X2,Y2);
}


//------------------------------------------------------------------------------
void RProblem2D::SetLimits(const RSize& limits)
{
	Limits=limits;
}


//------------------------------------------------------------------------------
void RProblem2D::Insert(RObj2D* obj)
{
	Objs.InsertPtr(obj);
}


//------------------------------------------------------------------------------
void RProblem2D::Insert(RConnection* con)
{
	Connections.InsertPtr(con);
}


//------------------------------------------------------------------------------
RObj2DConnector* RProblem2D::InsertConnector(size_t id,const RString& name)
{
	RObj2DConnector* Connector(new RObj2DConnector(this,id,name));
	GetDefaultConfig()->Add(new RObj2DConfigConnector(Connector));
	return(Connector);
}


//------------------------------------------------------------------------------
RProblem2D::~RProblem2D(void)
{
}
