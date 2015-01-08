/*

	R Project Library

	RPrgVarInst.h

	Variable representing a instance of class - Header.

	Copyright 2008-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RPrgVarInstH
#define RPrgVarInstH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward class declaration
class RPrg;
class RPrgClass;


//------------------------------------------------------------------------------
/**
* The RPrgVarInst provides a class for an instance of a class.
* @author Pascal Francq
* @short Class Instance.
*/
class RPrgVarInst : public RPrgVar
{
protected:

	/**
	 * Class of the variable.
	 */
	RPrgClass* Class;

public:

	/**
	* Construct an instance of a class.
	* @param name           Name.
	* @param c              Class.
	*/
	RPrgVarInst(const RString& name,RPrgClass* c);

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
	 * Get the class of the instance.
	 */
	RPrgClass* GetClass(void) const {return(Class);}

	/**
	* Destruct the variable.
	*/
	virtual ~RPrgVarInst(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
