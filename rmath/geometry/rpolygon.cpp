/*

	R Project Library

	Polygon.cpp

	Polygon - Implentation.

	Copyright 1999-2004 by the Universit�Libre de Bruxelles.

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
// include files for ANSI C/C++
#include <iostream>


//------------------------------------------------------------------------------
// include files for R Project
#include <rmath/rpolygon.h>
#include <rmath/rpoints.h>
#include <rmath/rrect.h>
#include <rmath/rrects.h>
#include <rmath/rline.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// Class RPolygon
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPolygon::RPolygon(void)
	: RContainer<RPoint,true,false>(20,10)
{
}


//------------------------------------------------------------------------------
RPolygon::RPolygon(const int Max)
	: RContainer<RPoint,true,false>(Max,10)
{
}


//------------------------------------------------------------------------------
RPolygon::RPolygon(const RPolygon* poly)
	: RContainer<RPoint,true,false>(poly)
{
}


//------------------------------------------------------------------------------
RPolygon::RPolygon(const RPolygon& poly)
	: RContainer<RPoint,true,false>(poly)
{
}


//------------------------------------------------------------------------------
RPolygon& RPolygon::operator=(const RPolygon& poly)
{
	RContainer<RPoint,true,false>::operator=(poly);
	return(*this);
}


//------------------------------------------------------------------------------
bool RPolygon::operator==(const RPolygon& poly) const
{
	RPoint** pt;
	unsigned int i;

	if(NbPtr!=poly.NbPtr) return(false);
	for(i=NbPtr+1,pt=Tab;--i;pt++)
		if(!poly.IsVertice(*pt)) return(false);
	return(true);
}


//------------------------------------------------------------------------------
bool RPolygon::operator!=(const RPolygon& poly) const
{
	RPoint** pt;
	unsigned int i;

	for(i=NbPtr+1,pt=Tab;--i;pt++)
		if(!poly.IsVertice(*pt)) return(true);
	return(false);
}


//------------------------------------------------------------------------------
RPolygon& RPolygon::operator+=(const RPoint& pt) throw(std::bad_alloc)
{
	unsigned int i;
	RPoint** tab;

	for(i=NbPtr+1,tab=Tab;--i;tab++) (**tab)+=pt;
	return(*this);
}


//------------------------------------------------------------------------------
RPolygon& RPolygon::operator-=(const RPoint& pt) throw(std::bad_alloc)
{
	unsigned int i;
	RPoint** tab;

	for(i=NbPtr+1,tab=Tab;--i;tab++) (**tab)-=pt;
	return(*this);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetConX(const RPoint* pt) const
{
	RPoint **point,*next;
	unsigned int i;

	RReturnValIfFail(pt,0);
	point=Tab;
	i=0;
	while((**point)!=(*pt))
	{
		point++;
		i++;
	}

	// Look previous vertice
	if(i) next=(*(point-1)); else next=Tab[NbPtr-1];
	if(next->Y==pt->Y) return(next);

	// Look next vertice
	if(i<NbPtr-1)	next=(*(point+1)); else	next=(*Tab);
	if(next->Y==pt->Y) return(next);

	// Problems when arriving here
	return(0);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetConY(const RPoint* pt) const
{
	RPoint **point,*next;
	unsigned int i;

	RReturnValIfFail(pt,0);
	point=Tab;
	i=0;
	while((**point)!=(*pt))
	{
		point++;
		i++;
	}

	// Look previous vertice
	if(i) next=(*(point-1)); else next=Tab[NbPtr-1];
	if(next->X==pt->X) return(next);

	// Look next vertice
	if(i<NbPtr-1)	next=(*(point+1)); else	next=(*Tab);
	if(next->X==pt->X) return(next);

	// Problems when arriving here
	return(0);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetBottomLeft(void) const
{
	RPoint **point,*bl;
	unsigned int i;
	RCoord X,Y;

	bl=*Tab;
	for(i=NbPtr,point=&Tab[1];--i;point++)
	{
		X=(*point)->X;
		Y=(*point)->Y;
		if((Y<bl->Y)||((Y==bl->Y)&&(X<bl->X)))
			bl=(*point);
	}
	return(bl);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetBottomLeft(const RCoord MinX,const RCoord MinY,const RCoord MaxX) const
{
	RPoint **point,*bl;
	unsigned int i;
	RCoord X,Y;

	i=NbPtr+1;
	point=Tab;
	X=(*point)->X;
	Y=(*point)->Y;
	while((Y<MinY)||(X<MinX)&&(X>MaxX))
	{
		point++;
		i--;
		X=(*point)->X;
		Y=(*point)->Y;
	}
	bl=(*point);
	for(;--i;point++)
	{
		X=(*point)->X;
		Y=(*point)->Y;
		if(((Y>=MinY)&&(X>=MinX)&&(X<=MaxX))&&((Y<bl->Y)||((Y==bl->Y)&&(X<bl->X))))
			bl=(*point);
	}
	return(bl);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetLeftBottom(void) const
{
	RPoint **point,*lb;
	unsigned int i;
	RCoord X,Y;

	lb=*Tab;
	for(i=NbPtr,point=&Tab[1];--i;point++)
	{
		X=(*point)->X;
		Y=(*point)->Y;
		if((X<lb->X)||((X==lb->X)&&(Y<lb->Y)))
			lb=(*point);
	}
	return(lb);
}


//------------------------------------------------------------------------------
RPoint* RPolygon::GetLeftBottom(const RCoord MinX,const RCoord MinY,const RCoord MaxY) const
{
	RPoint **point,*lb;
	unsigned int i;
	RCoord X,Y;

	i=NbPtr+1;
	point=Tab;
	X=(*point)->X;
	Y=(*point)->Y;
	while((Y<MinY)||(X<MinX)||(Y>MaxY))
	{
		point++;
		i--;
		X=(*point)->X;
		Y=(*point)->Y;
	}
	lb=(*point);
	for(;--i;point++)
	{
		X=(*point)->X;
		Y=(*point)->Y;
		if(((Y>=MinY)&&(Y<=MaxY)&&(X>MinX))&&((X<lb->X)||((X==lb->X)&&(Y<lb->Y))))
			lb=(*point);
	}
	return(lb);
}


//------------------------------------------------------------------------------
bool RPolygon::Edge(const RCoord X,const RCoord Y) const
{
	RPoint **point,*deb;
	unsigned int i;
	RCoord DiffX,DiffY;

	point=Tab;
	i=NbPtr+1;
	while(--i)
	{
		deb=(*(point++));
		if(i==1)
		{
			point=Tab;
		}
		DiffX=deb->X-(*point)->X;
		DiffY=deb->Y-(*point)->Y;
		if(!DiffY)
		{
			// Horizontal edge
			if(Y!=deb->Y) continue;
			if(DiffX<0)
			{
				if((X>=deb->X)&&(X<=(*point)->X)) return(true);
			}
			else
			{
				if((X<=deb->X)&&(X>=(*point)->X)) return(true);
			}
		}
		else
		{
			// Vertical edge
			if(X!=deb->X) continue;
			if(DiffY<0)
			{
				if((Y>=deb->Y)&&(Y<=(*point)->Y)) return(true);
			}
			else
			{
				if((Y<=deb->Y)&&(Y>=(*point)->Y)) return(true);
			}
		}
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygon::Edge(const RPoint* pt1,const RPoint* pt2) const
{
	RPoint **point,*deb;
	unsigned int i;
	RCoord X,Y;

	RReturnValIfFail(pt1&&pt2,false);
	if((pt1->X!=pt2->X)&&(pt1->Y!=pt2->Y)) return(false);
	point=Tab;
	i=NbPtr+1;
	while(--i)
	{
		deb=(*(point++));
		if(i==1)
		{
			point=Tab;
		}
		X=deb->X-(*point)->X;
		Y=deb->Y-(*point)->Y;
		if(!Y)
		{
			// Horizontal edge
			if((pt1->Y!=deb->Y)||(pt2->Y!=deb->Y)) continue;
			if(X<0)
			{
				if((pt1->X>=deb->X)&&(pt1->X<=(*point)->X))
				{
					if((pt2->X>=deb->X)&&(pt2->X<=(*point)->X))
						return(true);
					else
						return(false);
				}
			}
			else
			{
				if((pt1->X<=deb->X)&&(pt1->X>=(*point)->X))
				{
					if((pt2->X<=deb->X)&&(pt2->X>=(*point)->X))
						return(true);
					else
						return(false);
				}
			}
		}
		else
		{
			// Vertical edge
			if((pt1->X!=deb->X)||(pt2->X!=deb->X)) continue;
			if(Y<0)
			{
				if((pt1->Y>=deb->Y)&&(pt1->Y<=(*point)->Y))
				{
					if((pt2->Y>=deb->Y)&&(pt2->Y<=(*point)->Y))
						return(true);
					else
						return(false);
				}
			}
			else
			{
				if((pt1->Y<=deb->Y)&&(pt1->Y>=(*point)->Y))
				{
					if((pt2->Y<=deb->Y)&&(pt2->Y>=(*point)->Y))
						return(true);
					else
						return(false);
				}
			}
		}
	}
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygon::IsVertice(const RPoint& pt) const
{
	RPoint **point;
	unsigned int i;

	for(i=NbPtr+1,point=Tab;--i;point++)
		if((**point)==pt) return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RPolygon::IsIn(const RCoord X,const RCoord Y) const
{
	RPoint p(X,Y),**tab,*act,*next;
	unsigned int i,count;
	RCoord y1,y2;

	// Special cases
	if(NbPtr==1) return(p==(*Tab[0]));
	if(NbPtr==2)
	{
		RDirection c=p.Classify(Tab[0],Tab[1]);
		return((c==Between)||(c==Origin)||(c==Destination));
	}
	if(NbPtr==4)
	{
		if((X>=Tab[0]->X)&&(Y>=Tab[0]->Y)&&(X<=Tab[2]->X)&&(Y<=Tab[2]->Y))
			return(true);
		return(false);
	}

	// Verify if not a vertice
	for(i=NbPtr+1,tab=Tab;--i;tab++)
		if(((*tab)->X==X)&&((*tab)->Y==Y))
			return(true);

	// Verify if not on an edge
	if(Edge(X,Y))
		return(true);

	// Count the intersections between the line (X,Y) and (MaxCoord,Y) and the edges
	count=0;
	i=NbPtr;
	act=GetBottomLeft();
	while(i>0)
	{
		// Look for act
		next=GetConY(act);
		if(act->Y>next->Y)
		{
			y1=next->Y;
			y2=act->Y;
		}
		else
		{
			y1=act->Y;
			y2=next->Y;
		}

		// Test Line e1,act
		if((act->X>=X)&&(Y>=y1)&&(Y<=y2))
			count++;
		i--;
		act=GetConX(act);
		i--;
		act=GetConY(act);
	}

	// if count%2==0 -> Not in
	return(count%2);
}


//------------------------------------------------------------------------------
bool RPolygon::IsIn(const RPolygon* poly) const
{
	RPoint **pt;
	unsigned int i;
	RRect r1,r2;
	RDirection FromDir;
	RPoint *start,*end;
	RCoord X,Y;
	unsigned int nbpts;

	// Polygon is a rectangle?
	if(NbPtr==4)
	{
		// Each vertex of poly have to be in it.
		for(i=poly->NbPtr,pt=poly->Tab;--i;pt++)
		{
			if(!IsIn(*pt))
				return(false);
		}
		return(true);
	}

	// Verify first if the boundary rectangles overlap
	Boundary(r1);
	poly->Boundary(r2);
	if(!r1.Overlap(&r2))
		return(false);

	// Test it and go through the other
	start=poly->GetBottomLeft();
	end=poly->GetConX(start);
	FromDir=Left;
	X=start->X;
	Y=start->Y;
	nbpts=poly->NbPtr;
	while(nbpts)
	{
		if(!IsIn(X,Y))
			return(false);

		// If end of an edge
		if((X==end->X)&&(Y==end->Y))
		{
			start=end;
			nbpts--;        // Next point
			X=start->X;
			Y=start->Y;
			if((FromDir==Left)||(FromDir==Right))
			{
				end=poly->GetConY(start);
				if(start->Y<end->Y) FromDir=Down; else FromDir=Up;
			}
			else           // Go to left/right
			{
				end=poly->GetConX(start);
				if(start->X<end->X) FromDir=Left; else FromDir=Right;
			}
		}
		else
			AdaptXY(X,Y,FromDir);
	}
	return(true);
}


//------------------------------------------------------------------------------
RCoord RPolygon::Area(void) const
{
	RRects r;

	RectDecomposition(&r);
	return(r.Area());
}


//------------------------------------------------------------------------------
void RPolygon::Boundary(RRect& rect) const
{
	RCoord MinX=MaxCoord,MinY=MaxCoord,MaxX=0,MaxY=0,X,Y;
	RPoint **ptr;
	int i;

	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		X=(*ptr)->X;
		Y=(*ptr)->Y;
		if(MinX>X) MinX=X;
		if(MinY>Y) MinY=Y;
		if(MaxX<X) MaxX=X;
		if(MaxY<Y) MaxY=Y;
	}
	rect.Pt1.X=MinX;
	rect.Pt1.Y=MinY;
	rect.Pt2.X=MaxX;
	rect.Pt2.Y=MaxY;
}


//------------------------------------------------------------------------------
void RPolygon::ChangeOrientation(const ROrientation o,RPoint& min)
{
	RCoord factx=1,facty=1,i,oldx,oldy;
	RPoint **ptr;
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
	min.X=min.Y=MaxCoord;

	// Make the transformation for each vertice
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		oldx = factx*(*ptr)->X;
		oldy = facty*(*ptr)->Y;
		(*ptr)->X = RCoord(co*oldx - si*oldy);
		(*ptr)->Y = RCoord(si*oldx + co*oldy);
		if((*ptr)->X<min.X) min.X=(*ptr)->X;
		if((*ptr)->Y<min.Y) min.Y=(*ptr)->Y;
	}

	// Replace (0,0) as the left-top point of the embedded rectangle
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
	{
		(*ptr)->X -= min.X;
		(*ptr)->Y -= min.Y;
	}
	ReOrder();    // Make the bottom-left point be the first
}


//------------------------------------------------------------------------------
void RPolygon::RectDecomposition(RRects* rects) const
{
	RPolygon work(this),tmpPoly(20);
	RRects tmpRects;
	RPoint *Pt11;                               // Point at (X1,Y1)
	RPoint *Pt12;                               // Point at (X1,Y2)
	RPoint *PtX2;                               // Point at (?,Y2)
	RPoint *Pt2Y;                               // Point at (X2,?)
	RPoint **point,*Test,tmp;
	RCoord X1,Y1,X2,Y2;                         // Vertices of the rectangle to insert
	unsigned int i=0,Count,Nb;
	bool bFind;                                 // To use with GetId
	bool bFind21;                               // True if Point (X2,?) is (X2,Y1)

	// Init
	RReturnIfFail(rects);
	rects->Clear();

	// While points  -> Construct the rectangle (X1,Y1,X2,Y2)
	while(work.NbPtr)
	{
		// Search the points (X1,Y1),(X1,Y2) and (?,Y2) -> Fix X1,Y1,Y2.
		Pt11=work.GetLeftBottom();
		RAssert(Pt11);
		Pt12=work.GetConY(Pt11);
		RAssert(Pt12);
		PtX2=work.GetConX(Pt12);
		RAssert(PtX2);
		X1=Pt11->X;
		Y1=Pt11->Y;
		Y2=Pt12->Y;

		// Delete Vertice1 and Vertice2
		work.DeletePtr(Pt11);
		work.DeletePtr(Pt12);

		// Find the point at (X2,?) -> Fix X2
		Pt2Y=work.GetLeftBottom(X1,Y1,Y2);
		RAssert(Pt2Y);
		X2=Pt2Y->X;

		// Insert Rectangle
		rects->InsertPtr(new RRect(X1,Y1,X2,Y2));

		// If Pt2Y is (X2,Y1) -> bFind=true
		if((Pt2Y->X==X2)&&(Pt2Y->Y==Y1))
			bFind21=true;
		else
			bFind21=false;

		// If point to add -> after PtX2
		if(!bFind21)
			i=work.GetId<RPoint*>(PtX2,bFind)+1;

		// Test if (X2,Y1) exists -> If yes delete it else create it
		tmp.X=X2;
		tmp.Y=Y1;
		Test=work.GetPtr<RPoint>(tmp);
		if(Test)
			work.DeletePtr(Test);
		else
			work.InsertPtrAt(new RPoint(tmp),i,false);

		// If point to add -> after Vertice4
		if(bFind21)
		{
			i=work.GetId<RPoint*>(PtX2,bFind)+1;
		}

		// Test if (X2,Y2) exists -> If yes delete it else create it
		tmp.X=X2;
		tmp.Y=Y2;
		Test=work.GetPtr<RPoint>(tmp);
		if(Test)
			work.DeletePtr(Test);
		else
			work.InsertPtrAt(new RPoint(tmp),i,false);

		// Verify if not multiple polygons necessary
		Count=0;	// Counting nb Vertices on the same vertical
		for(i=work.NbPtr+1,point=work.Tab;--i;point++)
			if(((*point)->X==X2)&&((*point)->Y<=Y2)&&((*point)->Y>=Y1))
			{
				Test=work.GetConX(*point);
				RAssert(Test);
				if(Test->X>X2) Count++;
			}
		if(Count%2) continue;
		Count/=2;       // if count Vertices found -> count/2 polygon are necessary
		while(Count>1)  // if more than one polygon is necessary
		{
			Count--;
			// Find the most bottom point at X2 at put point to it
			point=work.Tab;
			i=work.NbPtr+1;
			while((*point)->X!=X2)
			{
				point++;
				i--;
			}
			Test=(*point);
			for(;--i;point++)
				if(((*point)->X==X2)&&((*point)->Y<Test->Y))
					Test=(*point);
			point=work.Tab;
			i=0;
			while((*point)!=Test)
			{
				point++;
				i++;
			}
			tmpPoly.Clear();

			// Add All other Points
			Nb=work.NbPtr;
			tmpPoly.InsertPtr(new RPoint(Test));
			work.DeletePtr(Test);
			i++;
			if(i==Nb)
			{
				i=0;
				point=work.Tab;
			}
			while((*point)->X!=X2)
			{
				Test=(*point);
				tmpPoly.InsertPtr(new RPoint(Test));
				work.DeletePtr(Test);
				i++;
				if(i==Nb)
				{
					i=0;
					point=work.Tab;
				}
			}
			Test=(*point);
			tmp=(*Test);
			tmpPoly.InsertPtr(new RPoint(Test));
			work.DeletePtr(Test);

			// Verify if no points left under the last inserted
			point=work.Tab;
			i=work.NbPtr+1;
			bFind=false;
			while(--i)
			{
				Test=(*point);
				if((Test->X==X2)&&(Test->Y>tmp.Y))
					bFind=false;
				if(((Test->X==X2)&&(Test->Y<tmp.Y))||bFind)
				{
					tmpPoly.InsertPtr(new RPoint(Test));
					work.DeletePtr(Test);
					bFind=true;
				}
				else
					point++;
			}

			// Calculate the rectangular decomposition of the sub-polygon and add them
			tmpPoly.RectDecomposition(&tmpRects);
			(*rects)+=tmpRects;
		}
	}
}


//------------------------------------------------------------------------------
void RPolygon::AddPoints(RPoints* points) const
{
	RPoint **point;
	unsigned int i;

	RReturnIfFail(points);
	for(i=NbPtr+1,point=Tab;--i;point++)
		points->InsertPtr(new RPoint(*point));
}


//------------------------------------------------------------------------------
void RPolygon::ReOrder(void)
{
	RPoint **tmp,**point,*next;
	unsigned int i;
	bool bX;            // Next Vertice is horizontal

	if(!NbPtr) return;
	point=tmp=new RPoint*[MaxPtr];
	next=GetBottomLeft();
	(*(point++))=next;
	next=GetConX(next);
	(*(point++))=next;
	bX=false;
	i=NbPtr-1;
	while(--i)
	{
		if(bX) next=GetConX(next); else next=GetConY(next);
		(*(point++))=next;
		bX=(!bX);
	}
	delete[] Tab;
	Tab=tmp;
}


//------------------------------------------------------------------------------
void RPolygon::ReValid(void)
{
	unsigned int i;
	RPoint *pt1,*nextx,*nexty,*pt2;
	bool c=false;

	for(i=0;i<NbPtr;i++)
	{
		pt1=Tab[i];
		nextx=GetConX(pt1);
		if((nextx)&&(pt1->X==nextx->X-1))
		{
			pt2=GetConY(nextx);
			pt1->Y=pt2->Y;
			DeletePtr(nextx);
			DeletePtr(pt2);
			c=true;
		}
		nexty=GetConY(pt1);
		if((nexty)&&(pt1->Y==nexty->Y-1))
		{
			pt2=GetConX(nexty);
			pt1->X=pt2->X;
			DeletePtr(nexty);
			DeletePtr(pt2);
			c=true;
		}
	}
	if(NbPtr<4)
	{
		Clear();
	}
	else
	{
		if(c) ReOrder();    // The points must order anti-clockwise.
	}
}


//------------------------------------------------------------------------------
bool RPolygon::DuplicatePoints(void) const
{
	unsigned int i,j;
	RPoint **point1,**point2;

	for(i=0,point1=Tab;i<NbPtr-1;point1++,i++)
		for(j=i+1,point2=&Tab[i+1];j<NbPtr;point2++,j++)
			if((**point1)==(**point2))
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
	Middle.Set(r.Width()/2,r.Height()/2);
	if(IsIn(Middle))
		return(Middle);

	// Find the Vertice the most closed to Middle
	min=MaxCoord;
	for(Start();!End();Next())
	{
		act=Middle.EuclideanDist((*this)());
		if(act<min)
		{
			pt=((*this)());
			min=act;
		}
	}
	return(pt);
}


//------------------------------------------------------------------------------
void RPolygon::Save(RTextFile& f)
{
	f<<NbPtr<<endl;
	for(Start();!End();Next())
		(*this)()->Save(f);
}


//------------------------------------------------------------------------------
RPoint RPolygon::Calibrate(void)
{
	RPoint pt,**tab;
	unsigned int i;

	pt.Set(MaxCoord,MaxCoord);
	for(i=NbPtr+1,tab=Tab;--i;tab++)
	{
		if((*tab)->X<pt.X) pt.X=(*tab)->X;
		if((*tab)->Y<pt.Y) pt.Y=(*tab)->Y;
	}

	for(i=NbPtr+1,tab=Tab;--i;tab++)
		(**tab)-=pt;
	return(pt);
}


//------------------------------------------------------------------------------
RCursor<RPoint> RPolygon::GetPointsCursor(void)
{
	RCursor<RPoint> cur(this);
	return(cur);
}
