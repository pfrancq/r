/*

	R Project Library

	RPrgFunc.cpp

	Generic function - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
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
#include <rprgfunc.h>
using namespace R;



//------------------------------------------------------------------------------
//
// RPrgFunc
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RPrgFunc::RPrgFunc(const RString& name,const RString desc)
	: Name(name), Description(desc)
{
}


//------------------------------------------------------------------------------
int RPrgFunc::Compare(const RPrgFunc& f) const
{
	return(Name.Compare(f.Name));
}


//------------------------------------------------------------------------------
int RPrgFunc::Compare(const RString& f) const
{
	return(Name.Compare(f));
}


//------------------------------------------------------------------------------
RString RPrgFunc::GetName(void) const
{
	return(Name);
}


//------------------------------------------------------------------------------
RString RPrgFunc::GetDescription(void) const
{
	return(Description);
}


//------------------------------------------------------------------------------
RPrgFunc::~RPrgFunc(void)
{
}
