/*

	R Project Library

	RIOFile.cpp

	Generic File for Input/Output - Implementation.

	Copyright 1999-2007 by the Université Libre de Bruxelles.

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
#if defined(_BSD_SOURCE) || defined(__GNUC__) || defined(__APPLE_)
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
RDownload RIOFile::Get;


//------------------------------------------------------------------------------
RIOFile::RIOFile(const RURI& uri)
  : RFile(uri), handle(-1), Size(0), Pos(0)
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
	bool local=false;
	
	// If file seems to be open -> close it
	if(handle!=-1)
		Close();

	// Verify that it is not a urn
	if(URI.GetScheme()=="urn")
		throw RIOException(this,"URI does not represent a file");
	
	// look if it is a local file
	if(URI.GetScheme()=="file")
	{
		local=true;
		File=URI.GetPath();
	}
	else
		File=RFile::GetTempFile();  // File must be download in a temporary file
	
	RFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			localmode=O_RDONLY;
			CanWrite=false;
			CanRead=true;
			break;

		case RIO::Write:
			if(!local)
				throw RIOException(this,"Cannot write with an URL");
			localmode=O_WRONLY | O_CREAT;
			CanWrite=true;
			CanRead=false;
			break;

		case RIO::ReadWrite:
			if(!local)
				throw RIOException(this,"Cannot write with an URL");			
			localmode=O_RDWR | O_CREAT;
			CanWrite=true;
			CanRead=true;
			break;

		case RIO::Append:
			if(!local)
				throw RIOException(this,"Cannot appen to an URL");			
			localmode=O_WRONLY | O_CREAT | O_APPEND;
			CanWrite=true;
			CanRead=true;
			break;

		case RIO::Create:
			if(!local)
				throw RIOException(this,"Cannot create with an URL");			
			localmode=O_WRONLY | O_CREAT | O_TRUNC;
			CanWrite=true;
			CanRead=false;
			break;

		default:
			throw(RIOException(this,"No valid mode"));
	};
	
	// If not local -> Download it
	if(!local)
		Get.DownloadFile(URI,File);
	
	#if defined(_BSD_SOURCE) || defined(__GNUC__) || defined(__APPLE_)
		//do nothing
	#else
        localmode|=O_BINARY;
	#endif	
	if(Mode==RIO::Read)
	{
		handle=open(File.Latin1(),localmode);
	}
	else
		handle=open(File.Latin1(),localmode,S_IREAD|S_IWRITE);
	if(handle==-1)
		throw(RIOException(this,"Can't open the file '"+URI+"'"));
	fstat(handle, &statbuf);
	Size=statbuf.st_size;
	if(Mode==RIO::Append)
		Pos=Size;
	else
		Pos=0;
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
	// If non-local file -> remove the temporary file
	if(URI.GetScheme()!="file")
			Get.DeleteFile(File);
	File=RString::Null;
	Pos=Size=0;
	Mode=RIO::Undefined;
}


//------------------------------------------------------------------------------
unsigned int RIOFile::Read(char* buffer,unsigned int nb)
{
	if(handle==-1)
		throw(RIOException(this,"Can't read in the file"));
	if(!CanRead)
		throw(RIOException(this,"No Read access"));
	if(End())
		throw(RIOException(this,"End of the file reached"));
	if(nb>Size-Pos)
		nb=Size-Pos;
	read(handle,buffer,nb);

	// Next position
	Pos+=nb;
	return(nb);
}


//------------------------------------------------------------------------------
void RIOFile::Write(const char* buffer,unsigned int nb)
{
	if(handle==-1)
		throw(RIOException(this,"Can't write into the file"));
	if(!CanWrite)
		throw(RIOException(this,"No write access"));
	write(handle,buffer,nb);
	#ifdef windows
		flushall();
	#endif

	// Increase the size only if the current position is at the end
	if(Mode==RIO::Append)
		Size+=nb;
	else
		if(Pos>=Size)
			Size=Pos+nb;

	// Next position
	Pos+=nb;
}


//------------------------------------------------------------------------------
void RIOFile::Seek(unsigned long long pos)
{
	if((pos>=Size)&&(!CanWrite))
		throw(RIOException(this,"Position outside of the file"));
	lseek(handle,pos,SEEK_SET);
	Pos=pos;
}


//------------------------------------------------------------------------------
void RIOFile::SeekRel(long long rel)
{
	if(Pos<static_cast<unsigned int>(labs(rel)))
		throw(RIOException(this,"Position before beginning of the file"));
	if((Pos+rel>=Size)&&(!CanWrite))
		throw(RIOException(this,"Position outside of the file"));
	lseek(handle,rel,SEEK_CUR);
	Pos+=rel;
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
	Close();
}
