/*

	R Project Library

	RApplication.h

	Generic Application - Header.

	Copyright 2006 by Pascal Francq.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
#ifndef RApplication_H
#define RApplication_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RApplication class provides a representation for a generic application.
* @author Pascal Francq
* @short Generic Application.
*/
class RApplication
{
	/**
	* Name of the application.
	*/
	RString Name;

	/**
	* Name of executable file.
	*/
	RString File;

	/**
	* Arguments of the application.
	*/
	RContainer<RString,true,false> Args;

public:

	/**
	* Construct an application.
	* @param name            Name of the application.
	* @param argc            Number of arguments of the program (received from
	*                        main).
	* @param argv            Arguments of the program (received from main).
	*/
	RApplication(const RString& name,int argc, char** argv);

	/**
	* Get the name of the application.
	*/
	RString GetName(void) const;

	/**
	* Get the name of the file representing the executable.
	*/
	RString GetApplicationFile(void) const;

	/**
	* Initialize the application.
	*/
	virtual void Init(void);

	/**
	* Run the application.
	*/
	virtual void Run(void);

	/**
	* Destructor of the application.
	*/
	virtual ~RApplication(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
