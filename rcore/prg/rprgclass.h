/*

	R Project Library

	RPrgClass.h

	Generic Class - Header.

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
#ifndef RPrgClassH
#define RPrgClassH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>


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
	RContainer<RPrgVar,unsigned int,true,true> Vars;

	/**
	* Methods defined in the class.
	*/
	R::RContainer<RPrgFunc,unsigned int,true,true> Methods;

public:

	/**
	* Concstructor of a class.
	* @param name           Name.
	*/
	RPrgClass(const char* name) throw(bad_alloc);

	/**
	* This methods compares two classes using their names and works like the
	* "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param f               Pointer to a class.
	* @return int
	*/
	int Compare(const RPrgClass* c) const;

	/**
	* This methods compares the name of a class with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param n               String representing the name of a class.
	* @return int
	*/
	int Compare(const RString& c) const;

	/**
	* This methods compares the name of a class with a string and works like
	* the "strcpy" function of the ANSI C/C++ library.
	* @see R::RContainer.
	* @param n               String representing the name of a class.
	* @return int
	*/
	int Compare(const char* c) const;

	/**
	* Find a method.
	* @param method         Name of the method.
	* @returns Pointer to RPrgInst.
	*/
	RPrgFunc* GetMethod(const char* method) const;

	/**
	* Destructor of a class.
	*/
	virtual ~RPrgClass(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
