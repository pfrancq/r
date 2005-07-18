/*

	R Project Library

	RFile.cpp

	Generic File for Input/Output - Implementation.

	Copyright 1999-2005 by the Universit�Libre de Bruxelles.

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
#include <riofile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RIOFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RIOFile::RIOFile(const RString& name)
  : RFile(name), handle(-1), Size(0), Pos(0)
{
}

//------------------------------------------------------------------------------
RIOFile::RIOFile(RIOFile& file)
	: RFile(file), handle(-1), Size(0), Pos(0)
{
}


//------------------------------------------------------------------------------
void RIOFile::Open(RIO::ModeType mode)
{
	int localmode;
	struct stat statbuf;

	// If file seems to be open -> close it
	if(handle!=-1)
		Close();

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
	{
		handle=open(Name.Latin1(),localmode);
	}
	else
		handle=open(Name.Latin1(),localmode,S_IREAD|S_IWRITE);
	if(handle==-1)
		throw(RIOException(this,"Can't open the file"));
	if((Mode==RIO::Read)||(Mode==RIO::Append))
	{
		fstat(handle, &statbuf);
		Size=statbuf.st_size;
		if(Mode==RIO::Append)
			Pos=Size;
	}
}


//------------------------------------------------------------------------------
bool RIOFile::IsOpen(void) const
{
	return(handle!=-1);
}


//------------------------------------------------------------------------------
void RIOFile::Close(void)
{
	if(handle!=-1)
	{
		close(handle);
		handle=-1;
	}
	Pos=Size=0;
	Mode=RIO::Undefined;
}


//------------------------------------------------------------------------------
unsigned int RIOFile::Read(char* buffer,unsigned int nb)
{
	if(handle==-1)
		throw(RIOException(this,"Can't read in the file"));
	if(Mode!=RIO::Read)
		throw(RIOException(this,"No Read access"));
	if(End())
		throw(RIOException(this,"End of the file reached"));
	if(nb>Size-Pos)
		nb=Size-Pos;
	read(handle,buffer,nb);
	Pos+=nb;
	return(nb);
}


//------------------------------------------------------------------------------
void RIOFile::Write(const char* buffer,unsigned int nb)
{
	if(handle==-1)
		throw(RIOException(this,"Can't write into the file"));
	if(Mode==RIO::Read)
		throw(RIOException(this,"No write access"));
	write(handle,buffer,nb);
	#ifdef windows
		flushall();
	#endif
	Size+=nb;
	Pos+=nb;
}


//------------------------------------------------------------------------------
void RIOFile::Seek(unsigned int pos)
{
	if(pos>=Size)
		throw(RIOException(this,"Position outside of the file"));
	lseek(handle,pos,SEEK_SET);
	Pos=pos;
}


//------------------------------------------------------------------------------
bool RIOFile::End(void) const
{
	return(Pos>=Size);
}


//------------------------------------------------------------------------------
unsigned int RIOFile::GetSize(void) const
{
	return(Size);
}


//------------------------------------------------------------------------------
unsigned int RIOFile::GetPos(void) const
{
	return(Pos);
}


//------------------------------------------------------------------------------
RIOFile::~RIOFile(void)
{
}
