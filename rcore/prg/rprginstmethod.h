/*

	R Project Library

	RPrgInstMethod.h

	Method of a class - Header.

	Copyright 2002-2003 by the Universit� Libre de Bruxelles.

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
#ifndef RPrgInstMethodH
#define RPrgInstMethodH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rprg/rprginst.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrgFunc;
class RPrgVar;


//------------------------------------------------------------------------------
/**
* The RPrgInstMethod provides a class for a class method.
* @author Pascal Francq
* @short Class Method.
*/
class RPrgInstMethod : public RPrgInst
{
protected:

	/**
	* Pointer to the method.
	*/
	RPrgFunc* Method;

	/**
	* Parameters of the method.
	*/
	RContainer<RPrgVar,unsigned int,true,false> Params;

public:

	/**
	* Constructor of a class method.
	* @param inst           Function to execute.
	*/
	RPrgInstMethod(RPrgFunc* inst) throw(std::bad_alloc);

	/**
	* Add a parameter to the method.
	*/
	void AddParam(RPrgVar* var) throw(std::bad_alloc);

	/**
	* Run the method.
	* @param prg            Program.
	* @param o              Output.
	*/
	virtual void Run(RPrg* prg,RPrgOutput* o) throw(RException);

	/**
	* Destructor of a class method.
	*/
	virtual ~RPrgInstMethod(void);
};


}  //-------- End of namespace R ----------------------------------------


//-----------------------------------------------------------------------------
#endif 