/*

	R Project Library

	RPrgVarVal.h

	Variable containing a value - Header.

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
#ifndef RPrgVarValH
#define RPrgVarValH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprg/rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPrgVarCons provides a class for a variable with a value.
* @author Pascal Francq
* @short Variable.
*/
class RPrgVarVal : public RPrgVar
{
	/**
	* Value of the variable.
	*/
	RString Value;

public:

	/**
	* Costructor of a variable.
	* @param name           Name.
	* @param value          Value.
	*/
	RPrgVarVal(const char* name,const char* value) throw(bad_alloc);

	/**
	* Assign some data to the variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data) throw(RException);

	/**
	* Get the value of the variable.
	* @param prg            Program.
	* @return "C" string representing the value of the variable.
	*/
	virtual const char* GetValue(RPrg* prg) throw(RException);

	/**
	* Destructor of a variable.
	*/
	virtual ~RPrgVarVal(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
