/*

	R Project Library

	RDir.cpp

	Directory - Implementation.

	Copyright 2004 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <dirent.h>
/*#include <ctype.h>
#include <sys/stat.h>
#ifdef _BSD_SOURCE
	#include <unistd.h>
#else
	#include <io.h>
#endif*/


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rdir.h>
using namespace R;
using namespace std;



//-----------------------------------------------------------------------------
//
// RDir::Internal
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct RDir::Internal
{
	DIR* Handle;

	Internal(void) : Handle(0) {}
};



//-----------------------------------------------------------------------------
//
// RDir
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
RDir::RDir(const RString& name)
	: RFile(name), Data(0)
{
	Data=new Internal();
}


//-----------------------------------------------------------------------------
void RDir::Open(RIO::ModeType mode)
{
	RFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			Data->Handle=opendir(GetName().Latin1());
			if(!Data->Handle)
				throw(RIOException(this,"Directory does not exist"));
			break;

		case RIO::Append:
			throw(RIOException(this,"Cannot append a directory"));
			break;

		case RIO::Create:

			break;

		default:
			throw(RIOException(this,"No Valid Mode"));
	};
}


//-----------------------------------------------------------------------------
void RDir::Close(void)
{
	RFile::Close();
	if(Data->Handle)
	{
		closedir(Data->Handle);
		Data->Handle=0;
	}
}


//-----------------------------------------------------------------------------
RDir::~RDir(void)
{
	if(Data)
		delete Data;
}
