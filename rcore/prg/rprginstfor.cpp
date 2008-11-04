/*

	R Project Library

	RPrgInstFor.cpp

	"for" Instruction - Implementation.

	Copyright 2002-2008 by the Université Libre de Bruxelles.

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
#include <rprginstfor.h>
#include <rprgvarstring.h>
#include <rprgvarliteral.h>
#include <rprg.h>
#include <rinterpreter.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstFor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstFor::RPrgInstFor(RInterpreter* prg,size_t t)
	: RPrgInstBlock(prg,t), Values(20,10)
{
	// Read name of variable
	Var=prg->GetWord();

	// Read next word -> must be "in"
	RString Cmd(prg->GetWord());
	if(Cmd=="in")
	{
		// The variable takes values from a list given as parameter
		prg->AnalyseParam(Values); // Read Values
	}
	else if(Cmd=="from")
	{
		bool ok;

		// The variable takes a list of numbers given by a range and a step
		RString Next(prg->GetWord());
		long start=Next.ToLong(ok);
		if(!ok)
			throw RPrgException(prg,"'"+Next+"' is not valid parameter of 'from'");
		Next=prg->GetWord();
		if(Next!="to")
			throw RPrgException(prg,"'to' is excepted and not '"+Next+"'");
		Next=prg->GetWord();
		long end=Next.ToLong(ok);
		if(!ok)
			throw RPrgException(prg,"'"+Next+"' is not valid parameter of 'to'");
		Next=prg->GetWord();
		if(Next!="step")
			throw RPrgException(prg,"'step' is excepted and not '"+Next+"'");
		Next=prg->GetWord();
		long step=Next.ToLong(ok);
		if(!ok)
			throw RPrgException(prg,"'"+Next+"' is not valid parameter of 'step'");
		for(long i=start;i<=end;i+=step)
		{
			Values.InsertPtr(new RPrgVarLiteral(RString::Number(i)));
		}
	}
	else
		throw RPrgException(prg,"'"+Cmd+"' is not valid for a 'for' instruction");
}


//------------------------------------------------------------------------------
void RPrgInstFor::Run(RInterpreter* prg,RPrgOutput* o)
{
	RPrgVarString* local=new RPrgVarString(Var,"");

	prg->Blocks.Push(this);
	prg->AddVar(local);
	RCursor<RPrgVar> Cur(Values);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		local->Assign(Cur()->GetValue(prg));
		RCursor<RPrgInst> Cur2(Insts);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()->Run(prg,o);
	}
	Vars.Clear();
	prg->DelVar(local);
	prg->Blocks.Pop();
}


//------------------------------------------------------------------------------
RPrgInstFor::~RPrgInstFor(void)
{
}
