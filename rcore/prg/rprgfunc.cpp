/*

	R Project Library

	RPrgFunc.cpp

	Generic function - Implementation.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

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
#include <rprg/rprgfunc.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgFunc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgFunc::RPrgFunc(const char* name) throw(std::bad_alloc)
	: Name(name)
{
}


//------------------------------------------------------------------------------
int RPrgFunc::Compare(const RPrgFunc* f) const
{
	return(Name.Compare(f->Name));
}


//------------------------------------------------------------------------------
int RPrgFunc::Compare(const RString& f) const
{
	return(Name.Compare(f));
}


//------------------------------------------------------------------------------
int RPrgFunc::Compare(const char* f) const
{
	return(Name.Compare(f));
}


//------------------------------------------------------------------------------
void RPrgFunc::Run(RPrg*,RPrgOutput*,RContainer<RPrgVar,unsigned int,true,false>*) throw(RException)
{
}


//------------------------------------------------------------------------------
RPrgFunc::~RPrgFunc(void)
{
}
