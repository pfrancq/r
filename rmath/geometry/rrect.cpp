/*

	R Project Library

	RRect.cpp

	Rectangle - Implemtation.

	(C) 1999-2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for R Project
#include <rgeometry/rrect.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;



//-----------------------------------------------------------------------------
//
// Class "RRect"
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeometry2D::RRect::RRect(void)
	: Pt1(),Pt2()
{
}


//-----------------------------------------------------------------------------
RGeometry2D::RRect::RRect(const RRect *rect)
	: Pt1(),Pt2()
{
	if(rect)
	{
		Pt1=rect->Pt1;
		Pt2=rect->Pt2;
	}
	else
		RReturnIfFail(rect);
}


//-----------------------------------------------------------------------------
RGeometry2D::RRect::RRect(const RPoint *pt1,const RPoint *pt2)
	: Pt1(), Pt2()
{
	RReturnIfFail(pt1&&pt2);
	Pt1=(*pt1);
	Pt2=(*pt2);
}


//-----------------------------------------------------------------------------
RGeometry2D::RRect::RRect(const RCoord MinX,const RCoord MinY,const RCoord MaxX,const RCoord MaxY)
	: Pt1(MinX,MinY),Pt2(MaxX,MaxY)
{
}


//-----------------------------------------------------------------------------
RCoord RGeometry2D::RRect::Width(void) const
{
	return(labs(Pt2.X-Pt1.X+1));
}


//-----------------------------------------------------------------------------
RCoord RGeometry2D::RRect::Height(void) const
{
	return(labs(Pt2.Y-Pt1.Y+1));
}


//-----------------------------------------------------------------------------
RRect& RGeometry2D::RRect::operator+=(const RPoint &pt) throw(bad_alloc)
{
	Pt1+=pt;
	Pt2+=pt;
	return(*this);
}


//-----------------------------------------------------------------------------
RRect& RGeometry2D::RRect::operator-=(const RPoint &pt) throw(bad_alloc)
{
	Pt1-=pt;
	Pt2-=pt;
	return(*this);
}


//-----------------------------------------------------------------------------
bool RGeometry2D::RRect::Clip(const RRect &clip)
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


//-----------------------------------------------------------------------------
bool RGeometry2D::RRect::Clip(const RPoint &limits)
{
	bool bClip=false;

	// Test Min X
	if(Pt1.X>limits.X)
	{
		bClip=true;
		Pt1.X=limits.X;
	}

	// Test Min Y
	if(Pt1.Y>limits.Y)
	{
		bClip=true;
		Pt1.Y=limits.Y;
	}

	// Test Max X
	if(Pt2.X>limits.X)
	{
		bClip=true;
		Pt2.X=limits.X;
	}

	// Test Max Y
	if(Pt2.Y>limits.Y)
	{
		bClip=true;
		Pt2.Y=limits.Y;
	}

	// Return value
	return(bClip);
}


//-----------------------------------------------------------------------------
void RGeometry2D::RRect::Translation(const RCoord x,const RCoord y)
{
	Pt1.X+=x;
	Pt1.Y+=y;
	Pt2.X+=x;
	Pt2.Y+=y;
}


//-----------------------------------------------------------------------------
bool RGeometry2D::RRect::Overlap(const RRect *rect) const
{
	RReturnValIfFail(rect,false);
	
	// Is up or bottom of rect
	if((Pt1.Y>rect->Pt2.Y)||(Pt2.Y<rect->Pt1.Y)) return(false);

	// Is left or right of rect
	if((Pt1.X>rect->Pt2.X)||(Pt2.X<rect->Pt1.X)) return(false);

	return(true);
}


//-----------------------------------------------------------------------------
bool RGeometry2D::RRect::IsIn(const RCoord X,const RCoord Y) const
{
	// Is up or bottom of rect
	if((Y>Pt2.Y)||(Y<Pt1.Y)) return(false);

	// Is left or right of rect
	if((X>Pt2.X)||(X<Pt1.X)) return(false);

	return(true);
}


//-----------------------------------------------------------------------------
bool RGeometry2D::RRect::IsIn(const RPolygon* poly) const
{
	RPoint **tab;
	unsigned int i;

	for(i=poly->NbPtr+1,tab=poly->Tab;--i;tab++)
	{
		if(!IsIn(*tab))
			return(false);
	}
	return(true);
}


//-----------------------------------------------------------------------------
RRect* RGeometry2D::RRect::GetRect(void)
{
	return(GetTemporaryObject<RRect,30>());
}
