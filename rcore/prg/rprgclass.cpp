/*

	R Project Library

	RPrgClass.cpp

	Generic Class - Implementation.

	Copyright 2002-2003 by the Universit� Libre de Bruxelles.

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
#include <rprg/rprgclass.h>
#include <rprg/rprgfunc.h>
#include <rprg/rprgvar.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgClass
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgClass::RPrgClass(const char* name) throw(std::bad_alloc)
	: Name(name), Vars(20,10), Methods(30,15)
{
}


//------------------------------------------------------------------------------
int RPrgClass::Compare(const RPrgClass* c) const
{
	return(Name.Compare(c->Name));
}


//------------------------------------------------------------------------------
int RPrgClass::Compare(const RString& c) const
{
	return(Name.Compare(c));
}

//------------------------------------------------------------------------------
int RPrgClass::Compare(const char* c) const
{
	return(Name.Compare(c));
}


//------------------------------------------------------------------------------
RPrgFunc* RPrgClass::GetMethod(const char* method) const
{
	return(Methods.GetPtr<const char*>(method));
}


//------------------------------------------------------------------------------
RPrgClass::~RPrgClass(void)
{
}