/*

	R Project Library

	RXMLAttr.h

	Attribute for a XML Tag - Header.

	(c) 2000-2001 by P. Francq.

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



//-----------------------------------------------------------------------------
#ifndef RXMLAttrH
#define RXMLAttrH


//-----------------------------------------------------------------------------
// incluce files for R Project
#include <rstd/rstring.h>
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class provides a representation for a attribute of a XML tag.
* @short XML Tag.
* @author Pascal Francq
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
	* @param name		The name of the attribute.
	* @param value		The value of the attribute.
	*	
	*/
	RXMLAttr(char *name,char *value);

	/**
	* Compare the name of the attribute with a given string. This function is
	* used by RContainer.
	* @param name	The C string used for the comparaison.
	* @returns 	-1,0 or 1 if the attribute is greather, the same or lesser than
	*				parameter.
	* @see RContainer
	*/
	int Compare(const char *name) {return(Name.Compare(name));}

	/**
	* Compare the name of the attribute with a given string. This function is
	* used by RContainer.
	* @param name	The string used for the comparaison.
	* @returns 	-1,0 or 1 if the attribute is greather, the same or lesser than
	*				parameter.
	*/
	int Compare(const RString &name) {return(Name.Compare(name));}

	/**
	* Compare the names of two attributes. This function is used by RContainer.
	* @param attr	The attribute used for the comparaison.
	* @returns 	-1,0 or 1 if the attribute is greather, the same or lesser than
	*				parameter.
	*/
	int Compare(const RXMLAttr &attr) { return(Name.Compare(attr.Name)); }

	/**
	* Compare the names of two attributes. This function is used by RContainer.
	* @param attr	The attribute used for the comparaison.
	* @returns 	-1,0 or 1 if the attribute is greather, the same or lesser than
	*				parameter.
	*/
	int Compare(const RXMLAttr *attr) { return(Name.Compare(attr->Name)); }

	/**
	*	Returns the value of the attribute.
	* @returns a string containing the value;
	*/
	RString& GetValue(void);

	/**
	* Destruct the attribute.
	*/
	~RXMLAttr(void);
};


}  //-------- End of namespace RStd -----------------------------------------


//---------------------------------------------------------------------------
#endif
