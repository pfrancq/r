/*

	R Project Library

	RVector.h

	Class representing vector - Header

	Copyright 2008-2009 by Pascal Francq (pascal@francq.info).

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
#ifndef RVectorH
#define RVectorH


//------------------------------------------------------------------------------
// include files for R Project
#include <rnumcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RVector class provides a representation for a vector.
 * @author Pascal Francq
 * @short RVector
 */
class RVector : public RNumContainer<double,false>
{
public:

	/**
	* Construct a vector.
	* @param max             Maximum number of values.
	*/
	RVector(size_t max);

	/**
	* Copy constructor.
	* @param vector          Vector to copy.
	*/
	RVector(const RVector& vector);

	/**
	 * Destruct the vector.
	 * @return
	 */
	~RVector(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
