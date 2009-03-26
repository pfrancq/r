/*

	R Project Library

	RPrgVar.h

	Generic variable - Header.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RPrgVarH
#define RPrgVarH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RInterpreter;


//------------------------------------------------------------------------------
/**
* The RPrgVar provides a class for a generic variable of a given type.
* @author Pascal Francq
* @short Generic Variable.
*/
class RPrgVar
{
protected:

	/**
	* Name of the variable.
	*/
	RString Name;

	/**
	 * Type of the variable.
	 */
	RString Type;

public:

	/**
	* Constructor of a variable.
	* @param name           Name.
	* @param type           Type of the variable.
	*/
	RPrgVar(const RString& name,const RString& type);

	/**
	* Constructor of a variable.
	* @param var             Variable.
	*/
	RPrgVar(const RPrgVar& var);

	/**
	* This methods compares two variables using their names and works like the
	* "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param v               Pointer to a variable.
	* @return int
	*/
	int Compare(const RPrgVar& v) const;

	/**
	* This methods compares the name of a variable with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param v               String representing the name of the variable.
	* @return int
	*/
	int Compare(const RString& v) const;

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
	 * Get the type of the variable.
	 */
	RString GetType(void) const {return(Type);}

	/**
	 * Get the name of the variable.
	 */
	RString GetName(void) const {return(Name);}

	/**
	* Destruct the variable.
	*/
	virtual ~RPrgVar(void);

	friend class RInterpreter;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
