/*

	R Project Library

	RPrgVarLiteral.h

	Const Variable - Header.

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
#ifndef RPrgVarLiteralH
#define RPrgVarLiteralH


//------------------------------------------------------------------------------
// include files for R Project
#include <rprgvar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPrgVarLiteral provides a class for a literal variable of the 'String'.
* @author Pascal Francq
* @short Literal Variable.
*/
class RPrgVarLiteral : public RPrgVar
{
public:

	/**
	* Construct the literal variable.
	* @param value          Name.
	*/
	RPrgVarLiteral(const RString& value);

	/**
	* Assign some data to the literal variable.
	* @param data           Data.
	*/
	virtual void Assign(const void* data);

	/**
	* Get the value of the literal variable.
	* @param prg            Program.
	*/
	virtual RString GetValue(RInterpreter* prg);

	/**
	* Destruct the literal variable.
	*/
	virtual ~RPrgVarLiteral(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
