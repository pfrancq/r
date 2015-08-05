/*

	R Project Library

	RPrgScope.h

	Scope holding a set of variables - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RPrgScopeH
#define RPrgScopeH


//------------------------------------------------------------------------------
// include files for R project
#include <rcontainer.h>
#include <rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RPrgScope class provides a scope for a set of variables (in general
 * associated with a block of instructions).
 * @author Pascal Francq
 * @short Variables Scope.
 */
class RPrgScope : public RContainer<RPrgVar,true,true>
{
public:

	/**
	 * Constructor.
	 */
	RPrgScope(void);

	/**
	 * Compare method used by RContainer.
	 */
	int Compare(const RPrgScope&) const {return(-1);}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
