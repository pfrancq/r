/*

	R Project Library

	RXMLAttr.h

	Attribute for a XML Tag - Header.

	Copyright 2000-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

	Version $Revision$

	Last Modify: $Date$

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
// incluce files for R Project
#include <rstd/rcursor.h>
#include <rstd/rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation for a attribute of a XML tag.
* @short XML Tag.
* @author Pascal Francq and Thomas L'Eglise.
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

public:

	/**
	* Construct a XML Attribute.
	* @param name           The name of the attribute.
	* @param value          The value of the attribute.
	*/
	RXMLAttr(const RString& name,const RString& value);

	/**
	* Construct a XML Attribute.
	* @param name           The name of the attribute.
	* @param value          The value of the attribute.
	*/
	RXMLAttr(const char* name,const double value);

	/**
	* Compare the name of the attribute with a given string. This function is
	* used by RContainer.
	* @param name           The C string used for the comparaison.
	* @returns -1,0 or 1 if the attribute is greather, the same or lesser than
	* parameter.
	* @see RContainer
	*/
	int Compare(const char* name) {return(Name.Compare(name));}

	/**
	* Compare the name of the attribute with a given string. This function is
	* used by RContainer.
	* @param name           The string used for the comparaison.
	* @returns -1,0 or 1 if the attribute is greather, the same or lesser than
	* parameter.
	*/
	int Compare(const RString &name) {return(Name.Compare(name));}

	/**
	* Compare the names of two attributes. This function is used by RContainer.
	* @param attr           The attribute used for the comparaison.
	* @returns -1,0 or 1 if the attribute is greather, the same or lesser than
	* parameter.
	*/
	int Compare(const RXMLAttr &attr) { return(Name.Compare(attr.Name)); }

	/**
	* Compare the names of two attributes. This function is used by RContainer.
	* @param attr           The attribute used for the comparaison.
	* @returns -1,0 or 1 if the attribute is greather, the same or lesser than
	* parameter.
	*/
    int Compare(const RXMLAttr* attr) { return(Name.Compare(attr->Name)); }

	/**
	* Return the name of the tag.
	* @returns a string containing the name.
	*/
	const RString& GetName(void) const {return(Name);}

	/**
	* Returns the value of the attribute.
	* @returns a string containing the value;
	*/
	const RString& GetValue(void) const {return(Value);}

	/**
	* Destruct the attribute.
	*/
	~RXMLAttr(void);
};


//------------------------------------------------------------------------------
/**
* The RXMLAttrCursor class provides a way to go trough a set of XML attributes.
* @short XML attributes Cursor
*/
CLASSCURSOR(RXMLAttrCursor,RXMLAttr,unsigned int)


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
