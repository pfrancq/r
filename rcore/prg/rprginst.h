/*

	R Project Library

	RPrgInst.h

	Generic instruction - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//------------------------------------------------------------------------------
#ifndef RPrgInstH
#define RPrgInstH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration
class RPrg;
class RPrgOutput;


//------------------------------------------------------------------------------
/**
* The RPrgInst provides a class for a generic instruction.
*
* The Compare methods needed by R::RContainer are implemented so that it is
* impossible to construct an ordered container of instructions.
* @author Pascal Francq
* @short Generic Instruction.
*/
class RPrgInst
{
public:

	/**
	* Constructor of a generic instruction.
	*/
	RPrgInst(void) throw(bad_alloc);

	/**
	* This method compares two instructions.
	* @see R::RContainer.
	* @return -1
	*/
	int Compare(const RPrgInst* t) const;

	/**
	* This method compares two instructions.
	* @see R::RContainer.
	* @return -1
	*/
	int Compare(const char* t) const;

	/**
	* Run the instruction.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RPrg* prg,RPrgOutput* o) throw(RException);

	/**
	* Destructor of a generic instruction.
	*/
	virtual ~RPrgInst(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif 
