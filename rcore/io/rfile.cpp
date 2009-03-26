/*

	R Project Library

	RFile.cpp

	Generic File - Implementation.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
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
	remove(uri.GetPath());
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
