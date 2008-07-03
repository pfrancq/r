/*

	R Project Library

	RIOFile.cpp

	Generic File for Input/Output - Implementation.

	Copyright 1999-2008 by the Université Libre de Bruxelles.

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
// Global constants
const size_t InternalBufferSize=10240;



//------------------------------------------------------------------------------
//
// RIOFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDownload RIOFile::Get;


//------------------------------------------------------------------------------
RIOFile::RIOFile(const RURI& uri)
  : RFile(uri), Handle(-1), Size(0), Pos(0), Internal(0), InternalToRead(0), RealPos(0), CurByte(0)
{
}


//------------------------------------------------------------------------------
RIOFile::RIOFile(RIOFile& file)
	: RFile(file), Handle(-1), Size(0), Pos(0), Internal(0), InternalToRead(0), RealPos(0), CurByte(0)
{
}


//------------------------------------------------------------------------------
void RIOFile::Open(RIO::ModeType mode)
{
	int localmode;
	struct stat statbuf;
	bool local=false;
	
	// If file seems to be open -> close it
	if(Handle!=-1)
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
			if(!Internal)
				Internal=new char[InternalBufferSize];
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
			if(!Internal)
				Internal=new char[InternalBufferSize];			
			break;

		case RIO::Append:
			if(!local)
				throw RIOException(this,"Cannot appen to an URL");			
			localmode=O_WRONLY | O_CREAT | O_APPEND;
			CanWrite=true;
			CanRead=true;
			if(!Internal)
				Internal=new char[InternalBufferSize];			
			break;

		case RIO::Create:
			if(!local)
				throw RIOException(this,"Cannot create with an URL");			
			localmode=O_WRONLY | O_CREAT | O_TRUNC;
			CanWrite=true;
			CanRead=false;
			break;

		default:
			throw RIOException(this,"No valid mode");
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
		Handle=open(File.Latin1(),localmode);
	}
	else
		Handle=open(File.Latin1(),localmode,S_IREAD|S_IWRITE);
	if(Handle==-1)
		throw RIOException(this,"Can't open the file '"+URI+"'");
	fstat(Handle,&statbuf);
	Size=statbuf.st_size;
	if(Mode==RIO::Append)
		Pos=Size;
	else
		Pos=0;
}


//------------------------------------------------------------------------------
bool RIOFile::IsOpen(void) const
{
	return(Handle!=-1);
}


//------------------------------------------------------------------------------
void RIOFile::Close(void)
{
	if(Handle!=-1)
	{
		close(Handle);
		Handle=-1;
	}
	// If non-local file -> remove the temporary file
	if(URI.GetScheme()!="file")
			Get.DeleteFile(File);
	File=RString::Null;
	RealPos=InternalToRead=Pos=Size=0;
	Mode=RIO::Undefined;
	CurByte=0;
}


//------------------------------------------------------------------------------
size_t RIOFile::Read(char* buffer,unsigned int nb,bool move)
{
	// Verify all internal conditions
	if(Handle==-1)
		throw RIOException(this,"Can't read in the file");
	if(!CanRead)
		throw RIOException(this,"No Read access");
	if(End())
		throw RIOException(this,"End of the file reached");
	
	// Verify if the number of bytes to read must be adapted
	if(nb>Size-Pos)
		nb=Size-Pos;

	// Verify if the internal buffer is empty
	if((!InternalToRead)&&(nb<InternalBufferSize))
	{
		if(Size-RealPos<InternalBufferSize)
			InternalToRead=Size-RealPos;
		else
			InternalToRead=InternalBufferSize;
		RealPos+=read(Handle,Internal,InternalToRead);
		CurByte=Internal;
	}
	
	// Verify if all the bytes to read are in the internal buffer?
	if(nb<=InternalToRead)
	{
		// Copy the part of the internal buffer
		memcpy(buffer,CurByte,nb);		
	}
	else
	{
		// Copy the rest of the buffer
		memcpy(buffer,CurByte,InternalToRead);
		buffer+=InternalToRead;
		
		// Load the necessary bytes from the file without to touch to the internal buffer
		read(Handle,buffer,nb-InternalToRead);
		
		if(move)
			RealPos=Pos+nb;   // Correct position in the real file
		else
			lseek(Handle,nb-InternalToRead,SEEK_CUR);  // OK -> Go back
	}

	// Move internal pointers if necessary
	if(move)
	{
		CurByte+=nb;
		Pos+=nb;
		InternalToRead-=nb;
	}
	
	// Return number of bytes read
	return(nb);
}


//------------------------------------------------------------------------------
/*void RIOFile::UnRead(char* buffer,unsigned int nb)
{
	// Verify all internal conditions
	if(handle==-1)
		throw RIOException(this,"Can't read in the file");
	if(!CanRead)
		throw RIOException(this,"No Read access");
	if(!RealPos)
		throw RIOException(this,"Nothing was read from the file");
	if(nb>InternalBufferSize)
		throw RIOException(this,"Cannot unread more than "+RString::Number(InternalBufferSize)+" bytes");
	
	// Verify if the number of bytes to read must be adapted
	if(nb>Pos)
		nb=Pos;
	Pos-=nb;
	
	// Look if in the unread is still in the buffer
	if(nb>RealPos-InternalBufferSize)
	{
		// Just seek inside the buffer
		InternalToRead=RealPos-Pos;
		CurByte=&Internal[InternalToRead];
		memcpy(CurByte,buffer,nb);		
	}
	else
	{
		// Move and re-init the buffer
		// Seek the file
		RealPos=Pos;
		lseek(handle,RealPos,SEEK_CUR);
				
		// Dirty the internal buffer
		if(Size-RealPos<InternalBufferSize)
			InternalToRead=Size-RealPos;
		else
			InternalToRead=InternalBufferSize;
		RealPos+=read(handle,Internal,InternalToRead);
		memcpy(Internal,buffer,nb);
		CurByte=Internal;
	}
}
*/

