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
class RConfig::lParams
{
public:
	RString Cat;
	RContainer<RParam,true,true> Params;
	RContainer<lParams,true,true> Cats;

	lParams(const RString& cat);
	int Compare(const lParams& params) const;
	int Compare(const RString& params) const;
};


//------------------------------------------------------------------------------
RConfig::lParams::lParams(const RString& cat)
	: Cat(cat),Params(30), Cats(10)
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
	: Category(cat), Name(name), Root(new lParams("config"))
{
	Name.Replace('/','-');
}


//------------------------------------------------------------------------------
void RConfig::Analyze(RXMLTag* tag,lParams* cat)
{
	// Go through all existing parameters
	RCursor<RParam> Cur(cat->Params);
	for(Cur.Start();!Cur.End();Cur.Next())
		if(Cur()->Set(tag))
			return;

	// Tag does not exist -> create it
	RParam* ptr(0);
	if(tag->GetName()=="param")
		ptr=new RParamValue(tag);
	else if(tag->GetName()=="list")
		ptr=new RParamList(tag);
	else if(tag->GetName()=="struct")
		ptr=new RParamStruct(tag);
	if(ptr)
		cat->Params.InsertPtr(ptr);
}


//------------------------------------------------------------------------------
void RConfig::SetConfigInfos(const RString& cat,const RString& name)
{
	Name=name;
	Name.Replace('/','-');
	Category=cat;
}


//------------------------------------------------------------------------------
void RConfig::Load(lParams* cat,RXMLTag* tag)
{
	// Is the current tag a category
	if(tag->GetName()=="category")
	{
		lParams* Cat(cat->Cats.GetInsertPtr(tag->GetAttrValue("name")));
		RCursor<RXMLTag> SubLevel(tag->GetNodes());
		for(SubLevel.Start();!SubLevel.End();SubLevel.Next())
			Load(Cat,SubLevel());
	}
	else
		Analyze(tag,cat);
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
	RXMLTag* RootTag(Config.GetTop());
	try
	{
		RURI Where(App->GetHomeConfig()+"/config/"+Category+"/"+Name+".config");
		if(RootTag)
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
			RootTag=Config.GetTop();
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
	if(!RootTag)
		return;
	if(RootTag->GetName()!="config")
		ThrowRException("Not valid XML config file");

	// Parse the first level
	RCursor<RXMLTag> Level(RootTag->GetNodes());
	for(Level.Start();!Level.End();Level.Next())
		Load(Root,Level());
}


//------------------------------------------------------------------------------
void RConfig::Save(lParams* cat,RXMLStruct& config,RXMLTag* parent)
{
	// Store each parameter
	RCursor<RParam> Param(cat->Params);
	for(Param.Start();!Param.End();Param.Next())
		Param()->AddTag(&config,parent);

	// Treat the categories
	RCursor<lParams> Cur(cat->Cats);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		RXMLTag* ins(new RXMLTag("category"));
		config.AddTag(parent,ins);
		ins->InsertAttr("name",Cur()->Cat);
		Save(Cur(),config,ins);
	}
}


