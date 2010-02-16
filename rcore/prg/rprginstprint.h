/*

	R Project Library

	RPrgInstPrint.h

	Print instruction - Header.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef RPrgInstPrintH
#define RPrgInstPrintH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rprginst.h>
#include <rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPrgInstPrint provides a class for a delete instruction.
* @author Pascal Francq
* @short Delete Instruction.
*/
class RPrgInstPrint : public RPrgInst
{
	/**
	* What to print.
	*/
	RPrgVar* What;

public:

	/**
	* Construct a generic instruction.
	* @param prg            Program.
	* @param what           What to print.
	*/
	RPrgInstPrint(RInterpreter* prg,RPrgVar* what);

	/**
	* Run the instruction.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o);

	/**
	* Destruct the generic instruction.
	*/
	virtual ~RPrgInstPrint(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