//------------------------------------------------------------------------------
/*char RIOFile::Read(void)
{
	// Verify all internal conditions
	if(handle==-1)
		throw RIOException(this,"Can't read in the file");
	if(!CanRead)
		throw RIOException(this,"No Read access");
	if(End())
		throw RIOException(this,"End of the file reached");
	
	// Verify if the internal buffer is empty
	if(!InternalToRead)
	{
		if(Size-RealPos<InternalBufferSize)
			InternalToRead=Size-RealPos;
		else
			InternalToRead=InternalBufferSize;
		AlreadyRead+=read(handle,Internal,InternalToRead);
		CurByte=Internal;
	}
	
	// Increment positions
	Pos++;
	InternalToRead--;
	return((*CurByte)++);
}
*/

//------------------------------------------------------------------------------
void RIOFile::Write(const char* buffer,unsigned int nb)
{
	// Verify all internal conditions
	if(Handle==-1)
		throw RIOException(this,"Can't write into the file");
	if(!CanWrite)
		throw RIOException(this,"No write access");
	write(Handle,buffer,nb);
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
	// Verify all internal conditions
	if((pos>=Size)&&(!CanWrite))
		throw RIOException(this,"Position outside of the file");

	// Update current position
	Pos=pos;
	
	// Look if outside the internal buffer
	if((Pos>=RealPos)||((RealPos>InternalBufferSize)&&(Pos<RealPos-InternalBufferSize)))
	{
		// Seek the file
		RealPos=Pos;
		lseek(Handle,RealPos,SEEK_SET);
		
		// Dirty the internal buffer
		InternalToRead=0;
	}
	else
		CurByte=&Internal[RealPos-Pos];
}


//------------------------------------------------------------------------------
void RIOFile::SeekRel(long long rel)
{
	// Verify all internal conditions
	if(Pos+rel<0)//<static_cast<unsigned int>(labs(rel)))
		throw RIOException(this,"Position before beginning of the file");
	if((Pos+rel>Size)&&(!CanWrite))
		throw RIOException(this,"Position outside of the file");
	
	// Update current position
	Pos+=rel;
	
	// Look if outside the internal buffer
	if((Pos>=RealPos)||((RealPos>InternalBufferSize)&&(Pos<RealPos-InternalBufferSize)))
	{
		// Seek the file
		RealPos=Pos;
		lseek(Handle,rel,SEEK_CUR);
		
		// Dirty the internal buffer
		InternalToRead=0;
	}
	else
		CurByte+=rel;
}


//------------------------------------------------------------------------------
RIOFile::~RIOFile(void)
{
	Close();
	if(Internal)
		delete[] Internal;
}
