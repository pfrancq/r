/*

	R Project Library

	RGrid.cpp

	Grid for the 2D placement - Implementation

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rtrace.h>
#include <rga.h>
#include <rgrid.h>
#include <rgeoinfo.h>
using namespace R;


//------------------------------------------------------------------------------
// defines
//#define DOUBLESPACE



//------------------------------------------------------------------------------
//
// class RGrid
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RGrid::RGrid(RPoint &limits)
	: Limits(limits), OccupiedX(0), OccupiedY(0)
{
	#ifdef DOUBLESPACE
		InternalLimits.Set(Limits.X*2,Limits.Y*2);
	#else
		InternalLimits=Limits;
	#endif

	// Init Occupied
	OccupiedX = new size_t*[GetMaxX()];
	for(size_t R=0;R<GetMaxX();R++)
		OccupiedX[R] = new size_t[GetMaxY()];
	OccupiedY = new size_t*[GetMaxY()];
	for(size_t R=0;R<GetMaxY();R++)
		OccupiedY[R] = new size_t[GetMaxX()];
}


//------------------------------------------------------------------------------
void RGrid::Clear(void)
{
	RReturnIfFail(OccupiedX);
	RReturnIfFail(OccupiedY);

	// OccupiedX
	for(size_t R=0;R<GetMaxX();R++)
		memset(OccupiedX[R],0xFF,sizeof(size_t)*(GetMaxY()));

	// OccupiedY
	for(size_t R=0;R<GetMaxY();R++)
		memset(OccupiedY[R],0xFF,sizeof(size_t)*(GetMaxX()));
}


//------------------------------------------------------------------------------
void RGrid::Assign(RRect& rect,RPoint& pos,size_t id)
{
	size_t BeginX,BeginY,j,k,H,W;
	size_t **ptr1,*ptr2;

	RReturnIfFail(OccupiedX);
	RReturnIfFail(OccupiedY);

	BeginX=static_cast<size_t>(rect.X1+pos.X);
	BeginY=static_cast<size_t>(rect.Y1+pos.Y);
	H=static_cast<size_t>(rect.GetHeight()+1);
	W=static_cast<size_t>(rect.GetWidth()+1);

	// OccupiedX
	for(j=W,ptr1=&OccupiedX[BeginX];--j;ptr1++)
		for(k=H,ptr2=&((*ptr1)[BeginY]);--k;ptr2++)
			(*ptr2)=id;

	// OccupiedY
	for(j=H,ptr1=&OccupiedY[BeginY];--j;ptr1++)
		for(k=W,ptr2=&((*ptr1)[BeginX]);--k;ptr2++)
			(*ptr2)=id;
}


//------------------------------------------------------------------------------
bool RGrid::IsFree(tCoord x,tCoord y)
{
	RReturnValIfFail(OccupiedX,false);
	if(x<0||x>(InternalLimits.X)||y<0||y>(InternalLimits.Y))
		return(true);
	return(OccupiedX[static_cast<size_t>(x)][static_cast<size_t>(y)]==cNoRef);
}


//------------------------------------------------------------------------------
bool RGrid::IsOcc(tCoord x,tCoord y)
{
	RReturnValIfFail(OccupiedX,false);
	if(x<0||x>(InternalLimits.X)||y<0||y>(InternalLimits.Y))
		return(false);
	return(OccupiedX[static_cast<size_t>(x)][static_cast<size_t>(y)]!=cNoRef);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookLeft(RPoint& pt)
{
	size_t* ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X)];
	tCoord x=pt.X;

	while(x&&((*ptr)==cNoRef))
	{
		ptr--;
		x--;
	}
	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookRight(RPoint& pt)
{
	size_t *ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X)];
	tCoord x=pt.X;

	while((x<InternalLimits.X)&&((*ptr)==cNoRef))
	{
		ptr++;
		x++;
	}
	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookUp(RPoint& pt)
{
	size_t *ptr=&OccupiedX[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X)];
	tCoord y=pt.Y;

	while((y<=InternalLimits.Y)&&((*ptr)==cNoRef))
	{
		ptr++;
		y++;
	}
	return(y);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookDown(RPoint& pt)
{
	size_t *ptr=&OccupiedX[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X)];
	tCoord y=pt.Y;

	while(y&&((*ptr)==cNoRef))
	{
		ptr--;
		y--;
	}
	return(y);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtLeft(RPoint& pt,RRect& bound)
{
	size_t *ptr,*ptrU,*ptrD;
	tCoord x=pt.X;
	tCoord Limit=bound.X1;

	if(pt.X>0) ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X-1)]; else return(x);
	if(pt.Y>0) ptrD=&OccupiedY[static_cast<size_t>(pt.Y-1)][static_cast<size_t>(pt.X-1)]; else ptrD=0;
	if(pt.Y<InternalLimits.Y) ptrU=&OccupiedY[static_cast<size_t>(pt.Y+1)][static_cast<size_t>(pt.X-1)]; else ptrU=0;

	// While next point is free and not a bifurcation, go to left
	while((x>=Limit)&&((*ptr)==cNoRef)&&!((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))))
	{
		ptr--;
		if(ptrU) ptrU--;
		if(ptrD) ptrD--;
		x--;
	}

	// If bifucation and next left point is free, go to it
	if((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))&&((x!=pt.X)&&(x>=Limit)&&((*(ptr))==cNoRef)))
		x--;

	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtRight(RPoint& pt,RRect& bound)
{
	size_t *ptr,*ptrU,*ptrD;
	tCoord x=pt.X;
	tCoord Limit=bound.X2;

	if(pt.X<InternalLimits.X) ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X+1)]; else return(x);
	if(pt.Y>0) ptrD=&OccupiedY[static_cast<size_t>(pt.Y-1)][static_cast<size_t>(pt.X+1)]; else ptrD=0;
	if(pt.Y<InternalLimits.Y) ptrU=&OccupiedY[static_cast<size_t>(pt.Y+1)][static_cast<size_t>(pt.X+1)]; else ptrU=0;

   // While next point is free and not a bifurcation, go to right
	while((x<=Limit)&&((*ptr)==cNoRef)&&!((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))))
	{
		ptr++;
		if(ptrU) ptrU++;
		if(ptrD) ptrD++;
		x++;
	}

	// If bifucation and next right point is free, go to it
	if((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))&&((x!=pt.X)&&(x<=Limit)&&((*(ptr))==cNoRef)))
		x++;
	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtUp(RPoint& pt,RRect& bound)
{
	size_t *ptr,*ptrL,*ptrR;
	tCoord y=pt.Y;
	tCoord Limit=bound.Y2;

	if(pt.Y<InternalLimits.Y) ptr=&OccupiedX[static_cast<size_t>(pt.X)][static_cast<size_t>(pt.Y+1)]; else return(y);
	if(pt.X>0) ptrL=&OccupiedX[static_cast<size_t>(pt.X-1)][static_cast<size_t>(pt.Y+1)]; else ptrL=0;
	if(pt.X<InternalLimits.X) ptrR=&OccupiedX[static_cast<size_t>(pt.X+1)][static_cast<size_t>(pt.Y+1)]; else ptrR=0;

	// While next point is free and not a bifurcation, go to up
	while((y<=Limit)&&((*ptr)==cNoRef)&&!((ptrR&&((*ptrR)==cNoRef))&&(ptrL&&((*ptrL)==cNoRef))))
	{
		ptr++;
		if(ptrL) ptrL++;
		if(ptrR) ptrR++;
		y++;
	}

	// If bifucation and next up point is free, go to it
	if((ptrL&&((*ptrL)==cNoRef))&&(ptrR&&((*ptrR)==cNoRef))&&((y!=pt.Y)&&(y<=Limit)&&((*(ptr))==cNoRef)))
		y++;

	return(y);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtDown(RPoint& pt,RRect& bound)
{
	size_t *ptr,*ptrL,*ptrR;
	tCoord y=pt.Y;
	tCoord Limit=bound.Y1;

	if(pt.Y>0) ptr=&OccupiedX[static_cast<size_t>(pt.X)][static_cast<size_t>(pt.Y-1)]; else return(y);
	if(pt.X>0) ptrL=&OccupiedX[static_cast<size_t>(pt.X-1)][static_cast<size_t>(pt.Y-1)]; else ptrL=0;
	if(pt.X<InternalLimits.X) ptrR=&OccupiedX[static_cast<size_t>(pt.X+1)][static_cast<size_t>(pt.Y-1)]; else ptrR=0;

	// While next point is free and not a bifurcation, go to down
	while((y>=Limit)&&((*ptr)==cNoRef)&&!((ptrR&&((*ptrR)==cNoRef))&&(ptrL&&((*ptrL)==cNoRef))))
	{
		ptr--;
		if(ptrL) ptrL--;
		if(ptrR) ptrR--;
		y--;
	}

	// If bifucation and next down point is free, go to it
	if((ptrL&&((*ptrL)==cNoRef))&&(ptrR&&((*ptrR)==cNoRef))&&((y!=pt.Y)&&(y>=Limit)&&((*(ptr))==cNoRef)))
		y--;

	return(y);
}


//------------------------------------------------------------------------------
bool RGrid::CalculateFreePolygon(tCoord X,tCoord Y,RDirection from,RRect& bound,RPolygon& poly)
{
	RPoint *next,*first,pt;
	tCoord TestX,TestY;

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
	if( ((bound.X1>0)&&(X==bound.X1)) ||
	    (X==bound.X2)                    ||
	    ((bound.Y1>0)&&(Y==bound.Y1)) ||
	    (Y==bound.Y2)||((X==pt.X)&&(Y==pt.Y))  )
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
				if((TestY!=Y)&&((bound.Y1==0)||((bound.Y1>0)&&(TestY>bound.Y1))))
				{
					Y=TestY;
					from=Up;
					break;
				}
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.X2))
				{
					X=TestX;
					from=Left;
					break;
				}
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.Y2))
				{
					Y=TestY;
					from=Down;
					break;
				}
				return(false);
				break;

			case Right:
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.Y2))
				{
					Y=TestY;
					from=Down;
					break;
				}
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.X1==0)||((bound.X1>0)&&(TestX>bound.X1))))
				{
					X=TestX;
					from=Right;
					break;
				}
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.Y1==0)||((bound.Y1>0)&&(TestY>bound.Y1))))
				{
					Y=TestY;
					from=Up;
					break;
				}
				return(false);
				break;

			case Down:
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.X2))
				{
					X=TestX;
					from=Left;
					break;
				}
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.Y2))
				{
					Y=TestY;
					from=Down;
					break;
				}
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.X1==0)||((bound.X1>0)&&(TestX>bound.X1))))
				{
					X=TestX;
					from=Right;
					break;
				}
				return(false);
				break;

			case Up:
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.X1==0)||((bound.X1>0)&&(TestX>bound.X1))))
				{
					X=TestX;
					from=Right;
					break;
				}
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.Y1==0)||((bound.Y1>0)&&(TestY>bound.Y1))))
				{
					Y=TestY;
					from=Up;
					break;
				}
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.X2))
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
	next=poly[poly.GetNb()-1];
	if((first->X==next->X)&&(first->X==poly[1]->X))
	{
		if(poly.GetConY(next).X<first->X)
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
	if((first->Y==next->Y)&&(first->Y==poly[1]->Y))
	{
		if(poly.GetConX(next).Y<first->Y)
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


//------------------------------------------------------------------------------
void RGrid::AddFreePolygons(RGeoInfo* ins,RFreePolygons* free,RRect& bound)
{
	RPolygon Poly;          // Polygon representing the geometric information
	RPolygons NewOne;       // Polygons added now
	RPolygon New;           // New Polygon
	RPoint start,end;
	size_t nbpts;
	RDirection FromDir;
	tCoord X,Y;
	tCoord TestX,TestY;

	// Initialisation
	Poly=ins->GetPolygon();
	start=Poly.GetBottomLeft();
	end=Poly.GetConX(&start);
	FromDir=Left;
	X=start.X;
	Y=start.Y;
	nbpts=Poly.GetNb();

	// Go through the vertices
	while(nbpts)
	{
		TestX=X;
		TestY=Y;
		AdaptTestXY(TestX,TestY,FromDir);
		if(bound.IsIn(TestX,TestY)&&IsFree(TestX,TestY)&&(!NewOne.IsIn(TestX,TestY)))
		{
			if((free->GetNb()==1)&&(TestX==368)&&(TestY==183))
			{
				RTrace *m=RTrace::LookMsg("Debug");
				if(m)
				{
					RTrace::DeleteMsg(m);
				}
			}

			// Calculate Polygon
			if(CalculateFreePolygon(TestX,TestY,FromDir,bound,New))
			{
				New.ReOrder();    // The points must order anti-clockwise.
				New.ReValid();    // The vertex can't be close.
				if(New.GetNb())
				{
					NewOne.InsertPtr(new RPolygon(New));
					free->InsertPtr(new RFreePolygon(New));
				}
			}
		}

		// If end of an edge
		if((X==end.X)&&(Y==end.Y))
		{
			start=end;
			nbpts--;          // Next vertice
			TestX=X=start.X;
			TestY=Y=start.Y;
			if((FromDir==Left)||(FromDir==Right))    // Go to up/bottom
			{
				end=Poly.GetConY(&start);
				if(start.Y<end.Y) FromDir=Down; else FromDir=Up;
			}
			else      // Go to left/right
			{
				end=Poly.GetConX(&start);
				if(start.X<end.X) FromDir=Left; else FromDir=Right;
			}
		}
		else
			AdaptXY(X,Y,FromDir);
	}
}


//------------------------------------------------------------------------------
RGrid::~RGrid(void)
{
	if(OccupiedX)
	{
		for(size_t R=0;R<GetMaxX();R++)
			delete[] (OccupiedX[R]);
		delete[] OccupiedX;
	}
	if(OccupiedY)
	{
		for(size_t R=0;R<GetMaxY();R++)
			delete[] (OccupiedY[R]);
		delete[] OccupiedY;
	}
}
