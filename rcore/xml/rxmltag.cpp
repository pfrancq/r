/*

	R Project Library

	RXMLTag.cpp

	XML tag - Implementation.

	Copyright 2000-2012 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2001 by Thomas L'Eglise.
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rnodecursor.h>
using namespace R;
using namespace std;



//------------------------------write-----------------------------------------------
//
// class RXMLTag
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RXMLTag::RXMLTag(const RXMLTag& tag)
	: RNode<RXMLStruct,RXMLTag,true>(), Name(tag.Name), Contains(tag.Contains),
	  Attrs(tag.Attrs), Namespace(tag.Namespace)
{
}


//-----------------------------------------------------------------------------
RXMLTag::RXMLTag(const RString& name,RURI* xmlns)
	: RNode<RXMLStruct,RXMLTag,true>(), Name(name), Contains(), Attrs(20,10),
	  Namespace(xmlns)
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
RString RXMLTag::GetFullName(void) const
{
	if(Namespace)
		return((*Namespace)()+":"+Name);
	return(Name);
}


//-----------------------------------------------------------------------------
void RXMLTag::SetName(const RString& name)
{
	Name=name;
}


//-----------------------------------------------------------------------------
void RXMLTag::SetNamespace(RURI* xmlns)
{
	Namespace=xmlns;
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
	return(GetNode(name));
}


//-----------------------------------------------------------------------------
void RXMLTag::GetTags(const RString& name,RContainer<RXMLTag,false,false>& find) const
{
	RNodeCursor<RXMLStruct,RXMLTag> Tags(this);
	for(Tags.Start();!Tags.End();Tags.Next())
		if(Tags()->GetName()==name)
			find.InsertPtr(Tags());
}


//-----------------------------------------------------------------------------
RString RXMLTag::GetTagAttrValue(const RString& tag,const RString& attr) const
{
	RXMLTag* find=GetNode(tag);
	if(!find)
		return(RString::Null);
	return(find->GetAttrValue(attr));
}


//-----------------------------------------------------------------------------
void RXMLTag::InsertAttr(RXMLAttr* attr,bool overwrite)
{
	// Check if the attribute exist
	RXMLAttr* exist=GetAttr(attr->GetName());
	if(exist)
	{
		if(!overwrite)
			throw RException("Attribute "+attr->GetName()+" already exists for tag "+Name);
		if(exist!=attr)
			Attrs.DeletePtr(exist->GetName());
		else
			return;
	}
	Attrs.InsertPtr(attr);
}


//-----------------------------------------------------------------------------
void RXMLTag::InsertAttr(RXMLStruct* xml,const RString& name,const RString& value,const RString& xmlns,bool overwrite)
{
	if(xml)
		InsertAttr(xml->NewAttr(name,value,xmlns),overwrite);
	else
		InsertAttr(new RXMLAttr(name,value,0),overwrite);
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
bool RXMLTag::Merge(RXMLStruct* xml,const RXMLTag* merge)
{
	if(!xml)
		throw RException("Node "+Name+" has no parent structure");

	if(!xml->Compare(this,merge))
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
	RNodeCursor<RXMLStruct,RXMLTag> Cur(merge);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		size_t nb;
		RXMLTag* same=0;
		// Go trough each subnode and count the number of same
		RNodeCursor<RXMLStruct,RXMLTag> My(this);
		for(My.Start(),nb=0;!My.End();My.Next())
		{
			if(xml->Compare(Cur(),My()))
			{
				nb++;
				same=My();
			}
		}
		if(nb==1)
			same->Merge(xml,Cur());
		else
			xml->DeepCopy(Cur(),this);
	}

	// Merge OK
	return(true);
}


//-----------------------------------------------------------------------------
RXMLTag::~RXMLTag(void)
{
}
