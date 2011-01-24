/*

	R Project Library

	RPrgInstBlock.h

	Block of Instructions - Header.

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



//------------------------------------------------------------------------------
#ifndef RPrgInstBlockH
#define RPrgInstBlockH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <rprginst.h>
#include <rprgscope.h>


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
* The RPrgInstBlock provides a class for a block of instructions.
* @author Pascal Francq
* @short Instructions Block.
*/
class RPrgInstBlock : public RPrgInst
{
protected:

	/**
	* List of all "Instructions" to execute.
	*/
	R::RContainer<RPrgInst,true,false> Insts;

	/**
	* Scope of the block.
	*/
	RPrgScope* Vars;

	/**
	* Depth corresponding to this block.
	*/
	size_t Depth;

public:

	/**
	* Construct a block of instructions.
	* @param prg            Program.
	* @param depth          Indentation.
	*/
	RPrgInstBlock(RInterpreter* prg,size_t depth);

	/**
	* Add an instruction to the block.
	*/
	void AddInst(RPrgInst* ins);

	/**
	* Add a variable.
	* @param var             Pointer to the variable.
	*/
	void AddVar(RPrgVar* var);

	/**
	* Remove a variable.
	* @param var             Pointer to the variable.
	*/
	void DelVar(RPrgVar* var);

	/**
	* Get the number of tabs.
	* @return size_t.
	*/
	size_t GetDepth(void) const {return(Depth);}

	/**
	 * Clear the instructions associated to the block.
	 */
	void ClearInstructions(void);

	/**
	* Run the all instructions in the block. This is the method that should be
	* overloaded by child classes.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void RunBlock(RInterpreter* prg,RPrgOutput* o);

	/**
	* Run the blocks. In practice, the methods creates a local
	* scope and execute the method 'RunBlock'.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o);

	/**
	* Destruct the block of instructions.
	*/
	virtual ~RPrgInstBlock(void);

	friend class RInterpreter;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
