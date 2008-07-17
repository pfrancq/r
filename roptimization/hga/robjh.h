/*

	R Project Library

	RObjH.h

	Object to place at a specific Node - Header.

	Copyright 2001-2008 by the Université Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef RObjH_H
#define RObjH_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rattrlist.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RobjH class provides a representation of an object to place in the tree.
* @author Pascal Francq
* @short Object.
*/
class RObjH
{
public:
	/**
	* The identificator of the object.
	*/
	size_t Id;

	/**
	* Name of the object.
	*/
	RString Name;

	/**
	* The list of attributes for the object.
	*/
	RAttrList Attr;

public:

	/**
	* Construct the object.
	* @param id             Identificator.
	* @param name           Name of the object.
	* @param nb             Number of attributes representing the object.
	*/
	RObjH(const size_t id,const RString& name,const size_t nb);

	/**
	* Copy constructor.
	* @param obj            Object to copy.
	*/
	RObjH(const RObjH* obj);

	/**
	* Compare function used by container.
	*/
	int Compare(const RObjH& obj) const {return(Id-obj.Id);}

	/**
	* Return the object identifier.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Return the Name of the object.
	*/
	RString GetName(void) const;

	/**
	* Return a reference of the attributes.
	*/
	const RAttrList& GetAttr(void) const {return(Attr);}

	/**
	* Add a new attribute to the object.
	* @param a              Attribute to add.
	*/
	void AddAttribute(const size_t a) {Attr.Insert(a);}

	/**
	* Verify if an object has the exactly the same attributes.
	* @param obj            Object to verify.
	*/
	bool IsSame(const RObjH* obj) const;

	/**
	* Destruct the object.
	*/
	virtual ~RObjH(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
