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
#include "rpolygon.h"
#include "rline.h"
using namespace RGeometry2D;



//---------------------------------------------------------------------------
//
// Class "RPolygon"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPolygon::RPolygon(void)
	: RContainer<RPoint,unsigned int,true,false>(20,10)
{
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(int Max)
	: RContainer<RPoint,unsigned int,true,false>(Max,10)
{
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(RPolygon *poly)
	: RContainer<RPoint,unsigned int,true,false>(poly->NbPtr,10)
{
  RPoint **ptr;
  int i;

  for(i=poly->NbPtr+1,ptr=poly->Tab;--i;ptr++)
    InsertPtr(new RPoint(*ptr));
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(RPolygon &poly)
	: RContainer<RPoint,unsigned int,true,false>(poly.NbPtr,10)
{
  RPoint **ptr;
  int i;

  for(i=poly.NbPtr+1,ptr=poly.Tab;--i;ptr++)
    InsertPtr(new RPoint(*ptr));
}


//---------------------------------------------------------------------------
RPolygon& RPolygon::operator=(const RPolygon &poly)
{
	RContainer<RPoint,unsigned int,true,false>::operator=(poly);
  return(*this);
}


//---------------------------------------------------------------------------
bool RPolygon::operator==(const RPolygon &poly)
{
	RPoint **pt;
	unsigned int i;

	if(NbPtr!=poly.NbPtr) return(false);
	for(i=NbPtr+1,pt=Tab;--i;pt++)
		if(!poly.IsEdge(*pt)) return(false);
	return(true);
}


//---------------------------------------------------------------------------
bool RPolygon::operator!=(const RPolygon &poly)
{
	RPoint **pt;
	unsigned int i;

	for(i=NbPtr+1,pt=Tab;--i;pt++)
		if(!poly.IsEdge(*pt)) return(true);
	return(false);
}


//---------------------------------------------------------------------------
RPolygon& RPolygon::operator+=(const RPoint &pt) throw(bad_alloc)
{
	unsigned int i;
	RPoint **tab;

	for(i=NbPtr+1,tab=Tab;--i;tab++) (**tab)+=pt;
	return(*this);
}


//---------------------------------------------------------------------------
RPoint* RPolygon::GetConX(RPoint *pt)
{
	RPoint **point,*next;
	unsigned int i;

	point=Tab;
	i=0;
	while((**point)!=(*pt))
	{
		point++;
		i++;
	}

	// Look previous edge
	if(i) next=(*(point-1)); else next=Tab[NbPtr-1];
	if(next->Y==pt->Y) return(next);

	// Look next edge
	if(i<NbPtr-1)	next=(*(point+1)); else	next=(*Tab);
	if(next->Y==pt->Y) return(next);

	// Problems when arriving here
	return(NULL);
}


//---------------------------------------------------------------------------
RPoint* RPolygon::GetConY(RPoint *pt)
{
	RPoint **point,*next;
	unsigned int i;

	point=Tab;
	i=0;
	while((**point)!=(*pt))
	{
		point++;
		i++;
	}

	// Look previous edge
	if(i) next=(*(point-1)); else next=Tab[NbPtr-1];
	if(next->X==pt->X) return(next);

	// Look next edge
	if(i<NbPtr-1)	next=(*(point+1)); else	next=(*Tab);
	if(next->X==pt->X) return(next);

	// Problems when arriving here
	return(NULL);
}


//---------------------------------------------------------------------------
RPoint* RPolygon::GetBottomLeft(void)
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


//---------------------------------------------------------------------------
RPoint* RPolygon::GetBottomLeft(RCoord MinX,RCoord MinY,RCoord MaxX)
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


//---------------------------------------------------------------------------
RPoint* RPolygon::GetLeftBottom(void)
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


//---------------------------------------------------------------------------
RPoint* RPolygon::GetLeftBottom(RCoord MinX,RCoord MinY,RCoord MaxY)
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


//---------------------------------------------------------------------------
bool RPolygon::Vertex(RPoint *pt)
{
	RPoint **point,*deb;
	unsigned int i;
	RCoord X,Y;

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
			// Horizontal vertex
			if(pt->Y!=deb->Y) continue;
			if(X<0)
			{
				if((pt->X>=deb->X)&&(pt->X<=(*point)->X)) return(true);
			}
			else
			{
				if((pt->X<=deb->X)&&(pt->X>=(*point)->X)) return(true);
			}				
		}
		else
		{
			// Vertical vertex
			if(pt->X!=deb->X) continue;
			if(Y<0)
			{
				if((pt->Y>=deb->Y)&&(pt->Y<=(*point)->Y)) return(true);
			}
			else
			{
				if((pt->Y<=deb->Y)&&(pt->Y>=(*point)->Y)) return(true);
			}				
		}		
	}
	return(false);
}


//---------------------------------------------------------------------------
bool RPolygon::Vertex(RPoint *pt1,RPoint *pt2)
{
	RPoint **point,*deb;
	unsigned int i;
	RCoord X,Y;

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
			// Horizontal vertex
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
			// Vertical vertex
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


//---------------------------------------------------------------------------
bool RPolygon::IsEdge(const RPoint &pt) const
{
	RPoint **point;
	unsigned int i;

	for(i=NbPtr+1,point=Tab;--i;point++)
		if((**point)==pt) return(true);
	return(false);
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
  lt.Pt2.X=MaxCoord;
  lt.Pt2.Y=point.Y;
	last=Tab[NbPtr-1];	// Point last to ending point of Polygon
	for(i=NbPtr+1,pt=Tab;--i;pt++)
	{
		if(lt.IsIn(**pt)) continue;
		lp.Pt1=(**pt);
		lp.Pt2=(*last);
		last=(*pt);
		if(lp.Inter(lt)) count++;
  }
  return(count%2);
}


//---------------------------------------------------------------------------
bool RPolygon::IsIn(const RPolygon &poly)
{
	RPoint **pt;
	unsigned int i;

	for(i=poly.NbPtr+1,pt=poly.Tab;--i;pt++)
		if(!IsIn(*pt))
			return(false);
	return(true);
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
void RPolygon::ChangeOrientation(ROrientation o)
{
  RCoord factx=1,facty=1,i,minx,miny,oldx,oldy;
  RPoint **ptr;
  double co=1,si=0;

  // Determine scale and roration
  if((o==NormalX)||(o==NormalYX)||(o==Rota90X)||(o==Rota90YX))
    facty=-1;
  if((o==NormalY)||(o==Normal)||(o==Rota90Y)||(o==Rota90YX))
    factx=-1;
  if((o==Rota90)||(o==Rota90X)||(o==Rota90Y)||(o==Rota90YX))
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
	ReOrder();	// Make the top-left point be the first
}


//---------------------------------------------------------------------------
void RPolygon::RectDecomposition(RRects *rects)
{
	RPolygon work(this),tmpPoly(20);
	RRects tmpRects;
	RPoint *Pt11;								// Point at (X1,Y1)
	RPoint *Pt12;								// Point at (X1,Y2)
	RPoint *PtX2;								// Point at (?,Y2)
	RPoint *Pt2Y;								// Point at (X2,?)
	RPoint **point,*Test,tmp;
	RCoord X1,Y1,X2,Y2;					// Edges of the rectangle to insert
	unsigned int i,Count,Nb;
	bool bFind;									// To use with GetId
	bool bFind21;								// True if Point (X2,?) is (X2,Y1)

	// Init
	rects->Clear();

	// While points  -> Construct the rectangle (X1,Y1,X2,Y2)
	while(work.NbPtr)
	{
		// Search the points (X1,Y1),(X1,Y2) and (?,Y2) -> Fix X1,Y1,Y2.
		Pt11=work.GetLeftBottom();
		Pt12=work.GetConY(Pt11);
		PtX2=work.GetConX(Pt12);
		X1=Pt11->X;
		Y1=Pt11->Y;
		Y2=Pt12->Y;

		// Delete edge1 and edge2
		work.DeletePtr(Pt11);
		work.DeletePtr(Pt12);				

		// Find the point at (X2,?) -> Fix X2
		Pt2Y=work.GetLeftBottom(X1,Y1,Y2);
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
			work.InsertPtrAt(new RPoint(tmp),i);

  	// If point to add -> after edge4
		if(bFind21)
			i=work.GetId<RPoint*>(PtX2,bFind)+1;

		// Test if (X2,Y2) exists -> If yes delete it else create it
		tmp.X=X2;
		tmp.Y=Y2;
		Test=work.GetPtr<RPoint>(tmp);
		if(Test)
			work.DeletePtr(Test);
		else
			work.InsertPtrAt(new RPoint(tmp),i);

		// Verify if not multiple polygons necessary
		Count=0;	// Counting nb edges on the same vertical
		for(i=work.NbPtr+1,point=work.Tab;--i;point++)
			if(((*point)->X==X2)&&((*point)->Y<=Y2)&&((*point)->Y>=Y1))
			{
				Test=work.GetConX(*point);
				if(Test->X>X2) Count++;
			}
		if(Count%2) continue;
		Count/=2;	// if count edges found -> count/2 polygon are necessary
		while(Count>1)	// if more than one polygon is necessary
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


//---------------------------------------------------------------------------
void RPolygon::AddPoints(RPoints *points)
{
	RPoint **point;
	unsigned int i;

	for(i=NbPtr+1,point=Tab;--i;point++)
		points->InsertPtr(new RPoint(*point));
}


//---------------------------------------------------------------------------
void RPolygon::ReOrder(void)
{
	RPoint **tmp,**point,*next;
	unsigned int i;
	bool bX; 		// Next Vertex is horizontal

	point=tmp=new RPoint*[MaxPtr];
	bX=false;
	next=GetBottomLeft();
	(*(point++))=next;
	next=GetConX(next);
	(*(point++))=next;
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


//---------------------------------------------------------------------------
bool RPolygon::DuplicatePoints(void)
{
	unsigned int i,j;
	RPoint **point1,**point2;

	for(i=0,point1=Tab;i<NbPtr-1;point1++,i++)
		for(j=i+1,point2=&Tab[i+1];j<NbPtr;point2++,j++)
			if((**point1)==(**point2))
				return(true);
	return(false);
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
bool RPolygons::Vertex(RPoint *pt)
{
  RPolygon **poly;
	unsigned int i;
	
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		if((*poly)->Vertex(pt))
			return(true);
	return(false);	
}


//---------------------------------------------------------------------------
bool RPolygons::Vertex(RPoint *pt,RPolygon *poly)
{
  RPolygon **ptr;
	unsigned int i;
	
	for(i=NbPtr+1,ptr=Tab;--i;ptr++)
		if(((*ptr)!=poly)&&((*ptr)->Vertex(pt)))
			return(true);
	return(false);	
}


//---------------------------------------------------------------------------
bool RPolygons::Vertex(RPoint *pt1,RPoint *pt2)
{
  RPolygon **poly;
	unsigned int i;
	
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		if((*poly)->Vertex(pt1,pt2))
			return(true);
	return(false);	
}


//---------------------------------------------------------------------------
void RPolygons::PutPoints(RPoints *points)
{
  RPolygon **poly;
	RPoint **point;
	RPoint tmp;
	unsigned int i,j;
	
	points->Clear();
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		(*poly)->AddPoints(points);	
	for(i=NbPtr+1,poly=Tab;--i;poly++)
		for(j=(*poly)->NbPtr+1,point=(*poly)->Tab;--j;point++)
  	{
  		tmp.X=(*point)->X;
  		tmp.Y=(*point)->Y-1;
  		if(!points->IsIn<RPoint>(tmp))
  			if(Vertex(&tmp,*poly))
  				points->InsertPtr(new RPoint(tmp));
  		tmp.Y=(*point)->Y+1;
  		if(!points->IsIn<RPoint>(tmp))
  			if(Vertex(&tmp,*poly))
  				points->InsertPtr(new RPoint(tmp));
  		tmp.X=(*point)->X+1;
  		tmp.Y=(*point)->Y;
  		if(!points->IsIn<RPoint>(tmp))
  			if(Vertex(&tmp,*poly))
  				points->InsertPtr(new RPoint(tmp));
  		tmp.X=(*point)->X-1;
  		if(!points->IsIn<RPoint>(tmp))
  			if(Vertex(&tmp,*poly))
  				points->InsertPtr(new RPoint(tmp));
  	}
}


//---------------------------------------------------------------------------
// Make the union of all polygons contained in RPolygons -> Result in poly
void RPolygons::Union(RPolygon *upoly)
{
	RPoint *next,*first,*ins,*last;
	int FromDir;		// 0=left ; 1=right ; 2=up ; 3=down
  RPoints pts(100);

	// Init Part
	upoly->Clear();	
	PutPoints(&pts);

	// Find the most (left,bottom) point -> curpt,curpoly -> next pt on the right
	last=first=next=pts.FindBottomLeft();
	ins=new RPoint(next);
	upoly->InsertPtr(ins);
	next=pts.FindRight(next,this);
	FromDir=0;

	// While nextpt!=firspt
	while((*next)!=(*first))
	{		
		ins=new RPoint(next);
		upoly->InsertPtr(ins);
		last=next;
//		pts.DeletePtr(next);

  	switch(FromDir)
		{

			case 0 : // Coming from left
   			if((next=pts.FindBottom(last,this)))
  			{
  				FromDir=2;
  				break;
  			}
  			if((next=pts.FindRight(last,this)))
  			{
  				upoly->DeletePtr(ins);
					break;
        }
				if((next=pts.FindUp(last,this)))
				{
					FromDir=3;
					break;
				}
				FromDir=-1;
  			break;

			case 1 : // Coming from right
				if((next=pts.FindUp(last,this)))
				{
					FromDir=3;
					break;
				}
  			if((next=pts.FindLeft(last,this)))
  			{
  				upoly->DeletePtr(ins);
					break;
        }
   			if((next=pts.FindBottom(last,this)))
  			{
  				FromDir=2;
  				break;
  			}
				FromDir=-1;
  			break;

			case 2 : // Coming from up
  			if((next=pts.FindLeft(last,this)))
  			{
					FromDir=1;
					break;
        }
				if((next=pts.FindBottom(last,this)))
				{
  				upoly->DeletePtr(ins);
					break;
				}
  			if((next=pts.FindRight(last,this)))
  			{
  				FromDir=0;
					break;
        }
				FromDir=-1;
  			break;

			case 3 : // Coming from bottom
  			if((next=pts.FindRight(last,this)))
  			{
					FromDir=0;
					break;
        }
				if((next=pts.FindUp(last,this)))
				{
  				upoly->DeletePtr(ins);
					break;
				}
  			if((next=pts.FindLeft(last,this)))
  			{
  				FromDir=1;
					break;
        }
				FromDir=-1;
  			break;

    }
  }
}


//---------------------------------------------------------------------------
bool RPolygons::DuplicatePoints(void)
{
	RPoints tmp(500);

	PutPoints(&tmp);
	return(tmp.DuplicatePoints());
}


//---------------------------------------------------------------------------
RPolygons& RPolygons::operator=(const RPolygons &poly)
{
	RContainer<RPolygon,unsigned int,true,false>::operator=(poly);
	return(*this);
}