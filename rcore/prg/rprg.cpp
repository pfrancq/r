/*

	R Project Library

	RPrg.cpp

	Script Program - Implementation.

	Copyright 2002-2014 by Pascal Francq (pascal@francq.info).
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
// include files for ANSI C/C++
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rprg.h>
#include <rinterpreter.h>
#include <rprginstfor.h>
#include <rprginstdelete.h>
#include <rprginstassign.h>
#include <rprginstmethod.h>
#include <rprgvarliteral.h>
#include <rprgvarref.h>
#include <rprgclass.h>
#include <rprgfunc.h>
using namespace std;
using namespace R;



//-----------------------------------------------------------------------------
//
// RPrg
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RPrg::RPrg(const RURI& f)
	: RTextFile(f)
{
	// Program settings
	SetRem("#");
	SetRemStyle(RTextFile::SingleLineComment);
	SetParseSpace(RTextFile::LeaveSpaces);
}


//-----------------------------------------------------------------------------
RPrg::~RPrg(void)
{
}
