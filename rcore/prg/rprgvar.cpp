/*

	R Project Library

	RPrgVar.cpp

	Generic variable - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <rprg/rprgvar.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgVar
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgVar::RPrgVar(const char* name) throw(bad_alloc)
	: Name(name)
{
}


//------------------------------------------------------------------------------
int RPrgVar::Compare(const RPrgVar* v) const
{
	return(Name.Compare(v->Name));
}


//------------------------------------------------------------------------------
int RPrgVar::Compare(const RString& v) const
{
	return(Name.Compare(v));
}


//------------------------------------------------------------------------------
int RPrgVar::Compare(const char* v) const
{
	return(Name.Compare(v));
}


//------------------------------------------------------------------------------
void RPrgVar::Assign(const void*) throw(RException)
{
}


//------------------------------------------------------------------------------
const char* RPrgVar::GetValue(RPrg*) throw(RException)
{
	return(0);
}


//------------------------------------------------------------------------------
RPrgVar::~RPrgVar(void)
{
}
