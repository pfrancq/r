/*

	R Project Library

	RIO.cpp

	Generic Class - Implementation.

	Copyright 2002-2004 by the Universit�Libre de Bruxelles.

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
#include <rstd/rio.h>
#include <rstd/rfile.h>
#include <rstd/rtextfile.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RIOException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RIOException::RIOException(const char* str) throw(std::bad_alloc)
	: RException(str)
{
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RFile* file,const char* str) throw(std::bad_alloc)
	: RException()
{
	if(file)
		strcpy(Msg,file->GetName()+": "+str);
	else
		strcpy(Msg,str);
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RTextFile* file,const char* str) throw(std::bad_alloc)
	: RException()
{
	if(file)
		strcpy(Msg,file->GetName()+" ("+ltou(file->ActualLine())+"): "+str);
	else
		strcpy(Msg,str);
}
