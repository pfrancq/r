/*

	R Project Library

	Polygon.cpp

	Polygon - Implementation.

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
// include files for ANSI C/C++
#include <iostream>
#include <cstdarg>


//------------------------------------------------------------------------------
// include files for R Project
#include <rpolygon.h>
#include <rtextfile.h>
#include <rline.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RPolygon
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPolygon::RPolygon(void)
	: RContainer<RPoint,true,false>(20,10), Order(true), Rect(false)
{
}


//------------------------------------------------------------------------------
RPolygon::RPolygon(size_t nb,...)
	: RContainer<RPoint,true,false>(nb,10), Order(true), Rect(nb==4)
{
	va_list points;
   va_start(points,nb);
   for(size_t i = 0; i < nb; i++ )
	{
		double X(va_arg(points,double));
		double Y(va_arg(points,double));
		InsertPtr(new RPoint(X,Y));
   }
   va_end(points);
}


//------------------------------------------------------------------------------
RPolygon::RPolygon(const RPolygon& poly)
	: RContainer<RPoint,true,false>(poly), Order(poly.Order), Rect(poly.Rect)
{
}


//------------------------------------------------------------------------------
int RPolygon::Compare(const RPolygon& poly) const
{
	// Some easy comparisons
	if(!poly.GetNb())
	{
		if(GetNb())
			return(1);
		else
			return(0);
	}

	// ReOrder the two polygons
	const_cast<RPolygon*>(this)->ReOrder();
	const_cast<RPolygon&>(poly).ReOrder();

	// Verify if both polygon have the same vertices
	if(GetNb()==poly.GetNb())
	{
		bool Same(true);  // Let us suppose that all vertices are identical
		RCursor<RPoint> Cur(*this);
		RCursor<RPoint> Cur2(*this);
		for(Cur.Start(),Cur2.Start();(!Cur.End())&&Same;Cur.Next(),Cur2.Next())
			if((*Cur())!=(*Cur2()))
				Same=false;
		if(Same)
			return(true);
	}
	return((*this)[0]->Compare(*poly[0]));
}


//------------------------------------------------------------------------------
RCursor<RPoint> RPolygon::GetVertices(void) const
{
	return(RCursor<RPoint>(*this));
}


//-----------------------------------------------------------------------------
void RPolygon::GetEdges(R::RContainer<R::RLine,true,false>& edges) const
{
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t i=0;i<GetNb();Cur++,i++)
	{
		RPoint** Next;
		if(i==GetNb()-1)
			Next=RContainer<RPoint,true,false>::Tab;
		else
			Next=Cur+1;
		edges.InsertPtr(new RLine(**Cur,**Next));
	}
}


//------------------------------------------------------------------------------
void RPolygon::Clear(void)
{
	RContainer<RPoint,true,false>::Clear();
	Order=true;
	Rect=false;
}


//------------------------------------------------------------------------------
void RPolygon::InsertVertex(tCoord x,tCoord y)
{
	InsertPtr(new RPoint(x,y));
	Order=false;
}


//------------------------------------------------------------------------------
void RPolygon::InsertVertex(const RPoint& pt)
{
	InsertPtr(new RPoint(pt));
	Order=false;
}


//------------------------------------------------------------------------------
void RPolygon::InsertVertex(RPoint* pt)
{
	InsertPtr(pt);
	Order=false;
}


//------------------------------------------------------------------------------
void RPolygon::DeleteVertex(RPoint* pt)
{
	DeletePtr(*pt);
	Order=false;
}


//------------------------------------------------------------------------------
RPolygon& RPolygon::operator=(const RPolygon& poly)
{
	RContainer<RPoint,true,false>::operator=(poly);
	Order=poly.Order;
	Rect=poly.Rect;
	return(*this);
}


//------------------------------------------------------------------------------
bool RPolygon::operator==(const RPolygon& poly) const
{
	// An easy comparison
	if(GetNb()!=poly.GetNb()) return(false);

	// ReOrder the two polygons
	const_cast<RPolygon*>(this)->ReOrder();
	const_cast<RPolygon&>(poly).ReOrder();

	// Compare each vertex
	RCursor<RPoint> Cur(*this);
	RCursor<RPoint> Cur2(*this);
	for(Cur.Start(),Cur2.Start();!Cur.End();Cur.Next(),Cur2.Next())
		if((*Cur())!=(*Cur2()))
			return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool RPolygon::operator!=(const RPolygon& poly) const
{
	// An easy comparison
	if(GetNb()!=poly.GetNb()) return(true);

	// ReOrder the two polygons
	const_cast<RPolygon*>(this)->ReOrder();
	const_cast<RPolygon&>(poly).ReOrder();

	// Compare each vertex
	RCursor<RPoint> Cur(*this);
	RCursor<RPoint> Cur2(*this);
	for(Cur.Start(),Cur2.Start();!Cur.End();Cur.Next(),Cur2.Next())
		if((*Cur())!=(*Cur2()))
			return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPolygon& RPolygon::operator+=(const RPoint& pt)
{
	RCursor<RPoint> pts(*this);
	for(pts.Start();!pts.End();pts.Next())
		(*pts())+=pt;
	return(*this);
}


//------------------------------------------------------------------------------
RPolygon& RPolygon::operator-=(const RPoint& pt)
{
	RCursor<RPoint> pts(*this);
	for(pts.Start();!pts.End();pts.Next())
		(*pts())-=pt;
	return(*this);
}


//------------------------------------------------------------------------------
RPoint RPolygon::GetConX(const RPoint& pt) const
{
	const RPoint* next;
	size_t i;

	RCursor<RPoint> point(*this);
	point.Start();
	i=0;
	while((*point())!=pt)
	{
		point.Next();
		i++;
	}

	// Look previous vertex
	if(i)
		next=(*this)[i-1];
	else
		next=(*this)[GetNb()-1];
	if(next->Y==pt.Y)
		return(RPoint(*next));

	// Look next vertex
	if(i<GetNb()-1)
		next=(*this)[i+1];
	else
		next=(*this)[0];
	if(next->Y==pt.Y)
		return(RPoint(*next));

	// Problems when arriving here
	return(RPoint(cMaxCoord,cMaxCoord));
}


//------------------------------------------------------------------------------
RPoint RPolygon::GetConY(const RPoint& pt) const
{
	const RPoint* next;
	size_t i;

	RCursor<RPoint> point(*this);
	point.Start();
	i=0;
	while((*point())!=pt)
	{
		point.Next();
		i++;
	}

	// Look previous vertex
	if(i)
		next=(*this)[i-1];
	else
		next=(*this)[GetNb()-1];
	if(next->X==pt.X)
		return(RPoint(*next));

	// Look next vertex
	if(i<GetNb()-1)
		next=(*this)[i+1];
	else
		next=(*this)[0];
	if(next->X==pt.X)
		return(RPoint(*next));

	// Problems when arriving here
	return(RPoint(cMaxCoord,cMaxCoord));
}


//------------------------------------------------------------------------------
RPoint RPolygon::GetBottomLeft(void) const
{
	return(RPoint(*(*this)[GetBottomLeftIndex()]));
}


//------------------------------------------------------------------------------
size_t RPolygon::GetBottomLeftIndex(void) const
{
	RPoint* bl;
	size_t i,idx;
	tCoord X,Y;

	RCursor<RPoint> point(*this);
	point.Start();
	bl=point();
	idx=0;
	for(i=GetNb(),point.Next();--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if((Y<bl->Y)||((Y==bl->Y)&&(X<bl->X)))
		{
			bl=point();
			idx=point.GetPos();
		}
	}
	return(idx);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetBottomLeft(const tCoord minx,const tCoord miny,const tCoord maxx) const
{
	RPoint *bl;
	size_t i;
	tCoord X,Y;

	i=GetNb()+1;
	RCursor<RPoint> point(*this);
	point.Start();
	X=point()->X;
	Y=point()->Y;
	while((Y<miny)||((X<minx)&&(X>maxx)))
	{
		point.Next();
		i--;
		X=point()->X;
		Y=point()->Y;
	}
	bl=point();
	for(;--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if(((Y>=miny)&&(X>=minx)&&(X<=maxx))&&((Y<bl->Y)||((Y==bl->Y)&&(X<bl->X))))
			bl=point();
	}
	return(bl);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetLeftBottom(void) const
{
	RPoint *lb;
	size_t i;
	tCoord X,Y;

	RCursor<RPoint> point(*this);
	point.Start();
	lb=point();
	for(i=GetNb(),point.Next();--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if((X<lb->X)||((X==lb->X)&&(Y<lb->Y)))
			lb=point();
	}
	return(lb);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetLeftBottom(const tCoord minx,const tCoord miny,const tCoord maxy) const
{
	RPoint *lb;
	size_t i;
	tCoord X,Y;

	i=GetNb()+1;
	RCursor<RPoint> point(*this);
	point.Start();
	X=point()->X;
	Y=point()->Y;
	while((Y<miny)||(X<minx)||(Y>maxy))
	{
		point.Next();
		i--;
		X=point()->X;
		Y=point()->Y;
	}
	lb=point();
	for(;--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if(((Y>=miny)&&(Y<=maxy)&&(X>minx))&&((X<lb->X)||((X==lb->X)&&(Y<lb->Y))))
			lb=point();
	}
	return(lb);
}


//------------------------------------------------------------------------------
RPoint RPolygon::GetTopRight(void) const
{
	return(RPoint(*(*this)[GetTopRightIndex()]));
}


//------------------------------------------------------------------------------
size_t RPolygon::GetTopRightIndex(void) const
{
	RPoint* bl;
	size_t i,idx;
	tCoord X,Y;

	RCursor<RPoint> point(*this);
	point.Start();
	bl=point();
	idx=0;
	for(i=GetNb(),point.Next();--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if((Y>bl->Y)||((Y==bl->Y)&&(X>bl->X)))
		{
			bl=point();
			idx=point.GetPos();
		}
	}
	return(idx);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetRightTop(const tCoord minx,const tCoord miny,const tCoord maxx) const
{
	RPoint *bl;
	size_t i;
	tCoord X,Y;

	i=GetNb()+1;
	RCursor<RPoint> point(*this);
	point.Start();
	X=point()->X;
	Y=point()->Y;
	while((Y>miny)||((X>minx)&&(X<maxx)))
	{
		point.Next();
		i--;
		X=point()->X;
		Y=point()->Y;
	}
	bl=point();
	for(;--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if(((Y>=miny)&&(X>=minx)&&(X<=maxx))&&((Y>bl->Y)||((Y==bl->Y)&&(X>bl->X))))
			bl=point();
	}
	return(bl);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetRightTop(void) const
{
	RPoint *lb;
	size_t i;
	tCoord X,Y;

	RCursor<RPoint> point(*this);
	point.Start();
	lb=point();
	for(i=GetNb(),point.Next();--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if((X>lb->X)||((X==lb->X)&&(Y>lb->Y)))
			lb=point();
	}
	return(lb);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetTopRight(const tCoord minx,const tCoord miny,const tCoord maxy) const
{
	RPoint *lb;
	size_t i;
	tCoord X,Y;

	i=GetNb()+1;
	RCursor<RPoint> point(*this);
	point.Start();
	X=point()->X;
	Y=point()->Y;
	while((Y>miny)||(X>minx)||(Y<maxy))
	{
		point.Next();
		i--;
		X=point()->X;
		Y=point()->Y;
	}
	lb=point();
	for(;--i;point.Next())
	{
		X=point()->X;
		Y=point()->Y;
		if(((Y>=miny)&&(Y<=maxy)&&(X>minx))&&((X>lb->X)||((X==lb->X)&&(Y>lb->Y))))
			lb=point();
	}
	return(lb);
}

//------------------------------------------------------------------------------
bool RPolygon::IsVertex(const tCoord x,const tCoord y) const
{
	RPoint pt(x,y);
	RCursor<RPoint> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		if((*Cur())==pt) return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygon::IsOnEdge(const tCoord x,const tCoord y) const
{
	// Special cases
	if(!GetNb())
		return(false);
	RPoint pt(x,y);
	if(GetNb()==1)
		return(pt==(*((*this)[0])));

	// Look for each edge if the point is on it
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t i=0;i<GetNb();Cur++,i++)
	{
		// If the point is the current vertex -> It is on a edge
		if((**Cur)==pt)
			return(true);

		RPoint** Next;
		if(i==GetNb()-1)
			Next=RContainer<RPoint,true,false>::Tab;
		else
			Next=Cur+1;
		RLine Edge(**Cur,**Next);
		if(Edge.IsIn(pt))
			return(true);
	}

	return(false);
}


//------------------------------------------------------------------------------
bool RPolygon::IsOnEdge(const RPoint& pt1,const RPoint& pt2) const
{
	// Special cases
	if(!GetNb())
		return(false);
	if(GetNb()==1)
		return((pt1==(*((*this)[0])))&&(pt1==pt2));

	// Look for each edge if both points are on it
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t i=0;i<GetNb();Cur++,i++)
	{
		// If both points are the current vertex -> they are on the same edge
		if(((**Cur)==pt1)&&(pt1==pt2))
			return(true);

		RPoint** Next;
		if(i==GetNb()-1)
			Next=RContainer<RPoint,true,false>::Tab;
		else
			Next=Cur+1;
		RLine Edge(**Cur,**Next);
		if(Edge.IsIn(pt1)&&Edge.IsIn(pt2))
			return(true);
	}

	return(false);
}


//------------------------------------------------------------------------------
bool RPolygon::IsIn(const RPoint& pt,bool overlap) const
{
	// Special cases
	if(!GetNb())
		return(false);
	if(GetNb()==1)
		return(pt==(*((*this)[0])));

	// Count the intersection between each edge and the vertical line at x (cMaxCoord,y)
	RLine Ref(pt.X,pt.Y,pt.X,cMaxCoord);
	size_t count(0);
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t i=0;i<GetNb();Cur++,i++)
	{
		// If the point is the current vertex -> It is then also on two edges
		if((**Cur)==pt)
			return(overlap);  // Return overlap

		RPoint** Next;
		if(i==GetNb()-1)
			Next=RContainer<RPoint,true,false>::Tab;
		else
			Next=Cur+1;
		RLine Edge(**Cur,**Next);

		if(Edge.IsIn(pt))
			return(overlap); // If the point is on an edge -> return overlap
      else if(Edge.Inter(Ref))
			count++;
    }

	// if count%2==0 -> Not in
	return(count%2);
}


//------------------------------------------------------------------------------
bool RPolygon::IsIn(const RPolygon& poly,bool overlap) const
{
	// Special cases
	if(!GetNb())
		return(false);
	if(poly.GetNb()==1)
		return(IsIn(*poly[0]));

	// Go trough each vertex and edges of the polygon to test
	RPoint** Cur(poly.Tab);
	for(size_t i=0;i<poly.GetNb();Cur++,i++)
	{
		// The vertex must be in the polygon
		if(!IsIn(**Cur,overlap))  // Depending of overlap, the point may be or not on a edge
			return(false);

		RPoint** Next;
		if(i==poly.GetNb()-1)
			Next=poly.Tab;
		else
			Next=Cur+1;
		RLine Edge(**Cur,**Next);
		if(!IsIn(Edge))
			return(false);
	}

	// OK
	return(true);
}


//------------------------------------------------------------------------------
size_t RPolygon::GetNbInter(const RLine& line) const
{
	// Special cases
	if(!GetNb())
		return(0);

	size_t nb = 0;

	// We must compare Edge with the line
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t j=0;j<GetNb();Cur++,j++)
	{
		RPoint** Next;
		if(j==GetNb()-1)
				Next=Tab;//RContainer<RPoint,true,false>::Tab
		else
				Next=Cur+1;
		RLine Edge(**Cur,**Next);
		RPoint Inter;

		// If there is an intersection -> The polygon may not be contained
		if(Edge.Inter(line,Inter))
		{
			if(Inter.IsValid())
				 nb++;
			else
				 nb+=4;  // The line and the edge are combined
		}
	}

	// OK
	return(nb);
}


//------------------------------------------------------------------------------
bool RPolygon::IsIn(const RLine& line,bool overlap) const
{
	// Special cases
	if(!GetNb())
		return(false);

	if(!IsIn(line.GetPt1(),overlap))  // Depending of overlap, the point may be or not on a edge
		return(false);
	if(!IsIn(line.GetPt2(),overlap))  // Depending of overlap, the point may be or not on a edge
		return(false);
	if(!IsIn(RPoint((line.GetX1()+line.GetX2())/2,(line.GetY1()+line.GetY2())/2),overlap))  // Depending of overlap, the middle point may be inside the polygon
		return(false);

	// We must compare Edge with all the edges of the polygon
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t j=0;j<GetNb();Cur++,j++)
	{
		RPoint** Next;
		if(j==GetNb()-1)
			Next=Tab;
		else
			Next=Cur+1;
		RLine Edge(**Cur,**Next);
		RPoint Inter;

		// If there is an intersection -> The polygon may not be contained
		if(Edge.Inter(line,Inter))
		{
			if(!overlap)
				return(false);  // If no overlap is allowed -> we are sure to return false

			// Overlap is admitted
			if(Inter.IsValid())
			{
				// Verify that Inter is on Edge
				if(!Edge.IsIn(Inter))
						 return(false);

				// Verify if the neighbors of 'Inter' are inside the polygon
				tCoord a,b;         // y=ax+b
				if(Abs(line.GetX2()-line.GetX1())>=cEpsi)
				{
					a=static_cast<double>(line.GetY2()-line.GetY1())/static_cast<double>(line.GetX2()-line.GetX1());
					b=static_cast<double>(line.GetY1()-a*line.GetX1());
					RPoint pt1(Inter.X+cEpsi,a*(Inter.X+cEpsi)+b);
					RPoint pt2(Inter.X-cEpsi,a*(Inter.X+cEpsi)+b);
					if( (line.IsIn(pt1) && !(IsIn(pt1))) || (line.IsIn(pt2) && (!IsIn(pt2))) )
						 return(false);
				}
				else
				{
					RPoint pt1(line.GetX1(),Inter.Y+cEpsi);
					RPoint pt2(line.GetX1(),Inter.Y-cEpsi);
					if ((line.IsIn(pt1) && (!IsIn(pt1))) || (line.IsIn(pt2) && (!IsIn(pt2))) )
						 return(false);
				}
			}
		}
	}

	// OK
	return(true);
}


//------------------------------------------------------------------------------
bool RPolygon::Contained(const RRect& rect) const
{
	RCursor<RPoint> tab(*this);
	for(tab.Start();!tab.End();tab.Next())
	{
		if(!rect.IsIn(*tab()))
			return(false);
	}
	return(true);
}


//------------------------------------------------------------------------------
tCoord RPolygon::Area(void) const
{
	RContainer<RRect,true,false> r(50);
	double Area=0.0;

	RectDecomposition(r);
	RCursor<RRect> Cur(r);
	for(Cur.Start();!Cur.End();Cur.End())
		Area+=Cur()->GetArea();
	return(Area);
}


//------------------------------------------------------------------------------
void RPolygon::Boundary(RRect& rect) const
{
	if(!GetNb())
		mThrowRException("No points defined");

	tCoord MinX=cMaxCoord,MinY=cMaxCoord,MaxX=0,MaxY=0,X,Y;

	RCursor<RPoint> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		X=ptr()->X;
		Y=ptr()->Y;
		if(MinX>X) MinX=X;
		if(MinY>Y) MinY=Y;
		if(MaxX<X) MaxX=X;
		if(MaxY<Y) MaxY=Y;
	}
	rect.Set(MinX,MinY,MaxX,MaxY);
}


//------------------------------------------------------------------------------
void RPolygon::ChangeOrientation(const tOrientation o)
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

	// Make the transformation for each vertex
	RCursor<RPoint> ptr(*this);
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		oldx = factx*ptr()->X;
		oldy = facty*ptr()->Y;
		ptr()->X = tCoord(co*oldx - si*oldy);
		ptr()->Y = tCoord(si*oldx + co*oldy);
		if(ptr()->X<min.X) min.X=ptr()->X;
		if(ptr()->Y<min.Y) min.Y=ptr()->Y;
	}

	// Replace (0,0) as the left-top point of the embedded rectangle
	for(ptr.Start();!ptr.End();ptr.Next())
	{
		ptr()->X -= min.X;
		ptr()->Y -= min.Y;
	}
	ReOrder();    // Make the bottom-left point be the first
}


//------------------------------------------------------------------------------
void RPolygon::RectDecomposition(RContainer<RRect,true,false>& rects) const
{
	if(!IsRectangular())
		mThrowRException("Polygon not rectangular");

	RPolygon work(*this),tmpPoly;
	RContainer<RRect,true,false> tmpRects(20);
	RPoint Pt11;                               // Point at (X1,Y1)
	RPoint Pt12;                               // Point at (X1,Y2)
	RPoint PtX2;                               // Point at (?,Y2)
	RPoint Pt2Y;                               // Point at (X2,?)
	RPoint *Test,tmp;
	tCoord X1,Y1,X2,Y2;                         // Vertices of the rectangle to insert
	size_t i=0,Count,Nb;
	bool bFind;                                 // To use with GetId
	bool bFind21;                               // True if Point (X2,?) is (X2,Y1)

	// ReOrder the polygon
	const_cast<RPolygon*>(this)->ReOrder();

	// Init
	rects.Clear();

	// While points  -> Construct the rectangle (X1,Y1,X2,Y2)
	while(work.GetNb())
	{
		// Search the points (X1,Y1),(X1,Y2) and (?,Y2) -> Fix X1,Y1,Y2.
		Pt11=(*work.GetLeftBottom());
		Pt12=work.GetConY(Pt11);
		PtX2=work.GetConX(Pt12);
		X1=Pt11.X;
		Y1=Pt11.Y;
		Y2=Pt12.Y;

		// Delete Vertice1 and Vertice2
		work.DeletePtr(Pt11);
		work.DeletePtr(Pt12);

		// Find the point at (X2,?) -> Fix X2
		Pt2Y=(*work.GetLeftBottom(X1,Y1,Y2));
		X2=Pt2Y.X;

		// Insert Rectangle
		rects.InsertPtr(new RRect(X1,Y1,X2,Y2));

		// If Pt2Y is (X2,Y1) -> bFind=true
		if((Pt2Y.X==X2)&&(Pt2Y.Y==Y1))
			bFind21=true;
		else
			bFind21=false;

		// If point to add -> after PtX2
		if(!bFind21)
			i=work.GetIndex<RPoint>(PtX2,bFind)+1;

		// Test if (X2,Y1) exists -> If yes delete it else create it
		tmp.X=X2;
		tmp.Y=Y1;
		Test=work.GetPtr<RPoint>(tmp);
		if(Test)
			work.DeletePtr(*Test);
		else
			work.InsertPtrAt(new RPoint(tmp),i,false);

		// If point to add -> after Vertice4
		if(bFind21)
		{
			i=work.GetIndex<RPoint>(PtX2,bFind)+1;
		}

		// Test if (X2,Y2) exists -> If yes delete it else create it
		tmp.X=X2;
		tmp.Y=Y2;
		Test=work.GetPtr<RPoint>(tmp);
		if(Test)
			work.DeletePtr(*Test);
		else
			work.InsertPtrAt(new RPoint(tmp),i,false);

		// Verify if not multiple polygons necessary
		Count=0;	// Counting nb Vertices on the same vertical
		RCursor<RPoint> Cur(work);
		for(Cur.Start();!Cur.End();Cur.Next())
			if((Cur()->X==X2)&&(Cur()->Y<=Y2)&&(Cur()->Y>=Y1))
			{
				tmp=work.GetConX(*Cur());
				if(tmp.X>X2) Count++;
			}
		if(Count%2) continue;
		Count/=2;       // if count Vertices found -> count/2 polygon are necessary
		while(Count>1)  // if more than one polygon is necessary
		{
			Count--;
			// Find the most bottom point at X2 at put point to it
			Cur.Start();
			i=work.GetNb()+1;
			while(Cur()->X!=X2)
			{
				Cur.Next();
				i--;
			}
			Test=Cur();
			for(;--i;Cur.Next())
				if((Cur()->X==X2)&&(Cur()->Y<Test->Y))
					Test=Cur();
			Cur.Start();
			i=0;
			while(Cur()!=Test)
			{
				Cur.Next();
				i++;
			}
			tmpPoly.Clear();

			// Add All other Points
			Nb=work.GetNb();
			tmpPoly.InsertPtr(new RPoint(*Test));
			work.DeletePtr(*Test);
			i++;
			if(i==Nb)
			{
				i=0;
				Cur.Start();
			}
			while(Cur()->X!=X2)
			{
				Test=Cur();
				tmpPoly.InsertPtr(new RPoint(*Test));
				work.DeletePtr(*Test);
				i++;
				if(i==Nb)
				{
					i=0;
					Cur.Start();
				}
			}
			Test=Cur();
			tmp=(*Test);
			tmpPoly.InsertPtr(new RPoint(*Test));
			work.DeletePtr(*Test);

			// Verify if no points left under the last inserted
			Cur.Start();
			i=work.GetNb()+1;
			bFind=false;
			while(--i)
			{
				Test=Cur();
				if((Test->X==X2)&&(Test->Y>tmp.Y))
					bFind=false;
				if(((Test->X==X2)&&(Test->Y<tmp.Y))||bFind)
				{
					tmpPoly.InsertPtr(new RPoint(*Test));
					work.DeletePtr(*Test);
					bFind=true;
				}
				else
					Cur.Next();
			}

			// Calculate the rectangular decomposition of the sub-polygon and add them
			tmpPoly.RectDecomposition(tmpRects);
			rects+=tmpRects;
		}
	}
}


//------------------------------------------------------------------------------
void RPolygon::AddVertices(RContainer<RPoint,true,false>& points) const
{
	RCursor<RPoint> point(*this);
	for(point.Start();!point.End();point.Next())
		points.InsertPtr(new RPoint(*point()));
}


//------------------------------------------------------------------------------
bool RPolygon::IsClockwise(void) const
{
	// Some simple verifications
	if(GetNb()<2)
		return(false);
	if(Order)
		return(false);

	tCoord Sum(0.0);
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t i=0;i<GetNb();i++,Cur++)
	{
		RPoint** Next;
		if(i==GetNb()-1)
			Next=RContainer<RPoint,true,false>::Tab;
		else
			Next=Cur+1;
		Sum+=((*Next)->X-(*Cur)->X)*((*Next)->Y+(*Cur)->Y);

		// Verify if the polygon is rectangle
		if(Rect && ((Abs((*Next)->X-(*Cur)->X)>cEpsi) && (Abs((*Next)->Y-(*Cur)->Y)>cEpsi)))
			const_cast<RPolygon*>(this)->Rect=false;
	}
	return(Sum>0.0);
}


//------------------------------------------------------------------------------
void RPolygon::ReOrder(void)
{
	// Verify first if the polygon must be reorder
	if(Order||(GetNb()<2))
		return;

	// Find the most bottom-left vertex and shift the elements
	Rect=true;      // Suppose it is a rectangular polygon
	size_t idx(GetBottomLeftIndex());
	if(IsClockwise())
		Shift(idx,true);  // The elements must be shifted and reversed.
	else if(idx)
		Shift(idx,false);  // The elements must only be shifted.

	Order=true;
}


//------------------------------------------------------------------------------
void RPolygon::ReValid(double t)
{
	// Go trough the vertices
	size_t Idx(0);  // Current vertex analyzed
	RPoint** Cur(RContainer<RPoint,true,false>::Tab);
	for(size_t i=0;i<GetNb();i++)
	{
		RPoint** Next;
		size_t NextIdx;
		if(i==GetNb()-1)
		{
			Next=RContainer<RPoint,true,false>::Tab;
			NextIdx=0;
		}
		else
		{
			Next=Cur+1;
			NextIdx=Idx+1;
		}

		if((Abs((*Cur)->X-(*Next)->X)-t<cEpsi)&&(Abs((*Cur)->Y-(*Next)->Y)-t<cEpsi))
		{
			DeletePtrAt(NextIdx);
			Order=false;
		}
		else
		{
			Idx++;
			Cur++;
		}
	}
	ReOrder();
}


//------------------------------------------------------------------------------
bool RPolygon::DuplicateVertices(void) const
{
	size_t i;

	RCursor<RPoint> Cur1(*this);
	RCursor<RPoint> Cur2(*this);
	for(i=0,Cur1.Start();i<GetNb()-1;Cur1.Next(),i++)
		for(Cur2.GoTo(i+1);!Cur2.End();Cur2.End())
			if((*Cur1())==(*Cur2()))
				return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPoint RPolygon::GetCentralPoint(void)
{
	RPoint pt;
	RPoint Middle;
	RRect r;
	double min,act;

	Boundary(r);
	Middle.Set(r.GetWidth()/2,r.GetHeight()/2);
	if(IsIn(Middle))
		return(Middle);

	// Find the Vertex the most closed to Middle
	min=cMaxCoord;
	RCursor<RPoint> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		act=Middle.EuclideanDist(*Cur());
		if(act<min)
		{
			pt=(*Cur());
			min=act;
		}
	}
	return(pt);
}


//------------------------------------------------------------------------------
void RPolygon::Save(RTextFile& f)
{
	f<<GetNb()<<endl;
	RCursor<RPoint> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Save(f);
}


//------------------------------------------------------------------------------
RPoint RPolygon::Calibrate(void)
{
	RPoint pt;

	pt.Set(cMaxCoord,cMaxCoord);
	RCursor<RPoint> Cur(*this);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->X<pt.X) pt.X=Cur()->X;
		if(Cur()->Y<pt.Y) pt.Y=Cur()->Y;
	}
	for(Cur.Start();!Cur.End();Cur.Next())
		(*Cur())-=pt;
	return(pt);
}


//------------------------------------------------------------------------------
bool RPolygon::IsRect(void) const
{
	if(GetNb()!=4)
		return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool RPolygon::IsRectangular(void) const
{
	if(Order)
		return(Rect);
	const_cast<RPolygon*>(this)->Rect=true;      // Suppose it is a rectangular polygon
	IsClockwise();
	return(Rect);
}



//------------------------------------------------------------------------------
//
// Operators
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
ostream& std::operator<<(ostream& os, const RPolygon& poly)
{
	RCursor<RPoint> Cur(poly.GetVertices());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		os<<(*Cur());
		if(Cur.GetPos()<Cur.GetNb()-1)
			os<<";";
	}
   return(os);
}
