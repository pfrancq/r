/*

  rfreepolygon.cpp

  Description - Implementation.

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
// include files for Rainbow
#include <rga/rfreepolygon.h>
#include <rga/rgeoinfo.h>
using namespace RGA;



//-----------------------------------------------------------------------------
//
// class RFreePolygon
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RFreePolygon::RFreePolygon(RPolygon& poly)
	: RPolygon(poly)
{
	Pos=(*GetBottomLeft());
	(*this)-=Pos;
}


//-----------------------------------------------------------------------------
RFreePolygon::RFreePolygon(RFreePolygon* poly)
	: RPolygon(poly)
{
	Pos=poly->Pos;
}


//-----------------------------------------------------------------------------
int RFreePolygon::CanContain(RGeoInfo* info)
{
	if(!IsIn(info->GetBound())) return(0);
	return((100*info->GetArea())/Area());
}


//-----------------------------------------------------------------------------
RPoint& RFreePolygon::GetPos(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Pos;
	return(*pt);
}
