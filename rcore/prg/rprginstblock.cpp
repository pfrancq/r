/*

	R Project Library

	RPrgInstBlock.cpp

	Block of Instructions - Implementation.

	Copyright 2002-2011 by Pascal Francq (pascal@francq.info).
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
#include <rprginstblock.h>
#include <rprg.h>
#include <rprgvar.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstBlock
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstBlock::RPrgInstBlock(RInterpreter* prg,size_t depth)
	: RPrgInst(prg->GetLine()), Insts(50,25), Vars(0), Depth(depth)
{
}


//------------------------------------------------------------------------------
void RPrgInstBlock::AddInst(RPrgInst* ins)
{
	if(ins)
		Insts.InsertPtr(ins);
}


//-----------------------------------------------------------------------------
void RPrgInstBlock::AddVar(RPrgVar* var)
{
	if(!var)
		return;
//	cout<<"AddVar '"<<var->GetName()<<"' in "<<GetDepth()<<endl;
	Vars->InsertPtr(var,true);
}


//-----------------------------------------------------------------------------
void RPrgInstBlock::DelVar(RPrgVar* var)
{
	if(!var)
		return;
//	cout<<"DelVar '"<<var->GetName()<<"' in "<<GetDepth()<<endl;
	Vars->DeletePtr(*var);
}

//------------------------------------------------------------------------------
void RPrgInstBlock::ClearInstructions(void)
{
	Insts.Clear();
}


//-----------------------------------------------------------------------------
void RPrgInstBlock::RunBlock(RInterpreter* prg,RPrgOutput* o)
{
	RCursor<RPrgInst> Cur(Insts);
	for(Cur.Start();!Cur.End();Cur.Next())
		Cur()->Run(prg,o);
}


//------------------------------------------------------------------------------
void RPrgInstBlock::Run(RInterpreter* prg,RPrgOutput* o)
{
	prg->Scopes.Push(Vars=new RPrgScope());
	RunBlock(prg,o);
	prg->Scopes.Pop();
	Vars=0;
}


//------------------------------------------------------------------------------
RPrgInstBlock::~RPrgInstBlock(void)
{
	delete Vars;
}
