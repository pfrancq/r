/*

	R Project Library

	RPrgVarString.h

	Variable containing a string value - Header.

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
#ifndef RPrgVarStringH
#define RPrgVarStringH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPrgVarVal provides a class for a string variable.
* @author Pascal Francq
* @short String Variable.
*/
class RPrgVarString : public RPrgVar
{
	/**
	* Value of the variable.
	*/
	RString String;

public:

	/**
	* Construct the variable.
	* @param name           Name.
	* @param value          Value.
	*/
	RPrgVarString(const RString& name,const RString& value);

	/**
	* Assign some data to the variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data);

	/**
	* Get the value of the variable.
	* @param prg            Program.
	*/
	virtual RString GetValue(RInterpreter* prg);

	/**
	* Destruct the variable.
	*/
	virtual ~RPrgVarString(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
