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
	: RTree<RXMLTag,true,false>(100,50), Entities(20,10), Version("1.0"), Encoding("UTF-8")
{
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(const RString& name) const
{
	return(GetNode(name,false));
}


//-----------------------------------------------------------------------------
RString RXMLStruct::GetTagAttrValue(const RString& tag,const RString& attr) const
{
	RXMLTag* find=GetNode(tag,false);
	if(!find)
		return(RString::Null);
	return(find->GetAttrValue(attr));
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(const RString& name,const RXMLTag* parent) const
{
	if(!parent)
		return(GetNode(name,false));
	return(parent->GetNode(name,false));
}


//-----------------------------------------------------------------------------
RString RXMLStruct::GetTagAttrValue(const RString& tag,const RString& attr,const RXMLTag* parent) const
{
	if(!parent)
		GetTagAttrValue(tag,attr);
	RXMLTag* find=parent->GetNode(tag,false);
	if(!find)
		return(RString::Null);
	return(find->GetAttrValue(attr));
}


//-------------------------------------------------------------------------------
void RXMLStruct::AddTag(RXMLTag* parent,RXMLTag* tag)
{
	RTree<RXMLTag,true,false>::InsertNode(parent,tag);
}


//-------------------------------------------------------------------------------
void RXMLStruct::DeleteTag(RXMLTag* tag)
{
	RTree<RXMLTag,true,false>::DeleteNode(tag);
}


//-------------------------------------------------------------------------------
void RXMLStruct::InsertEntity(const RString& name,const RString& value)
{
	Entities.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
R::RCursor<RXMLAttr> RXMLStruct::GetXMLEntitiesCursor(void) const
{
	return(R::RCursor<RXMLAttr>(Entities));
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
