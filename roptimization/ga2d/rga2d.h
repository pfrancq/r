/*

	Rainbow Library Project

	RGA2D.h

	2D Placement Genetic Algorithm - Header

	(C) 1999-2000 by P. Francq.

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
#ifndef RGA2DH
#define RGA2DH


//-----------------------------------------------------------------------------
// include files for Rainbow
#include "rga.h"
using namespace RGA;
#include <rgeometry/rrect.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;

/**
* \defgroup 2DGA 2D-Genetic Algorithm.
*
* This classes represent a genetic algorithm for the 2D placement of polygons.
*/


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RGeoInfo;
class RObj2D;
class RObj2DContainer;
template<class cInst,class cChromo>	class RThreadData2D;
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo> class RInst2D;
template<class cInst,class cChromo,class cFit,class cThreaData,class cInfo> class RChromo2D;


//-----------------------------------------------------------------------------
// Heuristic Types
enum HeuristicType{BottomLeft,Edge,Center};


}//------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
// include files for GA
#include "rgeoinfo.h"
#include "robj2d.h"
#include "rinst2d.h"
#include "rchromo2d.h"
using namespace RGA;


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions of templates
#include "rinst2d.hh"
#include "rchromo2d.hh"


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
