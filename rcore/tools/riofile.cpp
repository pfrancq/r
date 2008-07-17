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
// Macros proposed by Christian Biere:
// http://osdir.com/ml/os.netbsd.devel.general/2007-02/msg00000.html
#define MAX_INT_VAL_STEP(t) \
        ((t) 1 << (CHAR_BIT * sizeof(t) - 1 - ((t) -1 < 1)))
#define MAX_INT_VAL(t) \
        ((MAX_INT_VAL_STEP(t) - 1) + MAX_INT_VAL_STEP(t))

#define MIN_INT_VAL(t) \
        ((t) -MAX_INT_VAL(t) - 1)



//------------------------------------------------------------------------------
//
// RIOFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
const off_t RIOFile::MaxSize=MAX_INT_VAL(off_t);
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
			throw RIOException(this,"No valid mode");
	};

	// If reading is possible -> create an internal buffer
	if(CanRead&&(!Internal))
		Internal=new char[InternalBufferSize];

	// If not local -> Download it
	if(!local)
		Get.DownloadFile(URI,File);

	// Play with internal switches
	#if defined(_BSD_SOURCE) || defined(__GNUC__) || defined(__APPLE_)
		//do nothing
	#else
		localmode|=O_BINARY;
	#endif

	// Open the file
	if(Mode==RIO::Read)
	{
		Handle=open(File.Latin1(),localmode);
	}
	else
		Handle=open(File.Latin1(),localmode,S_IREAD|S_IWRITE);
	if(Handle==-1)
		throw RIOException(this,"Can't open the file '"+URI+"'");

	// Read the total size of the file
	fstat(Handle,&statbuf);
	Size=statbuf.st_size;

	// Position virtually the file at the beginning or at the end if it is open in append mode
	if(Mode==RIO::Append)
		Pos=Size;
	else
		Pos=0;

	// Suppose nothing is in the buffer
	RealInternalPos=MaxSize;
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
size_t RIOFile::Read(char* buffer,size_t nb,bool move)
{
	// Verify all internal conditions
	if(Handle==-1)
		throw RIOException(this,"Can't read in the file");
	if(!CanRead)
		throw RIOException(this,"No Read access");
	if(End())
		throw RIOException(this,"End of the file reached");

	// Verify if the number of bytes to read must be adapted
	if(static_cast<off_t>(nb)>Size-Pos)
		nb=Size-Pos;

	size_t left(nb);   // Number of bytes left to read

	// Verify if something can be read from the internal buffer
	if((Pos>=RealInternalPos)&&(Pos<RealInternalPos+static_cast<off_t>(InternalBufferSize)))
	{
		// Everything can get from the internal buffer or not
		if(nb>InternalToRead)
		{
			memcpy(buffer,CurByte,InternalToRead);
			left=nb-InternalToRead;
			buffer+=InternalToRead;
			if(move)
			{
				Pos+=InternalToRead;
				InternalToRead=0;
			}
		}
		else
		{
			memcpy(buffer,CurByte,nb);
			if(move)
			{
				Pos+=nb;
				InternalToRead-=nb;
				CurByte+=nb;
			}
			left=0;
		}
	}

	// If nothing more to read -> finished
	if(!left)
		return(nb);

	// If there is more than the internal size to read or if nothing must be moved -> read (at least) a part directly
	if((left>InternalBufferSize)||(!move))
	{
		off_t total;
		if(!move)
			total=left;
		else
			total=left-InternalBufferSize;
		total=read(Handle,buffer,total);
		left-=total;
		buffer+=total;
		if(move)
		{
			Pos+=total;
			RealPos+=total;
		}
		else
			lseek(Handle,-total,SEEK_CUR);  // OK -> Go back
	}

	// If there is still something to read -> first read in the buffer
	if(left)
	{
		if(Size-RealPos<static_cast<off_t>(InternalBufferSize))
			InternalToRead=Size-RealPos;
		else
			InternalToRead=InternalBufferSize;
		RealInternalPos=RealPos;
		RealPos+=read(Handle,Internal,InternalToRead);
		CurByte=Internal;

		// Copy now the number of bytes for the user
		memcpy(buffer,CurByte,left);
		CurByte+=left;
		Pos+=left;
	}

	// Return number of bytes read
	return(nb);
}


//------------------------------------------------------------------------------
void RIOFile::Write(const char* buffer,size_t nb)
{
	// Verify all internal conditions
	if(Handle==-1)
		throw RIOException(this,"Can't write into the file");
	if(!CanWrite)
		throw RIOException(this,"No write access");

	// Write into the file
	write(Handle,buffer,nb);
	#ifdef windows
		flushall();
	#endif

	// Verify if a part of the buffer must be stored into the internal buffer
    if((Pos<RealInternalPos+static_cast<off_t>(InternalBufferSize))&&(Pos+static_cast<off_t>(nb)>=RealInternalPos))
	{
    	char* begini;
    	const char* beginb;
    	size_t len(nb);
    	if(Pos<RealInternalPos)
    	{
    		begini=Internal;
    		beginb=&buffer[RealInternalPos-Pos];
    		len-=RealInternalPos-Pos;
        	if(len>InternalBufferSize)
        		len=InternalBufferSize;
    	}
    	else
    	{
    		beginb=buffer;
    		begini=&Internal[Pos-RealInternalPos];
    		len=InternalBufferSize-Pos+RealInternalPos;
    	}
		memcpy(begini,beginb,len);
	}

	// Increase the size only if the current position is at the end
	if(Pos>=Size)
		Size=Pos+nb;

	// Next position
	Pos+=nb;
}


//------------------------------------------------------------------------------
void RIOFile::Seek(off_t pos)
{
	// Verify all internal conditions
	if((pos>=Size)&&(!CanWrite))
		throw RIOException(this,"Position outside of the file");

	// Update current position
	Pos=pos;

	// Look if outside the internal buffer
	if((Pos<RealInternalPos)||(Pos>RealInternalPos+static_cast<off_t>(InternalBufferSize)))
	{
		// Seek the file
		RealPos=Pos;
		lseek(Handle,RealPos,SEEK_SET);

		// Dirty the internal buffer
		InternalToRead=0;
		RealInternalPos=MaxSize;
	}
	else
		CurByte=&Internal[Pos-RealInternalPos];
}


//------------------------------------------------------------------------------
void RIOFile::SeekRel(off_t rel)
{
	// Verify all internal conditions
	if(static_cast<ssize_t>(Pos)+rel<0)//<static_cast<size_t>(labs(rel)))
		throw RIOException(this,"Position before beginning of the file");
	if((Pos+rel>Size)&&(!CanWrite))
		throw RIOException(this,"Position outside of the file");

	// Update current position
	Pos+=rel;

	// Look if outside the internal buffer
	if((Pos<RealInternalPos)||(Pos>RealInternalPos+static_cast<off_t>(InternalBufferSize)))
	{
		// Seek the file
		RealPos=Pos;
		lseek(Handle,rel,SEEK_CUR);

		// Dirty the internal buffer
		InternalToRead=0;
		RealInternalPos=MaxSize;
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
