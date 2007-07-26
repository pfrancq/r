/*

	R Project Library

	RRect.cpp

	Rectangle - Implemtation.

	Copyright 1999-2007 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rrect.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RRect
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RRect::RRect(void)
	: X1(0),Y1(0),X2(0),Y2(0)
{
}


//------------------------------------------------------------------------------
RRect::RRect(const RRect& rect)
	: X1(rect.X1),Y1(rect.Y1),X2(rect.X2),Y2(rect.Y2)
{
}


//------------------------------------------------------------------------------
RRect::RRect(const RPoint& pt1,const RPoint& pt2)
	: X1(pt1.X),Y1(pt1.Y),X2(pt2.X),Y2(pt2.Y)
{
}


//------------------------------------------------------------------------------
RRect::RRect(tCoord x1,tCoord y1,tCoord x2,tCoord y2)
	: X1(x1),Y1(y1),X2(x2),Y2(y2)
{
}


//------------------------------------------------------------------------------
void RRect::Translation(const tCoord x,const tCoord y)
{
	X1+=x;
	X2+=x;
	Y1+=y;
	Y2+=y;
}


//------------------------------------------------------------------------------
void RRect::SetPos(tCoord x,tCoord y)
{
	X2=x+GetWidth();
	Y2=y+GetHeight();
	X1=x;
	Y1=y;
}



//------------------------------------------------------------------------------
int RRect::Compare(const RRect&) const
{
	return(-1);
}


//------------------------------------------------------------------------------
bool RRect::Clip(const RRect& clip)
{
	bool bClip=false;

	// Test Min X
	if(X1<clip.X1)
	{
		bClip=true;
		X1=clip.X1;
	}
	if(X1>clip.X2)
	{
		bClip=true;
		X1=clip.X2;
	}

	// Test Min Y
	if(Y1<clip.Y1)
	{
		bClip=true;
		Y1=clip.Y1;
	}
	if(Y1>clip.Y2)
	{
		bClip=true;
		Y1=clip.Y2;
	}

	// Test Max X
	if(X2<clip.X1)
	{
		bClip=true;
		X2=clip.X1;
	}
	if(X2>clip.X2)
	{
		bClip=true;
		X2=clip.X2;
	}

	// Test Max Y
	if(Y2<clip.Y1)
	{
		bClip=true;
		Y2=clip.Y1;
	}
	if(Y2>clip.Y2)
	{
		bClip=true;
		Y2=clip.Y2;
	}

	// Return value
	return(bClip);
}


//------------------------------------------------------------------------------
bool RRect::Clip(const RPoint& limits)
{
	bool bClip=false;

	// Test Min X
	if(X1>limits.X)
	{
		bClip=true;
		X1=limits.X;
	}

	// Test Min Y
	if(Y1>limits.Y)
	{
		bClip=true;
		Y1=limits.Y;
	}

	// Test Max X
	if(X2>limits.X)
	{
		bClip=true;
		X2=limits.X;
	}

	// Test Max Y
	if(Y2>limits.Y)
	{
		bClip=true;
		Y2=limits.Y;
	}

	// Return value
	return(bClip);
}


//------------------------------------------------------------------------------
bool RRect::Overlap(const RRect& rect) const
{
	// Is up or bottom of rect
	if((Y1>rect.Y2)||(Y2<rect.Y1)) return(false);

	// Is left or right of rect
	if((X1>rect.X2)||(X2<rect.X1)) return(false);

	return(true);
}


//------------------------------------------------------------------------------
bool RRect::IsIn(const tCoord X,const tCoord Y) const
{
	// Is up or bottom of rect
	if((Y>Y2)||(Y<Y1)) return(false);

	// Is left or right of rect
	if((X>X2)||(X<X1)) return(false);

	return(true);
}
