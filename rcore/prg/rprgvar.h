/*

	R Project Library

	RPrgVar.h

	Generic variable - Header.

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
#ifndef RPrgVarH
#define RPrgVarH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrg;


//------------------------------------------------------------------------------
/**
* The RPrgVar provides a class for a generic variable.
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

public:

	/**
	* Constructor of a variable.
	* @param name           Name.
	*/
	RPrgVar(const char* name) throw(std::bad_alloc);

	/**
	* This methods compares two variables using their names and works like the
	* "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param v               Pointer to a variable.
	* @return int
	*/
	int Compare(const RPrgVar* v) const;

	/**
	* This methods compares the name of a variable with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param v               String representing the name of the variable.
	* @return int
	*/
	int Compare(const RString& v) const;

	/**
	* This methods compares the name of a variable with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param v               String representing the name of the variable.
	* @return int
	*/
	int Compare(const char* v) const;

	/**
	* Assign some data to the variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data) throw(RException);

	/**
	* Get the value of the variable.
	* @param prg            Program.
	* @return "C" string representing the variable.
	*/
	virtual const char* GetValue(RPrg* prg) throw(RException);

	/**
	* Destructor of the variable.
	*/
	virtual ~RPrgVar(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
