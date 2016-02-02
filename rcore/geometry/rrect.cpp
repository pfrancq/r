/*

	R Project Library

	RRect.cpp

	Rectangle - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
	: Pt1(0,0), Pt2(0,0)
{
}


//------------------------------------------------------------------------------
RRect::RRect(const RRect& rect)
	: Pt1(rect.Pt1), Pt2(rect.Pt2)
{
}


//------------------------------------------------------------------------------
RRect::RRect(const RPoint& pt1,const RPoint& pt2)
	: Pt1(pt1), Pt2(pt2)
{
	ReOrder();
}


//------------------------------------------------------------------------------
RRect::RRect(tCoord x1,tCoord y1,tCoord x2,tCoord y2)
	: Pt1(x1,y1), Pt2(x2,y2)
{
	ReOrder();
}


//------------------------------------------------------------------------------
void RRect::ReOrder(void)
{
	// Make the bottom-left point be X1,Y1
	if(Pt1.X>Pt2.X)
	{
		tCoord tmp(Pt1.X);
		Pt1.X=Pt2.X;
		Pt2.X=tmp;
	}
	if(Pt1.Y>Pt2.Y)
	{
		tCoord tmp(Pt1.Y);
		Pt1.Y=Pt2.Y;
		Pt2.Y=tmp;
	}
}


//------------------------------------------------------------------------------
int RRect::Compare(const RRect& rect) const
{
	int comp(Pt1.Compare(rect.Pt1));
	if(comp)
		return(comp);
	return(Pt2.Compare(rect.Pt2));

}


//------------------------------------------------------------------------------
void RRect::Set(const RPoint& pt1,const RPoint& pt2)
{
	Pt1=pt1;
	Pt2=pt2;
	ReOrder();
}


//------------------------------------------------------------------------------
void RRect::Set(tCoord x1,tCoord y1,tCoord x2,tCoord y2)
{
	Pt1.Set(x1,y1);
	Pt2.Set(x2,y2);
	ReOrder();
}


//------------------------------------------------------------------------------
void RRect::Translation(const RPoint& pt)
{
	Pt1+=pt;
	Pt2+=pt;
}


//------------------------------------------------------------------------------
void RRect::SetPos(const RPoint& pt)
{
	RPoint s(GetWidth(),GetHeight());
	Pt1=pt;
	Pt2=pt+s;
}


//------------------------------------------------------------------------------
bool RRect::Clip(const RRect& clip)
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


//------------------------------------------------------------------------------
bool RRect::Clip(const RPoint& limits)
{
	RRect rect(0,0,limits.X,limits.Y);
	return(Clip(rect));
}


//------------------------------------------------------------------------------
bool RRect::Overlap(const RRect& rect) const
{
	// Is up or bottom of the rectangle
	if((Pt1.Y>rect.Pt2.Y)||(Pt2.Y<rect.Pt1.Y)) return(false);

	// Is left or right of the rectangle
	if((Pt1.X>rect.Pt2.X)||(Pt2.X<rect.Pt1.X)) return(false);

	return(true);
}


//------------------------------------------------------------------------------
bool RRect::IsIn(const RPoint& pt,bool overlap) const
{
	// Verify if the point is on a edge
	if(overlap)
	{
		// Left edge
		if((pt.X==Pt1.X)&&(pt.Y>=Pt1.Y)&&(pt.Y<=Pt2.Y))
			return(true);

		// Right edge
		if((pt.X==Pt2.X)&&(pt.Y>=Pt1.Y)&&(pt.Y<=Pt2.Y))
			return(true);

		// Bottom edge
		if((pt.Y==Pt1.Y)&&(pt.X>=Pt1.X)&&(pt.X<=Pt2.X))
			return(true);

		// Upper edge
		if((pt.Y==Pt2.Y)&&(pt.X>=Pt1.X)&&(pt.X<=Pt2.X))
			return(true);
	}

	// Is up or bottom of the rectangle
	if((pt.Y>Pt2.Y)||(pt.Y<Pt1.Y)) return(false);

	// Is left or right of rectangle
	if((pt.X>Pt2.X)||(pt.X<Pt1.X)) return(false);

	return(true);
}


//------------------------------------------------------------------------------
void RRect::ChangeOrientation(const tOrientation o)
{
	tCoord factx=1,facty=1,oldx,oldy;
	double co=1,si=0;

	// Determine scale and rotation
	if((o==oNormalX)||(o==oNormalYX)||(o==oRota90X)||(o==oRota90YX))
		facty=-1;
	if((o==oNormalY)||(o==oNormal)||(o==oRota90Y)||(o==oRota90YX))
		factx=-1;
	if((o==oRota90)||(o==oRota90X)||(o==oRota90Y)||(o==oRota90YX))
	{
		co=0;
		si=1;
	}
	RPoint min(cMaxCoord,cMaxCoord);
	RPoint Left(Pt1);
//	Pt1-=Left;
//	Pt2-=Left;

	// Make the transformation for Pt1
	oldx = factx*Pt1.X;
	oldy = facty*Pt1.Y;
	Pt1.X = tCoord(co*oldx - si*oldy);
	Pt1.Y = tCoord(si*oldx + co*oldy);
	if(Pt1.X<min.X) min.X=Pt1.X;
	if(Pt1.Y<min.Y) min.Y=Pt1.Y;

	// Make the transformation for Pt2
	oldx = factx*Pt2.X;
	oldy = facty*Pt2.Y;
	Pt2.X = tCoord(co*oldx - si*oldy);
	Pt2.Y = tCoord(si*oldx + co*oldy);
	if(Pt2.X<min.X) min.X=Pt2.X;
	if(Pt2.Y<min.Y) min.Y=Pt2.Y;

	// Replace (0,0) as the left-top point of the embedded rectangle
	min-=Left;
	Pt1.X-=min.X;
	Pt1.Y-=min.Y;
	Pt2.X-=min.X;
	Pt2.Y-=min.Y;

//*	Pt1+=Left;
//	Pt2+=Left;


	ReOrder();
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::ostream& std::operator<<(std::ostream& os, const RRect& rect)
{
	os<<"("<<rect.GetX1()<<","<<rect.GetY1()<<")"<<";("<<rect.GetX2()<<","<<rect.GetY2()<<")";
   return(os);
}
