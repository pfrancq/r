/*

	R Project Library

	RPrgInstFor.cpp

	"for" Instruction - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <rprg/rprginstfor.h>
#include <rprg/rprgvarval.h>
#include <rprg/rprg.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstFor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstFor::RPrgInstFor(char* line,unsigned int t) throw(bad_alloc)
	: RPrgInst(), Values(20,10), Insts(50,25), Tabs(t)
{
	char* var;
	RPrgVar* r;

	// Read name of variable
	var=line;
	while((*line)&&((*line)!=' '))
		line++;
	(*(line++))=0;
	Var=var;

	// Skip "in "
	while((*line)&&((*line)!=' '))
		line++;
	(*(line++))=0;
	while((*line)&&((*line)==' '))
		line++;

	// Read Values
	while((*line))
	{
		r=RPrg::AnalyseParam(line);
		if(r)
			Values.InsertPtr(r);
	}
}


//------------------------------------------------------------------------------
void RPrgInstFor::AddInst(RPrgInst* ins) throw(bad_alloc)
{
	Insts.InsertPtr(ins);
}


//------------------------------------------------------------------------------
void RPrgInstFor::Run(RPrg* prg,RPrgOutput* o) throw(RException)
{
	RPrgVarVal* local=new RPrgVarVal(Var,"");

	prg->AddVar(local);
	for(Values.Start();!Values.End();Values.Next())
	{
		local->Assign(Values()->GetValue(prg));
		for(Insts.Start();!Insts.End();Insts.Next())
		{
			Insts()->Run(prg,o);
		}
	}
	prg->DelVar(local);
}


//------------------------------------------------------------------------------
RPrgInstFor::~RPrgInstFor(void)
{
}
