/*

	R Project Library

	RXMLStruct.h

	XML structure - Header.

	Copyright 2000-2004 by the Université Libre de Bruxelles.

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
#include <rstd/rtree.h>
#include <rstd/rxmltag.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class provides a representation of a XML structure.
* @short XML Structure.
* @author Pascal Francq and Thomas L'Eglise.
*/
class RXMLStruct : private RTree<RXMLTag,true,false>
{
	/**
	* The entities of the structure.
	*/
	RContainer<RXMLAttr,true,true> Entities;

	/**
	* Top tag of the XML structure.
	*/
	RXMLTag* TopTag;

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
	RXMLTag* GetTag(RString name);

	/**
	* Find the first tag with a gtiven name and a given parent.
	* @param name           Name of the tag to find.
	* @param parent         Parent of the tag to find.
	*/
	RXMLTag* GetTag(RString name,RXMLTag* parent);

	/**
	* Return the Top Nodes of the XML struct.
	*/
	RXMLTag* GetTop(void);

	/**
	* Add a tag to the a given one which will be the parent. If the parent is
	* null the tag is considered as the root tag in the XML structure.
	* @param parent         The parent tag.
 	* @param tag            The tag to add.
	*/
	void AddTag(RXMLTag* parent,RXMLTag* tag) throw(std::bad_alloc);

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
	void InsertEntity(RString name,RString value);

	/**
	* Clear the tree XML structure.
	*/
	void Clear(void);

	/**
	* Get a Cursor on the entities.
	* @return GXMLAttrCursor.
	*/
	RXMLAttrCursor GetXMLEntitiesCursor(void);

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
	* Destruct the XML Structure.
	*/
	~RXMLStruct(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
