/*

	R Project Library

	RPrgOutput.h

	Generic output - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
	* Constructor of a generic output.
	*/
	RPrgOutput(void);

	/**
	* Method called when executing a sequence of instructions to output some
	* information.
	* @param str            String to output.
	*/
	virtual void WriteStr(const char* str);

	/**
	* Destructor of a generic output.
	*/
	virtual ~RPrgOutput(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
