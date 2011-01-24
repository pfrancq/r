/*

	R Project Library

	RPrgOutput.h

	Generic output - Header.

	Copyright 2002-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RPrgOutputH
#define RPrgOutputH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RPrgOutput class provides a representation for a generic output for a
* program.
* @author Pascal Francq
* @short Generic Output.
*/
class RPrgOutput
{
public:

	/**
	* Construct a generic output.
	*/
	RPrgOutput(void);

	/**
	* Method called when executing a sequence of instructions to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const R::RString& str);

	/**
	* Destruct the generic output.
	*/
	virtual ~RPrgOutput(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
