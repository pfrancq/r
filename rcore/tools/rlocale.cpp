/*

	R Project Library

	RMsg.cpp

	Message systems - Implementation.

	(c) 2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rinter/rlocale.h>
using namespace RStd;
using namespace RInter;



//-----------------------------------------------------------------------------
//
// class RLocale
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RInter::RLocale::RLocale(const RString& name)
	: Name(name)
{
}


//-----------------------------------------------------------------------------
RInter::RLocale::RLocale(const char* name)
	: Name(name)
{
}


//-----------------------------------------------------------------------------
int RInter::RLocale::Compare(const RLocale& l) const
{
	return(Name.Compare(l.Name));
}


//-----------------------------------------------------------------------------
int RInter::RLocale::Compare(const RLocale* l) const
{
	return(Name.Compare(l->Name));
}


//-----------------------------------------------------------------------------
int RInter::RLocale::Compare(const RStd::RString& n) const
{
	return(Name.Compare(n));
}


//-----------------------------------------------------------------------------
int RInter::RLocale::Compare(const char* n) const
{
	return(Name.Compare(n));
}


//-----------------------------------------------------------------------------
bool RInter::RLocale::operator==(const RLocale& l) const
{
	return(Name==l.Name);
}


//-----------------------------------------------------------------------------
bool RInter::RLocale::operator==(const RString& n) const
{
	return(Name==n);
}


//-----------------------------------------------------------------------------
bool RInter::RLocale::operator==(const char* n) const
{
	return(Name==n);
}


//-----------------------------------------------------------------------------
RInter::RLocale::~RLocale(void)
{
}
