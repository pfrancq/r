/*

	R Project Library

	RPoint.cpp

	Point - Implementation.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
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
tDirection RPoint::Classify(const RPoint& p0,const RPoint& p1) const
{
	RPoint a=p1-p0;
	RPoint b=(*this)-p0;
	double sa=a.X*b.Y-b.X*a.Y;

	if(sa>cEpsi) return(dLeft);
	if(sa<-cEpsi) return(dRight);
	if(((a.X*b.X)<cEpsi)||((a.Y*b.Y)<cEpsi)) return(dBehind);
	if(a.Length()<b.Length()) return(dBeyond);
	if((*this)==p0) return(dOrigin);
	if((*this)==p1) return(dDestination);
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
