/*

	R Project Library

	RXMLTag.cpp

	XML tag - Implementation.

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
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//-----------------------------------------------------------------------------
// include files foor R Project
#include <rxmltag.h>
#include <rxmlstruct.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// class RXMLTag
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RXMLTag::RXMLTag(void)
	: RNode<RXMLTag,false>(100,50), Name(), Contains(), Attrs(20,10)
{
}


//-----------------------------------------------------------------------------
RXMLTag::RXMLTag(const RString& name)
	: RNode<RXMLTag,false>(100,50), Name(name), Contains(), Attrs(20,10)
{
}


//-----------------------------------------------------------------------------
int RXMLTag::Compare(const RString& name) const
{
	return(Name.Compare(name));
}


//-----------------------------------------------------------------------------
int RXMLTag::Compare(const RXMLTag& tag) const
{
	return(Name.Compare(tag.Name));
}


//-----------------------------------------------------------------------------
void RXMLTag::SetName(const RString& name)
{
	Name=name;
}


//-----------------------------------------------------------------------------
RString RXMLTag::GetAttrValue(RString name) const
{
	RXMLAttr* attr;

	attr=Attrs.GetPtr<RString>(name);
	if(attr)
		return(attr->GetValue());
	return("");
}


//-----------------------------------------------------------------------------
bool RXMLTag::IsAttrDefined(RString name) const
{
	return(Attrs.IsIn<RString>(name));
}


//-----------------------------------------------------------------------------
RXMLTag* RXMLTag::GetTag(const RString& name) const
{
	return(GetPtr(name,false));
}


//-----------------------------------------------------------------------------
void RXMLTag::InsertAttr(RXMLAttr* Attr)
{
	Attrs.InsertPtr(Attr);
}


//-----------------------------------------------------------------------------
void RXMLTag::InsertAttr(const RString& name,const RString& value)
{
	Attrs.InsertPtr(new RXMLAttr(name,value));
}


//-----------------------------------------------------------------------------
void RXMLTag::AddContent(const RString& text)
{
	Contains+=text;
}


//-----------------------------------------------------------------------------
bool RXMLTag::IsEmpty(void)
{
	return((!Contains.GetLen())&&(!GetNbNodes())&&(!Attrs.GetNb()));
}


//-----------------------------------------------------------------------------
void RXMLTag::DeleteEmptyTags(RXMLStruct* s)
{
#warning verify this behavior
	RContainer<RXMLTag,false,false> ToDel(20,10);

	// Go through the subtags.
	RCursor<RXMLTag> Cur(GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		Cur()->DeleteEmptyTags(s);
		if(Cur()->IsEmpty())
			ToDel.InsertPtr(Cur());
	}

	Cur.Set(ToDel);
	int i;
	for(i=1,Cur.Start();!Cur.End();Cur.Next(),i++)
		s->DeleteTag(Cur());
}


//-----------------------------------------------------------------------------
RCursor<RXMLAttr> RXMLTag::GetAttr(void)
{
	return(RCursor<RXMLAttr>(Attrs));
}


//-----------------------------------------------------------------------------
RXMLTag::~RXMLTag(void)
{
}
