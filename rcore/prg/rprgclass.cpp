/*

	R Project Library

	RPrgClass.cpp

	Generic Class - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#include <rprgclass.h>
#include <rprgfunc.h>
#include <rprg.h>
#include <rinterpreter.h>
#include <rprgvarstring.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgClass::RPrgClass(const RString& name)
	: Name(name), Vars(20,10), Methods(30,15)
{
}


//------------------------------------------------------------------------------
int RPrgClass::Compare(const RPrgClass& c) const
{
	return(Name.Compare(c.Name));
}


//------------------------------------------------------------------------------
int RPrgClass::Compare(const RString& c) const
{
	return(Name.Compare(c));
}


//------------------------------------------------------------------------------
RPrgFunc* RPrgClass::GetMethod(const RString& method) const
{
	return(Methods.GetPtr(method));
}


//------------------------------------------------------------------------------
RString RPrgClass::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
RCursor<RPrgFunc> RPrgClass::GetMethods(void) const
{
	return(RCursor<RPrgFunc>(Methods));
}


//------------------------------------------------------------------------------
RPrgClass::~RPrgClass(void)
{
}



//------------------------------------------------------------------------------
//
// class RPrgStringClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgStringClass::RPrgStringClass(void)
	: RPrgClass("String")
{
}


//------------------------------------------------------------------------------
RPrgVar* RPrgStringClass::NewVar(RInterpreter* prg,RPrgOutput*,const RString& name,RContainer<RPrgVar,true,false>& params)
{
	if(params.GetNb()>1)
		throw RPrgException(prg,"String constructor has maximum one parameter");
	RPrgVarString* Var;
	if(params.GetNb())
		Var=new RPrgVarString(name,params[0]->GetValue(prg));
	else
		Var=new RPrgVarString(name,"");
	return(Var);
}
