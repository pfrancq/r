/*

	R Project Library

	RIO.cpp

	Generic Class - Implementation.

	Copyright 2002-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
// include files for R Project
#include <rio.h>
#include <rfile.h>
#include <rstring.h>
#include <rfile.h>
#include <rtextfile.h>
using namespace R;



//------------------------------------------------------------------------------
//
// class RIO::RSmartTempFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RIO::RSmartTempFile::RSmartTempFile(void)
	: Name(), Requested(false), Remove(false)
{
}


//------------------------------------------------------------------------------
RURI RIO::RSmartTempFile::GetName(bool remove)
{
	Remove=remove;
	if(!Requested)
	{
		Name=RFile::GetTempFile();
		Requested=true;
	}
	return(Name);
}


//------------------------------------------------------------------------------
RIO::RSmartTempFile::~RSmartTempFile(void)
{
	try
	{
		if(Remove)
			RFile::RemoveFile(Name);
	}
	catch(...)
	{
	}
}



//------------------------------------------------------------------------------
//
// class RIOException
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RIOException::RIOException(const char* str) throw()
	: RException(str)
{
}


//------------------------------------------------------------------------------
RIOException::RIOException(const char* func,long where,const char* str) throw()
	: RException(func,where,str)
{
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RFile* file,const char* str) throw()
	: RException()
{
	if(file)
		SetMsg(file->GetURI()()+": "+str);
	else
		SetMsg(str);
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RFile* file,const char* func,long where,const char* str) throw()
	: RException()
{
	if(file)
		SetMsg(func,where,file->GetURI()()+": "+str);
	else
		SetMsg(func,where,str);
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RIOFile* file,const char* str) throw()
	: RException()
{
	if(file)
		SetMsg(file->GetURI()()+" ("+RString::Number(file->GetPos())+"): "+str);
	else
		SetMsg(str);
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RIOFile* file,const char* func,long where,const char* str) throw()
	: RException()
{
	if(file)
		SetMsg(func,where,file->GetURI()()+" ("+RString::Number(file->GetPos())+"): "+str);
	else
		SetMsg(func,where,str);
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RTextFile* file,const char* str) throw()
	: RException()
{
	if(file)
		SetMsg(file->GetURI()()+" ("+RString::Number(file->GetLineNb())+"): "+str);
	else
		SetMsg(str);
}


//------------------------------------------------------------------------------
RIOException::RIOException(const RTextFile* file,const char* func,long where,const char* str) throw()
	: RException()
{
	if(file)
		SetMsg(func,where,file->GetURI()()+" ("+RString::Number(file->GetLineNb())+"): "+str);
	else
		SetMsg(func,where,str);
}
