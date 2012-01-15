/*

	R Project Library

	RRect.h

	Rectangle - Header

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RRects_H
#define RRects_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>
#include <rrect.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a container of rectangles.
* @author Pascal Francq
* @short Rectangles container.
*/
class RRects : public RContainer<RRect,true,false>
{
public:

	/**
	* Construct a container of rectangles.
	*/
	RRects(void);

	/**
	* Construct a container of rectangles with a initial maximal size.
	* @param max            Initial maximal size.
	*/
	RRects(size_t max);

	/**
	* Construct a container of rectangles from another.
	* @param rects          The container used as reference.
	*/
	RRects(const RRects& rects);

	/**
	* Return the area of all the rectangles.
	*/
	tCoord Area(void) const;

	/**
	 * @return cursor over the rectangle.
	 */
	RCursor<RRect> GetRects(void) const {return(RCursor<RRect>(*this));}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
