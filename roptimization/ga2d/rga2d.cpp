/*

	Rainbow Library Project

  RGA2D.cpp

  2D Placement Genetic Algorithm - Header

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

	As a special exception to the GNU General Public License, permission is
	granted for additional uses of the text contained in its release
	of the Rainbow Library.

	The exception is that, if you link the Rainbow with other files
	to produce an executable, this does not by itself cause the
	resulting executable to be covered by the GNU General Public License.
	Your use of that executable is in no way restricted on account of
	linking the Rainbow library code into it.

	This exception does not however invalidate any other reasons why
	the executable file might be covered by the GNU General Public License.

	This exception applies only to the code released under the
	name Rainbow.  If you copy code from other releases into a copy of
	RAinbow, as the General Public License permits, the exception does
	not apply to the code that you add in this way.  To avoid misleading
	anyone as to the status of such modified files, you must delete
	this exception notice from them.

	If you write modifications of your own for Rainbow, it is your choice
	whether to permit this exception to apply to your modifications.
	If you do not wish that, delete this exception notice.

*/



//---------------------------------------------------------------------------
// include files for Rainbow
#include "rga2d.h"
using namespace RGA;


//---------------------------------------------------------------------------
//
// General Variables
//
//---------------------------------------------------------------------------
RRect RGA::Limits;
RCoord RGA::AvLen;
RCoord RGA::AvWidth;



//---------------------------------------------------------------------------
//
// General Functions
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void RGA::RandomPlace(RCoord& X,RCoord& Y)
{
	bool IncX,IncY;

	IncX=RRand(1);
	IncY=RRand(1);

	// Change X et Y
	if(IncX) X+=AvLen; else	X-=AvLen;
	if(IncY) Y+=AvWidth; else	Y-=AvWidth;

	// Verify if X in Limits
	if(X<Limits.Pt1.X) X=Limits.Pt2.X-(Limits.Pt1.X-X);
	if(X>Limits.Pt2.X) X=Limits.Pt1.X+(X-Limits.Pt2.X);

	// Verify if Y in Limits
	if(Y<Limits.Pt1.Y) Y=Limits.Pt2.Y-(Limits.Pt1.Y-Y);
	if(Y>Limits.Pt2.Y) Y=Limits.Pt1.Y+(Y-Limits.Pt2.Y);
}


//---------------------------------------------------------------------------
void RGA::LocalOptimisation(const RRects &Rects,RCoord &PosX,RCoord &PosY,unsigned int **OccX,unsigned int **OccY)
{
	bool bCanPush;
  unsigned int i;
	RCoord j;
  RRect **rect;
  unsigned int *nptr;

	// Push Left
  bCanPush=true;
	while(bCanPush&&(PosX>Limits.Pt1.X))
	{				
		for(i=Rects.NbPtr+1,rect=Rects.Tab;(--i)&&bCanPush;rect++)
			for(j=(*rect)->Width()+1,nptr=&OccX[PosX+(*rect)->Pt1.X-1][PosY+(*rect)->Pt1.Y];(--j)&&bCanPush;nptr++)
				if((*nptr)!=NoObject) bCanPush=false;	
		if(bCanPush) PosX--;		// Push it left
	}

	// Push Down
  bCanPush=true;
	while(bCanPush&&(PosY>Limits.Pt1.Y))
	{				
		for(i=Rects.NbPtr+1,rect=Rects.Tab;(--i)&&bCanPush;rect++)
			for(j=(*rect)->Length()+1,nptr=&OccY[PosY+(*rect)->Pt1.Y-1][PosX+(*rect)->Pt1.X];(--j)&&bCanPush;nptr++)
				if((*nptr)!=NoObject) bCanPush=false;	
		if(bCanPush) PosY--;		// Push it down
	}
}