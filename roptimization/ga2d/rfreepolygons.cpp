/*

	R Project Library

	RFreePolygons.cpp

	Description - Implementation.

	(c) 2000-2003 by P. Francq.

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
#include <rfreepolygons.h>
#include <rgeoinfo.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RFreePolygons
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RFreePolygons::RFreePolygons(void)
	: RContainer<RFreePolygon,true,false>(30,15)
{
}


//------------------------------------------------------------------------------
RFreePolygons::RFreePolygons(const RFreePolygons& cont)
	: RContainer<RFreePolygon,true,false>(cont)
{
}


//------------------------------------------------------------------------------
RPoint RFreePolygons::CanPlace(RGeoInfo* info)
{
	char o;
	RPoint pt;
	RObj2D* obj=info->GetObj();

	RCursor<RFreePolygon> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		for(o=0;o<obj->NbPossOri;o++)
		{
			info->SetOri(o);
			if(Cur()->CanContain(info,pt))
			{
				DeletePtr(Cur());
				return(pt);
			}
		}
	}
	pt.Set(MaxCoord,MaxCoord);
	return(pt);
}


//------------------------------------------------------------------------------
RFreePolygons::~RFreePolygons(void)
{
}
