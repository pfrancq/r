/*

	Rainbow Library Project

	RPlacementEdge.h

	Edge Heuristic for Placement - Header

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
#ifndef RPlacementEdgeH
#define RPlacementEdgeH


//---------------------------------------------------------------------------
// include files for Rainbow
#include "rplacementheuristic.h"


//---------------------------------------------------------------------------
namespace RGA{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* The RPlacementEdge class provides an edge placement heuristic.
* @author Pascal Francq
* @short Edge Heuristic class.
*/
class RPlacementEdge : public RPlacementHeuristic
{
	RPoint Levels[40];
	unsigned int NbLevels;
	RPoint Actual;
	RPoint Max;
	RPoint Last;
	unsigned int CurLevel;
public:
	RPlacementEdge(unsigned int maxobjs);
	virtual void Init(RPoint &limits,RGrid *grid,RObj2D** objs,RGeoInfo **infos,unsigned int nbobjs);
	virtual bool NextObjectOri(void);
};


}//------- End of namespace RGA ---------------------------------------------


//---------------------------------------------------------------------------
#endif