//------------------------------------------------------------------------------
void RConfig::Save(void)
{
	// Create the XML structure that will be used
	RXMLStruct Config;
	RXMLTag* RootTag(new RXMLTag("config"));
	Config.AddTag(0,RootTag);

	// Save the parameters.
	Save(Root,Config,RootTag);

	RString Home(App->GetHomeConfig());
	RString Where=Home+"/config/"+Category+"/"+Name+".config";
	try
	{
		// Always save locally
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
RParam* RConfig::GetParam(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	lParams* Search;
	if(cat!=RString::Null)
	{
		Search=Root->Cats.GetPtr(cat);
		if(!Search)
			return(0);
		if(subcat!=RString::Null)
		{
			Search=Search->Cats.GetPtr(subcat);
			if(!Search)
				return(0);
			if(subsubcat!=RString::Null)
			{
				Search=Search->Cats.GetPtr(subsubcat);
				if(!Search)
					return(0);
			}
		}
	}
	else
		Search=Root;
	return(Search->Params.GetPtr(name));
}


//-----------------------------------------------------------------------------
RCursor<RParam> RConfig::GetParams(const RString& cat,const RString& subcat,const RString& subsubcat)
{
	lParams* Search;
	if(cat!=RString::Null)
	{
		Search=Root->Cats.GetPtr(cat);
		if(!Search)
			return(RCursor<RParam>());
		if(subcat!=RString::Null)
		{
			Search=Search->Cats.GetPtr(subcat);
			if(!Search)
				return(RCursor<RParam>());
			if(subsubcat!=RString::Null)
			{
				Search=Search->Cats.GetPtr(subsubcat);
				if(!Search)
					return(RCursor<RParam>());
			}
		}
	}
	else
		Search=Root;
	return(RCursor<RParam>(Search->Params));
}


//-----------------------------------------------------------------------------
void RConfig::GetCategories(RContainer<RString,true,false>& cats,const RString& cat,const RString& subcat)
{
	cats.Clear();
	lParams* Search;
	if(cat!=RString::Null)
	{
		Search=Root->Cats.GetPtr(cat);
		if(!Search)
			return;
		if(subcat!=RString::Null)
		{
			Search=Search->Cats.GetPtr(subcat);
			if(!Search)
				return;
		}
	}
	else
		Search=Root;
	RCursor<lParams> Cur(Search->Cats);
	for(Cur.Start();!Cur.End();Cur.Next())
		cats.InsertPtr(new RString(Cur()->Cat));
}


//-----------------------------------------------------------------------------
void RConfig::InsertParam(RParam* param,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	lParams* Search;
	if(cat!=RString::Null)
	{
		Search=Root->Cats.GetInsertPtr(cat);
		if(subcat!=RString::Null)
			Search=Search->Cats.GetInsertPtr(subcat);
		if(subsubcat!=RString::Null)
			Search=Search->Cats.GetInsertPtr(subsubcat);
	}
	else
		Search=Root;
	if(Search->Params.GetPtr(param->GetName()))
		delete param; // Parameter already defined -> delete the argument.
	else
		Search->Params.InsertPtr(param);
}


//-----------------------------------------------------------------------------
void RConfig::Reset(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParam* param=FindParam<RParam>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->Reset();
}


//-----------------------------------------------------------------------------
RString RConfig::Get(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->Get());
}


//-----------------------------------------------------------------------------
int RConfig::GetInt(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetInt());
}


//-----------------------------------------------------------------------------
unsigned int RConfig::GetUInt(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetUInt());
}


//-----------------------------------------------------------------------------
long RConfig::GetLong(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetLong());
}


//-----------------------------------------------------------------------------
unsigned long RConfig::GetULong(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetULong());
}


//-----------------------------------------------------------------------------
double RConfig::GetDouble(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetDouble());
}


//-----------------------------------------------------------------------------
bool RConfig::GetBool(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetBool());
}


//-----------------------------------------------------------------------------
RCursor<RString> RConfig::GetList(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamList* param=FindParam<RParamList>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetList());
}


//-----------------------------------------------------------------------------
RCursor<RParam> RConfig::GetStruct(const RString& name,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamStruct* param=FindParam<RParamStruct>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	return(param->GetStruct());
}


//-----------------------------------------------------------------------------
void RConfig::Set(const RString& name,const RString& v,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->Set(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetInt(const RString& name,int v,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->SetInt(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetUInt(const RString& name,size_t v,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->SetUInt(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetDouble(const RString& name,double v,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->SetDouble(v);
}


//-----------------------------------------------------------------------------
void RConfig::SetBool(const RString& name,bool v,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamValue* param=FindParam<RParamValue>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->SetBool(v);
}


//-----------------------------------------------------------------------------
void RConfig::AddToList(const RString& name,const RString& v,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamList* param=FindParam<RParamList>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	param->Insert(v);
}


//-----------------------------------------------------------------------------
void RConfig::AddToStruct(const RString& name,RParam* param,const RString& cat,const RString& subcat,const RString& subsubcat)
{
	RParamStruct* ptr=FindParam<RParamStruct>(name,cat,subcat,subsubcat);
	if(!param)
		ThrowRException("Parameter '"+name+"' not found in ('"+cat+"','"+subcat+"','"+subsubcat+"')");
	ptr->Insert(param);
}


//-----------------------------------------------------------------------------
RConfig::~RConfig(void)
{
	delete Root;
}
