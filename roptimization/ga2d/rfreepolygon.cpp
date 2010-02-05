/*

	R Project Library

	RFreePolygon.cpp

	Free Polygons are hole in a construction - Implementation.

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
#include <rfreepolygon.h>
#include <rgeoinfo.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RFreePolygon
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RFreePolygon::RFreePolygon(const RPolygon& poly)
	: RPolygon(poly), Pos(cNoCoord,cNoCoord)
{
	// Find the translation
	RCursor<RPoint> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->X<Pos.X)
			Pos.X=Cur()->X;
		if(Cur()->Y<Pos.Y)
			Pos.Y=Cur()->Y;
	}
	Area=RPolygon::Area();
}


//------------------------------------------------------------------------------
RFreePolygon::RFreePolygon(const RFreePolygon& poly)
	: RPolygon(poly)
{
	Pos=poly.Pos;
}


//------------------------------------------------------------------------------
int RFreePolygon::CanContain(const RGeoInfo* info,RPoint& pos) const
{
	RPoint act,e1,e2;
	size_t nbpts;
	RPolygon Test;

	nbpts=GetNb();
	act=GetBottomLeft();
	while(nbpts>0)
	{
		// Look for act
		e1=GetConX(act);
		e2=GetConY(act);
//		if((act->X<e1->X)&&(act->Y<e2->Y))
		{
			Test=info->GetConfig()->GetPolygon();
			Test+=act;
			if(IsIn(Test))
			{
				pos=act;
				return(static_cast<int>((100*info->GetArea())/Area));
			}
		}

		nbpts--;
		act=GetConX(act);
		nbpts--;
		act=GetConY(act);
	}
	return(0);
}


//------------------------------------------------------------------------------
RPoint RFreePolygon::GetPos(void)
{
	return(Pos);
}
