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
#include <rblockfile.h>
#include <rcursor.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
// Constants
const size_t HeaderSize=2048;



//------------------------------------------------------------------------------
//
// RBlockFile::Block
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RBlockFile::Block
{
public:
	size_t Id;
	size_t NbAccess;
	bool Dirty;
	char* Data;

	Block(size_t id,size_t size) : Id(id), NbAccess(0), Dirty(false), Data(0)
	{
		Data=new char[size];
	 /*memset(Data,0,sizeof(char)*size);*/
	}
	inline int Compare(const Block& block) const {return(CompareIds(Id,block.Id));}
	inline int Compare(size_t id) const {return(CompareIds(Id,id));}
	inline void Load(RIOFile* file,size_t size) {file->Read(Data,size);}
	inline void Save(RIOFile* file,size_t size) {file->Write(Data,size);}
	~Block(void)
	{
		if(Data)
		{
			delete[] Data;
			Data=0;
		}
	}
};



//------------------------------------------------------------------------------
//
// RBlockFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RBlockFile::RBlockFile(const RURI& uri,size_t nbcaches)
  : RIOFile(uri), Type(WriteBack), BlockSize(cNoRef), Cache(nbcaches),
    Current(0), NbBlocks(0)
{
	if(!BlockSize)
		mThrowRIOException(this,"Block size cannot be null");
}


//------------------------------------------------------------------------------
RBlockFile::RBlockFile(const RURI& uri,size_t blocksize,size_t nbcaches)
  : RIOFile(uri), Type(WriteBack), BlockSize(blocksize*1024), Cache(nbcaches),
    Current(0), NbBlocks(0)
{
	if(!BlockSize)
		mThrowRIOException(this,"Block size cannot be null");
}


//------------------------------------------------------------------------------
void RBlockFile::Open(RIO::ModeType mode)
{
	RIOFile::Open(mode);
	if(End())
	{
		// Write the architecture and the block size
		char Arch(static_cast<char>(sizeof(size_t)));
		RIOFile::Write(&Arch,sizeof(char));
		if(BlockSize==cNoRef)
			BlockSize=1024*1024;
		RIOFile::Write((char*)&BlockSize,sizeof(size_t));

		// Fill the rest with 0
		size_t Size(HeaderSize-sizeof(char)+sizeof(size_t));
		char Header[HeaderSize];
		memset(Header,0,Size);
		RIOFile::Write(Header,Size);
	}
	else
	{
		char Arch;
		RIOFile::Seek(0);
		RIOFile::Read(&Arch,sizeof(char));
		if(Arch!=static_cast<char>(sizeof(size_t)))
			mThrowRIOException(this,"Wrong architecture.");
		size_t Size;
		RIOFile::Read((char*)&Size,sizeof(size_t));
		if(BlockSize!=Size)
			mThrowRIOException(this,"Wrong block size.");
	}
	NbBlocks=(GetSize()-HeaderSize)/BlockSize;
}


//------------------------------------------------------------------------------
void RBlockFile::Close(void)
{
	if(IsOpen())
		Flush();
}


//------------------------------------------------------------------------------
void RBlockFile::Clear(void)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Close the file, erase it, initialize the parameter
	RIOFile::Close();
	RFile::RemoveFile(GetURI());
	Cache.Clear();
	Current=0;
	NbBlocks=0;

	// Open the file
	Open(RIO::ReadWrite);
}


//------------------------------------------------------------------------------
void RBlockFile::SetCacheType(CacheType type)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	CacheType Old(Type);
	Type=type;
	if((Old==WriteBack)&&(Type==WriteThrough))
		Flush();
}


//------------------------------------------------------------------------------
void RBlockFile::Flush(void)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Go trough all the blocks in memory and save those who are dirty
	RCursor<Block> Cur(Cache);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		if(Cur()->Dirty)
		{
			RIOFile::Seek(((Cur()->Id-1)*BlockSize)+HeaderSize);
			Cur()->Save(this,BlockSize);
			Cur()->Dirty=true;
		}
	}
}


//------------------------------------------------------------------------------
int RBlockFile::sortOrderAccess(const void* a,const void* b)
{
	size_t af((*((RBlockFile::Block**)(a)))->NbAccess);
	size_t bf((*((RBlockFile::Block**)(b)))->NbAccess);

	if(af==bf) return(0);
	if(af>bf)
		return(-1);
	else
		return(1);
}


