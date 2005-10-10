/*

	R Project Library

	RXMLTag.h

	XML tag - Header.

	Copyright 2000-2005 by the Universit�Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef RXMLTag_H
#define RXMLTag_H



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rnode.h>
#include <rxmlattr.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Forward class declaration
class RXMLStruct;
class RXMLFile;
class RXMLTagCursor;


//-----------------------------------------------------------------------------
/**
* This class provides a representation for a XML tag. It inherits from RNode.
* @code
* #include <rxmlstruct.h>
* #include <rxmlattr.h>
* #include <rcursor.h>
* using namespace R;
*
* void ViewAttr(RXMLTag* tag)
* {
*    cout<<"The tag "<<tag->GetName()<<" has the following attributes:"<<endl;
*    RCursor<RXMLAttr> Cur(tag->GetAttrs());
*    for(Cur.Start();!Cur.End();Cur.Next())
*       cout<<Cur()->GetName()<<"="<<Cur()->GetValue()<<endl;
* }
* @endcode
* @short XML Tag.
* @author Pascal Francq and Thomas L'Eglise.
*/
class RXMLTag : public RNode<RXMLTag,true,false>
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
	RXMLTag(RXMLStruct* xml);

	/**
	* Construct a XML Tag.
	* @param name          The name of the tag.
	*/
	RXMLTag(RXMLStruct* xml,const RString& name);

	/**
	* Compare a tag with a given name.
	* @param name           Name used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RString& name) const;

	/**
	* Compare two tags.
	* @param tag            Tag used for the comparaison.
	* @returns 0 if the same, -1 or +1 if different.
	*/
	int Compare(const RXMLTag& tag) const;

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
	RString GetAttrValue(RString name) const;

	/**
	* Test if an attribute is defined.
	* param name            Name of the attribute.
	* @returns True if the attribute exists.
	*/
	bool IsAttrDefined(RString name) const;

	/**
	* Find the first tag with a gtiven name.
	* @param name           Name of the tag to find.
	* @return Pointer to the element.
	*/
	RXMLTag* GetTag(const RString& name) const;

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
	* Add a string to the content of the tag.
	* @param text           Text to add.
	*/
	void AddContent(const RString& text);

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
	virtual bool IsEmpty(void);

	/**
	* Get a Cursor on the attributes.
	* @return GXMLAttrCursor.
	*/
	R::RCursor<RXMLAttr> GetAttrs(void);

	/**
	* Destruct the XML Tag.
	*/
	virtual ~RXMLTag(void);

	// friend classes
	friend class RXMLStruct;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
