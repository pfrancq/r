/*

	R Project Library

	RPrgInstAssign.h

	Assignment Instructions - Implementation.

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
#include <rprginstassign.h>
#include <rprgvarstring.h>
#include <rprgclass.h>
#include <rprg.h>
#include <rcursor.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstNew
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstNew::RPrgInstNew(RPrg* prg,const RString& name,RPrgClass* c)
	: RPrgInst(prg->Line), Var(name), Class(c), Params(20,10)
{
	// Read Values
	prg->AnalyseParam(Params);
}


//------------------------------------------------------------------------------
void RPrgInstNew::Run(RPrg* prg,RPrgOutput* o)
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
RPrgInstAssignVar::RPrgInstAssignVar(RPrg* prg,const RString& name,RPrgVar* assign)
	: RPrgInst(prg->Line), Var(name), Assign(assign)
{
}


//------------------------------------------------------------------------------
void RPrgInstAssignVar::Run(RPrg* prg,RPrgOutput*)
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
