/*

	R Project Library

	RLine.cpp

	Line - Implementation.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <rexception.h>
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


//------------------------------------------------------------------------------
RLine::RLine(const RLine& line)
	: Pt1(line.Pt1), Pt2(line.Pt2), Segment(line.Segment),
	  OrientedTop(line.OrientedTop), OrientedRight(line.OrientedRight)
{
}


//------------------------------------------------------ -----------------------
RLine::RLine(tCoord x1,tCoord y1,tCoord x2,tCoord y2,bool seg,bool reorder)
	: Pt1(x1,y1), Pt2(x2,y2), Segment(seg), OrientedTop(y1<y2), OrientedRight(x1<x2)
{
	if(reorder)
		ReOrder();
}


//------------------------------------------------------ -----------------------
RLine::RLine(const RPoint& pt1,const RPoint& pt2,bool seg,bool reorder)
	: Pt1(pt1.X,pt1.Y), Pt2(pt2.X,pt2.Y), Segment(seg), OrientedTop(pt1.Y<pt2.Y), OrientedRight(pt1.X<pt2.X)
{
	if(reorder)
		ReOrder();
}


//------------------------------------------------------------------------------
void RLine::ReOrder(void)
{
	// Verify if Pt2 is the first point
	if(((Abs(Pt1.Y-Pt2.Y)<=cEpsi)&&(Pt2.X<Pt1.X))||(Pt1.Y>Pt2.Y))
	{
		RPoint Tmp(Pt1);
		Pt1=Pt2;
		Pt2=Tmp;
	}
}


//------------------------------------------------------------------------------
int RLine::Compare(const RLine& line) const
{
	int comp(Pt1.Compare(line.Pt1));
	if(comp)
		return(comp);
	return(Pt2.Compare(line.Pt2));
}


//------------------------------------------------------------------------------
tCoord  RLine::GetLength(void) const
{
	tCoord X=Abs(Pt2.X-Pt1.X),Y=Abs(Pt2.Y-Pt1.Y);
	return(sqrt(static_cast<double>(X*X+Y*Y)));
}


//------------------------------------------------------------------------------
tCoord  RLine::GetAngle(bool ccw) const
{
	tCoord X=(Pt2.X-Pt1.X);
	tCoord Y;
	if(ccw)
		Y=-(Pt2.Y-Pt1.Y);
	else
		Y = (Pt2.Y-Pt1.Y);
	if(X)
	{
		if (Pt2.X > Pt1.X)
		{
			double angle(atan(static_cast<double>(Y/X))*(180)/3.14159265);
			if(angle<0)
				angle+=360;
			return(angle);
		}
		else
		{
			//X<0
			double angle(atan(static_cast<double>(Y/X))*(180)/3.14159265);
			//if(Y > 0)
			//    angle = 180-angle;
			//else
			angle=180+angle;
			return(angle);
		}
	}
	else
   {
		if(Y>0)
			return(90);
		else
			return(270);
   }
}


//------------------------------------------------------ -----------------------
void RLine::SetPoints(tCoord x1,tCoord y1,tCoord x2,tCoord y2,bool seg,bool reorder)
{
	Pt1.Set(x1,y1);
   Pt2.Set(x2,y2);
	Segment=seg;
	OrientedTop=y1<y2;
	OrientedRight=x1<x2;
	if(reorder)
		ReOrder();
}


//------------------------------------------------------------------------------
bool RLine::IsOrientedTop(void) const
{
	if(!Segment)
		mThrowRException("The line is not a segment");
	return(OrientedTop);
}


//------------------------------------------------------------------------------
bool RLine::IsOrientedRight(void) const
{
	if(!Segment)
		mThrowRException("The line is not a segment");
	return(OrientedRight);
}


//------------------------------------------------------------------------------
tCoord  RLine::GetAngle(const RLine& line,bool ccw) const
{
    double angle(GetAngle(ccw)-line.GetAngle(ccw));
	if(angle>0)
		return(angle);
	return(angle+360);
}


//------------------------------------------------------------------------------
int RLine::CounterClockwise(const RPoint& pt) const
{
	tCoord dx1(Pt2.X-Pt1.X);   // őĒX between two extremities
	tCoord dx2(pt.X-Pt1.X);    // őĒX between the point and the first extremity.
	tCoord dy1(Pt2.Y-Pt1.Y);   // őĒY between two extremities
	tCoord dy2(pt.Y-Pt1.Y);    // őĒY between the point and the first extremity.

	// Compare the angle of the segment and the one formed by the point and the first extremity
	tCoord tmp1(dx1*dy2); // Temporary values since dx1/dy1?dx2/dy2=>dx1*dy2?dx2*dy1;
	tCoord tmp2(dx2*dy1);
	if(tmp1>tmp2) return(1);  // Must go anticlockwise to go from Pt1 to Pt2 by passing by pt
	if(tmp1<tmp2) return(-1); // Must go clockwise to go go from Pt1 to Pt2 by passing by pt

	// The three points are collinear
	if((dx1*dx2<0)||(dy1*dy2<0))
		return(-1);      // pt is between Pt1 and Pt2
	if((dx1*dx1+dy1*dy1)<(dx2*dx2+dy2*dy2))
		return(1);       // pt is not between Pt1 and Pt2
	return(0);
}


//------------------------------------------------------------------------------
bool RLine::Inter(const RLine& line,RPoint& pt) const
{
	pt.Set(cMaxCoord,cMaxCoord);

	// Look if the two lines have common extremities. Test the four pairs.
	if(Pt1==line.Pt1)
	{
		// Different line => unique intersection
		if(!((IsIn(line.Pt2)||(line.IsIn(Pt2)))))
			 pt.Set(Pt1.X,Pt1.Y);
		return(true);
	}
	if(Pt2==line.Pt2)
	{
		// Different line => unique intersection
		if(!((IsIn(line.Pt1)||(line.IsIn(Pt1)))))
			 pt.Set(Pt2.X,Pt2.Y);
		return(true);
	}
	if(Pt1==line.Pt2)
	{
		// Different line => unique intersection
		if(!((IsIn(line.Pt1)||(line.IsIn(Pt2)))))
			 pt.Set(Pt1.X,Pt1.Y);
		return(true);
	}
	if(Pt2==line.Pt1)
	{
		// Different line => unique intersection
		if(!((IsIn(line.Pt2)||(line.IsIn(Pt1)))))
			 pt.Set(Pt2.X,Pt2.Y);
		return(true);
	}

	// If both segments are on the same line -> verify if they overlap
	if((Abs(GetAngle()-line.GetAngle())<=cEpsi)||(Abs(GetAngle()-line.GetAngle()-180)<=cEpsi))
	{
		if(IsIn(line.Pt1) || IsIn(line. Pt2) || line.IsIn(Pt1) || line.IsIn(Pt2))
			return(true);  // Joint lines
		else
			return(false);  // Parallel distinct lines
	}

	// Intersecting lines
	double a1,b1;         // y1=a1x+b1
	double a2,b2;         // y2=a2x+b2
	double x,y,x1,x2;     // y1 = y2;

	if(Abs(Pt2.X-Pt1.X)>=cEpsi)
	{
		a1=static_cast<double>(Pt2.Y-Pt1.Y)/static_cast<double>(Pt2.X-Pt1.X);
		b1=static_cast<double>(Pt1.Y-a1*(Pt1.X));
	}
	else
	{
		// Vertical line
		a1 = cMaxCoord;
		x1 = Pt1.X;
	}
	if(Abs(line.Pt2.X-line.Pt1.X)>cEpsi)
	{
		// Non-vertical line
		a2=static_cast<double>(line.Pt2.Y-line.Pt1.Y)/static_cast<double>(line.Pt2.X-line.Pt1.X);
		b2=static_cast<double>(line.Pt1.Y-a2*(line.Pt1.X));
	}
	else
	{
		// Vertical line
		a2 = cMaxCoord;
		x2 = line.Pt1.X;
	}

	if(a1 == cMaxCoord)
	{
		x = x1;
		y = a2 * x + b2;
	}
	else
	{
		if (a2 == cMaxCoord)
		{
			 x = x2;
			 y = a1 * x + b1;
		}
		else
		{
			 x = -(b2-b1)/(a2-a1);
			 y = a1 * x + b1;
		}
	}

	if ((!IsSegment() || (Segment && IsIn(RPoint(x,y))))
    && (!line.IsSegment() || (line.IsSegment() && line.IsIn(RPoint(x,y)))))
	{
		 pt.Set(x,y);
		 return(true);
	}
	else
		 return(false);
}


//------------------------------------------------------------------------------
bool RLine::Inter(const RLine& line) const
{
	return((CounterClockwise(line.Pt1)*CounterClockwise(line.Pt2)<=0)&&(line.CounterClockwise(Pt1)*line.CounterClockwise(Pt2)<=0));
}


//------------------------------------------------------------------------------
bool RLine::IsIn(const RPoint& pt) const
{
	if(Abs(Pt2.X-Pt1.X)<=cEpsi)
	{
		// The line is vertical -> Verify that the point is on the same vertical
      if(Abs(pt.X-Pt1.X)<=cEpsi)
		{
			if(Segment)
			{
				if((Abs(pt.Y-Pt1.Y)<=cEpsi)||(Abs(pt.Y-Pt2.Y)<=cEpsi))
					return(true);  // Points are very closed
				else
					return(((pt.Y-Pt1.Y)*(pt.Y-Pt2.Y))<=cEpsi); // Point cannot be lower or upper to both extremities
			}
			else
				return(true);  // Point is in the line
		}
		else
			 return(false);
	}
	else
	{
		double a(static_cast<double>(Pt2.Y-Pt1.Y)/static_cast<double>(Pt2.X-Pt1.X));
		double eq(fabs(a*(pt.X-Pt1.X)+Pt1.Y-pt.Y));
		if(eq<=cEpsi)
		{
			// The point is on the line -> Verify if it is in the segment
			if(Segment)
			{
				if((Abs(pt.X-Pt1.X)<=cEpsi)||(Abs(pt.X-Pt2.X)<=cEpsi))
					return(true);  // Points are very closed
				else
					return(((pt.X-Pt1.X)*(pt.X-Pt2.X))<=cEpsi); // Point cannot be left or right to both extremities
			}
			else
				return(true);
		}
	}
	return(false);
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
std::ostream& std::operator<<(std::ostream& os, const RLine& line)
{
	os<<"("<<line.GetX1()<<","<<line.GetY1()<<")"<<";("<<line.GetX2()<<","<<line.GetY2()<<")";
   return(os);
}
