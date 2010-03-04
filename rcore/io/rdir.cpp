/*

	R Project Library

	RDir.cpp

	Directory - Implementation.

	Copyright 2004-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

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
#include <sys/stat.h>
#include <errno.h>
#if defined(_BSD_SOURCE) || defined(__GNUC__) || defined(__APPLE_)
	#include <unistd.h>
#else
	#include <io.h>
#endif
#include <fcntl.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rdir.h>
#include <riofile.h>
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
RDir::RDir(const RURI& uri)
	: RFile(uri), Data(0), Entries(10,5)
{
	Data=new Internal();
}


//-----------------------------------------------------------------------------
void RDir::Open(RIO::ModeType mode)
{
	int err;

	RFile::Open(mode);
	switch(Mode)
	{
		case RIO::Read:
			OpenEntries();
			break;

		case RIO::Append:
			throw(RIOException(this,"Cannot append a directory"));
			break;

		case RIO::Create:
			err=mkdir(URI.GetPath().Latin1(),S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH);
			if(err!=0)
			{
				switch(errno)
				{
					case EEXIST:
						throw(RIOException(this,"Directory already exists"));

					default:
						throw(RIOException(this,"Cannot create the directory"));
				}
			}
			OpenEntries();
			break;

		default:
			throw(RIOException(this,"No Valid Mode"));
	};
}


//-----------------------------------------------------------------------------
void RDir::OpenEntries(void)
{
	struct dirent* ep;
	RString Name;
	struct stat statbuf;
	int handle;

	Data->Handle=opendir(URI.GetPath().Latin1());
	if(!Data->Handle)
		throw(RIOException(this,"Directory does not exist"));
	RString Path=URI.GetPath()+RFile::GetDirSeparator();
	while((ep=readdir(Data->Handle)))
	{
		// Name of the 'file"
		Name=ep->d_name;

		if((Name==".")||(Name==".."))
			continue;

		// Open file
		handle=open(Path+Name,O_RDONLY);
		fstat(handle, &statbuf);

		// Look if it is a directoy
		if(S_ISDIR(statbuf.st_mode))
			Entries.InsertPtr(new RDir(Path+Name));
		else
			Entries.InsertPtr(new RIOFile(Path+Name));

		// Close file
		close(handle);
	}
}


//-----------------------------------------------------------------------------
void RDir::Clear(void)
{
	struct dirent* ep;
	RString Name;
	struct stat statbuf;
	int handle;
	bool IsDir;

	Data->Handle=opendir(URI.GetPath().Latin1());
	if(!Data->Handle)
		throw(RIOException(this,"Directory does not exist"));
	RString Path=URI.GetPath()+RFile::GetDirSeparator();
	while((ep=readdir(Data->Handle)))
	{
		// Name of the 'file"
		Name=ep->d_name;
		if((Name==".")||(Name==".."))
			continue;

		// If it is a directory, remove first its content
		handle=open(Path+Name,O_RDONLY);
		fstat(handle, &statbuf);
		IsDir=S_ISDIR(statbuf.st_mode);
		close(handle);
		if(IsDir)
			Clear();

		// Remove the file/directory.
		remove(Path+Name);
	}
}


//-----------------------------------------------------------------------------
RCursor<RFile> RDir::GetEntries(void) const
{
	if(!Data->Handle)
		ThrowRIOException(this,"directory not opened");
	return(RCursor<RFile>(Entries));
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
	Entries.Clear();
}


//-----------------------------------------------------------------------------
void RDir::CreateDirIfNecessary(const RString& dir,bool cascade)
{
	if(!dir.GetLen()) return;
	if(cascade)
	{
		int pos=dir.Find(RFile::GetDirSeparator(),-1);
		if(pos!=-1)
		{
			RString Parent=dir.Mid(0,pos);
			CreateDirIfNecessary(Parent);
		}
	}
	try
	{
		RDir Dir(dir);
		Dir.Open(RIO::Create);
	}
	catch(RIOException& )
	{
		// Ok, nothing to do.
	}
}


//-----------------------------------------------------------------------------
RDir::~RDir(void)
{
	if(Data)
		delete Data;
}
