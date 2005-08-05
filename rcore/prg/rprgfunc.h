/*

	R Project Library

	RPrgFunc.h

	Generic function - Header.

	Copyright 2002-2003 by the Universit�Libre de Bruxelles.

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
#ifndef RPrgFuncH
#define RPrgFuncH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration
class RPrgVar;
class RPrg;
class RPrgOutput;


//------------------------------------------------------------------------------
/**
* The RPrgFunc provides a class for a generic function.
* @author Pascal Francq
* @short Generic Function.
*/
class RPrgFunc
{
protected:

	/**
	* Name of the function.
	*/
	RString Name;

public:

	/**
	* Costructor of a function.
	* @param name           Name.
	*/
	RPrgFunc(const char* name);

	/**
	* This methods compares two functions using their names and works like the
	* "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param f               Pointer to a function.
	* @return int
	*/
	int Compare(const RPrgFunc& f) const;

	/**
	* This methods compares the name of a function with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param f               String representing the name of the function.
	* @return int
	*/
	int Compare(const RString& f) const;

	/**
	* This methods compares the name of a function with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param f               String representing the name of the function.
	* @return int
	*/
	int Compare(const char* f) const;

	/**
	* Execute the function.
	* @param prg            Program.
	* @param o              Output.
	* @param args           Parameters of the function.
	*/
	virtual void Run(RPrg* prg,RPrgOutput* o,RContainer<RPrgVar,true,false>* args);

	/**
	* Destructor of a function.
	*/
	virtual ~RPrgFunc(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
