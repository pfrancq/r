/*

  GraphStruct

  Graphical Structure

  By (c) P. Francq, 1999.

  Version 1.0.0.0

  Last Modify: 1999/09/24

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
#include "polygons.h"
using namespace RGeometry;



//---------------------------------------------------------------------------
//
// Class "RPolygon"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPolygon::RPolygon(void) : RContainer<RPoint,unsigned,true,false>(20,10)
{
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(int Max) : RContainer<RPoint,unsigned,true,false>(Max,10)
{
}


//---------------------------------------------------------------------------
RPolygon::RPolygon(RPolygon *poly) : RContainer<RPoint,unsigned,true,false>(poly->NbPtr,10)
{
  RPoint **ptr;
  int i;

  for(i=poly->NbPtr+1,ptr=poly->Tab;--i;ptr++)
    InsertPtr(new RPoint(*ptr));
}


//---------------------------------------------------------------------------
RPolygon& RPolygon::operator=(const RPolygon &poly)
{
  RPoint **ptr;
  int i;

  for(NbPtr++,ptr=Tab;--NbPtr;ptr++)
    delete (*ptr);
  for(i=poly.NbPtr+1,ptr=poly.Tab;--i;ptr++)
    InsertPtr(new RPoint(*ptr));
  return(*this);
}

//---------------------------------------------------------------------------
int RPolygon::Compare(RPolygon*)
{
	return(0);
}


//---------------------------------------------------------------------------
bool RPolygon::IsIn(RPoint *pt)
{
/*  RLine lt,lp;
ss  RPoint **ptr;
  int i;

  long count=0;
  lt.Pt1=(*pt);
  lt.Pt1.X=DBL_MAX;
  lt.Pt1.Y=pt->Y;
  return(count%1);*/
  return(true);
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
  int factx=1,facty=1,i,minx,miny,oldx,oldy;
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
    (*ptr)->X = (RCoord)(co*oldx - si*oldy);
    (*ptr)->Y= (RCoord)(si*oldx + co*oldy);
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
void RPolygon::Fill(bool **Occupied,RPoint Pt)
{
  RCoord i;
  bool **ptr;
  RPoint **pts,*pt1,*pt2;
  unsigned Nb=NbPtr;

  // Suppose the hole Region is Occupated
  for(i=Pt.X+1,ptr=Occupied;--i;ptr++)
    memset(*ptr,Pt.Y*sizeof(bool),1);

  // Substract the non Occupied Regions
  pts=Tab;
  pt1=*pts;
  while(--Nb)
  {
    pts++;
    pt2=(*pts);


    pt1=pt2;
  }
}



//---------------------------------------------------------------------------
//
// Class "RPolygons"
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RPolygons::RPolygons(void) : RContainer<RPolygon,unsigned,true,false>(20,10)
{
}


//---------------------------------------------------------------------------
void RPolygons::Union(RPolygon *poly)
{
}



//---------------------------------------------------------------------------

