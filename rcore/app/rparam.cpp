/*

	R Project Library

	RParam.cpp

	Parameter - Implementation.

	Copyright 2003-2006 by the Universit�Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
#include <values.h>
#include <rxmlstruct.h>
#include <rparam.h>
using namespace R;


//------------------------------------------------------------------------------
//
// class RParam
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RParam::RParam(const RString& n)
	: Name(n)
{
}


//------------------------------------------------------------------------------
int RParam::Compare(const RString& name) const
{
	if(Name==RString::Null)
	{
		if(name==RString::Null)
			return(0);
		else
			return(-1);
	}
	else
		if(name==RString::Null)
			return(1);
	return(Name.Compare(name));
}


//------------------------------------------------------------------------------
int RParam::Compare(const RParam& param) const
{
	if(Name==RString::Null)
	{
		if(param.Name==RString::Null)
			return(0);
		else
			return(-1);
	}
	else
		if(param.Name==RString::Null)
			return(1);
	return(Name.Compare(param.Name));
}


//------------------------------------------------------------------------------
void RParam::AddTag(RXMLStruct*,RXMLTag*)
{
}


//------------------------------------------------------------------------------
bool RParam::Set(RXMLTag*)
{
	return(false);
}


//------------------------------------------------------------------------------
void RParam::Reset(void)
{
}


//------------------------------------------------------------------------------
RParam::~RParam(void)
{
}



//------------------------------------------------------------------------------
//
// class RParamValue
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RParamValue::RParamValue(const RXMLTag* tag)
	: RParam(tag->GetAttrValue("name"))
{
	Value=tag->GetAttrValue("value");
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,const RString& v)
	: RParam(n), Value(v)
{
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,const char* v)
	: RParam(n), Value(v)
{
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,int v)
		: RParam(n)
{
	SetInt(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,unsigned int v)
		: RParam(n)
{
	SetUInt(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,double v)
		: RParam(n)
{
	SetDouble(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,bool v)
		: RParam(n)
{
	SetBool(v);
}


//------------------------------------------------------------------------------
int RParamValue::GetInt(void)
{
	int v;
	sscanf(Value,"%i",&v);
	return(v);
}


//------------------------------------------------------------------------------
unsigned int RParamValue::GetUInt(void)
{
	unsigned int v;
	sscanf(Value,"%u",&v);
	return(v);
}


//------------------------------------------------------------------------------
double RParamValue::GetDouble(void)
{
	double v;
	sscanf(Value,"%lf",&v);
	return(v);
}


//------------------------------------------------------------------------------
RString RParamValue::Get(void)
{
	return(Value);
}


//------------------------------------------------------------------------------
bool RParamValue::GetBool(void)
{
	if(Value=="True")
		return(true);
	return(false);
}


//------------------------------------------------------------------------------
bool RParamValue::Set(RXMLTag* tag)
{
	if((tag->GetName()!="param")||(tag->GetAttrValue("name")!=Name))
		return(false);
	Value=tag->GetAttrValue("value");
	return(true);
}


//------------------------------------------------------------------------------
void RParamValue::SetInt(int v)
{
	Value=RString::Number(v);
}


//------------------------------------------------------------------------------
void RParamValue::SetUInt(unsigned int v)
{
	Value=RString::Number(v);
}


//------------------------------------------------------------------------------
void RParamValue::SetDouble(double v)
{
	Value=RString::Number(v);
}


//------------------------------------------------------------------------------
void RParamValue::Set(const RString& v)
{
	Value=v;
}


//------------------------------------------------------------------------------
void RParamValue::SetBool(bool v)
{
	if(v)
		Value="True";
	else
		Value="False";
}


//------------------------------------------------------------------------------
void RParamValue::AddTag(RXMLStruct* xml,RXMLTag* parent)
{
	RXMLTag* ptr;
	xml->AddTag(parent,ptr=new RXMLTag("param"));
	ptr->InsertAttr("name",Name);
	ptr->InsertAttr("value",Value);
}


//------------------------------------------------------------------------------
void RParamValue::Reset(void)
{
	Value=RString::Null;
}



//------------------------------------------------------------------------------
//
// class RParamList
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RParamList::RParamList(const RXMLTag* tag)
	: RParam(tag->GetAttrValue("name")), Values(tag->GetNbNodes())
{
	RCursor<RXMLTag> Cur(tag->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetName()!="item")
			throw RException("Not valid XML tag");
		Values.InsertPtr(new RString(Cur()->GetAttrValue("value")));
	}
}


//------------------------------------------------------------------------------
RParamList::RParamList(const RString& name)
	: RParam(name), Values(20)
{
}


//------------------------------------------------------------------------------
RCursor<RString> RParamList::GetList(void) const
{
	return(RCursor<RString>(Values));
}


//------------------------------------------------------------------------------
size_t RParamList::GetPos(const RString& value) const
{
	bool Find;
	size_t pos=Values.GetIndex(value,Find);
	if(!Find)
		return(SIZE_MAX);
	return(pos);
}


//------------------------------------------------------------------------------
void RParamList::Insert(const RString& value)
{
	Values.GetInsertPtr(value);
}


//------------------------------------------------------------------------------
bool RParamList::Set(RXMLTag* tag)
{
	if((tag->GetName()!="list")||(tag->GetAttrValue("name")!=Name))
		return(false);
	RCursor<RXMLTag> Cur(tag->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetName()!="item")
			throw RException("Not valid XML tag");
		Insert(Cur()->GetAttrValue("value"));
	}
	return(true);
}


//------------------------------------------------------------------------------
void RParamList::AddTag(RXMLStruct* xml,RXMLTag* parent)
{
	RXMLTag* ptr;
	RXMLTag* ptr2;

	xml->AddTag(parent,ptr=new RXMLTag("list"));
	ptr->InsertAttr("name",Name);
	RCursor<RString> Cur(Values);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		xml->AddTag(ptr,ptr2=new RXMLTag("item"));
		ptr2->InsertAttr("value",*Cur());
	}
}


//------------------------------------------------------------------------------
void RParamList::Reset(void)
{
	Values.Clear();
}



//------------------------------------------------------------------------------
//
// class RParamStruct
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RParamStruct::RParamStruct(const RXMLTag* tag)
	: RParam(tag->GetAttrValue("name")), Parameters(tag->GetNbNodes())
{
	RCursor<RXMLTag> Cur(tag->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetName()=="param")
			Parameters.InsertPtr(new RParamValue(Cur()));
		if(Cur()->GetName()=="list")
			Parameters.InsertPtr(new RParamList(Cur()));
		if(Cur()->GetName()=="struct")
			Parameters.InsertPtr(new RParamStruct(Cur()));
	}
}


//------------------------------------------------------------------------------
RParamStruct::RParamStruct(const RString& name)
	: RParam(name), Parameters(20)
{
}


//------------------------------------------------------------------------------
RCursor<RParam> RParamStruct::GetStruct(void) const
{
	return(RCursor<RParam>(Parameters));
}


//------------------------------------------------------------------------------
void RParamStruct::Insert(const RParam* param)
{
	RParam* ptr=Parameters.GetPtr(*param);
	if(ptr)
		Parameters.DeletePtr(*ptr);
	Parameters.InsertPtr(param);
}


//------------------------------------------------------------------------------
bool RParamStruct::Set(RXMLTag* tag)
{
	if((tag->GetName()!="struct")||(tag->GetAttrValue("name")!=Name))
		return(false);
	RCursor<RXMLTag> Cur(tag->GetNodes());
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetName()=="param")
			Insert(new RParamValue(Cur()));
		if(Cur()->GetName()=="list")
			Insert(new RParamList(Cur()));
		if(Cur()->GetName()=="struct")
			Insert(new RParamStruct(Cur()));
	}
	return(true);
}


//------------------------------------------------------------------------------
void RParamStruct::AddTag(RXMLStruct* xml,RXMLTag* parent)
{
	RXMLTag* ptr;

	xml->AddTag(parent,ptr=new RXMLTag("struct"));
	ptr->InsertAttr("name",Name);
	RCursor<RParam> Cur(Parameters);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->AddTag(xml,ptr);
}


//------------------------------------------------------------------------------
void RParamStruct::Reset(void)
{
	Parameters.Clear();
}
