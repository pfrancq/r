/*

	R Project Library

	RXMLStruct.cpp

	XML Structure - Implementation.

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
// include files for R Project
#include <rstd/rxmlstruct.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RXMLStruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLStruct::RXMLStruct(void)
 : RTree<RXMLTag,true,false>(100,50), Entities(20,10)
{
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(const char* name)
{
	return(GetPtr<const char*>(name,false));
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(const char* name,RXMLTag* parent)
{
	if(!parent)
		return(0);
	return(parent->GetPtr<const char*>(name,false));
}


//-------------------------------------------------------------------------------
void RXMLStruct::InsertEntity(const char* name,const char* value)
{
	Entities.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
RXMLAttrCursor& RXMLStruct::GetXMLEntitiesCursor(void)
{
	RXMLAttrCursor *cur=RXMLAttrCursor::GetTmpCursor();
	cur->Set(Entities);
	return(*cur);
}

//------------------------------------------------------------------------------
RXMLStruct::~RXMLStruct(void)
{
}
