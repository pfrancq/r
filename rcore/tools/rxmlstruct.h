/*

	R Project Library

	RXMLStruct.h

	XML structure - Header.

	Copyright 2000-2005 by the Université Libre de Bruxelles.

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
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//----------------------------------------------------------------------------
#ifndef RXMLStruct_H
#define RXMLStruct_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rtree.h>
#include <rxmltag.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation of a XML structure. It is implemented as
* a RTree of RXMLTag.
* @code
* #include <rxmlstruct.h>
* #include <rcursor.h>
* using namespace R;
*
* RXMLStruct XML;
* XML.AddTag(0,new RXMLTag(&XML,"html"));
* XML.AddTag(XML.GetTop(),new RXMLTag(&XML,"head"));
* XML.AddTag(XML.GetTop(),new RXMLTag(&XML,"body"));
* ...
* // Parse all the child nodes of the body tag
* RCursor<RXMLTag> Cur(XML.GetTag("body")->GetNodes());
* for(Cur.Start();!Cur.End();Cur.Next())
*    cout<<Cur()->GetName()<<endl;
* @endcode
* @short XML Structure.
* @author Pascal Francq and Thomas L'Eglise.
*/
class RXMLStruct : public RTree<RXMLTag,true,false>
{
	/**
	* The entities of the structure.
	*/
	RContainer<RXMLAttr,true,true> Entities;

	/**
	* Version of the XML standard.
	*/
	RString Version;

	/**
	* Encoding of the corresponding XML file.
	*/
	RString Encoding;

public:

	/**
	* Construct the XML Structure.
	*/
	RXMLStruct(void);

	/**
	* Find the first tag with a given name.
	* @param name           Name of the tag to find.
	* @returns a pointer to the tag of 0 if no tag exists.
	*/
	RXMLTag* GetTag(const RString& name) const;

	/**
	* Get the value of an attribute of the first tag with a given name.
	* param tag             Name of the tag.
	* param attr            Name of the attribute.
	* @returns a string containing the value of the attribute or a null string
	* if either the tag wasn't foud or the attribute was not found.
	*/
	RString GetTagAttrValue(const RString& tag,const RString& attr) const;

	/**
	* Find the first tag with a given name and a given parent.
	* @param name           Name of the tag to find.
	* @param parent         Parent of the tag to find.
	*/
	RXMLTag* GetTag(const RString& name,const RXMLTag* parent) const;

	/**
	* Get the value of an attribute of the first tag with a given name and a
	* given parent.
	* @param tag            Name of the tag.
	* @param attr           Name of the attribute.
	* @param parent         Parent of the tag to find.
	* @returns a string containing the value of the attribute or a null string
	* if either the tag wasn't foud or the attribute was not found.
	*/
	RString GetTagAttrValue(const RString& tag,const RString& attr,const RXMLTag* parent) const;

	/**
	* Add a tag to the a given one which will be the parent. If the parent is
	* null the tag is considered as the root tag in the XML structure.
	* @param parent         The parent tag.
 	* @param tag            The tag to add.
	*/
	void AddTag(RXMLTag* parent,RXMLTag* tag);

	/**
	* Delete a tag from the XML structure.
	* @param tag            The tag to delete.
	*/
	void DeleteTag(RXMLTag* tag);

	/**
	* Insert an entity.
	* @param name           Name of thae entity.
	* @param value          Value of thae entity.
	*/
	void InsertEntity(const RString& name,const RString& value);

	/**
	* Get a Cursor on the entities.
	* @return GXMLAttrCursor.
	*/
	R::RCursor<RXMLAttr> GetXMLEntitiesCursor(void) const;

	/**
	* Set the version of the XML standard.
	*/
	void SetVersion(const RString& version);

	/**
	* Get the version of the XML standard.
	* @return RString.
	*/
	RString GetVersion(void) const;

	/**
	* Set the encoding of the corresponding XML file.
	*/
	void SetEncoding(const RString& encoding);

	/**
	* Get the encoding of the corresponding XML file.
	* @return RString.
	*/
	RString GetEncoding(void) const;

	/**
	* Compare two tags to see if it is the same tag. By default, the names
	* must be the same, and that all common attributes have the same values.
	* @param tag1            First tag.
	* @param tag2            Second tag.
	*/
	virtual bool Compare(const RXMLTag* tag1,const RXMLTag* tag2);

	/**
	* Merge two xml structures.
	* @param xml             XML structure to merge.
	* @return true if the merge could be done.
	*/
	bool Merge(const RXMLStruct& xml);

	/**
	* Create a new tag with a given name. By default, the method creates a
	* RXMLTag.
	* @param tag             Name of the tag.
	* @return Pointer to a tag.
	*/
	virtual RXMLTag* NewTag(const RString& tag);

	/**
	* Create a new attribute with a given name and value. By default, the
	* method creates a RXMLAttr.
	* @param name            Name of the attribute.
	* @param value           Value of the attribute.
	* @return Pointer to a attribute.
	*/
	virtual RXMLAttr* NewAttr(const RString& name,const RString& value);

	/**
	* Destruct the XML Structure.
	*/
	virtual ~RXMLStruct(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
