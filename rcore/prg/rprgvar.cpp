/*

	R Project Library

	RPrgVar.cpp

	Generic variable - Implementation.

	Copyright 2002-2010 by Pascal Francq (pascal@francq.info).
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
#include <rprgvar.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgVar
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgVar::RPrgVar(const RString& name,const RString& type)
	: Name(name), Type(type)
{
}


//------------------------------------------------------------------------------
RPrgVar::RPrgVar(const RPrgVar& var)
	: Name(var.Name), Type(var.Type)
{
}


//------------------------------------------------------------------------------
int RPrgVar::Compare(const RPrgVar& v) const
{
	return(Name.Compare(v.Name));
}


//------------------------------------------------------------------------------
int RPrgVar::Compare(const RString& v) const
{
	return(Name.Compare(v));
}


//------------------------------------------------------------------------------
void RPrgVar::Assign(const void*)
{
}


//------------------------------------------------------------------------------
RString RPrgVar::GetValue(RInterpreter*)
{
	return(RString::Null);
}


//------------------------------------------------------------------------------
RPrgVar::~RPrgVar(void)
{
}
