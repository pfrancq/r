/*

	R Project Library

	RXMLTag.cpp

	XML tag - Implementation.

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
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//------------------------------------------------------------------------------
// include files foor R Project
#include <rstd/rxmltag.h>
#include <rstd/rxmlstruct.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RXMLTag
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLTag::RXMLTag(void)
	: RNode<RXMLTag,false>(100,50), Attrs(20,10)
{
}


//------------------------------------------------------------------------------
RXMLTag::RXMLTag(RString _name)
	: RNode<RXMLTag,false>(100,50), Name(_name), Attrs(20,10)
{
}


//------------------------------------------------------------------------------
void RXMLTag::SetName(const RString& name)
{
	Name=name;
}


//------------------------------------------------------------------------------
RString RXMLTag::GetAttrValue(const char* name)
{
	RXMLAttr *attr;

	attr=Attrs.GetPtr<const char*>(name);
	if(attr)
		return(attr->GetValue());
	return("");
}


//------------------------------------------------------------------------------
RString RXMLTag::GetAttrValue(RString name)
{
	RXMLAttr *attr;

	attr=Attrs.GetPtr<RString>(name);
	if(attr)
		return(attr->GetValue());
	return("");
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(RXMLAttr* Attr)
{
	Attrs.InsertPtr(Attr);
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(const RString& name,const RString& value)
{
	Attrs.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(const char* name,const RString& value)
{
	Attrs.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(const RString& name,const char* value)
{
	Attrs.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(const char* name,const char* value)
{
	Attrs.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(const RChar* name,const RChar* value)
{
	Attrs.InsertPtr(new RXMLAttr(name,value));
}


//------------------------------------------------------------------------------
bool RXMLTag::IsAttrDefined(RString name)
{
	return(Attrs.IsIn<RString>(name));
}


//------------------------------------------------------------------------------
RXMLTag* RXMLTag::GetTag(const char* name)
{
	return(GetPtr<const char*>(name,false));
}


//------------------------------------------------------------------------------
RXMLTag* RXMLTag::GetTag(RString name)
{
	return(GetPtr<RString>(name,false));
}


//------------------------------------------------------------------------------
void RXMLTag::AddContent(const char* text)
{
	Contains+=text;
}


//------------------------------------------------------------------------------
void RXMLTag::AddContent(const RChar* text)
{
	Contains+=text;
}

//------------------------------------------------------------------------------
void RXMLTag::AddContent(RString text)
{
	Contains+=text;
}


//------------------------------------------------------------------------------
bool RXMLTag::IsEmpty(void)
{
	return((!Contains.GetLen())&&(!GetNb())&&(!Attrs.GetNb()));
}


//------------------------------------------------------------------------------
void RXMLTag::DeleteEmptyTags(RXMLStruct* s)
{
#warning verify this behavior
	unsigned int i;

	// Go through the subtags.
	RCursor<RXMLTag> ptr(*this);
	for(i=GetNb()+1,ptr.Start();--i;)
	{
		ptr()->DeleteEmptyTags(s);
		if(ptr()->IsEmpty())
		{
			// If the tags is empty -> Delete it but don't increase ptr
			// because DeletePtr will move everything to the left
			s->DeleteTag(ptr());
		}
		else
			ptr.Next();  // No empty tag -> Go to the next.
	}
}


//------------------------------------------------------------------------------
RCursor<RXMLAttr> RXMLTag::GetXMLAttrCursor(void)
{
	return(RCursor<RXMLAttr>(Attrs));
}


//------------------------------------------------------------------------------
RCursor<RXMLTag> RXMLTag::GetXMLTagsCursor(void)
{
	return(RCursor<RXMLTag>(*this));
}


//------------------------------------------------------------------------------
RXMLTag::~RXMLTag(void)
{
}
