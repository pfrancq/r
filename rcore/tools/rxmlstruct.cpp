/*

	R Project Library

	RXMLStruct.cpp

	Description - Implementation.

	(c) 2000-2001 by P. Francq.

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
#include <rstd/rxmlstruct.h>
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class RXMLStruct
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RXMLStruct::RXMLStruct(void)
 : RTree<RXMLTag,true,false>(30,15)
{
}


//-----------------------------------------------------------------------------
RXMLTag* RXMLStruct::GetTag(const char* name)
{
	return(GetPtr<const char*>(name,false));
}


//-----------------------------------------------------------------------------
RXMLStruct::~RXMLStruct(void)
{
}
