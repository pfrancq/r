/*

	R Project Library

	RObjs.h

	Container of objectss used as input for Genetic Algorithms - Header

	(C) 1998-2001 by P. Francq.

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
#ifndef RObjsH
#define RObjsH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//-----------------------------------------------------------------------------
namespace RGA{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The Robjs class provides a container for objects on which a genetic
* algorithms has to work.
* @param cObj               The type of the objects.
* @author Pascal Francq
* @short Objects.
*/
template<class cObj>
	class RObjs : public RStd::RContainer<cObj,unsigned int,true,false>
{
public:

	/**
	* Construct the objects.
	* @param max            Maximum objects that wilk be created.
	*/
	RObjs(const unsigned int max) : RStd::RContainer<cObj,unsigned int,true,false>(max,max/2) {}
};


}  //------- End of namespace RGA ---------------------------------------------


//-----------------------------------------------------------------------------
#endif
