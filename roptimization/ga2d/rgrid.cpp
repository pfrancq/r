/*

	R Project Library

	RGrid.cpp

	Grid for the 2D placement - Implementation

	(C) 1998-2001 by By P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rmsg.h>
using namespace RStd;
#include <rga2d/rgrid.h>
#include <rga2d/rgeoinfo.h>
using namespace RGA2D;


//-----------------------------------------------------------------------------
// defines
//#define DOUBLESPACE



//-----------------------------------------------------------------------------
//
// RGrid
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RGA2D::RGrid::RGrid(RPoint &limits) throw(bad_alloc)
	: Limits(limits), OccupiedX(0), OccupiedY(0)
{
	#ifdef DOUBLESPACE
		InternalLimits.Set(Limits.X*2,Limits.Y*2);
	#else
		InternalLimits=Limits;
	#endif
	// Init Occupied
	OccupiedX = new unsigned int*[InternalLimits.X+1];
	for(RCoord R=0;R<InternalLimits.X+1;R++)
		OccupiedX[R] = new unsigned int[InternalLimits.Y+1];
	OccupiedY = new unsigned int*[InternalLimits.Y+1];
	for(RCoord R=0;R<InternalLimits.Y+1;R++)
		OccupiedY[R] = new unsigned int[InternalLimits.X+1];
}


//-----------------------------------------------------------------------------
void RGA2D::RGrid::Clear(void)
{
	RReturnIfFail(OccupiedX);
	RReturnIfFail(OccupiedY);

	// OccupiedX
	for(RCoord R=0;R<InternalLimits.X+1;R++)
		memset(OccupiedX[R],0xFF,sizeof(unsigned int)*(InternalLimits.Y));

	// OccupiedY
	for(RCoord R=0;R<InternalLimits.Y+1;R++)
		memset(OccupiedY[R],0xFF,sizeof(unsigned int)*(InternalLimits.X));
}


//-----------------------------------------------------------------------------
void RGA2D::RGrid::Assign(RRect &rect,RPoint &pos,unsigned int id)
{
	RCoord BeginX,BeginY,j,k,H,W;
	unsigned int **ptr1,*ptr2;

	RReturnIfFail(OccupiedX);
	RReturnIfFail(OccupiedY);

	BeginX=rect.Pt1.X+pos.X;
	BeginY=rect.Pt1.Y+pos.Y;
	H=rect.Height()+1;
	W=rect.Width()+1;

	// OccupiedX
	for(j=W,ptr1=&OccupiedX[BeginX];--j;ptr1++)
		for(k=H,ptr2=&((*ptr1)[BeginY]);--k;ptr2++)
			(*ptr2)=id;

	// OccupiedY
	for(j=H,ptr1=&OccupiedY[BeginY];--j;ptr1++)
		for(k=W,ptr2=&((*ptr1)[BeginX]);--k;ptr2++)
			(*ptr2)=id;
}


//-----------------------------------------------------------------------------
bool RGA2D::RGrid::IsFree(RCoord x,RCoord y)
{
	RReturnValIfFail(OccupiedX,false);
	if(x<0||x>(InternalLimits.X)||y<0||y>(InternalLimits.Y))
		return(true);
	return(OccupiedX[x][y]==NoObject);
}


//-----------------------------------------------------------------------------
bool RGA2D::RGrid::IsOcc(RCoord x,RCoord y)
{
	RReturnValIfFail(OccupiedX,false);
	if(x<0||x>(InternalLimits.X)||y<0||y>(InternalLimits.Y))
		return(false);
	return(OccupiedX[x][y]!=NoObject);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::LookLeft(RPoint& pt)
{
	unsigned int *ptr=&OccupiedY[pt.Y][pt.X];
	RCoord x=pt.X;

	while(x&&((*ptr)==NoObject))
	{
		ptr--;
		x--;	
	}
	return(x);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::LookRight(RPoint& pt)
{
	unsigned int *ptr=&OccupiedY[pt.Y][pt.X];
	RCoord x=pt.X;

	while((x<InternalLimits.X)&&((*ptr)==NoObject))
	{
		ptr++;
		x++;	
	}
	return(x);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::LookUp(RPoint& pt)
{
	unsigned int *ptr=&OccupiedX[pt.X][pt.Y];
	RCoord y=pt.Y;

	while((y<=InternalLimits.Y)&&((*ptr)==NoObject))
	{
		ptr++;
		y++;	
	}
	return(y);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::LookDown(RPoint& pt)
{
	unsigned int *ptr=&OccupiedX[pt.X][pt.Y];
	RCoord y=pt.Y;

	while(y&&((*ptr)==NoObject))
	{
		ptr--;
		y--;	
	}
	return(y);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::SkirtLeft(RPoint& pt,RRect &bound)
{
	unsigned int *ptr,*ptrU,*ptrD;
	RCoord x=pt.X;
	RCoord Limit=bound.Pt1.X;	

	if(pt.X>0) ptr=&OccupiedY[pt.Y][pt.X-1]; else return(x);
	if(pt.Y>0) ptrD=&OccupiedY[pt.Y-1][pt.X-1]; else ptrD=0;
	if(pt.Y<InternalLimits.Y) ptrU=&OccupiedY[pt.Y+1][pt.X-1]; else ptrU=0;

	// While next point is free and not a bifurcation, go to left
	while((x>=Limit)&&((*ptr)==NoObject)&&!((ptrU&&((*ptrU)==NoObject))&&(ptrD&&((*ptrD)==NoObject))))
	{
		ptr--;
		if(ptrU) ptrU--;
		if(ptrD) ptrD--;
		x--;	
	}

	// If bifucation and next left point is free, go to it
	if((ptrU&&((*ptrU)==NoObject))&&(ptrD&&((*ptrD)==NoObject))&&((x!=pt.X)&&(x>=Limit)&&((*(ptr))==NoObject)))
		x--;

	return(x);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::SkirtRight(RPoint& pt,RRect &bound)
{
	unsigned int *ptr,*ptrU,*ptrD;
	RCoord x=pt.X;
	RCoord Limit=bound.Pt2.X;

	if(pt.X<InternalLimits.X) ptr=&OccupiedY[pt.Y][pt.X+1]; else return(x);
	if(pt.Y>0) ptrD=&OccupiedY[pt.Y-1][pt.X+1]; else ptrD=0;
	if(pt.Y<InternalLimits.Y) ptrU=&OccupiedY[pt.Y+1][pt.X+1]; else ptrU=0;

   // While next point is free and not a bifurcation, go to right
	while((x<=Limit)&&((*ptr)==NoObject)&&!((ptrU&&((*ptrU)==NoObject))&&(ptrD&&((*ptrD)==NoObject))))
	{
		ptr++;
		if(ptrU) ptrU++;
		if(ptrD) ptrD++;
		x++;	
	}

	// If bifucation and next right point is free, go to it
	if((ptrU&&((*ptrU)==NoObject))&&(ptrD&&((*ptrD)==NoObject))&&((x!=pt.X)&&(x<=Limit)&&((*(ptr))==NoObject)))
		x++;
	return(x);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::SkirtUp(RPoint& pt,RRect &bound)
{
	unsigned int *ptr,*ptrL,*ptrR;
	RCoord y=pt.Y;
	RCoord Limit=bound.Pt2.Y;

	if(pt.Y<InternalLimits.Y) ptr=&OccupiedX[pt.X][pt.Y+1]; else return(y);
	if(pt.X>0) ptrL=&OccupiedX[pt.X-1][pt.Y+1]; else ptrL=0;
	if(pt.X<InternalLimits.X) ptrR=&OccupiedX[pt.X+1][pt.Y+1]; else ptrR=0;

	// While next point is free and not a bifurcation, go to up
	while((y<=Limit)&&((*ptr)==NoObject)&&!((ptrR&&((*ptrR)==NoObject))&&(ptrL&&((*ptrL)==NoObject))))
	{
		ptr++;
		if(ptrL) ptrL++;
		if(ptrR) ptrR++;
		y++;
	}

	// If bifucation and next up point is free, go to it
	if((ptrL&&((*ptrL)==NoObject))&&(ptrR&&((*ptrR)==NoObject))&&((y!=pt.Y)&&(y<=Limit)&&((*(ptr))==NoObject)))
		y++;

	return(y);
}


//-----------------------------------------------------------------------------
RCoord RGA2D::RGrid::SkirtDown(RPoint& pt,RRect &bound)
{
	unsigned int *ptr,*ptrL,*ptrR;
	RCoord y=pt.Y;
	RCoord Limit=bound.Pt1.Y;

	if(pt.Y>0) ptr=&OccupiedX[pt.X][pt.Y-1]; else return(y);
	if(pt.X>0) ptrL=&OccupiedX[pt.X-1][pt.Y-1]; else ptrL=0;
	if(pt.X<InternalLimits.X) ptrR=&OccupiedX[pt.X+1][pt.Y-1]; else ptrR=0;

	// While next point is free and not a bifurcation, go to down
	while((y>=Limit)&&((*ptr)==NoObject)&&!((ptrR&&((*ptrR)==NoObject))&&(ptrL&&((*ptrL)==NoObject))))
	{
		ptr--;
		if(ptrL) ptrL--;
		if(ptrR) ptrR--;
		y--;	
	}

	// If bifucation and next down point is free, go to it
	if((ptrL&&((*ptrL)==NoObject))&&(ptrR&&((*ptrR)==NoObject))&&((y!=pt.Y)&&(y>=Limit)&&((*(ptr))==NoObject)))
		y--;

	return(y);
}


//-----------------------------------------------------------------------------
bool RGA2D::RGrid::CalculateFreePolygon(RCoord X,RCoord Y,RDirection from,RRect &bound,RPolygon& poly)
{
	RPoint *next,*first,pt;
	RCoord TestX,TestY;

	// Init Part
	poly.Clear();	
	poly.InsertPtr(first=new RPoint(X,Y));
	pt.Set(X,Y);

	// Find the next vertice of the polygon
	switch(from)
	{
		case Left:
			Y=SkirtDown(pt,bound);
			from=Up;
			break;
		case Right:
			Y=SkirtUp(pt,bound);
			from=Down;
			break;
		case Down:
			X=SkirtRight(pt,bound);
			from=Left;
			break;
		case Up:
			X=SkirtLeft(pt,bound);
			from=Right;
			break;
		case NoDirection:
			RAssertMsg("Direction can't be undefined");
			break;
		default:
			RAssertMsg("Not a valid Direction in this context");
			break;
	}

	// Test if Valid one and insert it
	if( ((bound.Pt1.X>0)&&(X==bound.Pt1.X)) ||
	    (X==bound.Pt2.X)                    ||
	    ((bound.Pt1.Y>0)&&(Y==bound.Pt1.Y)) ||
	    (Y==bound.Pt2.Y)||((X==pt.X)&&(Y==pt.Y))  )
		return(false);

	// Find next Vertices
	while((first->X!=X)||(first->Y!=Y))
	{
		poly.InsertPtr(next=new RPoint(X,Y));
		pt.Set(X,Y);
		switch(from)
		{
			case Left:
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.Pt1.Y==0)||((bound.Pt1.Y>0)&&(TestY>bound.Pt1.Y))))
				{
					Y=TestY;
					from=Up;
					break;
				}
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.Pt2.X))
				{
					X=TestX;
					from=Left;
					break;
				}
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.Pt2.Y))
				{
					Y=TestY;
					from=Down;
					break;
				}
				return(false);
				break;

			case Right:
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.Pt2.Y))
				{
					Y=TestY;
					from=Down;
					break;
				}
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.Pt1.X==0)||((bound.Pt1.X>0)&&(TestX>bound.Pt1.X))))
				{
					X=TestX;
					from=Right;
					break;
				}
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.Pt1.Y==0)||((bound.Pt1.Y>0)&&(TestY>bound.Pt1.Y))))
				{
					Y=TestY;
					from=Up;
					break;
				}
				return(false);
				break;

			case Down:
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.Pt2.X))
				{
					X=TestX;
					from=Left;
					break;
				}
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.Pt2.Y))
				{
					Y=TestY;
					from=Down;
					break;
				}
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.Pt1.X==0)||((bound.Pt1.X>0)&&(TestX>bound.Pt1.X))))
				{
					X=TestX;
					from=Right;
					break;
				}
				return(false);
				break;

			case Up:
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.Pt1.X==0)||((bound.Pt1.X>0)&&(TestX>bound.Pt1.X))))
				{
					X=TestX;
					from=Right;
					break;
				}
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.Pt1.Y==0)||((bound.Pt1.Y>0)&&(TestY>bound.Pt1.Y))))
				{
					Y=TestY;
					from=Up;
					break;
				}
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.Pt2.X))
				{
					X=TestX;
					from=Left;
					break;
				}
				return(false);
				break;

			case NoDirection:
				RAssertMsg("Direction can't be undefined");
				break;

			default:
				RAssertMsg("Not a valid Direction in this context");
				break;
		}
	}

	// Verify that the not same X or not same Y
	next=poly.Tab[poly.NbPtr-1];
	if((first->X==next->X)&&(first->X==poly.Tab[1]->X))
	{
		if(poly.GetConY(next)->X<first->X)
		{
			poly.InsertPtr(new RPoint(next->X-1,next->Y));
			poly.InsertPtr(new RPoint(next->X-1,first->Y));
		}
		else
		{
			poly.InsertPtr(new RPoint(next->X+1,next->Y));
			poly.InsertPtr(new RPoint(next->X+1,first->Y));
		}
		poly.DeletePtr(next);
	}
	if((first->Y==next->Y)&&(first->Y==poly.Tab[1]->Y))
	{
		if(poly.GetConX(next)->Y<first->Y)
		{
			poly.InsertPtr(new RPoint(next->X,next->Y-1));
			poly.InsertPtr(new RPoint(first->X-1,next->Y-1));
		}
		else
		{
			poly.InsertPtr(new RPoint(next->X,next->Y+1));
			poly.InsertPtr(new RPoint(first->X,next->Y+1));
		}
		poly.DeletePtr(next);
	}

	// Ok
	return(true);
}


//-----------------------------------------------------------------------------
void RGA2D::RGrid::AddFreePolygons(RGeoInfo *ins,RFreePolygons *free,RRect &bound)
{
	RPolygon Poly;          // Polygon representing the geometric information
	RPolygons NewOne;       // Polygons added now
	RPolygon New;           // New Polygon
	RPoint *start,*end;
	unsigned int nbpts;
	RDirection FromDir;
	RCoord X,Y;
	RCoord TestX,TestY;

	// Initialisation
	Poly=ins->GetPolygon();
	start=Poly.GetBottomLeft();
	end=Poly.GetConX(start);
	FromDir=Left;
	X=start->X;
	Y=start->Y;
	nbpts=Poly.NbPtr;

	// Go through the vertices
	while(nbpts)
	{
		TestX=X;
		TestY=Y;
		AdaptTestXY(TestX,TestY,FromDir);
		if(bound.IsIn(TestX,TestY)&&IsFree(TestX,TestY)&&(!NewOne.IsIn(TestX,TestY)))
		{
			if((free->NbPtr==1)&&(TestX==368)&&(TestY==183))
			{
				RMsg *m=LookMsg("Debug");
				if(m)
				{
					DeleteMsg(m);
				}
			}

			// Calculate Polygon
			if(CalculateFreePolygon(TestX,TestY,FromDir,bound,New))
			{
				New.ReOrder();    // The points must order anti-clockwise.
				New.ReValid();    // The vertex can't be close.
				if(New.NbPtr)
				{
					NewOne.InsertPtr(new RPolygon(New));
					free->InsertPtr(new RFreePolygon(New));
				}
			}
		}

		// If end of an edge
		if((X==end->X)&&(Y==end->Y))
		{
			start=end;
			nbpts--;          // Next vertice
			TestX=X=start->X;
			TestY=Y=start->Y;
			if((FromDir==Left)||(FromDir==Right))    // Go to up/bottom
			{
				end=Poly.GetConY(start);
				if(start->Y<end->Y) FromDir=Down; else FromDir=Up;
			}
			else      // Go to left/right
			{
				end=Poly.GetConX(start);
				if(start->X<end->X) FromDir=Left; else FromDir=Right;
			}
		}
		else
			AdaptXY(X,Y,FromDir);
	}
}


//-----------------------------------------------------------------------------
RGA2D::RGrid::~RGrid(void)
{
	if(OccupiedX)
	{
		for(RCoord R=0;R<InternalLimits.X+1;R++)
			delete[] (OccupiedX[R]);
		delete[] OccupiedX;
	}
	if(OccupiedY)
	{
		for(RCoord R=0;R<InternalLimits.Y+1;R++)
			delete[] (OccupiedY[R]);
		delete[] OccupiedY;
	}
}
