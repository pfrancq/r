/*

	R Project Library

	RPrgInstSub.cpp

	Definition of a subroutine - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#include <rprginstsub.h>
#include <rprgvarstring.h>
#include <rprgvarref.h>
#include <rprg.h>
#include <rinterpreter.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstSub
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstSub::RPrgInstSub(RInterpreter* prg,size_t t,const RString& sub,RContainer<RPrgVar,false,false>& params)
	: RPrgInstBlock(prg,t), Name(sub), Params(20,10)
{
	RCursor<RPrgVar> Cur(params);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(!dynamic_cast<RPrgVarRef*>(Cur()))
			throw RPrgException(prg,"Definition of a subroutine '"+Name+"' allows only parameters names and not '"+Cur()->GetName()+"'");
		Params.InsertPtr(new RString(Cur()->GetName()));
	}
	params.Clear();
}


//------------------------------------------------------------------------------
void RPrgInstSub::RunBlock(RInterpreter*,RPrgOutput*)
{
}


//------------------------------------------------------------------------------
void RPrgInstSub::Execute(RInterpreter* prg,RPrgOutput* o,RContainer<RPrgVar,true,false>& args)
{
	if(Params.GetNb()!=args.GetNb())
		throw RPrgException(prg,"Subroutine '"+Name+"' needs "+Params.GetNb()+" parameters and "+args.GetNb()+" are provided");
	prg->Scopes.Push(Vars=new RPrgScope());
	RContainer<RPrgVarString,false,false> LocalVars(Params.GetNb());
	RCursor<RPrgVar> Value(args);
	RCursor<RString> Name(Params);
	for(Name.Start(),Value.Start();!Name.End();Name.Next(),Value.Next())
	{
		RPrgVarString* ptr=new RPrgVarString(*Name(),Value()->GetValue(prg));
		AddVar(ptr);
		LocalVars.InsertPtr(ptr);
	}
	RPrgInstBlock::RunBlock(prg,o);
	RCursor<RPrgVarString> Var(LocalVars);
	for(Var.Start();!Var.End();Var.Next())
		DelVar(Var());
	prg->Scopes.Pop();
	Vars=0;
}


//------------------------------------------------------------------------------
RPrgInstSub::~RPrgInstSub(void)
{
}
