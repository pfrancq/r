/*

	Rainbow Library Project

	RPlacementBottomLeft.h

	Bottom Left Heuristic for Placement - Header

	(C) 1998-2000 by P. Francq.

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
#ifndef RPlacementBottomLeftH
#define RPlacementBottomLeftH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rplacementheuristic.h"


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* The RPlacementHeuristic class provides an abstract class for placement
* heuristics.
* @author Pascal Francq
* @short Generic Placement Heuristic class.
*/
class RPlacementBottomLeft : public RPlacementHeuristic
{
	RPoint Actual;
	RPoint Last;
public:
	RPlacementBottomLeft(unsigned int maxobjs);
	virtual void Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs);
	virtual bool NextObjectOri(void);
};


}//------- End of namespace RGA ---------------------------------------------


//---------------------------------------------------------------------------
#endif