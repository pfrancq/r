/*

	R Project Library

	RBlockFile.cpp

	Blocks File - Implementation.

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#include <rblockfileaccess.h>
#include <rblockfile.h>
using namespace R;
using namespace std;




//------------------------------------------------------------------------------
//
// RBlockFileAccess
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RBlockFileAccess::RBlockFileAccess(RBlockFile* file)
	: File(file), BlockId(0), CurrentPos(cNoRef), CurrentData(0), Lock(false)
{
	if(!File)
		mThrowRException("Null file pointer");
}


//------------------------------------------------------------------------------
RBlockFileAccess::RBlockFileAccess(RBlockFile* file,size_t blockid,size_t pos)
	: File(file), BlockId(0), CurrentPos(cNoRef), CurrentData(0), Lock(false)
{
	if(!File)
		mThrowRException("Null file pointer");
	File->Seek(BlockId,CurrentPos);
}


//------------------------------------------------------------------------------
RBlockFileAccess::RBlockFileAccess(RBlockFile& file)
	: File(&file), BlockId(0), CurrentPos(cNoRef), CurrentData(0), Lock(false)
{
}


//------------------------------------------------------------------------------
RBlockFileAccess::RBlockFileAccess(RBlockFile& file,size_t blockid,size_t pos)
	: File(&file), BlockId(0), CurrentPos(cNoRef), CurrentData(0), Lock(false)
{
	File->Seek(BlockId,CurrentPos);
}


//------------------------------------------------------------------------------
void RBlockFileAccess::ReadLock(void)
{
	if(!Block)
		mThrowRException("No block selected");
	Block->lData.ReadLock();
	Lock=true;
}


//------------------------------------------------------------------------------
void RBlockFileAccess::ReadUnLock(void)
{
	if(!Block)
		mThrowRException("No block selected");
	Block->lData.ReadUnLock();
	Lock=false;
}


//------------------------------------------------------------------------------
void RBlockFileAccess::WriteLock(void)
{
	if(!Block)
		mThrowRException("No block selected");
	Block->lData.WriteLock();
	Lock=true;
}


//------------------------------------------------------------------------------
void RBlockFileAccess::WriteUnLock(void)
{
	if(!Block)
		mThrowRException("No block selected");
	Block->lData.WriteUnLock();
	Lock=false;
}


//------------------------------------------------------------------------------
RBlockFileAccess::~RBlockFileAccess(void)
{
}