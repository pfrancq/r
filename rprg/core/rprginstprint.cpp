/*

	R Project Library

	RPrgInstPrint.cpp

	Print instruction - Implementation.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#include <rprginstprint.h>
#include <rinterpreter.h>
using namespace std;
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgInstPrint
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgInstPrint::RPrgInstPrint(RInterpreter* prg,RPrgVar* what)
	: RPrgInst(prg->GetLine()), What(what)
{
}


//------------------------------------------------------------------------------
void RPrgInstPrint::Run(RInterpreter* prg,RPrgOutput*)
{
	cout<<What->GetValue(prg)<<endl;
}


//------------------------------------------------------------------------------
RPrgInstPrint::~RPrgInstPrint(void)
{
	delete What;
}
