/*

	R Project Library

	RFile.cpp

	Generic File - Implementation.

	Copyright 1999-2011 by Pascal Francq (pascal@francq.info).
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
#include <cstdlib>
#include <errno.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rfile.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
//
// RFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RFile::RFile(void)
	: Mode(RIO::Undefined), URI()
{
}


//------------------------------------------------------------------------------
RFile::RFile(const RURI& uri)
  : Mode(RIO::Undefined), URI(uri)
{
}


//------------------------------------------------------------------------------
RFile::RFile(const RFile& file)
  : Mode(file.Mode), URI(file.URI)
{
}


//------------------------------------------------------------------------------
void RFile::Open(RIO::ModeType mode)
{
	Mode=mode;
}


//------------------------------------------------------------------------------
void RFile::Open(const RURI& uri,RIO::ModeType mode)
{
	URI=uri;
	Open(mode);
}


//------------------------------------------------------------------------------
void RFile::Close(void)
{
}


//------------------------------------------------------------------------------
int RFile::Compare(const RFile& file) const
{
	return(URI.Compare(file.URI));
}


//------------------------------------------------------------------------------
int RFile::Compare(const RFile* file) const
{
	return(URI.Compare(file->URI));
}


//------------------------------------------------------------------------------
int RFile::Compare(const RString& uri) const
{
	return(URI.Compare(uri));
}


//------------------------------------------------------------------------------
void RFile::SetURI(const RURI& uri)
{
	URI=uri;
}


//------------------------------------------------------------------------------
const RString RFile::GetFileName(void) const
{
	RString res;
	RString Name(URI.GetPath());

	int pos=Name.Find('/',-1)+1;
	if(pos<=0)
		res=Name;
	else if(static_cast<size_t>(pos)<Name.GetLen())
		res=Name.Mid(pos);
	return(res);
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
void RFile::RemoveFile(const RURI& uri)
{
	if(!remove(uri.GetPath()))
		return;
	switch(errno)
	{
		case EACCES:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"The permissions of "+uri()+" deny to remove it");
		case EBUSY:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,uri()+" is used by the system");
		case ENOENT:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,uri()+" doesn't exist");
		case EPERM:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,uri()+" is a directory");
		case EROFS:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"Directory containing "+uri()+"is on a read-only");
	}
}


//------------------------------------------------------------------------------
void RFile::RenameFile(const RURI& olduri,const RURI& newuri)
{
	if(!rename(olduri.GetPath(),newuri.GetPath()))
		return;
	switch(errno)
	{
		case EACCES:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"Invalid permission on "+newuri()+" or "+olduri());
		case EBUSY:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"Directory named "+newuri()+" or "+olduri()+" is used by the system");
		case EEXIST:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"The directory "+newuri()+" isn't empty");
		case EINVAL:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,olduri()+" is a directory that contains "+newuri());
		case EISDIR:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,newuri()+" is a directory but the "+olduri()+" isn't");
		case EMLINK:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"Parent directory of "+newuri()+" would have too entries");
		case ENOENT:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,olduri()+" doesn't exist");
		case ENOSPC:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"No room to rename the file");
		case EROFS:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"The operation would involve writing to a directory on a read-only system");
		case EXDEV:
			throw RIOException(__PRETTY_FUNCTION__,__LINE__,"The two file names "+newuri()+" and "+olduri()+" are on different file systems");
	}
}


//------------------------------------------------------------------------------
RURI RFile::GetTempFile(void)
{
	return(RURI(tmpnam(0)));
}


//------------------------------------------------------------------------------
bool RFile::Exists(const RURI& uri)
{
    if(FILE* file=fopen(uri.GetPath(),"r"))
    {
        fclose(file);
        return(true);
    }
    return(false);
}


//------------------------------------------------------------------------------
RFile::~RFile(void)
{
	Close();
}
