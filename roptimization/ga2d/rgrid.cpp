/*

	R Project Library

	RGrid.cpp

	Grid for the 2D placement - Implementation

	Copyright 1998-2012 by Pascal Francq (pascal@francq.info).
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
RGrid::RGrid(const RSize &limits)
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
	RReturnIfFail(OccupiedX);
	RReturnIfFail(OccupiedY);

	size_t BeginX(static_cast<size_t>(rect.GetX1()+pos.X));
	if(BeginX>=InternalLimits.GetWidth())
		ThrowRException("BeginX ("+RString::Number(BeginX)+")>="+RString::Number(InternalLimits.GetWidth()));
	size_t BeginY(static_cast<size_t>(rect.GetY1()+pos.Y));
	if(BeginY>=InternalLimits.GetHeight())
		ThrowRException("BeginY ("+RString::Number(BeginY)+")>="+RString::Number(InternalLimits.GetHeight()));
	size_t H(static_cast<size_t>(rect.GetHeight()+1));
	if(H>=InternalLimits.GetHeight())
		ThrowRException("H ("+RString::Number(H)+")>="+RString::Number(InternalLimits.GetHeight()));
	size_t W(static_cast<size_t>(rect.GetWidth()+1));
	if(W>=InternalLimits.GetWidth())
		ThrowRException("W ("+RString::Number(W)+")>="+RString::Number(InternalLimits.GetWidth()));

	// OccupiedX
	size_t **ptr1,*ptr2;
	size_t j,k;
	for(j=W,ptr1=&OccupiedX[BeginX];--j;ptr1++)
		for(k=H,ptr2=&((*ptr1)[BeginY]);--k;ptr2++)
			(*ptr2)=id;

	// OccupiedY
	for(j=H,ptr1=&OccupiedY[BeginY];--j;ptr1++)
		for(k=W,ptr2=&((*ptr1)[BeginX]);--k;ptr2++)
			(*ptr2)=id;
}


//------------------------------------------------------------------------------
bool RGrid::IsFree(tCoord x,tCoord y) const
{
	RReturnValIfFail(OccupiedX,false);
	if(x<0||x>(InternalLimits.GetWidth())||y<0||y>(InternalLimits.GetHeight()))
		return(true);
	return(OccupiedX[static_cast<size_t>(x)][static_cast<size_t>(y)]==cNoRef);
}


//------------------------------------------------------------------------------
bool RGrid::IsOcc(tCoord x,tCoord y) const
{
	RReturnValIfFail(OccupiedX,false);
	if(x<0||x>(InternalLimits.GetWidth())||y<0||y>(InternalLimits.GetHeight()))
		return(false);
	return(OccupiedX[static_cast<size_t>(x)][static_cast<size_t>(y)]!=cNoRef);
}


//------------------------------------------------------------------------------
bool RGrid::IsOcc(const RRect& rect) const
{
	if(rect.GetX1()<0||rect.GetX2()>(InternalLimits.GetWidth())||rect.GetY1()<0||rect.GetY2()>(InternalLimits.GetHeight()))
		return(false);
	size_t X1(static_cast<size_t>(rect.GetX1()));
	size_t Y1(static_cast<size_t>(rect.GetY1()));
	size_t X2(static_cast<size_t>(rect.GetX2()));
	size_t Y2(static_cast<size_t>(rect.GetY2()));

	for(size_t x=X1;x<=X2;x++)
	{
		for(size_t y=Y1;y<=Y2;y++)
			if(OccupiedX[x][y]!=cNoRef)
				return(true);
	}
	return(false);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookLeft(const RPoint& pt) const
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
tCoord RGrid::LookRight(const RPoint& pt) const
{
	size_t *ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X)];
	tCoord x=pt.X;

	while((x<InternalLimits.GetWidth())&&((*ptr)==cNoRef))
	{
		ptr++;
		x++;
	}
	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookUp(const RPoint& pt) const
{
	size_t *ptr=&OccupiedX[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X)];
	tCoord y=pt.Y;

	while((y<=InternalLimits.GetHeight())&&((*ptr)==cNoRef))
	{
		ptr++;
		y++;
	}
	return(y);
}


//------------------------------------------------------------------------------
tCoord RGrid::LookDown(const RPoint& pt) const
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
tCoord RGrid::SkirtLeft(const RPoint& pt,const RRect& bound) const
{
	size_t *ptr,*ptrU,*ptrD;
	tCoord x=pt.X;
	tCoord Limit=bound.GetX1();

	if(pt.X>0) ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X-1)]; else return(x);
	if(pt.Y>0) ptrD=&OccupiedY[static_cast<size_t>(pt.Y-1)][static_cast<size_t>(pt.X-1)]; else ptrD=0;
	if(pt.Y<InternalLimits.GetHeight()) ptrU=&OccupiedY[static_cast<size_t>(pt.Y+1)][static_cast<size_t>(pt.X-1)]; else ptrU=0;

	// While next point is free and not a bifurcation, go to left
	while((x>=Limit)&&((*ptr)==cNoRef)&&!((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))))
	{
		ptr--;
		if(ptrU) ptrU--;
		if(ptrD) ptrD--;
		x--;
	}

	// If bifurcation and next left point is free, go to it
	if((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))&&((x!=pt.X)&&(x>=Limit)&&((*(ptr))==cNoRef)))
		x--;

	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtRight(const RPoint& pt,const RRect& bound) const
{
	size_t *ptr,*ptrU,*ptrD;
	tCoord x=pt.X;
	tCoord Limit=bound.GetX2();

	if(pt.X<InternalLimits.GetWidth()) ptr=&OccupiedY[static_cast<size_t>(pt.Y)][static_cast<size_t>(pt.X+1)]; else return(x);
	if(pt.Y>0) ptrD=&OccupiedY[static_cast<size_t>(pt.Y-1)][static_cast<size_t>(pt.X+1)]; else ptrD=0;
	if(pt.Y<InternalLimits.GetHeight()) ptrU=&OccupiedY[static_cast<size_t>(pt.Y+1)][static_cast<size_t>(pt.X+1)]; else ptrU=0;

   // While next point is free and not a bifurcation, go to right
	while((x<=Limit)&&((*ptr)==cNoRef)&&!((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))))
	{
		ptr++;
		if(ptrU) ptrU++;
		if(ptrD) ptrD++;
		x++;
	}

	// If bifurcation and next right point is free, go to it
	if((ptrU&&((*ptrU)==cNoRef))&&(ptrD&&((*ptrD)==cNoRef))&&((x!=pt.X)&&(x<=Limit)&&((*(ptr))==cNoRef)))
		x++;
	return(x);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtUp(const RPoint& pt,const RRect& bound) const
{
	size_t *ptr,*ptrL,*ptrR;
	tCoord y=pt.Y;
	tCoord Limit=bound.GetY2();

	if(pt.Y<InternalLimits.GetHeight()) ptr=&OccupiedX[static_cast<size_t>(pt.X)][static_cast<size_t>(pt.Y+1)]; else return(y);
	if(pt.X>0) ptrL=&OccupiedX[static_cast<size_t>(pt.X-1)][static_cast<size_t>(pt.Y+1)]; else ptrL=0;
	if(pt.X<InternalLimits.GetWidth()) ptrR=&OccupiedX[static_cast<size_t>(pt.X+1)][static_cast<size_t>(pt.Y+1)]; else ptrR=0;

	// While next point is free and not a bifurcation, go to up
	while((y<=Limit)&&((*ptr)==cNoRef)&&!((ptrR&&((*ptrR)==cNoRef))&&(ptrL&&((*ptrL)==cNoRef))))
	{
		ptr++;
		if(ptrL) ptrL++;
		if(ptrR) ptrR++;
		y++;
	}

	// If bifurcation and next up point is free, go to it
	if((ptrL&&((*ptrL)==cNoRef))&&(ptrR&&((*ptrR)==cNoRef))&&((y!=pt.Y)&&(y<=Limit)&&((*(ptr))==cNoRef)))
		y++;

	return(y);
}


//------------------------------------------------------------------------------
tCoord RGrid::SkirtDown(const RPoint& pt,const RRect& bound) const
{
	size_t *ptr,*ptrL,*ptrR;
	tCoord y=pt.Y;
	tCoord Limit=bound.GetY1();

	if(pt.Y>0) ptr=&OccupiedX[static_cast<size_t>(pt.X)][static_cast<size_t>(pt.Y-1)]; else return(y);
	if(pt.X>0) ptrL=&OccupiedX[static_cast<size_t>(pt.X-1)][static_cast<size_t>(pt.Y-1)]; else ptrL=0;
	if(pt.X<InternalLimits.GetWidth()) ptrR=&OccupiedX[static_cast<size_t>(pt.X+1)][static_cast<size_t>(pt.Y-1)]; else ptrR=0;

	// While next point is free and not a bifurcation, go to down
	while((y>=Limit)&&((*ptr)==cNoRef)&&!((ptrR&&((*ptrR)==cNoRef))&&(ptrL&&((*ptrL)==cNoRef))))
	{
		ptr--;
		if(ptrL) ptrL--;
		if(ptrR) ptrR--;
		y--;
	}

	// If bifurcation and next down point is free, go to it
	if((ptrL&&((*ptrL)==cNoRef))&&(ptrR&&((*ptrR)==cNoRef))&&((y!=pt.Y)&&(y>=Limit)&&((*(ptr))==cNoRef)))
		y--;

	return(y);
}


//------------------------------------------------------------------------------
bool RGrid::CalculateFreePolygon(tCoord X,tCoord Y,tDirection from,const RRect& bound,RPolygon& poly) const
{
	RPoint *next,*first,pt;
	tCoord TestX,TestY;

	// Init Part
	poly.Clear();
	poly.InsertVertex(first=new RPoint(X,Y));
	pt.Set(X,Y);

	// Find the next vertex of the polygon
	switch(from)
	{
		case dLeft:
			Y=SkirtDown(pt,bound);
			from=dUp;
			break;
		case dRight:
			Y=SkirtUp(pt,bound);
			from=dDown;
			break;
		case dDown:
			X=SkirtRight(pt,bound);
			from=dLeft;
			break;
		case dUp:
			X=SkirtLeft(pt,bound);
			from=dRight;
			break;
		case dNoDirection:
			RAssertMsg("Direction can't be undefined");
			break;
		default:
			RAssertMsg("Not a valid Direction in this context");
			break;
	}

	// Test if Valid one and insert it
	if( ((bound.GetX1()>0)&&(X==bound.GetX1())) ||
	    (X==bound.GetX2())                    ||
	    ((bound.GetY1()>0)&&(Y==bound.GetY1())) ||
	    (Y==bound.GetY2())||((X==pt.X)&&(Y==pt.Y))  )
		return(false);

	// Find next Vertices
	while((first->X!=X)||(first->Y!=Y))
	{
		poly.InsertVertex(next=new RPoint(X,Y));
		pt.Set(X,Y);
		switch(from)
		{
			case dLeft:
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.GetY1()==0)||((bound.GetY1()>0)&&(TestY>bound.GetY1()))))
				{
					Y=TestY;
					from=dUp;
					break;
				}
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.GetX2()))
				{
					X=TestX;
					from=dLeft;
					break;
				}
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.GetY2()))
				{
					Y=TestY;
					from=dDown;
					break;
				}
				return(false);
				break;

			case dRight:
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.GetY2()))
				{
					Y=TestY;
					from=dDown;
					break;
				}
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.GetX1()==0)||((bound.GetX1()>0)&&(TestX>bound.GetX1()))))
				{
					X=TestX;
					from=dRight;
					break;
				}
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.GetY1()==0)||((bound.GetY1()>0)&&(TestY>bound.GetY1()))))
				{
					Y=TestY;
					from=dUp;
					break;
				}
				return(false);
				break;

			case dDown:
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.GetX2()))
				{
					X=TestX;
					from=dLeft;
					break;
				}
				TestY=SkirtUp(pt,bound);
				if((TestY!=Y)&&(TestY<bound.GetY2()))
				{
					Y=TestY;
					from=dDown;
					break;
				}
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.GetX1()==0)||((bound.GetX1()>0)&&(TestX>bound.GetX1()))))
				{
					X=TestX;
					from=dRight;
					break;
				}
				return(false);
				break;

			case dUp:
				TestX=SkirtLeft(pt,bound);
				if((TestX!=X)&&((bound.GetX1()==0)||((bound.GetX1()>0)&&(TestX>bound.GetX1()))))
				{
					X=TestX;
					from=dRight;
					break;
				}
				TestY=SkirtDown(pt,bound);
				if((TestY!=Y)&&((bound.GetY1()==0)||((bound.GetY1()>0)&&(TestY>bound.GetY1()))))
				{
					Y=TestY;
					from=dUp;
					break;
				}
				TestX=SkirtRight(pt,bound);
				if((TestX!=X)&&(TestX<bound.GetX2()))
				{
					X=TestX;
					from=dLeft;
					break;
				}
				return(false);
				break;

			case dNoDirection:
				RAssertMsg("Direction can't be undefined");
				break;

			default:
				RAssertMsg("Not a valid Direction in this context");
				break;
		}
	}

	// Verify that the not same X or not same Y
	next=poly[poly.GetNbVertices()-1];
	if((first->X==next->X)&&(first->X==poly[1]->X))
	{
		if(poly.GetConY(*next).X<first->X)
		{
			poly.InsertVertex(new RPoint(next->X-1,next->Y));
			poly.InsertVertex(new RPoint(next->X-1,first->Y));
		}
		else
		{
			poly.InsertVertex(new RPoint(next->X+1,next->Y));
			poly.InsertVertex(new RPoint(next->X+1,first->Y));
		}
		poly.DeleteVertex(next);
	}
	if((first->Y==next->Y)&&(first->Y==poly[1]->Y))
	{
		if(poly.GetConX(*next).Y<first->Y)
		{
			poly.InsertVertex(new RPoint(next->X,next->Y-1));
			poly.InsertVertex(new RPoint(first->X-1,next->Y-1));
		}
		else
		{
			poly.InsertVertex(new RPoint(next->X,next->Y+1));
			poly.InsertVertex(new RPoint(first->X,next->Y+1));
		}
		poly.DeleteVertex(next);
	}

	// Ok
	return(true);
}


//------------------------------------------------------------------------------
void RGrid::AddFreePolygons(RGeoInfo* ins,RFreePolygons* free,const RRect& bound)
{
	RPolygon Poly;          // Polygon representing the geometric information
	RPolygons NewOne;       // Polygons added now
	RPolygon New;           // New Polygon
	RPoint start,end;
	size_t nbpts;
	tDirection FromDir;
	tCoord X,Y;
	tCoord TestX,TestY;

	// Initialization
	Poly=ins->GetPlacedPolygon();
	start=Poly.GetBottomLeft();
	end=Poly.GetConX(start);
	FromDir=dLeft;
	X=start.X;
	Y=start.Y;
	nbpts=Poly.GetNbVertices();

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
				if(New.GetNbVertices())
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
			nbpts--;          // Next vertex
			TestX=X=start.X;
			TestY=Y=start.Y;
			if((FromDir==dLeft)||(FromDir==dRight))    // Go to up/bottom
			{
				end=Poly.GetConY(start);
				if(start.Y<end.Y) FromDir=dDown; else FromDir=dUp;
			}
			else      // Go to left/right
			{
				end=Poly.GetConX(start);
				if(start.X<end.X) FromDir=dLeft; else FromDir=dRight;
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
