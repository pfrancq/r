/*

	R Project Library

	RPrgInstBlock.h

	Block of Instructions - Header.

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
#ifndef RPrgInstBlockH
#define RPrgInstBlockH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <rprginst.h>


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
	* List of all local "Variables" defined in the block.
	*/
	R::RContainer<RPrgVar,true,false> Vars;

	/**
	* Indentation corresponding to this block.
	*/
	size_t Tabs;

public:

	/**
	* Construct a block of instructions.
	* @param line           Rest of the line.
	* @param t              Indentation.
	*/
	RPrgInstBlock(size_t t);

	/**
	* Add an instruction to the for.
	*/
	void AddInst(RPrgInst* ins);

	/**
	* Get the number of tabs.
	* @return size_t.
	*/
	size_t GetTabs(void) const {return(Tabs);}

	/**
	* Destruct the block of instructions.
	*/
	virtual ~RPrgInstBlock(void);

	friend class RPrg;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
