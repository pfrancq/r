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
#include <rprg.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstFor
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstFor::RPrgInstFor(RPrg* prg,size_t t)
	: RPrgInstBlock(t), Values(20,10)
{
	// Read name of variable
	Var=prg->Prg.GetWord();

	// Read next word -> must be "in"
	RString Cmd(prg->Prg.GetWord());
	if(Cmd!="in")
		throw RPrgException(prg,"'"+Cmd+"' is not valid for a 'for' instruction");

	// Read Values
	prg->AnalyseParam(Values);

	// Analyze the rest of the line (which must contain a ':')
	Cmd=prg->Prg.GetLine().Trim();
	if(Cmd!=":")
		throw RPrgException(prg,"for must finish with a :");
}


//------------------------------------------------------------------------------
void RPrgInstFor::Run(RPrg* prg,RPrgOutput* o)
{
	RPrgVarString* local=new RPrgVarString(Var,"");

	prg->AddVar(local);
	RCursor<RPrgVar> Cur(Values);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		local->Assign(Cur()->GetValue(prg));
		RCursor<RPrgInst> Cur2(Insts);
		for(Cur2.Start();!Cur2.End();Cur2.Next())
			Cur2()->Run(prg,o);
	}
	prg->DelVar(local);
}


//------------------------------------------------------------------------------
RPrgInstFor::~RPrgInstFor(void)
{
}
