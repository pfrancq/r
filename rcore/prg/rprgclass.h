/*

	R Project Library

	RPrgClass.h

	Generic Class - Header.

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
#ifndef RPrgClassH
#define RPrgClassH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgVar;
class RPrgFunc;


//------------------------------------------------------------------------------
/**
* The RPrgClass provides a class for a generic class.
* @author Pascal Francq
* @short Generic Class.
*/
class RPrgClass
{
protected:

	/**
	* Name of the class.
	*/
	RString Name;

	/**
	* Variables defined in the class.
	*/
	R::RContainer<RPrgVar,true,true> Vars;

	/**
	* Methods defined in the class.
	*/
	R::RContainer<RPrgFunc,true,true> Methods;

public:

	/**
	* Concstructor of a class.
	* @param name           Name.
	*/
	RPrgClass(const RString& name);

	/**
	* This methods compares two classes using their names and works like the
	* "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param c               Pointer to a class.
	* @return int
	*/
	int Compare(const RPrgClass& c) const;

	/**
	* This methods compares the name of a class with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param c               String representing the name of a class.
	* @return int
	*/
	int Compare(const RString& c) const;

	/**
	* Find a method.
	* @param method         Name of the method.
	* @returns Pointer to RPrgInst.
	*/
	RPrgFunc* GetMethod(const RString& method) const;

	/**
	* Get the name of the function.
	*/
	RString GetName(void) const;

	/**
	* Get a cursor over the methods defined in the class.
	*/
	RCursor<RPrgFunc> GetMethods(void) const;

	/**
	* Destructor of a class.
	*/
	virtual ~RPrgClass(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
