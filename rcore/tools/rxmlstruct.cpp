/*

	R Project Library

	RXMLStruct.cpp

	XML Structure - Implementation.

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



//------------------------------------------------------------------------------
// include files for R Project
#include <rxmlstruct.h>
#include <rcursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RXMLStruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLStruct::RXMLStruct(void)
	: RTree<RXMLTag,true,false>(100,50), Entities(20,10), TopTag(0),
	  Version("1.0"), Encoding("UTF-8")
{
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(RString name)
{
	return(GetPtr(name,false));
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(RString name,RXMLTag* parent)
{
	if(!parent)
		return(0);
	return(parent->GetPtr(name,false));
}


//-------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTop(void)
{
	return(TopTag);
}


//-------------------------------------------------------------------------------
void RXMLStruct::AddTag(RXMLTag* parent,RXMLTag* tag)
{
	RTree<RXMLTag,true,false>::AddNode(parent,tag);
	if(!parent)
	{
		//if(TopTag) throw RException("Problem");
		TopTag=tag;
	}
}


//-------------------------------------------------------------------------------
void RXMLStruct::DeleteTag(RXMLTag* tag)
{
	RTree<RXMLTag,true,false>::DeleteNode(tag);
}


//-------------------------------------------------------------------------------
void RXMLStruct::InsertEntity(RString name,RString value)
{
	Entities.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
R::RCursor<RXMLAttr> RXMLStruct::GetXMLEntitiesCursor(void)
{
	R::RCursor<RXMLAttr> cur(Entities);
	return(cur);
}


//------------------------------------------------------------------------------
void RXMLStruct::SetVersion(const RString& version)
{
	Version=version;
}


//------------------------------------------------------------------------------
RString RXMLStruct::GetVersion(void) const
{
	return(Version);
}


//------------------------------------------------------------------------------
void RXMLStruct::SetEncoding(const RString& encoding)
{
	Encoding=encoding;
}


//------------------------------------------------------------------------------
RString RXMLStruct::GetEncoding(void) const
{
	return(Encoding);
}


//------------------------------------------------------------------------------
RXMLStruct::~RXMLStruct(void)
{
}
