/*

	R Project Library

	RPrgVarConst.h

	Const Variable - Header.

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



//------------------------------------------------------------------------------
#ifndef RPrgVarConstH
#define RPrgVarConstH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprg/rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPrgVarCons provides a class for a const variable.
* @author Pascal Francq
* @short Const Variable.
*/
class RPrgVarConst : public RPrgVar
{
public:

	/**
	* Constructor of a const variable.
	* @param value          Name.
	*/
	RPrgVarConst(const char* value) throw(std::bad_alloc);

	/**
	* Assign some data to the const variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data) throw(RException);

	/**
	* Get the value of the const variable.
	* @param prg            Program.
	* @return "C" string representing the content of the const variable.
	*/
	virtual const char* GetValue(RPrg* prg) throw(RException);

	/**
	* Destructor of a const variable.
	*/
	virtual ~RPrgVarConst(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif 
