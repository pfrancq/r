/*

	R Project Library

	RRect.h

	Rectangle - Header

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


//---------------------------------------------------------------------------
#ifndef RRectsH
#define RRectsH


//---------------------------------------------------------------------------
// include files for RGeometry
#include "rrect.h"
using namespace RGeometry2D;


//---------------------------------------------------------------------------
namespace RGeometry2D{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* This class implements a container of rectangles.
* @author Pascal Francq
* @short Rectangles container.
*/
class RRects : public RStd::RContainer<RRect,unsigned int,true,false>
{
public:

	/**
	* Construct a container of rectangles.
	*/
	RRects(void);

	/**
	* Construct a container of rectangles with a intial maximal size.
    * @param max	Initial maxiaml size.
	*/
	RRects(unsigned int max);

	/**
	* Construct a container of rectangles from another.
	* @param rects	The container used as reference.
	*/
	RRects(RRects *rects);

	/**
	* Return the area of all the rectangles.
	*/
	RCoord Area(void);
};


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
