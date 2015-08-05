/*

	R Project Library

	RIndexFile.cpp

	Generic Index File - Template Implementation.

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
//
// RKeyValueFile
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
template<class K>
	RKeyValueFile<K>::RKeyValueFile(const RURI& uri,size_t blocksize,size_t nbcaches,size_t tolerance)
	: RBlockFile(uri,blocksize,nbcaches), FreeSpaces(30), Tolerance(tolerance*1024)
{
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Open(RIO::ModeType mode)
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
template<class K>
	void RKeyValueFile<K>::Close(void)
{
	RBlockFile::Close();
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Clear(void)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Clear the block file and the free spaces
	RBlockFile::Clear();
	FreeSpaces.Clear();
}


//------------------------------------------------------------------------------
template<class K>
	size_t RKeyValueFile<K>::GetIndex(size_t block,K& key,bool& find)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	size_t NbMin(0),NbMax,i=0,cur;
	const char* Table;
	bool CompNeg(true);
	bool Cont=true,NotLast=true;

	// Read number of records in that block
	RBlockFile::Seek(block,cLen);
	Read((char*)&NbRecs,cLen);
	Table=GetPtr(NbRecs*(key.GetSize()+cLen));

	find=false;
	if(!NbRecs)
		return(0);
	NbMax=NbRecs-1;
	NbMin=0;
	while(Cont)
	{
		i=(NbMax+NbMin)/2;
		int Ret(key.Compare(Table+i*(key.GetSize()+cLen)));
		if(Ret==0)
		{
			find=true;
			return(i);
		}
		else if(Ret>0)
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
template<class K>
	void RKeyValueFile<K>::MoveRecords(size_t blockid,size_t entry,long rel)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Go to the record position in the record address table
	size_t Pos;
	size_t First(cNoRef);
	K key;
	RBlockFile::Seek(blockid,cLen2+(entry*(key.GetSize()+cLen)));

	// Go trough all the next record entries to update the record position
	while(entry<NbRecs)
	{
		RBlockFile::SeekRel(key.GetSize());
		RBlockFile::Read((char*)&Pos,cLen);
		Pos+=rel;
		if(First==cNoRef)
			First=Pos;
		RBlockFile::SeekRel(-cLen);
		RBlockFile::Write((char*)&Pos,cLen);
		entry++;
	}

	// Move the (First-Pos+rel) bytes in memory starting from Pos-rel
	RBlockFile::MoveBlock(blockid,Pos-rel,Pos,First-Pos);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::ModifyFreeSpace(size_t blockid,long rel)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	RBlockFile::Seek(blockid,0);
	rel+=FreeSpaces[blockid-1];
	FreeSpaces.InsertAt(rel,blockid-1,true);
	RBlockFile::Write((char*)&rel,cLen);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::NewRecord(size_t blockid,K& key,size_t entry,size_t size)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	size_t Pos;

	if(!NbRecs)
	{
		// Write that only one record is present
		RBlockFile::Seek(blockid,0);
		size_t Info(BlockSize-size-cLen2-key.GetSize()-cLen);
		FreeSpaces.InsertAt(Info,blockid-1,true);
		Write((char*)(&Info),cLen);
		NbRecs=1;
		Write((char*)(&NbRecs),cLen);

		// Write an entry and position to file to right place
		key.Write(*this);
		Pos=BlockSize-size;
		Write((char*)(&Pos),cLen);
	}
	else if(entry==NbRecs)
	{
		// The entry is the last one :
		// Go to the last entry and the position is obtained
		K index;
		RBlockFile::Seek(blockid,cLen2+((entry-1)*(key.GetSize()+cLen)));

		// Read the old index and the record position
		index.Read(*this);
		Read((char*)(&Pos),cLen);

		// Compute the new index and record position and write them
		Pos-=size;
		key.Write(*this);
		Write((char*)(&Pos),cLen);

		// The spaces and the number of entries in the records table are inserted
		ModifyFreeSpace(blockid,-size-key.GetSize()-cLen);
		RBlockFile::Seek(blockid,cLen);
		NbRecs++;
		Write((char*)(&NbRecs),cLen);
	}
	else
	{
		// The record must be inserted at 'entry':
		// 1. Find where the current record and index 'entry' ends its data -> Pos
		// 2. The new record will be inserted at 'entry' and the position Pos-size :
		// 3. Decrease all the records starting from 'entry' from 'size' positions.
		// 4. Move the records in memory from 'size'.

		// Initialize
		K newindex,oldindex;
		size_t newpos,oldpos,LastPos(0);

		// First Step
		if(entry)
		{
			// There are some entry before the first one to move
			// Read the position of the record corresponding to 'entry-1'
			//		-> It is where the current record at index 'entry' ends
			RBlockFile::Seek(blockid,cLen2+((entry-1)*(key.GetSize()+cLen)));
			newindex.Read(*this);
			Read((char*)(&newpos),cLen);
			Pos=newpos;  // This is where ends the last records
		}
		else
		{
			// No record before -> the current record at index 'entry' ends at the end of the block
			newpos=Pos=BlockSize;
			RBlockFile::Seek(blockid,cLen2+(entry)*(key.GetSize()+cLen));
		}

		// Second Step
		newindex=key;
		newpos-=size;

		// Third Step : Go trough the entries
		while(entry<=NbRecs)
		{
			// Read the old index and the old record position at 'entry' if necessary
			if(entry<NbRecs)
			{
				oldindex.Read(*this);
				Read((char*)(&oldpos),cLen);
				LastPos=oldpos;       // Suppose it it the last record to move

				// Write to old position
				RBlockFile::SeekRel(-key.GetSize()-cLen);
			}

			// Write the new entry
			newindex.Write(*this);
			Write((char*)(&newpos),cLen);
			newpos=oldpos-size;
			newindex=oldindex;
			entry++;
		}

		// Fourth step : Move the Pos-LastPos bytes in memory starting from LastPos to newpos
		if(LastPos>Pos)
			mThrowRException("Corrupted index file '"+GetURI()()+"'");
		if(Pos-LastPos)
			RBlockFile::MoveBlock(blockid,LastPos,newpos,Pos-LastPos);

		// The spaces and the number of entries in the records table are inserted
		ModifyFreeSpace(blockid,-size-key.GetSize()-cLen);
		RBlockFile::Seek(blockid,cLen);
		NbRecs++;
		Write((char*)(&NbRecs),cLen);

		// Pos must point where the new record was inserted
		Pos-=size;
	}

	// Position the block at the right place
	RBlockFile::Seek(blockid,Pos);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Seek(off_t)
{
	mThrowRIOException(this,"Method cannot be called");
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Write(const char* buffer,size_t nb)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	RBlockFile::Write(buffer,nb);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Read(char* buffer,size_t nb)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	RBlockFile::Read(buffer,nb);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::EraseRecord(size_t blockid,K& key)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Find the entry in the index table
	bool find;
	size_t entry(GetIndex(blockid,key,find));
	if(!find)
		mThrowRIOException(this,"No record "+key.GetKey()+" in block "+RString::Number(blockid));

	// To erase a record :
	// 1. Look if it is the first record :
	//    a) Yes : It ends at the end of the block.
	//    b) No : It ends at the beginning of block 'entry-1'
	//   Compute the size of bytes to skip -> Size
	//  2. Move all the records after 'entry' from Size position to the end
    // 3. Move the records in memory to delete the record entry

	// Initialize
	K Entry;
	size_t Pos,OldPos,Size,FirstPos;

	// Step 1
	if(entry)
	{
		// Not the first record -> Read both entries
		RBlockFile::Seek(blockid,cLen2+((entry-1)*(key.GetSize()+cLen)));
		Entry.Read(*this);
		RBlockFile::Read((char*)&Size,cLen);
		Entry.Read(*this);
		RBlockFile::Read((char*)&OldPos,cLen);
		Size-=OldPos;
	}
	else
	{
		// First record
		RBlockFile::Seek(blockid,cLen2+(entry*(key.GetSize()+cLen)));
		Entry.Read(*this);
		RBlockFile::Read((char*)&OldPos,cLen);
		Size=BlockSize-OldPos;
	}
	FirstPos=OldPos;

	//
	// Go trough all the next record entries (if necessary) and move them Size bytes to the end
	entry++;
	if(entry<NbRecs)
	{
		while(entry<NbRecs)
		{
			Entry.Read(*this);
			RBlockFile::Read((char*)&Pos,cLen);
			RBlockFile::SeekRel(-2*(key.GetSize()+cLen));
			OldPos=Pos;  // Suppose last record
			Pos+=Size;
			Entry.Write(*this);
			RBlockFile::Write((char*)&Pos,cLen);
			RBlockFile::SeekRel(key.GetSize()+cLen);
			entry++;
		}

		// Move the OldPos-Pos+Size bytes in memory starting from Pos-Size to Pos
		if(FirstPos>OldPos)
			RBlockFile::MoveBlock(blockid,Pos-Size,Pos,FirstPos-OldPos);
		else
			mThrowRIOException(this,"Problem: FirstPos="+RString::Number(FirstPos)+"; OldPos="+RString::Number(OldPos)+"; Move="+RString::Number(OldPos-FirstPos));
	}

	// Modify the free space and the number of records
	ModifyFreeSpace(blockid,Size+key.GetSize()+cLen);   // The spaces and the entry in the records table are removed
	NbRecs--;
	RBlockFile::Seek(blockid,cLen);
	RBlockFile::Write((char*)&NbRecs,cLen);

	// Position the block at the right place
	RBlockFile::Seek(blockid,FirstPos+Size);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Seek(size_t blockid,K& key)
{
	if(!IsOpen())
		mThrowRIOException(this,"File not opened");

	// Find the right index in the table
	bool Find;
	size_t Entry(GetIndex(blockid,key,Find));
	if(!Find)
		mThrowRIOException(this,"No record "+key.GetKey()+" in block "+RString::Number(blockid));

	// Read the position of the record and seek to it
	RBlockFile::Seek(blockid,cLen2+(Entry*(key.GetSize()+cLen))+key.GetSize());
	size_t Pos;
	Read((char*)&Pos,cLen);
	RBlockFile::Seek(blockid,Pos);
}


//------------------------------------------------------------------------------
template<class K>
	void RKeyValueFile<K>::Seek(size_t& blockid,K& key,size_t size)
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
		Entry=GetIndex(blockid,key,Find);
		if(!Find)
			mThrowRIOException(this,"No record "+key.GetKey()+" in block "+RString::Number(blockid));

		// Compute the size previously allocated
		RBlockFile::Seek(blockid,cLen2+(Entry*(key.GetSize()+cLen))+key.GetSize());
		Read((char*)&Pos,cLen);
		if(Entry)
		{
			// The size of the record is the position of the current index with the previous one
			size_t Next;
			RBlockFile::SeekRel(-2*key.GetSize()-cLen); // Skip the current identifier and position and the previous identifier in the table
			Read((char*)&Next,cLen);
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
			EraseRecord(blockid,key);
		}
	}

	// A block must be search
	// Look if an existing block can hold the size asked
	blockid=0;
	RNumCursor<size_t> Cur(FreeSpaces);
	for(Cur.Start();!Cur.End();Cur.Next())
	{
		blockid++;
		if((Cur()>Tolerance)&&(Cur()>=(size+key.GetSize()+cLen)))
			break;
	}
	if(Cur.End())
	{
		// New block to create :
		// Position it to the beginning and specify that they are one record.
		blockid=GetNbBlocks()+1;
		NbRecs=0;   // Specify that no records are in the new block
		NewRecord(blockid,key,0,size);
		return;
	}

	// Find the position in the entry table where to store the identifier
	bool Find;
	size_t NewEntry(GetIndex(blockid,key,Find));
	if(Find)
		std::cout<<"RKeyValueFile::Seek(size_t&,K&,size_t) : Existing entry will be overwritten"<<std::endl;
/*	if(Find)
		throw RIOException(this,"RKeyValueFile::Seek(size_t&,size_t,size_t) : Big internal problem");*/

	NewRecord(blockid,key,NewEntry,size);
}


//------------------------------------------------------------------------------
template<class K>
	RKeyValueFile<K>::~RKeyValueFile(void)
{
	Close();
}
