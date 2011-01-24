/*

	R Project Library

	RObjG.h

	Objects to Group - Header.

	Copyright 2001-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2001-2008 by the Université Libre de Bruxelles (ULB).

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



//------------------------------------------------------------------------------
#ifndef RObjG_H
#define RObjG_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RObjg class provides a representation of an object to place in a group.
* @author Pascal Francq
* @short Object.
*/
class RObjG
{
protected:

	/**
	* The identifier of the object.
	*/
	size_t Id;

	/**
	* Name of the object.
	*/
	RString Name;

public:

	/**
	* Construct the object.
	* @param id             Identifier.
	* @param name           Name of the object.
	*/
	RObjG(const size_t id,const RString& name);

	/**
	* Comparison function
	*/
	int Compare(const size_t id) const;

	/**
	* Comparison function
	*/
	int Compare(const RObjG& obj) const;

	/**
	* Comparison function
	*/
	int Compare(const RObjG* obj) const;

	/**
	* Return the identifier of the object.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Return the Name of the object.
	*/
	RString GetName(void) const;

	/**
	* Destruct the object.
	*/
	virtual ~RObjG(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
