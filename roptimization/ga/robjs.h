/*

	R Project Library

	RObjs.h

	Container of objects used as input for Genetic Algorithms - Header

	Copyright 1998-2009 by Pascal Francq (pascal@francq.info).
	Copyright 1998-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RObjs_H
#define RObjs_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The Robjs class provides a container for objects on which a genetic
* algorithms has to work.
* @param cObj               The type of the objects.
* @author Pascal Francq
* @short Objects.
*/
template<class cObj>
	class RObjs : public RContainer<cObj,true,true>
{
public:

	/**
	* Construct the objects.
	* @param max            Maximum objects that wilk be created.
	*/
	RObjs(const size_t max) : RContainer<cObj,true,true>(max,max/2) {}
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
