/*

	R Project Library

	RPrgInstAssign.h

	Assignment Instructions - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RPrgInstAssignH
#define RPrgInstAssignH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprginst.h>
#include <rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgVar;
class RPrg;
class RPrgClass;
class RPrgOutput;


//------------------------------------------------------------------------------
/**
* The RPrgInstNew provides a class for a "new" instruction.
* @short "new" Instruction.
*/
class RPrgInstNew : public RPrgInst
{
	/**
	* Variable to create.
	*/
	RString Var;

	/**
	 * Class of the variable.
	 */
	RPrgClass* Class;

	/**
	* Parameters of the constructors.
	*/
	RContainer<RPrgVar,true,false> Params;

public:

	/**
	* Construct a "new" instruction.
	* @param prg            Program.
	* @param name           Name of the variable.
	* @param c              Class of the variable.
	* @param init           Initialization parameters.
	*/
	RPrgInstNew(RInterpreter* prg,const RString& name,RPrgClass* c,RContainer<RPrgVar,false,false>& init);

	/**
	* Run the instructions in the for for the different values of its variable.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o);

	/**
	* Destruct the "new" instruction.
	*/
	virtual ~RPrgInstNew(void);
};


//------------------------------------------------------------------------------
/**
* The RPrgInstAssignRef provides a class for the assignment instruction.
* @short Assignment Instruction (=).
*/
class RPrgInstAssignVar : public RPrgInst
{
	/**
	* Variable to assign to.
	*/
	RString Var;

	/**
	 * What to assign.
	 */
	RPrgVar* Assign;

public:

	/**
	* Construct a assignment instruction.
	* @param prg            Program.
	* @param name           Name of the variable.
	* @param assign         What to assign. It is destroy by the instruction.
	*/
	RPrgInstAssignVar(RInterpreter* prg,const RString& name,RPrgVar* assign);

	/**
	* Run the instructions in the for for the different values of its variable.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o);

	/**
	* Destruct the assignment instruction.
	*/
	virtual ~RPrgInstAssignVar(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
