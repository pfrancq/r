/*

	R Project Library

	RFreePolygon.cpp

	Free Polygons are hole in a construction - Implementation.

	(c) 2000-2003 by P. Francq.

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rga2d/rfreepolygon.h>
#include <rga2d/rgeoinfo.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RFreePolygon
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RFreePolygon::RFreePolygon(RPolygon& poly)
	: RPolygon(poly), Pos(MaxCoord,MaxCoord)
{
	unsigned int i;
	RPoint **tab;

	// Find the translation
	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		if((*tab)->X<Pos.X) Pos.X=(*tab)->X;
		if((*tab)->Y<Pos.Y) Pos.Y=(*tab)->Y;
	}
	Area=RPolygon::Area();
}


//------------------------------------------------------------------------------
RFreePolygon::RFreePolygon(RFreePolygon* poly)
	: RPolygon(poly)
{
	Pos=poly->Pos;
}


//------------------------------------------------------------------------------
int RFreePolygon::CanContain(RGeoInfo* info,RPoint& pos)
{
	RPoint *act,*e1,*e2;
	unsigned int nbpts;
	RPolygon Test;

	nbpts=NbPtr;
	act=GetBottomLeft();
	while(nbpts>0)
	{
		// Look for act
		e1=GetConX(act);
		e2=GetConY(act);
//		if((act->X<e1->X)&&(act->Y<e2->Y))
		{
			Test=info->GetBound();
			Test+=(*act);
			if(IsIn(Test))
			{
				pos=(*act);
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
RPoint& RFreePolygon::GetPos(void)
{
	RPoint *pt=RPoint::GetPoint();

	(*pt)=Pos;
	return(*pt);
}
