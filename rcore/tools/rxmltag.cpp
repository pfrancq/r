/*

	R Project Library

	RXMLTag.cpp

	XML tag - Implementation.

	(c) 2000-2001 by P. Francq and T. L'Eglise.

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



//---------------------------------------------------------------------------
// include files for ANSI C/C++
#include <ctype.h>


//---------------------------------------------------------------------------
// include files foor R Project
#include <rstd/rxmltag.h>
#include <rstd/rxmlfile.h>
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class RXMLTag
//
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
RStd::RXMLTag::RXMLTag(void)
 : RNode<RXMLTag,false>(100,50), Attrs(20,10)
{
}


//------------------------------------------------------------------------------
RStd::RXMLTag::RXMLTag(RString _name)
 : RNode<RXMLTag,false>(100,50), Name(_name), Attrs(20,10)
{
}


//------------------------------------------------------------------------------
void RStd::RXMLTag::Save(RXMLFile *f, int depth) throw(RString)
{
	int i;
	RString text;
	RXMLAttr **ptr;
	RXMLTag **ptr1;

	for(int i=0;i<depth;i++) (*f)<<"\t";
	if(Attrs.NbPtr)
	{
		(*f)<<"<"+Name;
		for (i=Attrs.NbPtr, ptr=Attrs.Tab; --i; ptr++)
		{
			(*f)<<(*ptr)->GetName()+"=\""+(*ptr)->GetValue()+"\"";
		}
		if(NbPtr||Contains.GetLen())
		{
			(*f)<<(*ptr)->GetName()+"=\""+(*ptr)->GetValue()+"\">"<<endl;
		}
		else
		{
			(*f)<<(*ptr)->GetName()+"=\""+(*ptr)->GetValue()+"\"/>"<<endl;
		}
	}
	else
	{
		if(NbPtr||Contains.GetLen())
			(*f)<<"<"+Name+">"<<endl;
		else
			(*f)<<"<"+Name+"/>"<<endl;
	}
	if(Contains.GetLen())
		(*f)<<Contains<<endl;
	for (i=NbPtr+1, ptr1=Tab; --i; ptr1++)
	{
		(*ptr1)->Save(f,depth+1);
	}
	if(NbPtr||Contains.GetLen())
	{
		for(int i=0;i<depth;i++) (*f)<<"\t";
		(*f)<<"</"+Name+">"<<endl;
	}
}


//------------------------------------------------------------------------------
void RStd::RXMLTag::Load(RXMLFile *f,RXMLStruct *xmlstruct) throw(RString)
{
	char *ptr,*ptr2,c;
	char *attrn,*attrv;
	RXMLTag *tag;

	// If not a tag -> Error
	if(!f->BeginTag())
		throw("Not a tag");

	// Read and Analyse the tag
	ptr=f->GetTag();
	ptr++; // Skip '<'
	while(isspace(*ptr)) ptr++; // Skip Spaces
	ptr2=ptr;
	// Read Name
	while((!isspace(*ptr))&&((*ptr)!='>')&&((*ptr)!='/'))
		ptr++;
	c=(*ptr);
	(*ptr)=0;
	Name=ptr2;
	(*ptr)=c;
	while(isspace(*ptr)) ptr++; // Skip Spaces
	if(!strcmp(Name(),"!DOCTYPE"))
	{
		ptr2=ptr;
		while((!isspace(*ptr))&&((*ptr)!='>')) ptr++; // Read DocType
		c=(*ptr);
		(*ptr)=0;
		Contains=ptr2;
		return;
	}

	// Treat Attributes
	while((*ptr)&&((*ptr)!='>')&&((*ptr)!='/'))
	{
		attrn=ptr;
		while((!isspace(*ptr))&&((*ptr)!='=')) ptr++;
		(*ptr)=0;
		ptr++;
		while((isspace(*ptr))||((*ptr)=='=')) ptr++; // Skip Spaces & '='
		ptr++; //Skip '"'
		attrv=ptr;
		while((*ptr)!='"') ptr++;
		(*ptr)=0;
		Attrs.InsertPtr(new RXMLAttr(attrn,attrv));
		ptr++;
		while(isspace(*ptr)) ptr++; // Skip Spaces
	}
	if((*ptr)=='/') return;	// No Sub Tag

	// Treat sub-Tag
	while(!f->EndTag())
	{
		if(f->BeginTag())
		{
			tag=new RXMLTag();
			tag->Load(f,xmlstruct);
			xmlstruct->AddNode(this,tag);
		}
		else
		{
			ptr=f->GetContains();
			Contains=ptr;
		}
	}

	// Read the close tag, but nothing to do.
	ptr=f->GetTag();
}


//------------------------------------------------------------------------------
RString& RStd::RXMLTag::GetName(void)
{
	RString *tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//------------------------------------------------------------------------------
RString& RStd::RXMLTag::GetAttrValue(const char *name)
{
	RString *tmp=RString::GetString();
	RXMLAttr *attr;

	attr=Attrs.GetPtr<const char*>(name);
	if(attr)
		(*tmp)=attr->GetValue();
	else
		(*tmp)="";
	return(*tmp);
}


//------------------------------------------------------------------------------
void RXMLTag::InsertAttr(RXMLAttr *Attr)
{
	Attrs.InsertPtr(Attr);
}


//------------------------------------------------------------------------------
bool RStd::RXMLTag::IsAttrDefined(const char *name)
{
	return(Attrs.IsIn<const char*>(name));
}


//-----------------------------------------------------------------------------
RXMLTag* RStd::RXMLTag::GetTag(const char* name)
{
	return(GetPtr<const char*>(name,false));
}


//------------------------------------------------------------------------------
RStd::RXMLTag::~RXMLTag(void)
{
}
