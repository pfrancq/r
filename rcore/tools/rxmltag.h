/*

	R Project Library

	RXMLTag.h

	XML tag - Header.

	Copyright 2000-2004 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	long with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RXMLTag_H
#define RXMLTag_H



//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rnode.h>
#include <rstd/rcursor.h>
#include <rstd/rxmlattr.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RXMLStruct;
class RXMLFile;
class RXMLTagCursor;


//------------------------------------------------------------------------------
/**
* This class provides a representation for a XML tag.
* @short XML Tag.
* @author Pascal Francq and Thomas L'Eglise.
*/
class RXMLTag : public RNode<RXMLTag,false>
{
	/**
	* The name of the tag.
	*/
	RString Name;

	/**
	* The contain of the tag
	*/
	RString Contains;

	/**
	* The attributes of the tag.
	*/
	RContainer<RXMLAttr,true,true> Attrs;

public:

	/**
	* Construct a XML Tag.
	*/
	RXMLTag(void);

	/**
	* Construct a XML Tag.
	* @param _name          The name of the tag.
	*/
	RXMLTag(RString _name);

	/**
	* Compare a tag with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RString& name) const {return(Name.Compare(name));}

	/**
	* Compare a tag with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const char* name) const {return(Name.Compare(name));}

	/**
	* Compare two tags.
	* @param tag            Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RXMLTag* tag) const {return(Name.Compare(tag->Name));}

	/**
	* Compare two tags.
	* @param tag            Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RXMLTag& tag) const {return(Name.Compare(tag.Name));}

	/**
	* Return the name of the tag.
	* @returns a string containing the name.
	*/
	RString GetName(void) const {return(Name);}

	/**
	* Set the name of the tag.
	* @param name           Name of the tag.
	*/
	void SetName(const RString& name);

	/**
	* Return the name of the tag.
	* param name            Name of the attribute.
	* @returns a string containing the name.
	*/
	RString GetAttrValue(const char* name);

	/**
	* Return the name of the tag.
	* param name            Name of the attribute.
	* @returns a string containing the name.
	*/
	RString GetAttrValue(RString name);

	/**
	* Test if an attribute is defined.
	* param name            Name of the attribute.
	* @returns True if the attribute exists.
	*/
	bool IsAttrDefined(RString name);

	/**
	* Find the first tag with a gtiven name.
	* @param name           Name of the tag to find.
	* @return Pointer to the element.
	*/
	RXMLTag* GetTag(const char* name);

	/**
	* Find the first tag with a gtiven name.
	* @param name           Name of the tag to find.
	* @return Pointer to the element.
	*/
	RXMLTag* GetTag(RString name);

	/**
	* Insert an attribute.
	* @param Attr           Attribute to insert
	*/
	void InsertAttr(RXMLAttr* Attr);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const RString& name,const RString& value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const char* name,const RString& value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const RString& name,const char* value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const char* name,const char* value);

	/**
	* Insert an attribute.
	* @param name           Name of thae attribute.
	* @param value          Value of thae attribute.
	*/
	void InsertAttr(const RChar* name,const RChar* value);

	/**
	* Add a string to the content of the tag.
	* @param text           Text to add.
	*/
	void AddContent(const char* text);

	/**
	* Add a string to the content of the tag.
	* @param text           Text to add.
	*/
	void AddContent(const RChar* text);

	/**
	* Add a string to the content of the tag.
	* @param text           Text to add.
	*/
	void AddContent(RString text);

	/**
	* @returns the text containning in the tag.
	*/
	RString GetContent(void) const {return(Contains);}

	/**
	* See if the tag has a content.
	* @returns bool.
	*/
	bool HasContent(void) const {return(Contains.GetLen());}

	/**
	* Test if the tag is empty, i.e. it has no subnodes, no parameters and no
	* content.
	*/
	bool IsEmpty(void);

	/**
	* Delete all the empty subtags attached to the current tag.
	* @param s              Structure of the node.
	*/
	void DeleteEmptyTags(RXMLStruct* s);

	/**
	* Get a Cursor on the attributes.
	* @return GXMLAttrCursor.
	*/
	RXMLAttrCursor GetXMLAttrCursor(void);

	/**
	* Get a Cursor on the sub tags.
	* @return GXMLTagCursor.
	*/
	RXMLTagCursor GetXMLTagsCursor(void);

	/**
	* Destruct the XML Tag.
	*/
	~RXMLTag(void);

	// friend classes
	friend class RXMLStruct;
	friend class RXMLTagCursor;
};


//------------------------------------------------------------------------------
/**
* The RXMLTagCursor class provides a way to go trough a set of XML tags.
* @short XML Tags Cursor
*/
CLASSCURSOR(RXMLTagCursor,RXMLTag);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
