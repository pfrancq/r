/*

	R Project Library

	RPrgInstFor.cpp

	"for" Instruction - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
RPrgInstFor::RPrgInstFor(const RString& line,unsigned int t) throw(std::bad_alloc)
	: RPrgInst(), Values(20,10), Insts(50,25), Tabs(t)
{
	unsigned int pos;
	int len;
	RString Params;
	RCharCursor Cur(line);

	// Read name of variable
	pos=0;
	len=line.Find(' ');
	Var=line.Mid(0,len);

	// Skip "in "
	pos=len+1;
	Cur.GoTo(len+1);
	while((!Cur.End())&&(!Cur().IsSpace()))
		Cur.Next();
	while((!Cur.End())&&(Cur().IsSpace()))
		Cur.Next();

	// Read Values
	Params=line.Mid(Cur.GetPos(),line.GetLen()-Cur.GetPos()+1);
	RPrg::AnalyseParam(Params,&Values);
}


//------------------------------------------------------------------------------
void RPrgInstFor::AddInst(RPrgInst* ins) throw(std::bad_alloc)
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
