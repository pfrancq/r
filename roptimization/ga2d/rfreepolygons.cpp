/*

  rfreepolygons.cpp

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
#include <rga/rfreepolygons.h>
#include <rga/rgeoinfo.h>
using namespace RGA;



//-----------------------------------------------------------------------------
//
// class RFreePolygons
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RFreePolygons::RFreePolygons(void)
	: RContainer<RFreePolygon,unsigned int,true,false>(30,15)
{
}


//-----------------------------------------------------------------------------
RFreePolygons::RFreePolygons(RFreePolygons *cont)
	: RContainer<RFreePolygon,unsigned int,true,false>(cont)
{
}


//-----------------------------------------------------------------------------
RPoint& RFreePolygons::CanPlace(RGeoInfo *info)
{
	char o;
	unsigned int i;
	RFreePolygon **tab;
	RPoint* pt=RPoint::GetPoint();
	RObj2D* obj=info->GetObj();
	
  	for(i=NbPtr+1,tab=Tab;--i;tab++)	
	{	
		for(o=0;o<obj->NbPossOri;o++)
   	{
	   	info->SetOri(o);   		
   		if((*tab)->CanContain(info))
   		{
   			(*pt)=(*tab)->GetPos();
   			DeletePtr(*tab);
   			return(*pt);
   		}
   	}
	}
	pt->Set(MaxCoord,MaxCoord);
	return(*pt);
}


//-----------------------------------------------------------------------------
RFreePolygons::~RFreePolygons(void)
{
}
