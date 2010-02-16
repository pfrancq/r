/*

	R Project Library

	RPrg.h

	Script Program - Header.

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
#ifndef RPrgH
#define RPrgH


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
#include <rstack.h>
#include <rcursor.h>
#include <rinterpreter.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RPrg provides a class for a script program.
* @author Pascal Francq
* @short Script Program.
*/
class RPrg : public RTextFile
{
public:

	/**
	* Constructor of a program.
	* @param f              Name of the file.
	*/
	RPrg(const RURI& f);

	/**
	* Destruct of the program.
	*/
	virtual ~RPrg(void);

};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
