/*

	R Project Library

	RStd.cpp

	R Standard Library - Implementation.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
#include <rstd/rstd.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RException::RException(void) throw()
{
	Msg[0]='\0';
}


//------------------------------------------------------------------------------
RException::RException(const char* str) throw()
{
	if(str)
	{
	    strncpy(Msg,str,1024);
	    Msg[1023] = '\0';
	}
	else
		Msg[0]='\0';
}


//------------------------------------------------------------------------------
void RException::SetMsg(const char* str)
{
	if(str)
	{
	    strncpy(Msg,str,1024);
	    Msg[1023] = '\0';
	}
	else
		Msg[0]='\0';
}


//------------------------------------------------------------------------------
RException::~RException(void) throw()
{
}
