/*

	R Project Library

	RStd.cpp

	Rainbow Standard Library - Implementation.

	(C) 1999-2001 by P. Francq.

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



//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
using namespace RStd;



//-----------------------------------------------------------------------------
//
// class RException
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RException::RException(const char* str) throw(bad_alloc)
	: Msg(0)
{
	if(str)
		Msg=strdup(str);
}


//-----------------------------------------------------------------------------
RException::~RException(void)
{
	if(Msg)
		delete Msg;
}
