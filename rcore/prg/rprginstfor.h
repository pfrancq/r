/*

	R Project Library

	RPrgInstFor.h

	"for" Instruction - Header.

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
#ifndef RPrgInstForH
#define RPrgInstForH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rcontainer.h>
#include <rprg/rprginst.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgVar;
class RPrg;
class RPrgOutput;


//------------------------------------------------------------------------------
/**
* The RPrgInstFor provides a class for a "for" instruction.
* @author Pascal Francq
* @short "for" Instruction.
*/
class RPrgInstFor : public RPrgInst
{
	/**
	* Variable defined in the for.
	*/
	RString Var;

	/**
	* Values of the variable.
	*/
	RContainer<RPrgVar,unsigned int,true,false> Values;

	/**
	* List of all "Instructions" to execute.
	*/
	R::RContainer<RPrgInst,unsigned int,true,false> Insts;

	/**
	* Identation of for.
	*/
	unsigned int Tabs;

public:

	/**
	* Cronstructor of a "for" instruction.
	* @param line           Rest of the line.
	* @param t              Identation.
	*/
	RPrgInstFor(char* line,unsigned int t) throw(std::bad_alloc);

	/**
	* Add an instruction to the for.
	*/
	void AddInst(RPrgInst* ins) throw(std::bad_alloc);

	/**
	* Run the instructions in the for for the different values of its variable.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RPrg* prg,RPrgOutput* o) throw(RException);

	/**
	* Get the number of tabs.
	* @return unsigned int.
	*/
	unsigned int GetTabs(void) const {return(Tabs);}

	/**
	* Destructor of a "for" instruction.
	*/
	virtual ~RPrgInstFor(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif 
