/*

	R Project Library

	RXMLAttr.h

	Attribute for a XML Tag - Header.

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2001 by Thomas L'Eglise.
	Copyright 2000-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef RXMLAttr_H
#define RXMLAttr_H


//------------------------------------------------------------------------------
// include files for R Project
#include <ruri.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation for a attribute of a XML tag.
* @short XML Tag.
* @see RXMLTag
*/
class RXMLAttr
{
	/**
	* The name of the attribute.
	*/
	RString Name;

	/**
	* The value of the attribute.
	*/
	RString Value;

	/**
	 * Namespace.
	 */
	RURI* Namespace;

public:

	/**
	* Copy constructor.
	*/
	RXMLAttr(const RXMLAttr& attr);

	/**
	* Construct a XML Attribute.
	* @param name           The name of the attribute.
	* @param value          The value of the attribute.
	* @param xmlns          Namespace.
	*/
	RXMLAttr(const RString& name,const RString& value,RURI* xmlns=0);

	/**
	* Construct a XML Attribute.
	* @param name           The name of the attribute.
	* @param value          The value of the attribute.
	* @param xmlns          Namespace.
	*/
	RXMLAttr(const RString& name,const double value,RURI* xmlns=0);

	/**
	* Return the namespace of the tag.
	* @returns a string containing the namespace.
	*/
	RURI* GetNamespace(void) const {return(Namespace);}

	/**
	* Compare the name of the attribute with a given string. This function is
	* used by RContainer.
	* @param name           The string used for the comparison.
	* @returns -1,0 or 1 if the attribute is greater, the same or lesser than
	* parameter.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* Compare the names of two attributes. This function is used by RContainer.
	* @param attr           The attribute used for the comparison.
	* @returns -1,0 or 1 if the attribute is greater, the same or lesser than
	* parameter.
	*/
	int Compare(const RXMLAttr& attr) const {return(Name.Compare(attr.Name)); }

	/**
	* Return the full name of the attribute (including the namespace).
	* @returns a string containing the name.
	*/
	RString GetFullName(void) const;

	/**
	* Return the name of the attribute.
	* @returns a string containing the name.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Returns the value of the attribute.
	* @returns a string containing the value;
	*/
	RString GetValue(void) const {return(Value);}

	/**
	* Set a new value to the attribute.
	* @param value           Value to assign.
	*/
	void SetValue(const RString& value);

	/**
	* Add something to the current value of the attribute.
	*/
	void AddValue(const RString& value);

	/**
	* Destruct the attribute.
	*/
	virtual ~RXMLAttr(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
