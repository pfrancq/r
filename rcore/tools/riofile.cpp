/*

	R Project Library

	RFile.cpp

	Generic File for Input/Output - Implementation.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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
#include <rstd/riofile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RIOFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RIOFile::RIOFile(const RString &name)
  : RFile(name)
{
}


//------------------------------------------------------------------------------
void RIOFile::Open(RIO::ModeType mode)
{
	int localmode;

	RFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			localmode=O_RDONLY;
			break;

		case RIO::Append:
			localmode=O_WRONLY | O_CREAT | O_APPEND;
			break;

		case RIO::Create:
			localmode=O_WRONLY | O_CREAT | O_TRUNC;
			break;

		default:
			throw(RIOException(this,"No Valid Mode"));
	};
	#ifndef _BSD_SOURCE
		localmode|=O_BINARY;
	#endif
	if(Mode==RIO::Read)
		handle=open(Name,localmode);
	else
		handle=open(Name,localmode,S_IREAD|S_IWRITE);
	if(handle==-1)
		throw(RIOException(this,"Can't open the file"));
}


//------------------------------------------------------------------------------
void RIOFile::Close(void)
{
	if(handle!=-1)
	{
		close(handle);
		handle=-1;
	}
}


//------------------------------------------------------------------------------
void RIOFile::Read(char* buffer,unsigned int nb)
{
	if(handle==-1)
		throw(RIOException(this,"Can't read in the file"));
	read(handle,buffer,nb);
}


//------------------------------------------------------------------------------
void RIOFile::Write(char* buffer,unsigned int nb)
{
	if(handle==-1)
		throw(RIOException(this,"Can't write into the file"));
	write(handle,buffer,nb);
}


//------------------------------------------------------------------------------
RIOFile::~RIOFile(void)
{
}
