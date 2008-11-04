/*

	R Project Library

	RLine.cpp

	Line - Implementation.

	Copyright 1999-2008 by the Université Libre de Bruxelles.

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
#include <rline.h>
using namespace R;



//------------------------------------------------------------------------------
//
// Class RLine
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLine::RLine(void)
	: Pt1(),Pt2(),Segment(false)
{
}


//------------------------------------------------------ -----------------------
RLine::RLine(const RPoint* pt1,const RPoint* pt2,const bool seg)
	: Pt1(*pt1),Pt2(*pt2)
{
	RReturnIfFail(pt1&&pt2);
	Segment=seg;
}


//------------------------------------------------------------------------------
double RLine::Length(void) const
{
	tCoord X=Abs(Pt2.X-Pt1.X),Y=Abs(Pt2.Y-Pt1.Y);
	return(sqrt(static_cast<double>(X*X+Y*Y)));
}


//------------------------------------------------------------------------------
int RLine::CCW(const RPoint& pt) const
{
	tCoord dx1,dx2,dy1,dy2;

	dx1=Pt2.X-Pt1.X;
	dx2=pt.X-Pt2.X;
	dy1=Pt2.Y-Pt1.Y;
	dy2=pt.Y-Pt2.Y;
	if(dx1*dy2>dy1*dx2) return(1);
	if(dx1*dy2<dy1*dx2) return(-1);
	if((dx1*dx2<0)||(dy1*dy2<0)) return(-1);
	if(dx1*dx1+dy1*dy1>=dx2*dx2+dy2*dy2) return(0);
	return(1);
}


//------------------------------------------------------------------------------
bool RLine::Inter(const RLine* line) const
{
	RReturnValIfFail(line,false);
	return((CCW(line->Pt1)*CCW(line->Pt2)<=0)&&(line->CCW(Pt1)*line->CCW(Pt2)<=0));
}


//------------------------------------------------------------------------------
bool RLine::Inter(const RLine& line) const
{
	return((CCW(line.Pt1)*CCW(line.Pt2)<=0)&&(line.CCW(Pt1)*line.CCW(Pt2)<=0));
}


//------------------------------------------------------------------------------
bool RLine::IsIn(const RPoint& pt) const
{
	double a,b;    // y=ax+b
	double eq;     // y(pt);

	a=static_cast<double>(Pt2.Y-Pt1.Y)/static_cast<double>(Pt2.X-Pt1.X);
	b=static_cast<double>(Pt1.Y+Pt2.Y-a*(Pt1.X+Pt2.X))/2.0;
	eq=fabs(a*pt.X+b-pt.Y);
	return(eq<=Epsi);
}
