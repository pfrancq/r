/*

	R Project Library

	RPrgInstMethod.cpp

	Method of a class - Implementation.

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
#include <rprginstmethod.h>
#include <rprgvarinst.h>
#include <rprginstsub.h>
#include <rprg.h>
#include <rinterpreter.h>
#include <rprgclass.h>
#include <rprgfunc.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInst
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstMethod::RPrgInstMethod(RInterpreter* prg,const RString& method,RContainer<RPrgVar,false,false>& params)
	: RPrgInst(prg->GetLine()), Inst(RString::Null), Method(method), Params(params)
{
	params.Clear();
}


//------------------------------------------------------------------------------
RPrgInstMethod::RPrgInstMethod(RInterpreter* prg,const RString& name,const RString& method,RContainer<RPrgVar,false,false>& params)
	: RPrgInst(prg->GetLine()), Inst(name), Method(method), Params(params)
{
	if(Inst.IsEmpty())
		throw RPrgException(prg,"Instance name cannot be empty");
	params.Clear();
}


//------------------------------------------------------------------------------
void RPrgInstMethod::Run(RInterpreter* prg,RPrgOutput* r)
{
	if(Inst.IsEmpty())
	{
		RPrgInstSub* Sub(prg->Subroutines.GetPtr(Method));
		if(!Sub)
			throw RPrgException(prg,"Unknown function '"+Method+"'");
		Sub->Execute(prg,r,Params);
	}
	else
	{
		RPrgVar* Var(prg->Find(Inst));
		RPrgVarInst* Instance(dynamic_cast<RPrgVarInst*>(Var));
		if(!Instance)
			throw RPrgException(prg,"Variable '"+Inst+"' is not an object instance.");
		RPrgFunc* Func(Instance->GetClass()->GetMethod(Method));
		if(!Func)
			throw RPrgException(prg,"Unknown method '"+Method+"' for object '"+Inst+"'");
		Func->Run(prg,r,Instance,Params);
	}

}


//------------------------------------------------------------------------------
void RPrgInstMethod::AddParam(RPrgVar* var)
{
	Params.InsertPtr(var);
}


//------------------------------------------------------------------------------
RPrgInstMethod::~RPrgInstMethod(void)
{
}
