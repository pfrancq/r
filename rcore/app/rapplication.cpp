/*

	R Project Library

	RApplication.cpp

	Generic Application - Implementation.

	Copyright 2006-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2006-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rapplication.h>
using namespace R;


//-----------------------------------------------------------------------------
// Static variables
RApplication* R::App=0;



//-----------------------------------------------------------------------------
//
// class RApplication
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RApplication::RApplication(const RString& name,int argc, char** argv)
	: Name(name), Args(argc), Params(argc/2), HomeConfig(RString::Null), Config("app",name), HasInitApp(false)
{
	if(App)
		ThrowRException("Already one application running");
	App=this;

	// Set locale information specified by the user.
	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "POSIX");

	// Verify parameters
	RAssert(argc);
	RAssert(argv);
	RAssert(name.GetLen());

	// Initialize information
	File=argv[0];
	RString Param(RString::Null);
    for(int i=1;i<argc;i++)
    {
    	RString* Arg=new RString(argv[i]);
    	if(Arg->Mid(0,2)=="--")
    	{
    		// A parameter is found
    		if(!Param.IsEmpty())       // If a parameter is already defined -> insert it
    			Params.InsertPtr(new RParamValue(Param,RString::Null));
    		Param=Arg->Mid(2);
    	}
    	else
    	{
    		if(!Param.IsEmpty())
    		{
    			// Value of a parameter
    			Params.InsertPtr(new RParamValue(Param,*Arg));
    			Param=RString::Null;
    		}
    	}
		Args.InsertPtr(Arg);
    }
	if(!Param.IsEmpty())       // If a parameter is already defined -> insert it
		Params.InsertPtr(new RParamValue(Param,RString::Null));

	// Configuration directory specified
	RParamValue* ptr(Params.GetPtr("config"));
	if(ptr)
		HomeConfig=ptr->Get();
	if(HomeConfig.IsEmpty())
		HomeConfig=RString(getenv("HOME"))+"/.r";
}


//-----------------------------------------------------------------------------
RString RApplication::GetName(void) const
{
	return(Name);
}


//-----------------------------------------------------------------------------
RString RApplication::GetApplicationFile(void) const
{
	return(File);
}


//-----------------------------------------------------------------------------
RCursor<RString> RApplication::GetArgs(void) const
{
	return(RCursor<RString>(Args));
}


//-----------------------------------------------------------------------------
RString RApplication::GetParamValue(const RString& name) const
{
	RParamValue* ptr(Params.GetPtr(name));
	if(!ptr)
		return(RString::Null);
	return(ptr->Get());
}


//-----------------------------------------------------------------------------
RCursor<RParamValue> RApplication::GetParams(void) const
{
	return(RCursor<RParamValue>(Params));
}


//-----------------------------------------------------------------------------
void RApplication::CreateConfig(void)
{
}


//-----------------------------------------------------------------------------
void RApplication::Init(void)
{
	CreateConfig();
	Config.Load();
	HasInitApp=true;
}


//-----------------------------------------------------------------------------
void RApplication::Run(void)
{
}


//-----------------------------------------------------------------------------
void RApplication::Execute(void)
{
	if(!HasInitApp)
		Init();
	Run();
}


//-----------------------------------------------------------------------------
RApplication::~RApplication(void)
{
	Config.Save();
	App=0;
}
