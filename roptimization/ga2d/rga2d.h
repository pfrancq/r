/*

	R Project Library

	RGA2D.h

	2D Placement Genetic Algorithm - Header

	(C) 1999-2001 by P. Francq.

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
// include files for R Project
#include <rga/rga.h>
using namespace RGA;
#include <rgeometry/rrect.h>
#include <rgeometry/rpolygon.h>
using namespace RGeometry2D;


//-----------------------------------------------------------------------------
/**
* \namespace RGA2D
* \brief Genetic Algorithm Classes for 2D placement.
*
* This namespace declares classes resolving 2D placement with Genetic
* Algorithms.
*/


//-----------------------------------------------------------------------------
namespace RGA2D{
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
enum HeuristicType{BottomLeft=0,Edge=1,Center=2};


}//------- End of namespace RGA2D ---------------------------------------------


//-----------------------------------------------------------------------------
// include files for RGA2D
#include <rga2d/rgeoinfo.h>
#include <rga2d/robj2d.h>
#include <rga2d/rinst2d.h>
#include <rga2d/rchromo2d.h>
using namespace RGA;


//-----------------------------------------------------------------------------
namespace RGA2D{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions of templates
#include <rga2d/rinst2d.hh>
#include <rga2d/rchromo2d.hh>


}  //------- End of namespace RGA2D -------------------------------------------


//-----------------------------------------------------------------------------
#endif