//------------------------------------------------------------------------------
RBlockFile::Block* RBlockFile::LoadBlock(size_t id)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Search for the block in memory
	Block* ptr(Cache.GetPtr(id));
	if(ptr) return(ptr);

	// Block must be load in memory
	if(Cache.GetNb()<Cache.GetMaxNb())
	{
		// Cache is not full
		ptr=new Block(id,BlockSize);
		Cache.InsertPtr(ptr);
		if(id<=NbBlocks)
		{
			RIOFile::Seek(((id-1)*BlockSize)+HeaderSize);
			ptr->Load(this,BlockSize);
		}
		else
			NbBlocks=id;
	}
	else
	{
		// Cache is full -> The block must replace another one
		// Select the less used block in cache
		Cache.ReOrder(sortOrderAccess);
		ptr=Cache[0];
		if(ptr->Dirty)
		{
			// If necessary save the old block
			RIOFile::Seek(((ptr->Id-1)*BlockSize)+HeaderSize);
			ptr->Save(this,BlockSize);
			ptr->Dirty=false;
		}
		if(ptr==Current)
			Current=0;
		ptr->Id=id;
		if(id<=NbBlocks)
		{
			RIOFile::Seek(((id-1)*BlockSize)+HeaderSize);
			ptr->Load(this,BlockSize);
		}
		else
			NbBlocks=id;
		ptr->NbAccess=0;

		// Since a identifier was replaced, Cache must be re-ordered by identifiers
		Cache.ReOrder();
	}
	return(ptr);
}


//------------------------------------------------------------------------------
void RBlockFile::Read(char* buffer,size_t nb)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");
	if(!Current)
		mThrowRIOException(this,"No current block");
	if(CurrentPos+nb>BlockSize)
		mThrowRIOException(this,"Size of a block is limited to "+RString::Number(BlockSize));
	memcpy(buffer,CurrentData,nb*sizeof(char));
	CurrentPos+=nb;
	CurrentData+=nb;
	Current->NbAccess++;
}



//------------------------------------------------------------------------------
void RBlockFile::Write(const char* buffer,size_t nb)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");
	if(!Current)
		mThrowRIOException(this,"No current block");
	if(CurrentPos+nb>BlockSize)
		mThrowRIOException(this,"Size of a block is limited to "+RString::Number(BlockSize));
	memcpy(CurrentData,buffer,nb*sizeof(char));
	if(Type==WriteThrough)
	{
		// Must write it on file too
		RIOFile::Seek((Current->Id-1)*BlockSize+CurrentPos+HeaderSize);
		RIOFile::Write(buffer,nb);
	}
	else
		Current->Dirty=true;
	CurrentPos+=nb;
	CurrentData+=nb;
	Current->NbAccess++;
}


//------------------------------------------------------------------------------
void RBlockFile::MoveBlock(size_t blockid,size_t start,size_t end,size_t size)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");
	Seek(blockid,end+size);
	memmove(&Current->Data[end],&Current->Data[start],size);
	if(Type==WriteThrough)
	{
		// Must write it on file too
		RIOFile::Seek((Current->Id-1)*BlockSize+end+HeaderSize);
		RIOFile::Write(&Current->Data[end],size);
	}
	else
		Current->Dirty=true;
}


//------------------------------------------------------------------------------
const char* RBlockFile::GetPtr(size_t size)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");
	if(!Current)
		mThrowRIOException(this,"No current block");
	if(CurrentPos+size>BlockSize)
		mThrowRIOException(this,"Size of a block is limited to "+RString::Number(BlockSize));
	Current->NbAccess++;
	const char* ptr(CurrentData);
	CurrentPos+=size;
	CurrentData+=size;
	return(ptr);
}


//------------------------------------------------------------------------------
void RBlockFile::Seek(size_t blockid,size_t pos)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");
	if(pos>BlockSize)
		mThrowRIOException(this,"Size of a block is limited to "+RString::Number(BlockSize));

	// Select the current block
	if((!Current)||(Current->Id!=blockid))
	{
		Current=LoadBlock(blockid);
		CurrentPos=pos;
		CurrentData=&Current->Data[CurrentPos];
	}
	else if(CurrentPos!=pos)
	{
		CurrentPos=pos;
		CurrentData=&Current->Data[CurrentPos];
	}
}


//------------------------------------------------------------------------------
void RBlockFile::SeekRel(long pos)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");
	if(!Current)
		mThrowRIOException(this,"No current block");
	if(pos+CurrentPos>BlockSize)
		mThrowRIOException(this,"Size of a block is limited to "+RString::Number(BlockSize));
	if(static_cast<ssize_t>(CurrentPos)+pos<0)
		mThrowRIOException(this,"Relative position outside the block");

	CurrentPos+=pos;
	CurrentData+=pos;
}


//------------------------------------------------------------------------------
RBlockFile::~RBlockFile(void)
{
	Close();
}
