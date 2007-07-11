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
RXMLTag::RXMLTag(const RXMLTag& tag)
	: RNode<RXMLTag,true,false>(), Name(tag.Name), Contains(tag.Contains), Attrs(tag.Attrs)
{
}


//-----------------------------------------------------------------------------
RXMLTag::RXMLTag(const RString& name)
	: RNode<RXMLTag,true,false>(), Name(name), Contains(), Attrs(20,10)
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
RString RXMLTag::GetAttrValue(const RString& name) const
{
	RXMLAttr* attr;

	attr=Attrs.GetPtr(name);
	if(attr)
		return(attr->GetValue());
	return(RString::Null);
}


//-----------------------------------------------------------------------------
RXMLAttr* RXMLTag::GetAttr(const RString& name) const
{
	return(Attrs.GetPtr(name));
}


//-----------------------------------------------------------------------------
bool RXMLTag::IsAttrDefined(const RString& name) const
{
	return(Attrs.IsIn<RString>(name));
}


//-----------------------------------------------------------------------------
RXMLTag* RXMLTag::GetTag(const RString& name) const
{
	return(GetNode(name,false));
}


//-----------------------------------------------------------------------------
void RXMLTag::GetTags(const RString& name,RContainer<RXMLTag,false,false>& find) const
{
	RCursor<RXMLTag> Tags(GetNodes());
	for(Tags.Start();!Tags.End();Tags.Next())
		if(Tags()->GetName()==name)
			find.InsertPtr(Tags());
}


//-----------------------------------------------------------------------------
RString RXMLTag::GetTagAttrValue(const RString& tag,const RString& attr) const
{
	RXMLTag* find=GetNode(tag,false);
	if(!find)
		return(RString::Null);
	return(find->GetAttrValue(attr));
}


//-----------------------------------------------------------------------------
void RXMLTag::InsertAttr(RXMLAttr* Attr,bool overwritte)
{
	// Check if the attribute exist
	RXMLAttr* attr=GetAttr(Attr->GetName());
	if(attr)
	{
		if(!overwritte)
			throw RException("Attribute "+Attr->GetName()+" already exists for tag "+Name);
		if(attr!=Attr)
			Attrs.DeletePtr(attr->GetName());
		else
			return;
	}
	Attrs.InsertPtr(Attr);
}


//-----------------------------------------------------------------------------
void RXMLTag::InsertAttr(const RString& name,const RString& value,bool overwritte)
{
	if(Tree)
		InsertAttr(static_cast<RXMLStruct*>(Tree)->NewAttr(name,value),overwritte);
	else
		InsertAttr(new RXMLAttr(name,value),overwritte);
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
RCursor<RXMLAttr> RXMLTag::GetAttrs(void) const
{
	return(RCursor<RXMLAttr>(Attrs));
}


//-----------------------------------------------------------------------------
bool RXMLTag::Merge(const RXMLTag* merge)
{
	if(!Tree)
		throw RException("Node "+Name+" has no parent structure");

	if(!static_cast<RXMLStruct*>(Tree)->Compare(this,merge))
		return(false);

	// Merge the attributes
	RCursor<RXMLAttr> attr(merge->Attrs);
	for(attr.Start();!attr.End();attr.Next())
	{
		// Attribute exist -> Replace value
		// Else -> Insert new attribute
		RXMLAttr* here=Attrs.GetPtr(attr()->GetName());
		if(here)
			here->SetValue(attr()->GetValue());
		else
			Attrs.InsertPtr(new RXMLAttr(*attr()));
	}

	// Pass through the subnodes of merge
	RCursor<RXMLTag> Cur(merge->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		size_t nb;
		RXMLTag* same=0;
		// Go trough each subnode and count the number of same
		RCursor<RXMLTag> My(GetNodes());
		for(My.Start(),nb=0;!My.End();My.Next())
		{
			if(static_cast<RXMLStruct*>(Tree)->Compare(Cur(),My()))
			{
				nb++;
				same=My();
			}
		}
		if(nb==1)
			same->Merge(Cur());
		else
			Tree->DeepCopy(Cur(),this);
	}

	// Merge OK
	return(true);
}


//-----------------------------------------------------------------------------
RXMLTag::~RXMLTag(void)
{
}
