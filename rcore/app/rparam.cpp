/*

	R Project Library

	RParam.cpp

	Parameter - Implementation.

	Copyright 2003-2014 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R
#include <rparam.h>
#include <rxmlstruct.h>
#include <rnodecursor.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RParam
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RParam::RParam(const RString& n,const RString& desc)
	: Name(n), Description(desc)
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
	: RParam(tag->GetAttrValue("name"),tag->GetAttrValue("desc"))
{
	Value=tag->GetAttrValue("value");
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,const RString& v,const RString& desc)
	: RParam(n,desc), Value(v)
{
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,const char* v,const RString& desc)
	: RParam(n,desc), Value(v)
{
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,long v,const RString& desc)
		: RParam(n,desc)
{
	SetInt(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,unsigned long v,const RString& desc)
		: RParam(n,desc)
{
	SetUInt(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,int v,const RString& desc)
		: RParam(n,desc)
{
	SetInt(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,unsigned int v,const RString& desc)
		: RParam(n,desc)
{
	SetUInt(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,double v,const RString& desc)
		: RParam(n,desc)
{
	SetDouble(v);
}


//------------------------------------------------------------------------------
RParamValue::RParamValue(const RString& n,bool v,const RString& desc)
		: RParam(n,desc)
{
	SetBool(v);
}


//------------------------------------------------------------------------------
int RParamValue::GetInt(void) const
{
	bool b;
	int v=Value.ToInt(b);
	return(v);
}


//------------------------------------------------------------------------------
unsigned int RParamValue::GetUInt(void) const
{
	bool b;
	unsigned int v=Value.ToUInt(b);
	return(v);
}


//------------------------------------------------------------------------------
long RParamValue::GetLong(void) const
{
	bool b;
	long v=Value.ToLong(b);
	return(v);
}


//------------------------------------------------------------------------------
unsigned long RParamValue::GetULong(void) const
{
	bool b;
	unsigned long v=Value.ToULong(b);
	return(v);
}


//------------------------------------------------------------------------------
double RParamValue::GetDouble(void) const
{
	bool b;
	double v=Value.ToDouble(b);
	return(v);
}


//------------------------------------------------------------------------------
RString RParamValue::Get(void) const
{
	return(Value);
}


//------------------------------------------------------------------------------
bool RParamValue::GetBool(void) const
{
	return(Value.ToBool(false));
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
void RParamValue::SetInt(long long int v)
{
	Value=RString::Number(v);
}


//------------------------------------------------------------------------------
void RParamValue::SetUInt(unsigned long long int v)
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
	ptr->InsertAttr(xml,"name",Name);
	if(!Value.IsEmpty())
		ptr->InsertAttr(xml,"value",Value);
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
	: RParam(tag->GetAttrValue("name"),tag->GetAttrValue("desc")), Values(tag->GetNbNodes())
{
	RNodeCursor<RXMLStruct,RXMLTag> Cur(tag);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->GetName()!="item")
			throw RException("Not valid XML tag");
		Values.InsertPtr(new RString(Cur()->GetAttrValue("value")));
	}
}


//------------------------------------------------------------------------------
RParamList::RParamList(const RString& name,const RString& desc)
	: RParam(name,desc), Values(20)
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
	RNodeCursor<RXMLStruct,RXMLTag> Cur(tag);
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
	ptr->InsertAttr(xml,"name",Name);
	RCursor<RString> Cur(Values);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		xml->AddTag(ptr,ptr2=new RXMLTag("item"));
		ptr2->InsertAttr(xml,"value",*Cur());
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
	: RParam(tag->GetAttrValue("name"),tag->GetAttrValue("desc")), Parameters(tag->GetNbNodes())
{
	RNodeCursor<RXMLStruct,RXMLTag> Cur(tag);
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
RParamStruct::RParamStruct(const RString& name,const RString& desc)
	: RParam(name,desc), Parameters(20)
{
}


//------------------------------------------------------------------------------
RCursor<RParam> RParamStruct::GetStruct(void) const
{
	return(RCursor<RParam>(Parameters));
}


//------------------------------------------------------------------------------
void RParamStruct::Insert(RParam* param)
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
	RNodeCursor<RXMLStruct,RXMLTag> Cur(tag);
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
	ptr->InsertAttr(xml,"name",Name);
	RCursor<RParam> Cur(Parameters);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->AddTag(xml,ptr);
}


//------------------------------------------------------------------------------
void RParamStruct::Reset(void)
{
	Parameters.Clear();
}
