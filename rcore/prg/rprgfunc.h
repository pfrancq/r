/*

	R Project Library

	RPrgFunc.h

	Generic function - Header.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
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
class RInterpreter;
class RPrgVarInst;
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

	/**
	* Description of the function.
	*/
	RString Description;

public:

	/**
	* Construct the function.
	* @param name           Name.
	* @param desc           Description.
	*/
	RPrgFunc(const RString& name,const RString desc=RString::Null);

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
	* Get the name of the function.
	*/
	RString GetName(void) const;

	/**
	* Get the description of the function.
	*/
	RString GetDescription(void) const;

	/**
	* Execute the function.
	* @param prg            Program.
	* @param o              Output.
	* @param inst           Instance of the class.
	* @param args           Parameters of the function.
	*/
	virtual void Run(RInterpreter* prg,RPrgOutput* o,RPrgVarInst* inst,RContainer<RPrgVar,true,false>& args)=0;

	/**
	* Destruct the function.
	*/
	virtual ~RPrgFunc(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
