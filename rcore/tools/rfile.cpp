/*

	R Project Library

	RFile.cpp

	Generic File - Implementation.

	Copyright 1999-2004 by the Université libre de Bruxelles.

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
// include files for ANSI C/C++
#include <stdlib.h>
#include <stdio.h>
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


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RFile::RFile(const RString &name)
  : Mode(Undefined), Name(name)
{
}


//------------------------------------------------------------------------------
void RFile::Open(ModeType mode)
{
	Mode=mode;
}


//------------------------------------------------------------------------------
void RFile::Close(void)
{
}


//------------------------------------------------------------------------------
const RString& RFile::GetName(void) const
{
	return(this->Name);
}


//------------------------------------------------------------------------------
RChar RFile::GetDirSeparator(void)
{
#ifdef _BSD_SOURCE
    return('/');
#else
    return('\\');
#endif

}


//------------------------------------------------------------------------------
RFile::~RFile(void)
{
	Close();
}
