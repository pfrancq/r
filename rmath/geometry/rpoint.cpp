/*

	R Project Library

	RPoint.cpp

	Point - Implementation.

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
#include <rstd/rstd.h>
using namespace RStd;
#include <rgeometry/rpoint.h>
#include <rgeometry/rpoints.h>
#include <rgeometry/rpolygon.h>
#include <rgeometry/rpolygons.h>
using namespace RGeometry2D;



//-----------------------------------------------------------------------------
//
// Class "RPoint"
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGeometry2D::RPoint::RPoint(void)
{
	X=Y=0;
}


//-----------------------------------------------------------------------------
RGeometry2D::RPoint::RPoint(const RCoord x,const RCoord y)
{
	X=x;
	Y=y;
}


//-----------------------------------------------------------------------------
RGeometry2D::RPoint::RPoint(const RPoint& pt)
{
	X=pt.X;
	Y=pt.Y;
}


//-----------------------------------------------------------------------------
RGeometry2D::RPoint::RPoint(const RPoint *pt)
{
	RReturnIfFail(pt);
	if(!pt) return;
	X=pt->X;
	Y=pt->Y;
}


//-----------------------------------------------------------------------------
bool RGeometry2D::RPoint::Near(const RPoint *pt) const
{
	return((labs(X-pt->X)<=1)&&(labs(Y-pt->Y)<=1));
}


//-----------------------------------------------------------------------------
RCoord RGeometry2D::RPoint::ManhattanDist(const RPoint &pt) const
{
	return(labs(X-pt.X)+labs(Y-pt.Y));
}


//-----------------------------------------------------------------------------
double RGeometry2D::RPoint::EuclideanDist(const RPoint &pt) const
{
	double dx,dy;
	
	dx=X-pt.X;
	dy=Y-pt.Y;	
	return(sqrt(dx*dx+dy*dy));
}


//-----------------------------------------------------------------------------
RPoint* RGeometry2D::RPoint::GetPoint(void)
{
	return(GetTemporaryObject<RPoint,30>());
}


//-----------------------------------------------------------------------------
RDirection RGeometry2D::RPoint::Classify(const RPoint* p0,const RPoint* p1) const
{
	RPoint a=(*p1)-(*p0);
	RPoint b=(*this)-(*p0);
	double sa=a.X*b.Y-b.X*a.Y;

	if(sa>Epsi) return(Left);
	if(sa<-Epsi) return(Right);
	if(((a.X*b.X)<Epsi)||((a.Y*b.Y)<Epsi)) return(Behind);
	if(a.Length()<b.Length()) return(Beyond);
	if((*this)==(*p0)) return(Origin);
	if((*this)==(*p1)) return(Destination);
	return(Between);
}


//-----------------------------------------------------------------------------
void RGeometry2D::RPoint::ChangeOrientation(const ROrientation o)
{
	RCoord factx=1,facty=1,oldx,oldy;
	double co=1,si=0;

	// Determine scale and rotation
	if((o==NormalX)||(o==NormalYX)||(o==Rota90X)||(o==Rota90YX))
		facty=-1;
	if((o==NormalY)||(o==Normal)||(o==Rota90Y)||(o==Rota90YX))
		factx=-1;
	if((o==Rota90)||(o==Rota90X)||(o==Rota90Y)||(o==Rota90YX))
	{
		co=0;
		si=1;
	}

	// Make the transformation for the point
	oldx = factx*X;
	oldy = facty*Y;
	X = RCoord(co*oldx - si*oldy);
	Y = RCoord(si*oldx + co*oldy);
}


//-----------------------------------------------------------------------------
void RGeometry2D::RPoint::Save(RTextFile& f) const
{
	f<<X<<Y<<endl;
}



//-----------------------------------------------------------------------------
//
// Global Functions and Operators
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPoint& RGeometry2D::operator+(const RPoint &arg1,const RPoint &arg2)
{
	RPoint *res=RPoint::GetPoint();
	(*res)=arg1;
	return((*res)+=arg2);
}


//-----------------------------------------------------------------------------
RPoint& RGeometry2D::operator-(const RPoint &arg1,const RPoint &arg2)
{
	RPoint *res=RPoint::GetPoint();
	(*res)=arg1;
	return((*res)-=arg2);
}
