/*

	R Project Library

	RXMLAttrr.cpp

	Attribute for a XML Tag - Implementation.

	Copyright 2000-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

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
#include <rstd/rxmlattr.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RXMLAttr
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLAttr::RXMLAttr(const char *name,const char *value)
	: Name(name), Value(value)
{
}


//-------------------------------------------------------------------------------
RXMLAttr::RXMLAttr(const char *name,const double value)
	: Name(name), Value(30)
{
	char Str[30];
	sprintf(Str,"%f",value);
	Value=Str;
}


//-------------------------------------------------------------------------------
RString& RXMLAttr::GetName(void)
{
	RString *tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//------------------------------------------------------------------------------
RString& RXMLAttr::GetValue(void)
{
	RString* tmp=RString::GetString();

	(*tmp)=Value;
	return(*tmp);
}


//------------------------------------------------------------------------------
RXMLAttr::~RXMLAttr(void)
{
}
