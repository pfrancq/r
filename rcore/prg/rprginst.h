/*

	R Project Library

	RPrgInst.h

	Generic instruction - Header.

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
#ifndef RPrgInstH
#define RPrgInstH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration
class RInterpreter;
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
	/**
	 * Line in the source file.
	 */
	size_t Line;

public:

	/**
	* Construct a generic instruction.
	* @param line            Line.
	*/
	RPrgInst(size_t line);

	/**
	* This method compares two instructions.
	* @see R::RContainer.
	* @return -1
	*/
	int Compare(const RPrgInst& t) const;

	/**
	* This method compares two instructions.
	* @see R::RContainer.
	* @return -1
	*/
	int Compare(const RString& t) const;

	/**
	 * Get the line number of the instruction.
	 */
	size_t GetLine(void) const {return(Line);}

	/**
	* Run the instruction.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o);

	/**
	 * Need the instruction a block of instructions.
	 */
	virtual bool NeedBlock(void) const {return(false);}

	/**
	* Destruct the generic instruction.
	*/
	virtual ~RPrgInst(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
