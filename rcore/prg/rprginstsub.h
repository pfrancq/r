/*

	R Project Library

	RPrgInstSub.h

	Definition of a subroutine - Header.

	Copyright 2009 by Pascal Francq (pascal@francq.info).

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
#ifndef RPrgInstSubH
#define RPrgInstSubH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprginstblock.h>
#include <rprgfunc.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgVar;
class RInterpreter;
class RPrgOutput;


//------------------------------------------------------------------------------
/**
* The RPrgInstSub provides a class to define a subroutine.
* @author Pascal Francq
* @short Subroutine.
*/
class RPrgInstSub : public RPrgInstBlock
{
	/**
	 * Name of the subroutine.
	 */
	RString Name;

	/**
	* Parameter.
	*/
	RContainer<RString,true,false> Params;

public:

	/**
	* Construct a "for" instruction.
	* @param prg            Program.
	* @param t              Indentation.
	* @param sub            Name of the subroutine
	* @param params         Parameters
	*/
	RPrgInstSub(RInterpreter* prg,size_t t,const RString& sub,RContainer<RPrgVar,false,false>& params);

	/**
	* This method compares two functions.
	* @see R::RContainer.
	*/
	int Compare(const RPrgInstSub& t) const {return(Name.Compare(t.Name));}

	/**
	* This method compares two functions.
	* @see R::RContainer.
	*/
	int Compare(const RString& t) const {return(Name.Compare(t));}

	/**
	* Run the instructions in the for for the different values of its variable.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void RunBlock(RInterpreter* prg,RPrgOutput* o);

	/**
	* Execute the function.
	* @param prg            Program.
	* @param o              Output.
	* @param args           Parameters of the function.
	*/
	virtual void Execute(RInterpreter* prg,RPrgOutput* o,RContainer<RPrgVar,true,false>& args);

	/**
	* Destruct the "for" instruction.
	*/
	virtual ~RPrgInstSub(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
