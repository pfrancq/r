/*

	R Project Library

	RPoint.cpp

	Point - Implementation.

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
#include <rstd.h>
#include <rpoint.h>
#include <rtextfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class "RPoint"
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPoint RPoint::Null(cNoCoord,cNoCoord);


//------------------------------------------------------------------------------
RPoint::RPoint(void)
{
	X=Y=0;
}


//------------------------------------------------------------------------------
RPoint::RPoint(tCoord x,tCoord y)
{
	X=x;
	Y=y;
}


//------------------------------------------------------------------------------
RPoint::RPoint(const RPoint& pt)
{
	X=pt.X;
	Y=pt.Y;
}


//------------------------------------------------------------------------------
int RPoint::Compare(const RPoint& pt) const
{
	// Compare first the Y coordinates
	int comp(CompareDoubles(Y,pt.Y));
	if(comp)
		return(comp);
	return(CompareDoubles(X,pt.X));
}


//------------------------------------------------------------------------------
bool RPoint::Near(const RPoint& pt) const
{
	return((Abs(X-pt.X)<=1)&&(Abs(Y-pt.Y)<=1));
}


//------------------------------------------------------------------------------
tCoord RPoint::ManhattanDist(const RPoint& pt) const
{
	return(Abs(X-pt.X)+Abs(Y-pt.Y));
}


//------------------------------------------------------------------------------
double RPoint::EuclideanDist(const RPoint& pt) const
{
	double dx,dy;

	dx=X-pt.X;
	dy=Y-pt.Y;
	return(sqrt(dx*dx+dy*dy));
}


//------------------------------------------------------------------------------
tDirection RPoint::Classify(const RPoint& pt) const
{
	if(X>pt.X)
	{
		// The point pt is on the left
		if(Y>pt.Y)
			return(dLeftDown);
		else if(Y<pt.Y)
			return(dLeftUp);
		else
			return(dLeft);
	}
	else if(X<pt.X)
	{
		// The point pt is on the right
		if(Y>pt.Y)
			return(dRightDown);
		else if(Y<pt.Y)
			return(dRightUp);
		else
			return(dRight);
	}
	else
	{
		// Points are on the vertical line
		if(Y>pt.Y)
			return(dDown);
		else if(Y<pt.Y)
			return(dUp);
		else
			return(dNoDirection);
	}
}


//------------------------------------------------------------------------------
tDirection RPoint::Classify(const RPoint& pt0,const RPoint& pt1) const
{
	RPoint a=pt1-pt0;
	RPoint b=(*this)-pt0;
	double sa=a.X*b.Y-b.X*a.Y;

	if(sa>cEpsi) return(dLeft);
	if(sa<-cEpsi) return(dRight);
	if(((a.X*b.X)<cEpsi)||((a.Y*b.Y)<cEpsi)) return(dBehind);
	if(a.Length()<b.Length()) return(dBeyond);
	if((*this)==pt0) return(dOrigin);
	if((*this)==pt1) return(dDestination);
	return(dBetween);
}


//------------------------------------------------------------------------------
void RPoint::ChangeOrientation(const tOrientation o)
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

	// Make the transformation for the point
	oldx = factx*X;
	oldy = facty*Y;
	X = tCoord(co*oldx - si*oldy);
	Y = tCoord(si*oldx + co*oldy);
}


//------------------------------------------------------------------------------
void RPoint::Save(RTextFile& f) const
{
	f<<X<<Y<<endl;
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ostream& std::operator<<(ostream& os, const RPoint& pt)
{
	os<<"("<<pt.X<<","<<pt.Y<<")";
   return(os);
}
