/*

  RRect.cpp

  Rectangle - Implemtation.

  (C) 1999-2000 by P. Francq.

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


//---------------------------------------------------------------------------
#include "rrect.h"
using namespace RGeometry;



//---------------------------------------------------------------------------
//
// Class "RRect"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RRect::RRect(void) : Pt1(),Pt2()
{
}


//---------------------------------------------------------------------------
RRect::RRect(RRect *rect) : Pt1(rect->Pt1),Pt2(rect->Pt2)
{
}


//---------------------------------------------------------------------------
RRect::RRect(RPoint *pt1,RPoint *pt2) : Pt1(pt1),Pt2(pt2)
{
}


//---------------------------------------------------------------------------
RRect::RRect(RCoord MinX,RCoord MinY,RCoord MaxX,RCoord MaxY) : Pt1(MinX,MinY),Pt2(MaxX,MaxY)
{
}


//---------------------------------------------------------------------------
double RRect::Area(void)
{
  return(static_cast<double>(Height())*static_cast<double>(Width()));
}


//---------------------------------------------------------------------------
bool RRect::Clip(const RRect &clip)
{
	bool bClip=false;

	// Test Min X
	if(Pt1.X<clip.Pt1.X)
	{
		bClip=true;
		Pt1.X=clip.Pt1.X;
	}
	if(Pt1.X>clip.Pt2.X)
	{
		bClip=true;
		Pt1.X=clip.Pt2.X;
	}

	// Test Min Y
	if(Pt1.Y<clip.Pt1.Y)
	{
		bClip=true;
		Pt1.Y=clip.Pt1.Y;
	}
	if(Pt1.Y>clip.Pt2.Y)
	{
		bClip=true;
		Pt1.Y=clip.Pt2.Y;
	}

	// Test Max X
	if(Pt2.X<clip.Pt1.X)
	{
		bClip=true;
		Pt2.X=clip.Pt1.X;
	}
	if(Pt2.X>clip.Pt2.X)
	{
		bClip=true;
		Pt2.X=clip.Pt2.X;
	}

	// Test Max Y
	if(Pt2.Y<clip.Pt1.Y)
	{
		bClip=true;
		Pt2.Y=clip.Pt1.Y;
	}
	if(Pt2.Y>clip.Pt2.Y)
	{
		bClip=true;
		Pt2.Y=clip.Pt2.Y;
	}

	// Return value
	return(bClip);
}


//---------------------------------------------------------------------------
void RRect::Translation(RCoord x,RCoord y)
{
	Pt1.X+=x;
	Pt1.Y+=y;
	Pt2.X+=x;
	Pt2.Y+=y;
}


//---------------------------------------------------------------------------
bool RRect::Overlap(RRect *rect)
{
	// Is up or bottom of rect
	if((Pt1.Y>rect->Pt2.Y)||(Pt2.Y<rect->Pt1.Y)) return(false);

	// Is left or right of rect
	if((Pt1.X>rect->Pt2.X)||(Pt2.X<rect->Pt1.X)) return(false);

	return(true);
}


//---------------------------------------------------------------------------
bool RRect::IsIn(RCoord X,RCoord Y)
{
	// Is up or bottom of rect
	if((Y>Pt2.Y)||(Y<Pt1.Y)) return(false);

	// Is left or right of rect
	if((X>Pt2.X)||(X<Pt1.X)) return(false);

	return(true);
}



//---------------------------------------------------------------------------
//
// RRects
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RRects::RRects(void) : RContainer<RRect,unsigned int,true,false>(20,10)
{
}


//---------------------------------------------------------------------------
RRects::RRects(unsigned int max) : RContainer<RRect,unsigned int,true,false>(max,10)
{
}


//---------------------------------------------------------------------------
RRects::RRects(RRects *rects) : RContainer<RRect,unsigned int,true,false>(rects)
{
}


//---------------------------------------------------------------------------
RRects& RRects::operator=(const RRects &rects)
{
	RContainer<RRect,unsigned int,true,false>::operator=(rects);
	return(*this);
}