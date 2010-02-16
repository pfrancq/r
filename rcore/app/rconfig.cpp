/*

	R Project Library

	RConfig.cpp

	XML Structure representing a configuration - Implementation.

	Copyright 2003-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2006 by the Université Libre de Bruxelles (ULB).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Library General Public
	License as published by the Free Software Foundation; either
	version 2.0 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Library General Public License for more details.

	You should have received a copy of the GNU Library General Public
	License along with this library, as a file COPYING.LIB; if not, write
	to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
	Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R Project
#include <rapplication.h>
#include <rio.h>
#include <rxmlfile.h>
#include <rconfig.h>
#include <rdir.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// class RConfig::lParams
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RConfig::lParams::lParams(const RString& cat)
	: RContainer<RParam,true,true>(30), Cat(cat)
{
}


//------------------------------------------------------------------------------
int RConfig::lParams::Compare(const RConfig::lParams& params) const
{
	return(Cat.Compare(params.Cat));
}


//------------------------------------------------------------------------------
int RConfig::lParams::Compare(const RString& params) const
{
	return(Cat.Compare(params));
}



//------------------------------------------------------------------------------
//
// class RConfig
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RConfig::RConfig(const RString& cat,const RString& name)
	: Category(cat), Name(name), Params(5)
{
	Name.Replace('/','-');
}


//------------------------------------------------------------------------------
void RConfig::Analyze(RXMLTag* tag,lParams* cat)
{
	// Go through all existing parameters
	RCursor<RParam> Cur(*cat);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->Set(tag))
			return;

	// Tag does not exist -> create it
	RParam* ptr=0;
	if(tag->GetName()=="param")
		ptr=new RParamValue(tag);
	else if(tag->GetName()=="list")
		ptr=new RParamList(tag);
	else if(tag->GetName()=="struct")
		ptr=new RParamStruct(tag);
	if(ptr)
		cat->InsertPtr(ptr);
}


//------------------------------------------------------------------------------
void RConfig::SetConfigInfos(const RString& cat,const RString& name)
{
	Name=name;
	Name.Replace('/','-');
	Category=cat;
}


//------------------------------------------------------------------------------
void RConfig::Load(bool global)
{
	RXMLStruct Config;

	if(global)
	{
		// Load first the default config
		RURI Where("/usr/r/config/"+Category+"/"+Name+".config");
		try
		{
			RXMLFile File(Where,&Config);
			File.Open(R::RIO::Read);
		}
		catch(RException& e)
		{
			//std::cout<<"Error "<<e.GetMsg()<<std::endl;
		}
		catch(...)
		{
			//std::cout<<"Warning: Global config file '"<<Where()<<"' not found."<<std::endl;
		}
	}

	// Load user specific config
	RXMLTag* Root=Config.GetTop();
	try
	{
		RURI Where(App->GetHomeConfig()+"/config/"+Category+"/"+Name+".config");
		if(Root)
		{
			RXMLStruct Local;
			RXMLFile File(Where,&Local);
			File.Open(R::RIO::Read);
			Config.Merge(Local);
		}
		else
		{
			RXMLFile File(Where,&Config);
			File.Open(R::RIO::Read);
			Root=Config.GetTop();
		}
	}
	catch(RException& e)
	{
		//std::cout<<"Error "<<e.GetMsg()<<std::endl;
	}
	catch(...)
	{
		//std::cout<<"Warning: Local config file '"<<Home+"/.r/config/"+Category+"/"+Name+".config"<<"' not found."<<std::endl;
	}

	// Create the parameters based on the xml structure
	if(!Root)
		return;
	if(Root->GetName()!="config")
		throw RException("Not valid XML config file");

	// Parse the first level
	RCursor<RXMLTag> Level(Root->GetNodes());
	for(Level.Start();!Level.End();Level.Next())
	{
			// Is the current tag a category
		if(Level()->GetName()=="category")
		{
			lParams* cat=Params.GetInsertPtr(Level()->GetAttrValue("name"));
			RCursor<RXMLTag> SubLevel(Level()->GetNodes());
			for(SubLevel.Start();!SubLevel.End();SubLevel.Next())
				Analyze(SubLevel(),cat);
		}
		else
		{
			lParams* cat=Params.GetInsertPtr(RString::Null);
			Analyze(Level(),cat);
		}
	}
}


//------------------------------------------------------------------------------
void RConfig::Save(void)
{
	// Create the XML structure that will be used
	RXMLStruct Config;
	RXMLTag* Root=new RXMLTag("config");
	RXMLTag* ins;
	Config.AddTag(0,Root);

	// Go trough the categories
	RCursor<lParams> Cur(Params);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		// If current category is empty -> default one
		if(Cur()->Cat.IsEmpty())
			ins=Root;
		else
		{
			Config.AddTag(Root,ins=new RXMLTag("category"));
			ins->InsertAttr("name",Cur()->Cat());
		}

		// Store each param
		RCursor<RParam> Param(*Cur());
		for(Param.Start();!Param.End();Param.Next())
			Param()->AddTag(&Config,ins);
	}

	RString Home(App->GetHomeConfig());
	RString Where=Home+"/config/"+Category+"/"+Name+".config";
	try
	{
		// Always save localy
		RDir::CreateDirIfNecessary(Home+"/config/"+Category,true);
		RXMLFile File(Where,&Config);
		File.Open(R::RIO::Create);
	}
	catch(...)
	{
		std::cerr<<"Error: Problem saving local config file'"<<Where<<"'."<<std::endl;
	}
}


//-----------------------------------------------------------------------------
void RConfig::InsertParam(RParam* param,const RString& cat)
{
	lParams* ptr=Params.GetInsertPtr(cat);
	RParam* old=ptr->GetPtr(param->GetName());
	if(old)
		ptr->DeletePtr(*old);
	ptr->InsertPtr(param);
}


//-----------------------------------------------------------------------------
void RConfig::Reset(const RString& name,const RString& cat)
{
	RParam* param=FindParam<RParam>(name,cat);
	if(!param)
		return;
	param->Reset();
}


//-----------------------------------------------------------------------------
RString RConfig::Get(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(RString::Null);
	return(param->Get());
}


//-----------------------------------------------------------------------------
int RConfig::GetInt(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(0);
	return(param->GetInt());
}


//-----------------------------------------------------------------------------
unsigned int RConfig::GetUInt(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(0);
	return(param->GetUInt());
}


//-----------------------------------------------------------------------------
long RConfig::GetLong(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(0);
	return(param->GetLong());
}


//-----------------------------------------------------------------------------
unsigned long RConfig::GetULong(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(0);
	return(param->GetULong());
}


//-----------------------------------------------------------------------------
double RConfig::GetDouble(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(0.0);
	return(param->GetDouble());
}


//-----------------------------------------------------------------------------
bool RConfig::GetBool(const RString& name,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return(false);
	return(param->GetBool());
}


//-----------------------------------------------------------------------------
RCursor<RString> RConfig::GetList(const RString& name,const RString& cat)
{
	RParamList* param=FindParam<RParamList>(name,cat);
	if(!param)
		return(RCursor<RString>());
	return(param->GetList());
}


//-----------------------------------------------------------------------------
RCursor<RParam> RConfig::GetStruct(const RString& name,const RString& cat)
{
	RParamStruct* param=FindParam<RParamStruct>(name,cat);
	if(!param)
		return(RCursor<RParam>());
	return(param->GetStruct());
}


//-----------------------------------------------------------------------------
void RConfig::Set(const RString& name,const RString& v,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return;
	param->Set(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetInt(const RString& name,int v,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return;
	param->SetInt(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetUInt(const RString& name,size_t v,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return;
	param->SetUInt(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetDouble(const RString& name,double v,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return;
	param->SetDouble(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetBool(const RString& name,bool v,const RString& cat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat);
	if(!param)
		return;
	param->SetBool(v);
}


//-----------------------------------------------------------------------------
void RConfig::AddToList(const RString& name,const RString& v,const RString& cat)
{
	RParamList* param=FindParam<RParamList>(name,cat);
	if(!param)
		return;
	param->Insert(v);
}


//-----------------------------------------------------------------------------
void RConfig::AddToStruct(const RString& name,RParam* param,const RString& cat)
{
	RParamStruct* ptr=FindParam<RParamStruct>(name,cat);
	if(!ptr)
		return;
	ptr->Insert(param);
}


//-----------------------------------------------------------------------------
RConfig::~RConfig(void)
{
}
