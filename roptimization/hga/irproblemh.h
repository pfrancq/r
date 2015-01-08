/*

	R Project Library

	iRProblemH.h

	Interface for a hierarchical problem - Header

	Copyright 1998-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef iRProblemH_H
#define iRProblemH_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The iRProblemH provides an interface for a class that represents a
 * hierarchical problem.
 * @short Hierarchical Problem Interface.
 */
template<class cObj>
	class iRProblemH
{
public:

	/**
	 * Default constructor.
    */
	iRProblemH(void) {}

	/**
	 * Get the objects for which a hierarchy must be found.
    * @return a cursor over the objects.
    */
	virtual RCursor<cObj> GetObjs(void) const=0;

	/**
	 * Get a label for a given attribute.
    * @param id             Identifier of the attribute.
    * @return a label.
    */
	virtual RString GetAttrLabel(size_t id) const=0;

	/**
	 * Default destructor.
    */
	virtual ~iRProblemH(void) {}
};


}//------- End of namespace R --------------------------------------------------


//------------------------------------------------------------------------------
#endif

