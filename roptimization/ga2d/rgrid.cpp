/*

	Rainbow Library Project

	RGrid.cpp

	Grid for the 2D placement - Implementation

	(C) 1998-2000 by By P. Francq.

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



//---------------------------------------------------------------------------
// include files for Rainbow
#include "rgrid.h"
using namespace RGA;



//---------------------------------------------------------------------------
//
// RGrid
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
RGrid::RGrid(RPoint &limits) throw(bad_alloc)
	: Limits(limits), OccupiedX(0), OccupiedY(0)
{
	// Init Occupied
	OccupiedX = new unsigned int*[Limits.X*2];
	for(RCoord R=0;R<Limits.X*2;R++)
		OccupiedX[R] = new unsigned int[Limits.Y*2];
	OccupiedY = new unsigned int*[Limits.Y*2];
	for(RCoord R=0;R<Limits.Y*2;R++)
		OccupiedY[R] = new unsigned int[Limits.X*2];
}


//---------------------------------------------------------------------------
void RGrid::Clear(void)
{
	RReturnIfFail(OccupiedX);

	// OccupiedX
	for(RCoord R=0;R<Limits.X*2;R++)
		memset(OccupiedX[R],0xFF,sizeof(unsigned int)*(Limits.Y*2));

	// OccupiedY
	for(RCoord R=0;R<Limits.Y*2;R++)
		memset(OccupiedY[R],0xFF,sizeof(unsigned int)*(Limits.X*2));
}


//---------------------------------------------------------------------------
void RGrid::Assign(RRect &rect,RPoint &pos,unsigned int id)
{
	RCoord BeginX,BeginY,j,k,H,W;
	unsigned int **ptr1,*ptr2;

	RReturnIfFail(OccupiedX);

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


//---------------------------------------------------------------------------
void RGrid::AddFreePolygons(RGeoInfo *ins,RPolygons *polys,RRect &bound)
{
	RRect Rects;				// Rectangle eventually to add
	RPolygon Poly;			// Polygon representing the geometric information
	RPoint *start,*end;
	unsigned int nbpts;
	int FromDir;		// 0=left ; 1=right ; 2=up ; 3=down
/*	RCoord X,Y;

	// Test % limits
	if((pos.X<0)||(pos.Y<0)) return(false);
	Test=Rect;
	Test+=pos;
	if(Test.Clip(limits)) return(false);	

	// Test it and go through the other
	start=Bound->GetBottomLeft();
	end=Bound->GetConX(start);
	FromDir=0;
	X=start->X+pos.X;
	Y=start->Y+pos.Y;
	nbpts=Bound->NbPtr;

	// Test it and go through the other
	while(nbpts)
	{
		if(grid->IsOcc(X,Y)) return(false);

		// If end of an edge
		if((X==end->X+pos.X)&&(Y==end->Y+pos.Y))
		{
			start=end;
			nbpts--;			// Next point
			X=start->X+pos.X;
			Y=start->Y+pos.Y;
			if(FromDir<2)	// Go to up/bottom
			{
				end=Bound->GetConY(start);
				if(start->Y<end->Y) FromDir=2; else FromDir=3;
			}
			else		// Go to left/right
			{
				end=Bound->GetConX(start);
				if(start->X<end->X) FromDir=0; else FromDir=1;
			}
		}
		else
		{
			// Go to next pos
			switch(FromDir)
			{
				case 0: // from left
					X++;
					break;

				case 1: // from right
					X--;
					break;

				case 2: // from bottom
					Y++;
					break;

				case 3: // from up
					Y--;
					break;
			}
		}
	}
*/	
}


//---------------------------------------------------------------------------
RGrid::~RGrid(void)
{
	if(OccupiedX)
	{
		for(RCoord R=0;R<Limits.X*2;R++)
			delete[] (OccupiedX[R]);
		delete[] OccupiedX;
	}
	if(OccupiedY)
	{
		for(RCoord R=0;R<Limits.Y*2;R++)
			delete[] (OccupiedY[R]);
		delete[] OccupiedY;
	}
}