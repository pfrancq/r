/*

	R Project Library

	RMsg.cpp

	Message systems - Implementation.

	Copyright 2001-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
// include files for R Project
#include <rlocale.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RLocale
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RLocale::RLocale(const RString& name)
	: Name(name)
{
}


//------------------------------------------------------------------------------
RLocale::RLocale(const char* name)
	: Name(name)
{
}


//------------------------------------------------------------------------------
int RLocale::Compare(const RLocale& l) const
{
	return(Name.Compare(l.Name));
}


//------------------------------------------------------------------------------
int RLocale::Compare(const RString& n) const
{
	return(Name.Compare(n));
}


//------------------------------------------------------------------------------
bool RLocale::operator==(const RLocale& l) const
{
	return(Name==l.Name);
}


//------------------------------------------------------------------------------
bool RLocale::operator==(const RString& n) const
{
	return(Name==n);
}


//------------------------------------------------------------------------------
RLocale::~RLocale(void)
{
}
