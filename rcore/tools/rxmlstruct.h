/*

	R Project Library

	RXMLStruct.h

	XML structure - Header.

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
class RXMLStruct : public RTree<RXMLTag,true,false>
{
	/**
	* The entities of the structure.
	*/
	RContainer<RXMLAttr,unsigned,true,true> Entities;

	/**
	* Top tag of the XML structure.
	*/
	RXMLTag* TopTag;

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
	RXMLTag* GetTag(const char* name);

	/**
	* Find the first tag with a gtiven name and a given parent.
	* @param name           Name of the tag to find.
	* @param parent         Parent of the tag to find.
	*/
	RXMLTag* GetTag(const char* name,RXMLTag* parent);

	/**
	* Return the Top Nodes of the XML struct.
	*/
	RXMLTag* GetTop(void);

	/**
	* Insert an entity.
	* @param name           Name of thae entity.
	* @param value          Value of thae entity.
	*/
	void InsertEntity(const char* name,const char* value);

	/**
	* Get a Cursor on the entities.
	* @return GXMLAttrCursor.
	*/
	RXMLAttrCursor& GetXMLEntitiesCursor(void);

	/**
	* Destruct the XML Structure.
	*/
	~RXMLStruct(void);

	friend class RXMLTag;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
