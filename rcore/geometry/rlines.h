/*

	R Project Library

	RLines.h

	Lines - Header

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RLines_H
#define RLines_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rgeometry.h>
#include <rline.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class represents a set of lines regrouped in a container.
* @short Container of lines.
*/
class RLines : public RContainer<RLine,true,false>
{
public:

	/**
	* Construct a container of lines.
	*/
	RLines(void);

	/**
	* Construct a container of lines with an initial maximal size.
	* @param max            Initial maximal size of the container.
	*/
	RLines(const size_t max);

	/**
	* Construct a container of lines from another one.
	* @param lines           The container used as reference.
	*/
	RLines(const RLines& lines);

	/**
	* Assignment operator.
	* @param lines           The container used as reference.
	*/
	RLines& operator=(const RLines& lines);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
