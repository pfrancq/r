/*

	R Project Library

	RXMLAttrr.cpp

	Attribute for a XML Tag - Implementation.

	(c) 2000-2001 by P. Francq and T. L'Eglise.

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <time.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rxml/rxmlattr.h>
using namespace RXML;
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class RXMLAttr
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RXML::RXMLAttr::RXMLAttr(const char *name,const char *value)
	: Name(name), Value(value)
{
}


//------------------------------------------------------------------------------
RXML::RXMLAttr::RXMLAttr(const char *name,const double value)
	: Name(name), Value(30)
{
	char Str[30];
	sprintf(Str,"%f",value);
	Value=Str;
}


//------------------------------------------------------------------------------
RString& RXML::RXMLAttr::GetName(void)
{
	RString *tmp=RString::GetString();

	(*tmp)=Name;
	return(*tmp);
}


//-----------------------------------------------------------------------------
RString& RXML::RXMLAttr::GetValue(void)
{
	RString* tmp=RString::GetString();

	(*tmp)=Value;
	return(*tmp);
}


//-----------------------------------------------------------------------------
RXML::RXMLAttr::~RXMLAttr(void)
{
}
