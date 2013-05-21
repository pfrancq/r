	/*

	R Project Library

	RIOFile.cpp

	Generic File for Input/Output - Implementation.

	Copyright 1999-2008 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#include <errno.h>



//------------------------------------------------------------------------------
// include files for R Project
#include <riofile.h>
using namespace R;
using namespace std;


//------------------------------------------------------------------------------
// Global constants
static const size_t InternalBufferSize=10240;



//------------------------------------------------------------------------------
//
// RIOFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RDownload RIOFile::Get;


//------------------------------------------------------------------------------
inline void RIOFile::Test(void)
{
	off_t real=lseek(Handle,0,SEEK_CUR);
	if(real!=RealPos)
		cout<<"Internal problem"<<endl;
}


//------------------------------------------------------------------------------
RIOFile::RIOFile(void)
  : RFile(), Handle(-1), Size(0), Pos(0), Internal(0), InternalToRead(0), RealPos(0), CurByte(0)
{
}


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
		mThrowRIOException(this,"URI does not represent a file");

	// look if it is a local file
	if(URI.GetScheme()=="file")
	{
		local=true;
		File=URI.GetPath();
	}
	else
		File=RFile::GetTempFile();  // File must be download in a temporary file

	RFile::Open(mode);
	Pos=0;
	switch(Mode)
	{
		case RIO::Read:
			localmode=O_RDONLY;
			CanWrite=false;
			CanRead=true;
			break;

		case RIO::Write:
			if(!local)
				mThrowRIOException(this,"Cannot write with an URL");
			localmode=O_WRONLY | O_CREAT;
			CanWrite=true;
			CanRead=false;
			break;

		case RIO::ReadWrite:
			if(!local)
				mThrowRIOException(this,"Cannot write with an URL");
			localmode=O_RDWR | O_CREAT;
			CanWrite=true;
			CanRead=true;
			break;

		case RIO::Append:
			if(!local)
				mThrowRIOException(this,"Cannot append to an URL");
			localmode=O_WRONLY | O_CREAT | O_APPEND;
			CanWrite=true;
			CanRead=true;
			break;

		case RIO::Create:
			if(!local)
				mThrowRIOException(this,"Cannot create with an URL");
			localmode=O_WRONLY | O_CREAT | O_TRUNC;
			CanWrite=true;
			CanRead=false;
			break;

		default:
			mThrowRIOException(this,"No valid mode");
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
		Handle=open(File.GetPath().ToLatin1(),localmode);
	}
	else
		Handle=open(File.GetPath().ToLatin1(),localmode,S_IREAD|S_IWRITE);
	if(Handle==-1)
	{
		switch(errno)
		{
			case EACCES:
				mThrowRIOException(this,"The file exists but it cannot be access at the requested mode");
			case EEXIST:
				mThrowRIOException(this,"Both O_CREAT and O_EXCL are set, and the named file already exists");
			case EINTR:
				mThrowRIOException(this,"The open operation was interrupted by a signal");
			case EISDIR:
				mThrowRIOException(this,"The mode specified is write access, but the file is a directory");
			case EMFILE:
				mThrowRIOException(this,"The process has too many files open");
			case ENFILE:
				mThrowRIOException(this,"The system has too many files open");
			case ENOENT:
				mThrowRIOException(this,"The named file does not exist, and Create is not specified");
			case ENOSPC:
				mThrowRIOException(this,"No disk space left");
			case ENXIO:
				mThrowRIOException(this,"O_NONBLOCK and O_WRONLY are both set in the arguments, and the file is a FIFO");
			case EROFS:
				mThrowRIOException(this,"The file resides on a read-only file system or the file does not exist");
			default:
				mThrowRIOException(this,"Cannot open the file");
		}
	}

	// Read the total size of the file
	fstat(Handle,&statbuf);
	Size=statbuf.st_size;

	// Position virtually the file at the beginning or at the end if it is open in append mode
	if(Mode==RIO::Append)
		Pos=Size;

	// Suppose nothing is in the buffer
	RealInternalPos=MaxOffT;
}


//------------------------------------------------------------------------------
void RIOFile::Open(const RURI& uri,RIO::ModeType mode)
{
	RFile::Open(uri,mode);
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
		if(close(Handle)==-1)
		{
			switch(errno)
			{
				case EBADF:
					mThrowRIOException(this,"Not a valid descriptor");
				case EINTR:
					mThrowRIOException(this,"The close call was interrupted by a signal");
				case EIO:
			#ifndef WIN32
				case EDQUOT:
			#endif
					mThrowRIOException(this,"When the file is accessed by NFS, these errors from write can sometimes not be detected until close");
				default:
					mThrowRIOException(this,"Cannot close the file");
			}
		}
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
		mThrowRIOException(this,"Can't read in the file");
	if(!CanRead)
		mThrowRIOException(this,"No Read access");
	if(End())
		mThrowRIOException(this,"End of the file reached");

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
				RealInternalPos=MaxOffT;
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
		if(move)
		{
			CurByte+=left;
			Pos+=left;
			InternalToRead-=left;
		}
	}

	// Return number of bytes read
	return(nb);
}


//------------------------------------------------------------------------------
void RIOFile::Write(const char* buffer,size_t nb)
{
	// Verify all internal conditions
	if(Handle==-1)
		mThrowRIOException(this,"Can't write into the file");
	if(!CanWrite)
		mThrowRIOException(this,"No write access");

	// Verify that the internal positions are OK
	if(Pos!=RealPos)
	{
		RealPos=Pos;
		lseek(Handle,Pos,SEEK_SET);
	}

	// Write into the file
	off_t after=Pos+nb;
	const char* ptr=buffer;
	for(ssize_t towrite=nb;towrite;)
	{
		ssize_t written(write(Handle,ptr,towrite));
		towrite-=written;
		if(towrite)
			ptr=&buffer[written];
	}

	#ifdef windows
		flushall();
	#endif

	// Verify if a part of the buffer must be stored into the internal buffer
    if((Pos<RealInternalPos+static_cast<off_t>(InternalBufferSize))&&(after>=RealInternalPos))
	{
		// If after the write, we are still in the internal buffer -> update it
    	// Else dirtied it.
		if(after<RealInternalPos+static_cast<off_t>(InternalBufferSize))
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
				size_t max=InternalBufferSize+RealInternalPos-Pos;
				if(len>max)
					len=max;
			}
			memcpy(begini,beginb,len);

			size_t pos=static_cast<size_t>(after-RealInternalPos);
			CurByte=&Internal[pos];
			InternalToRead=InternalBufferSize-pos-1;
		}
		else
		{
			RealInternalPos=MaxOffT;
			InternalToRead=0;
		}
	}

	// Increase the size only if the current position is at the end
	if(after>=Size)
		Size=after;

	// Next position
	Pos=after;
	RealPos=after;
}


//------------------------------------------------------------------------------
void RIOFile::Seek(off_t pos)
{
	// Verify all internal conditions
	if(Handle==-1)
		mThrowRIOException(this,"Can't seek the file");
	if((pos>=Size)&&(!CanWrite))
		mThrowRIOException(this,"Position outside of the file");

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
		RealInternalPos=MaxOffT;
	}
	else
	{
		CurByte=&Internal[Pos-RealInternalPos];
		InternalToRead=InternalBufferSize+RealInternalPos-Pos;
	}
}


//------------------------------------------------------------------------------
void RIOFile::SeekRel(off_t rel)
{
	// Verify all internal conditions
	if(Handle==-1)
		mThrowRIOException(this,"Can't seek the file");
	if(static_cast<ssize_t>(Pos)+rel<0)//<static_cast<size_t>(labs(rel)))
		mThrowRIOException(this,"Position before beginning of the file");
	if((Pos+rel>Size)&&(!CanWrite))
		mThrowRIOException(this,"Position outside of the file");

	// Update current position
	Pos+=rel;

	// Look if outside the internal buffer
	if((Pos<RealInternalPos)||(Pos>RealInternalPos+static_cast<off_t>(InternalBufferSize)))
	{
		// Seek the file
		RealPos=Pos;
		lseek(Handle,Pos,SEEK_SET);

		// Dirty the internal buffer
		InternalToRead=0;
		RealInternalPos=MaxOffT;
	}
	else
	{
		CurByte+=rel;
		InternalToRead-=rel;
	}
}


//------------------------------------------------------------------------------
void RIOFile::SeekToEnd(void)
{
	// Verify all internal conditions
	if(Handle==-1)
		mThrowRIOException(this,"Can't seek the file");
	Seek(Size-1);
}


//------------------------------------------------------------------------------
void RIOFile::Truncate(off_t newsize)
{
	// Verify all internal conditions
	if(Handle==-1)
		mThrowRIOException(this,"Can't truncate the file");
	if(!CanWrite)
		mThrowRIOException(this,"No write access");

	if(ftruncate(Handle,newsize)==-1)
	{
		switch(errno)
		{
			case EACCES:
				mThrowRIOException(this,"File is a directory");
			case EINVAL:
				mThrowRIOException(this,"New length is negative");
			case EFBIG:
				mThrowRIOException(this,"New size beyond the limits of the operating system");
			case EIO:
				mThrowRIOException(this,"A hardware I/O error occurred");
			case EPERM:
				mThrowRIOException(this,"File is \"append-only\" or \"immutable\"");
			case EINTR:
				mThrowRIOException(this,"Operation was interrupted by a signal");
		}
	}

	Size=newsize;
	if(RealPos>=Size)
	{
		Seek(newsize);
	}
}


//------------------------------------------------------------------------------
RIOFile::~RIOFile(void)
{
	Close();
	if(Internal)
		delete[] Internal;
}
