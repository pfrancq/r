/*

	R Project Library

	RPrgVarInst.cpp

	Variable representing a instance of class - Implementation.

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
#include <rprgvarinst.h>
#include <rprgclass.h>
#include <rprg.h>
#include <rinterpreter.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgVarInst
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgVarInst::RPrgVarInst(const RString& name,RPrgClass* c)
	: RPrgVar(name,c->GetName()), Class(c)
{
}


//------------------------------------------------------------------------------
void RPrgVarInst::Assign(const void*)
{
}


//------------------------------------------------------------------------------
RString RPrgVarInst::GetValue(RInterpreter* prg)
{
	throw RPrgException(prg,"Instance has no value");
}


//------------------------------------------------------------------------------
RPrgVarInst::~RPrgVarInst(void)
{
}
