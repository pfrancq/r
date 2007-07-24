/*

	R Project Library

	RXMLAttrr.cpp

	Attribute for a XML Tag - Implementation.

	Copyright 2000-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Thomas L'Eglise.

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
#include <rxmlattr.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RXMLAttr
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RXMLAttr::RXMLAttr(const RXMLAttr& attr)
	: Name(attr.Name), Value(attr.Value), Namespace(attr.Namespace)
{
}


//------------------------------------------------------------------------------
RXMLAttr::RXMLAttr(const RString& name,const RString& value,RString* xmlns)
	: Name(name), Value(value), Namespace(xmlns)
{
}


//-------------------------------------------------------------------------------
RXMLAttr::RXMLAttr(const RString& name,const double value,RString* xmlns)
	: Name(name), Value(RString::Number(value)), Namespace(xmlns)
{
}


//------------------------------------------------------------------------------
void RXMLAttr::SetValue(const RString& value)
{
	Value=value;
}


//------------------------------------------------------------------------------
RXMLAttr::~RXMLAttr(void)
{
}
