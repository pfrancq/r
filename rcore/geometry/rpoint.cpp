/*

	R Project Library

	RPoint.cpp

	Point - Implementation.

	Copyright 1999-2006 by the Université Libre de Bruxelles.

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
#include <rstd.h>
#include <rpoint.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class "RPoint"
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPoint::RPoint(void)
{
	X=Y=0;
}


//------------------------------------------------------------------------------
RPoint::RPoint(const tCoord x,const tCoord y)
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
bool RPoint::Near(const RPoint* pt) const
{
	return((labs(X-pt->X)<=1)&&(labs(Y-pt->Y)<=1));
}


//------------------------------------------------------------------------------
tCoord RPoint::ManhattanDist(const RPoint& pt) const
{
	return(labs(X-pt.X)+labs(Y-pt.Y));
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
RDirection RPoint::Classify(const RPoint* p0,const RPoint* p1) const
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


//------------------------------------------------------------------------------
void RPoint::ChangeOrientation(const ROrientation o)
{
	tCoord factx=1,facty=1,oldx,oldy;
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
	X = tCoord(co*oldx - si*oldy);
	Y = tCoord(si*oldx + co*oldy);
}


//------------------------------------------------------------------------------
void RPoint::Save(RTextFile& f) const
{
	f<<X<<Y<<endl;
}
