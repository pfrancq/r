/*

  Polygons.cpp

  Polygons - Implentation.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iostream.h>


//---------------------------------------------------------------------------
// include files for RGeometry
#include "polygons.h"
#include "rline.h"
using namespace RGeometry;



//---------------------------------------------------------------------------
//
// Class "RPolygon"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPolygon::RPolygon(void) : RContainer<RPoint,unsigned int,true,false>(20,10)
{
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(int Max) : RContainer<RPoint,unsigned int,true,false>(Max,10)
{
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(RPolygon *poly) : RContainer<RPoint,unsigned int,true,false>(poly->NbPtr,10)
{
  RPoint **ptr;
  int i;

  for(i=poly->NbPtr+1,ptr=poly->Tab;--i;ptr++)
    InsertPtr(new RPoint(*ptr));
}


//---------------------------------------------------------------------------
RPolygon& RPolygon::operator=(const RPolygon &poly)
{
	RContainer<RPoint,unsigned int,true,false>::operator=(poly);
  return(*this);
}


//---------------------------------------------------------------------------
int RPolygon::Compare(RPolygon*)
{
	return(0);
}


//---------------------------------------------------------------------------
// Trace a line from point to the infini
// Calc nb intersections between this line and each segment of the polygon
// If nb intersections is odd -> point is in the polygon
bool RPolygon::IsIn(const RPoint &point)
{
  RLine lt,lp;
  RPoint **pt,*last;
  unsigned int i;
	long count=0;
	
  lt.Pt1=point;
  lt.Pt1.X=MaxCoord;
  lt.Pt1.Y=point.Y;
	last=Tab[NbPtr-1];	// Point last to ending point of Polygon
	for(i=NbPtr+1,pt=Tab;--i;pt++)
	{
		if(lt.IsIn(**pt)) continue;
		lp.Pt1=(**pt);
		lp.Pt2=(*last);
		last=(*pt);
		if(lp.Inter(lt)) count++;
  }
  return(count%1);
}


//---------------------------------------------------------------------------
RRect* RPolygon::Boundary()
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
  return(new RRect(MinX,MinY,MaxX,MaxY));
}


//---------------------------------------------------------------------------
void RPolygon::Boundary(RRect &rect)
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


//---------------------------------------------------------------------------
void RPolygon::Orientation(char Ori)
{
  RCoord factx=1,facty=1,i,minx,miny,oldx,oldy;
  RPoint **ptr;
  double co=1,si=0;

  // Determin scale and roration
  if(Ori==orNormalMirrorX||Ori==orNormalMirrorYX||Ori==orRota90MirrorX||Ori==orRota90MirrorYX)
    facty=-1;
  if(Ori==orNormalMirrorY||Ori==orNormalMirrorYX||Ori==orRota90MirrorY||Ori==orRota90MirrorYX)
    factx=-1;
  if(Ori==orRota90||Ori==orRota90MirrorX||Ori==orRota90MirrorY||Ori==orRota90MirrorYX)
  {
   co=0;
   si=1;
  }
  minx=miny=0;

  // Make the transformation for each vertex
  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
  {
    oldx = factx*(*ptr)->X;
    oldy = facty*(*ptr)->Y;
    (*ptr)->X = RCoord(co*oldx - si*oldy);
    (*ptr)->Y = RCoord(si*oldx + co*oldy);
    if((*ptr)->X<minx) minx=(*ptr)->X;
    if((*ptr)->Y<miny) miny=(*ptr)->Y;
  }

  // Replace (0,0) as the left-top point of the embedded rectangle
  for(i=NbPtr+1,ptr=Tab;--i;ptr++)
  {
    (*ptr)->X -= minx;
    (*ptr)->Y -= miny;
  }
}


//---------------------------------------------------------------------------
void RPolygon::RectDecomposition(RRects *rects)
{
	RPoints pts(NbPtr*2);
	RPoint **pt,*cur,*cur2;	
	RPoint dep,cor;
	unsigned int i;
	RCoord tmp;
	bool depc,corc;	
	RRect *rect;

	// Init
	rects->Clear();
	if(NbPtr==4)	// The polygon is already a rectangle
	{
		rect=new RRect();
		Boundary(*rect);
		rects->InsertPtr(rect);
		return;
	}
	for(i=NbPtr+1,pt=Tab;--i;pt++)
		pts.InsertPtr(new RPoint(*pt));

	// There is more than one rectangle to construct
	while(pts.NbPtr)	
	{
		// Find the Left-Down point and delete it
		pt=pts.Tab;
		cur=(*pt);
		for(i=pts.NbPtr,pt++;--i;pt++)
		{
			if((*pt)->X<cur->X)
				cur=(*pt);
			else
				if(((*pt)->X==cur->X)&&((*pt)->Y<cur->Y))
					cur=(*pt);
		}
		dep=(*cur);
		pts.DeletePtr(cur);

		// Find the corresponding vertex and delete it
		pt=pts.Tab;		
		i=pts.NbPtr+1;
		while((*pt)->X!=dep.X)
		{
			pt++;
			i--;
		}
		cur=(*pt);		
		for(--i,pt++;--i;pt++)
			if(((*pt)->X==dep.X)&&((*pt)->Y<cur->Y)) cur=(*pt);
		cor=(*cur);
		pts.DeletePtr(cur);

		// Find the next vertext of dep,cor
		cur=cur2=NULL;
		for(i=pts.NbPtr+1,pt=pts.Tab;--i;pt++)
		{
			if((*pt)->Y==dep.Y)
				if(!cur||(*pt)->X<cur->X)
					cur=(*pt);
			if((*pt)->Y==cor.Y)
				if(!cur2||(*pt)->X<cur2->X)
					cur2=(*pt);
    }

		// Look for rectangle
		tmp=cur->X-cur2->X;		
		if(!tmp)	// Already Rectangle
		{
			rects->InsertPtr(new RRect(dep.X,dep.Y,cur2->X,cur2->Y));
			depc=corc=true;
			for(i=pts.NbPtr+1,pt=pts.Tab;--i;pt++)
			{
				if((*pt)->X==cur->X&&(*pt)->Y<cur->Y) depc=false;
				if((*pt)->X==cur2->X&&(*pt)->Y>cur2->Y) corc=false;
			}	
			if(depc) pts.DeletePtr(cur);
			if(corc) pts.DeletePtr(cur2);			
		}
		else
			if(tmp<0)
			{
				rects->InsertPtr(new RRect(dep.X,dep.Y,cur->X,cor.Y));
				pts.InsertPtr(new RPoint(cur->X,cur2->Y));
				pts.DeletePtr(cur);
			}
		  else
			{
				rects->InsertPtr(new RRect(dep.X,dep.Y,cur2->X,cur2->Y));
				pts.InsertPtr(new RPoint(cur2->X,cur->Y));
				pts.DeletePtr(cur2);
			}
	}
}



//---------------------------------------------------------------------------
//
// Class "RPolygons"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPolygons::RPolygons(void) : RContainer<RPolygon,unsigned int,true,false>(20,10)
{
}


//---------------------------------------------------------------------------
void RPolygons::Union(RPolygon *poly)
{
}


//---------------------------------------------------------------------------
RPolygons& RPolygons::operator=(const RPolygons &poly)
{
	RContainer<RPolygon,unsigned int,true,false>::operator=(poly);
	return(*this);
}