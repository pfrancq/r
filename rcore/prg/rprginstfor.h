/*

	R Project Library

	RPrgInstFor.h

	"for" Instruction - Header.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef RPrgInstForH
#define RPrgInstForH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprginstblock.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgVar;
class RInterpreter;
class RPrgOutput;


//------------------------------------------------------------------------------
/**
* The RPrgInstFor provides a class for a "for" instruction.
* @author Pascal Francq
* @short "for" Instruction.
*/
class RPrgInstFor : public RPrgInstBlock
{
	/**
	* Variable defined in the for.
	*/
	RString Var;

	/**
	* Values of the variable.
	*/
	RContainer<RPrgVar,true,false> Values;

public:

	/**
	* Construct a "for" instruction.
	* @param prg            Program.
	* @param t              Indentation.
	*/
	RPrgInstFor(RInterpreter* prg,size_t t);

	/**
	* Run the instructions in the for for the different values of its variable.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o);

	/**
	 * Need the instruction a block of instructions.
	 */
	virtual bool NeedBlock(void) const {return(true);}

	/**
	* Destruct the "for" instruction.
	*/
	virtual ~RPrgInstFor(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
