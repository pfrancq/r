/*

	R Project Library

	RIndexesFile.cpp

	Generic Indexes File - Implementation.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#include <rindexesfile.h>
#include <rnumcursor.h>
using namespace R;
using namespace std;



//------------------------------------------------------------------------------
// constants
const size_t SizeT2=2*sizeof(size_t);
const size_t SizeT3=3*sizeof(size_t);
const size_t SizeT4=4*sizeof(size_t);
const size_t SizeT5=5*sizeof(size_t);
const size_t SizeT6=6*sizeof(size_t);



//------------------------------------------------------------------------------
//
// RIndexesFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
RIndexesFile::RIndexesFile(const RURI& uri,size_t blocksize,size_t nbcaches,size_t tolerance)
	: RBlockFile(uri,blocksize,nbcaches), FreeSpaces(30), Tolerance(tolerance*1024)
{
}


//------------------------------------------------------------------------------
void RIndexesFile::Open(RIO::ModeType mode)
{
	RBlockFile::Open(mode);
	FreeSpaces.Verify(GetNbBlocks()+10);
	for(size_t i=0;i<GetNbBlocks();i++)
	{
		// Read the free space and insert it
		size_t Free;
		RBlockFile::Seek(i+1,0);
		Read((char*)&Free,sizeof(size_t));
		FreeSpaces.InsertAt(Free,i,true);
	}
}


//------------------------------------------------------------------------------
void RIndexesFile::Close(void)
{
	RBlockFile::Close();
}


//------------------------------------------------------------------------------
void RIndexesFile::Clear(void)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Clear the block file and the free spaces
	RBlockFile::Clear();
	FreeSpaces.Clear();
}


//------------------------------------------------------------------------------
size_t RIndexesFile::GetIndex(size_t block,size_t id1,size_t id2,bool& find)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	struct Index
	{
		size_t Id1;
		size_t Id2;
		size_t Pos;
	};

	size_t NbMin(0),NbMax,i=0,cur1,cur2;
	const Index* Table;
	bool CompNeg(true);
	bool Cont=true,NotLast=true;

	// Read number of records in that block
	RBlockFile::Seek(block,sizeof(size_t));
	Read((char*)&NbRecs,sizeof(size_t));
	Table=reinterpret_cast<const Index*>(GetPtr(NbRecs*SizeT3));

	find=false;
	if(!NbRecs)
		return(0);
	NbMax=NbRecs-1;
	NbMin=0;
	while(Cont)
	{
		i=(NbMax+NbMin)/2;
		cur1=Table[i].Id1;
		if(cur1==id1)
		{
			cur2=Table[i].Id2;
			if(cur2==id2)
			{
				find=true;
				return(i);
			}
			if(cur2>id2)
			{
				CompNeg=false;
				NbMax = i;
				if(i) NbMax--;
			}
			else
			{
				NbMin = i+1;
				CompNeg=true;
			}
		}
		else if(cur1>id1)
		{
			CompNeg=false;
			NbMax = i;
			if(i) NbMax--;
		}
		else
		{
			NbMin = i+1;
			CompNeg=true;
		}
		Cont = NotLast;
		if(NbMin>=NbMax) NotLast=false;
	}
	if(CompNeg) i++;
	return(i);
}


//------------------------------------------------------------------------------
void RIndexesFile::MoveRecords(size_t blockid,size_t entry,long rel)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Go to the record position in the record address table
	size_t Pos;
	size_t First(cNoRef);
	RBlockFile::Seek(blockid,SizeT2+entry*SizeT3);

	// Go trough all the next record entries to update the record position
	while(entry<NbRecs)
	{
		RBlockFile::SeekRel(SizeT2);
		RBlockFile::Read((char*)&Pos,sizeof(size_t));
		Pos+=rel;
		if(First==cNoRef)
			First=Pos;
		RBlockFile::SeekRel(-sizeof(size_t));
		RBlockFile::Write((char*)&Pos,sizeof(size_t));
		entry++;
	}

	// Move the (First-Pos+rel) bytes in memory starting from Pos-rel
	RBlockFile::MoveBlock(blockid,Pos-rel,Pos,First-Pos);
}


//------------------------------------------------------------------------------
void RIndexesFile::ModifyFreeSpace(size_t blockid,long rel)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	RBlockFile::Seek(blockid,0);
	rel+=FreeSpaces[blockid-1];
	FreeSpaces.InsertAt(rel,blockid-1,true);
	RBlockFile::Write((char*)&rel,sizeof(size_t));
}


//------------------------------------------------------------------------------
void RIndexesFile::NewRecord(size_t blockid,size_t id1,size_t id2,size_t entry,size_t size)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	size_t Pos;

	if(!NbRecs)
	{
		// Write that only one record is present
		RBlockFile::Seek(blockid,0);
		size_t Info(BlockSize-size-SizeT2-SizeT3);
		FreeSpaces.InsertAt(Info,blockid-1,true);
		Write((char*)(&Info),sizeof(size_t));
		NbRecs=1;
		Write((char*)(&NbRecs),sizeof(size_t));

		// Write an entry and position to file to right place
		Info=id1;
		Write((char*)(&Info),sizeof(size_t));
		Info=id2;
		Write((char*)(&Info),sizeof(size_t));
		Pos=BlockSize-size;
		Write((char*)(&Pos),sizeof(size_t));
	}
	else if(entry==NbRecs)
	{
		// The entry is the last one :
		// Go to previous entry and the position is obtained
		size_t index1,index2;
		RBlockFile::Seek(blockid,SizeT2+(entry-1)*SizeT3);

		// Read the old index and the old record position
		Read((char*)(&index1),sizeof(size_t));
		Read((char*)(&index2),sizeof(size_t));
		Read((char*)(&Pos),sizeof(size_t));
		Pos-=size;
		index1=id1;
		index2=id2;
		Write((char*)(&index1),sizeof(size_t));
		Write((char*)(&index2),sizeof(size_t));
		Write((char*)(&Pos),sizeof(size_t));

		// The spaces and the number of entries in the records table are inserted
		ModifyFreeSpace(blockid,-size-SizeT3);
		RBlockFile::Seek(blockid,sizeof(size_t));
		NbRecs++;
		Write((char*)(&NbRecs),sizeof(size_t));
	}
	else
	{
		// The record must be inserted at 'entry':
		// 1. Read the position of the record corresponding to 'entry-1' -> Pos
		// 2. The new record will be inserted at 'entry' and the position Pos-size :
		// 3. Decrease all the records starting from 'entry' from size positions.
		// 4. Move the records in memory from size.

		// Initialize
		size_t newindex1,newindex2,oldindex1,oldindex2,newpos,oldpos,LastPos;

		// First Step
		RBlockFile::Seek(blockid,SizeT2+(entry-1)*SizeT3);
		Read((char*)(&newindex1),sizeof(size_t));
		Read((char*)(&newindex2),sizeof(size_t));
		Read((char*)(&newpos),sizeof(size_t));
		Pos=newpos;  // This is where ends the last records

		// Second Step
		newindex1=id1;
		newindex2=id2;
		newpos-=size;

		// Third Step : Go trough the entries
		while(entry<=NbRecs)
		{
			// Read the old index and the old record position at 'entry' if necessary
			if(entry<NbRecs)
			{
				Read((char*)(&oldindex1),sizeof(size_t));
				Read((char*)(&oldindex2),sizeof(size_t));
				Read((char*)(&oldpos),sizeof(size_t));
				LastPos=oldpos;       // Suppose it it the last record to move

				// Write to old position
				RBlockFile::SeekRel(-SizeT3);
			}

			// Write the new entry
			Write((char*)(&newindex1),sizeof(size_t));
			Write((char*)(&newindex2),sizeof(size_t));
			Write((char*)(&newpos),sizeof(size_t));
			newpos=oldpos-size;
			newindex1=oldindex1;
			newindex2=oldindex2;
			entry++;
		}

		// Fourth step : Move the Pos-LastPos bytes in memory starting from LastPos to newpos
		if(LastPos>Pos)
			mThrowRException("Corrupted index file '"+GetURI()()+"'");
		if(Pos-LastPos)
			RBlockFile::MoveBlock(blockid,LastPos,newpos,Pos-LastPos);

		// The spaces and the number of entries in the records table are inserted
		ModifyFreeSpace(blockid,-size-SizeT3);
		RBlockFile::Seek(blockid,sizeof(size_t));
		NbRecs++;
		Write((char*)(&NbRecs),sizeof(size_t));

		// Pos must point where the new record was inserted
		Pos-=size;
	}

	// Position the block at the right place
	RBlockFile::Seek(blockid,Pos);
}


//------------------------------------------------------------------------------
void RIndexesFile::Seek(off_t)
{
	mThrowRIOException(this,"Method cannot be called");
}


//------------------------------------------------------------------------------
void RIndexesFile::Write(const char* buffer,size_t nb)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	RBlockFile::Write(buffer,nb);
}


//------------------------------------------------------------------------------
void RIndexesFile::Read(char* buffer,size_t nb)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	RBlockFile::Read(buffer,nb);
}


//------------------------------------------------------------------------------
void RIndexesFile::EraseRecord(size_t blockid,size_t id1,size_t id2)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Find the entry in the index table
	bool find;
	size_t entry(GetIndex(blockid,id1,id2,find));
	if(!find)
		mThrowRIOException(this,"No record ("+RString::Number(id1)+","+RString::Number(id2)+") in block "+RString::Number(blockid));

	// To erase a record :
	// 1. Look if it is the first record :
	//    a) Yes : It ends at the end of the block.
	//    b) No : It ends at the beginning of block 'entry-1'
	// Compute the size of bytes to skip -> Size
	// 2. Move all the records after 'entry' from Size position to the end
   // 3. Move the records in memory to delete the record entry

	// Initialize
	size_t Id1,Id2,Pos,OldPos,Size,FirstPos;

	// Step 1
	if(entry)
	{
		// Not the first record -> Read both entries
		RBlockFile::Seek(blockid,SizeT2+(entry-1)*SizeT3);
		RBlockFile::Read((char*)&Id1,sizeof(size_t));
		RBlockFile::Read((char*)&Id2,sizeof(size_t));
		RBlockFile::Read((char*)&Size,sizeof(size_t));
		RBlockFile::Read((char*)&Id1,sizeof(size_t));
		RBlockFile::Read((char*)&Id2,sizeof(size_t));
		RBlockFile::Read((char*)&OldPos,sizeof(size_t));
		Size-=OldPos;
	}
	else
	{
		// First record
		RBlockFile::Seek(blockid,SizeT2+entry*SizeT3);
		RBlockFile::Read((char*)&Id1,sizeof(size_t));
		RBlockFile::Read((char*)&Id2,sizeof(size_t));
		RBlockFile::Read((char*)&OldPos,sizeof(size_t));
		Size=BlockSize-OldPos;
	}
	FirstPos=OldPos;

	//
	// Go trough all the next record entries and move them Size bytes to the end
	entry++;
	while(entry<=NbRecs)
	{
		RBlockFile::Read((char*)&Id1,sizeof(size_t));
		RBlockFile::Read((char*)&Id2,sizeof(size_t));
		RBlockFile::Read((char*)&Pos,sizeof(size_t));
		RBlockFile::SeekRel(-SizeT6);
		OldPos=Pos;  // Suppose last record
		Pos+=Size;
		RBlockFile::Write((char*)&Id1,sizeof(size_t));
		RBlockFile::Write((char*)&Id2,sizeof(size_t));
		RBlockFile::Write((char*)&Pos,sizeof(size_t));
		RBlockFile::SeekRel(SizeT3);
		entry++;
	}

	// Move the OldPos-Pos+Size bytes in memory starting from Pos-Size to Pos
	if(FirstPos-OldPos)
		RBlockFile::MoveBlock(blockid,Pos-Size,Pos,FirstPos-OldPos);

	// Modify the free space and the number of records
	ModifyFreeSpace(blockid,Size+SizeT3);   // The spaces and the entry in the records table are removed
	NbRecs--;
	RBlockFile::Seek(blockid,sizeof(size_t));
	RBlockFile::Write((char*)&NbRecs,sizeof(size_t));

	// Position the block at the right place
	RBlockFile::Seek(blockid,FirstPos+Size);
}


//------------------------------------------------------------------------------
void RIndexesFile::Seek(size_t blockid,size_t id1,size_t id2)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Find the right index in the table
	bool Find;
	size_t Entry(GetIndex(blockid,id1,id2,Find));
	if(!Find)
		mThrowRIOException(this,"No record ("+RString::Number(id1)+","+RString::Number(id2)+") in block "+RString::Number(blockid));

	// Read the position of the record and seek to it
	RBlockFile::Seek(blockid,SizeT4+(Entry*SizeT3));
	size_t Pos;
	Read((char*)&Pos,sizeof(size_t));
	RBlockFile::Seek(blockid,Pos);
}


//------------------------------------------------------------------------------
void RIndexesFile::Seek(size_t& blockid,size_t id1,size_t id2,size_t size)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	if(size>BlockSize)
		mThrowRIOException(this,"The size of a record ("+RString::Number(size)+") exceeds the block size ("+RString::Number(BlockSize)+")");

	size_t Entry,Pos;

	if(blockid)
	{
		size_t OldSize;

		// Find the right index in the table
		bool Find;
		Entry=GetIndex(blockid,id1,id2,Find);
		if(!Find)
			mThrowRIOException(this,"No record ("+RString::Number(id1)+","+RString::Number(id2)+") in block "+RString::Number(blockid));

		// Compute the size previously allocated
		RBlockFile::Seek(blockid,SizeT4+(Entry*SizeT3));
		Read((char*)&Pos,sizeof(size_t));
		if(Entry)
		{
			// The size of the record is the position of the current index with the previous one
			size_t Next;
			RBlockFile::SeekRel(-SizeT5); // Skip the current identifier and position and the previous identifier in the table
			Read((char*)&Next,sizeof(size_t));
			OldSize=Next-Pos;
		}
		else
		{
			// It is the last record
			OldSize=BlockSize-Pos;
		}

		if(OldSize==size)
		{
			// Everything is fine -> just position the file to right position
			RBlockFile::Seek(blockid,Pos);
			return;
		}
		else if(OldSize>size)
		{
			// We need less space than previous
			// -> Shift all the blocks to the end of the file (increase the addresses).
			long inc(OldSize-size);
			MoveRecords(blockid,Entry,inc);
			ModifyFreeSpace(blockid,inc);
			RBlockFile::Seek(blockid,Pos+inc);
			return;
		}
		else
		{
			// We need more space.
			if(size-OldSize<=FreeSpaces[blockid-1])
			{
				// Enough space free
				// -> Shift all the blocks to the empty spaces (decrease the addresses).
				long dec(OldSize-size);
				MoveRecords(blockid,Entry,dec);
				ModifyFreeSpace(blockid,dec);
				RBlockFile::Seek(blockid,Pos+dec);
				return;
			}

			// The data must be moved to another block
			// -> Liberate the entry in the file
			EraseRecord(blockid,id1,id2);
		}
	}

	// A block must be search
	// Look if an existing block can hold the size asked
	blockid=0;
	RNumCursor<size_t> Cur(FreeSpaces);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		blockid++;
		if((Cur()>Tolerance)&&(Cur()>=size+SizeT3))
			break;
	}
	if(Cur.End())
	{
		// New block to create :
		// Position it to the beginning and specify that they are one record.
		blockid=GetNbBlocks()+1;
		NbRecs=0;   // Specify that no records are in the new block
		NewRecord(blockid,id1,id2,0,size);
		return;
	}

	// Find the position in the entry table where to store the identifier
	bool Find;
	size_t NewEntry(GetIndex(blockid,id1,id2,Find));
	if(Find)
		cout<<"RIndexFile::Seek(size_t&,size_t,size_t) : Existing entry will be overwritten"<<endl;
/*	if(Find)
		throw RIOException(this,"RIndexFile::Seek(size_t&,size_t,size_t) : Big internal problem");*/

	NewRecord(blockid,id1,id2,NewEntry,size);
}


//------------------------------------------------------------------------------
RIndexesFile::~RIndexesFile(void)
{
	Close();
}
