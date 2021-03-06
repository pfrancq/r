/*

	R Project Library

	RPrgInstAssign.h

	Assignment Instructions - Implementation.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#include <rprginstassign.h>
#include <rprgvarstring.h>
#include <rprgclass.h>
#include <rprg.h>
#include <rinterpreter.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstNew
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstNew::RPrgInstNew(RInterpreter* prg,const RString& name,RPrgClass* c,RContainer<RPrgVar,false,false>& init)
	: RPrgInst(prg->GetLine()), Var(name), Class(c), Params(init)
{
	// Read Values
	init.Clear();
}


//------------------------------------------------------------------------------
void RPrgInstNew::Run(RInterpreter* prg,RPrgOutput* o)
{
	RPrgVar* Ptr=Class->NewVar(prg,o,Var,Params);
	prg->AddVar(Ptr);
}


//------------------------------------------------------------------------------
RPrgInstNew::~RPrgInstNew(void)
{
}



//------------------------------------------------------------------------------
//
// RPrgInstAssignVar
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstAssignVar::RPrgInstAssignVar(RInterpreter* prg,const RString& name,RPrgVar* assign)
	: RPrgInst(prg->GetLine()), Var(name), Assign(assign)
{
}


//------------------------------------------------------------------------------
void RPrgInstAssignVar::Run(RInterpreter* prg,RPrgOutput*)
{
	RPrgVar* Ptr=prg->Find(Var);
	if(!Ptr)
		throw RPrgException(prg,"Unknown Variable '"+Var+"'");
	Ptr->Assign(Assign->GetValue(prg));
}


//------------------------------------------------------------------------------
RPrgInstAssignVar::~RPrgInstAssignVar(void)
{
	delete Assign;
}
