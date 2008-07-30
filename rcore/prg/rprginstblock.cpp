/*

	R Project Library

	RPrgInstBlock.cpp

	Block of Instructions - Implementation.

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
#include <rprginstblock.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstBlock
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstBlock::RPrgInstBlock(size_t t)
	: RPrgInst(), Insts(50,25), Vars(20,20), Tabs(t)
{
}


//------------------------------------------------------------------------------
void RPrgInstBlock::AddInst(RPrgInst* ins)
{
	if(ins)
		Insts.InsertPtr(ins);
}


//------------------------------------------------------------------------------
RPrgInstBlock::~RPrgInstBlock(void)
{
}
