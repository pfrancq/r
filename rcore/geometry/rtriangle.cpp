/*

	R Project Library

	RTrianble.cpp

	Triangle - Implementation.

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
#include <rtriangle.h>
#include <rline.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RTriangle
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RTriangle::RTriangle(void)
	: Pt1(0.,0.),Pt2(0.,0.),Pt3(0.,0.)
{
}


//------------------------------------------------------------------------------
RTriangle::RTriangle(const RTriangle& triangle)
	: Pt1(triangle.Pt1), Pt2(triangle.Pt2), Pt3(triangle.Pt3)
{
}


//------------------------------------------------------ -----------------------
RTriangle::RTriangle(tCoord x1,tCoord y1,tCoord x2,tCoord y2,tCoord x3,tCoord y3,bool reorder)
	: Pt1(x1,y1), Pt2(x2,y2), Pt3(x3,y3)
{
	if(reorder)
		ReOrder();
}


//------------------------------------------------------ -----------------------
RTriangle::RTriangle(const RPoint& pt1,const RPoint& pt2,const RPoint& pt3,bool reorder)
	: Pt1(pt1), Pt2(pt2), Pt3(pt3)
{
	if(reorder)
		ReOrder();
}


//------------------------------------------------------------------------------
void RTriangle::ReOrder(void)
{
	if(Pt1.Compare(Pt2)<0.0)
	{
		// Pt1 is more bottom/left than Pt2
		if(Pt3.Compare(Pt1)<0.0)
		{
			// Pt3 is more bottom/left than Pt1 -> Pt3,Pt1 and Pt2
			RPoint Tmp(Pt1);
			Pt1=Pt3;
			Pt3=Pt2;
			Pt2=Tmp;
		}
		else
		{
			if(Pt3.Compare(Pt2)<0.0)
			{
				// Pt3 is more bottom/left than Pt2 -> Pt1,Pt3 and Pt2
				RPoint Tmp(Pt2);
				Pt2=Pt3;
				Pt3=Tmp;
			}
		}
	}
	else
	{
		// Pt2 is more bottom/left than Pt1
		if(Pt3.Compare(Pt2)<0.0)
		{
			// Pt3 is more bottom/left than Pt2 -> Pt3,Pt2 and Pt1
			RPoint Tmp(Pt1);
			Pt1=Pt3;
			Pt3=Tmp;
		}
		else
		{
			if(Pt3.Compare(Pt1)<0.0)
			{
				// Pt3 is more bottom/left than Pt1 -> Pt2,Pt3 and Pt1
				RPoint Tmp(Pt2);
				Pt2=Pt3;
				Pt3=Pt1;
				Pt1=Tmp;
			}
		}
	}
}


//------------------------------------------------------------------------------
int RTriangle::Compare(const RTriangle& triangle) const
{
	int comp(Pt1.Compare(triangle.Pt1));
	if(comp)
		return(comp);
	comp=Pt2.Compare(triangle.Pt2);
	if(comp)
		return(comp);
	return(Pt3.Compare(triangle.Pt3));
}


//------------------------------------------------------------------------------
tCoord  RTriangle::GetArea(void) const
{
	RLine a(Pt1,Pt2);
	RLine b(Pt2,Pt3);
	RLine c(Pt3,Pt1);
   double s((a.GetLength()+b.GetLength()+c.GetLength())/2.0);
   return(sqrt(s*(s-a.GetLength())*(s-b.GetLength())*(s-c.GetLength())));
}


//------------------------------------------------------ -----------------------
void RTriangle::SetPoints(tCoord x1,tCoord y1,tCoord x2,tCoord y2,tCoord x3,tCoord y3,bool reorder)
{
	Pt1.Set(x1,y1);
   Pt2.Set(x2,y2);
	Pt3.Set(x3,y3);
	if(reorder)
		ReOrder();
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::ostream& std::operator<<(std::ostream& os, const RTriangle& triangle)
{
	os<<"("<<triangle.GetX1()<<","<<triangle.GetY1()<<")"<<";("<<triangle.GetX2()<<","<<triangle.GetY2()<<");("<<triangle.GetX3()<<","<<triangle.GetY3()<<")";
   return(os);
}
