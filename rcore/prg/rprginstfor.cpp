/*

	R Project Library

	RPrgInstFor.cpp

	"for" Instruction - Implementation.

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
RPrgInstFor::RPrgInstFor(RInterpreter* prg,size_t t,const RString& var,RContainer<RPrgVar,false,false>& val)
	: RPrgInstBlock(prg,t), Var(var), Values(val)
{
	val.Clear();
}


//------------------------------------------------------------------------------
void RPrgInstFor::RunBlock(RInterpreter* prg,RPrgOutput* o)
{
	RPrgVarString* local;
	AddVar(local=new RPrgVarString(Var,""));
	RCursor<RPrgVar> Cur(Values);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		local->Assign(Cur()->GetValue(prg));
		RPrgInstBlock::RunBlock(prg,o);
	}
	DelVar(local);
}


//------------------------------------------------------------------------------
RPrgInstFor::~RPrgInstFor(void)
{
}
