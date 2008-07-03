/*

	R Project Library

	RXMLStruct.cpp

	XML Structure - Implementation.

	Copyright 2000-2008 by the Université Libre de Bruxelles.

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
	: RTree<RXMLTag,true,false>(100,50), Entities(20,10), Version("1.0"),
	  Encoding("UTF-8"), Namespaces(10)
{
}

	
//------------------------------------------------------------------------------
RURI* RXMLStruct::GetNamespace(const RString& xmlns)
{
	return(Namespaces.GetInsertPtr(xmlns));
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
void RXMLStruct::SetDTD(const RString& dtd)
{
	DTD=dtd;
}


//------------------------------------------------------------------------------
RString RXMLStruct::GetDTD(void) const
{
	return(DTD);
}


//------------------------------------------------------------------------------
void RXMLStruct::Clear(void)
{
	RTree<RXMLTag,true,false>::Clear();
	Entities.Clear();
	Version.Clear();
	Encoding.Clear();
	Namespaces.Clear();	
}


//------------------------------------------------------------------------------
bool RXMLStruct::Compare(const RXMLTag* tag1,const RXMLTag* tag2)
{
	// Test the name
	if(tag1->GetName()!=tag2->GetName())
		return(false);

	// Go trought the attributes of tag1 and verify if a same attributes exist
	// in tag2
	RCursor<RXMLAttr> Cur(tag1->GetAttrs());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RXMLAttr* attr=tag2->GetAttr(Cur()->GetName());
		if(attr&&(attr->GetValue()!=Cur()->GetValue()))
			return(false);
	}

	return(true);
}


//------------------------------------------------------------------------------
bool RXMLStruct::Merge(const RXMLStruct& xml)
{
	return(Top->Merge(xml.Top));
}


//------------------------------------------------------------------------------
RXMLTag* RXMLStruct::NewTag(const RString& tag,const RString& xmlns)
{
	RURI* uri(0);
	if(!xmlns.IsEmpty())
		uri=GetNamespace(xmlns);
	return(new RXMLTag(tag,uri));
}


//------------------------------------------------------------------------------
RXMLAttr* RXMLStruct::NewAttr(const RString& name,const RString& value,const RString& xmlns)
{
	RURI* uri(0);
	if(!xmlns.IsEmpty())
		uri=GetNamespace(xmlns);	
	return(new RXMLAttr(name,value,uri));
}


//------------------------------------------------------------------------------
RXMLAttr* RXMLStruct::NewAttr(const RString& name,const RString& xmlns)
{
	RURI* uri(0);
	if(!xmlns.IsEmpty())
		uri=GetNamespace(xmlns);	
	return(new RXMLAttr(name,RString::Null,uri));
}


//------------------------------------------------------------------------------
RXMLStruct::~RXMLStruct(void)
{
}
