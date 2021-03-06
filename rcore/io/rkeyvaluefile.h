/*

	R Project Library

	RKeyValueFile.h

	Generic Key-Value File - Header.

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
#ifndef RKeyValueFile_H
#define RKeyValueFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rblockfile.h>
#include <rnumcontainer.h>
#include <rcursor.h>
#include <rnumcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* The RKeyValueFile class represents a generic file for managing (key,value)
* pairs (such as an inverted file used by search engines).
*
* The approach is based on Zobel, Moffat, and Sack-Davis (1993):
* - Each file is composed from several blocks.
* - Each block manages a set of pairs. In practice, a block contains an address
* table (storing for each key the position inside the block of the corresponding
* value), several values, and some free space.
*
* It is supposed that all keys have the same size (for example an integer or a
* hash code). Moreover, The size of a value cannot exceed the size of a single
*  block.
*
* Each pair has a given key and a given block number. In practice, a block is
* composed from:
* -# The number of bytes free in the block (size_t).
* -# The number of records in the block (size_t).
* -# The block table representing for each key the corresponding address
*    (key size,size_t).
* -# Some free spaces.
* -# The values at the different addresses.
*
* The values are stored like a memory heap: starting from the end, new
* values have decreasing internal addresses.
*
* The methods RKeyValueFile<K>::Seek are used to position the block to a
* specific value corresponding to a block number and a key. The basic
*  RKeyValueFile<K>::Read and RKeyValueFile<K>::Write methods can then be used
* to read or write data.
*
* The key must be managed through a class that must be given as parameter of
* this template. This class must define several methods:
* @code
* class cKey
* {
* public:
*   cKey(const cKey& key);                         // Copy constructor.
*   size_t GetSize(void) const;                    // Size of a key.
*   int Compare(const char* data) const;           // Compare method.
*   void Read(R::RKeyValueFile<cKey>& file);       // Read the key from the current position.
*   void Write(R::RKeyValueFile<cKey>& file);      // Write the key to the current position.
*   R::RString GetKey(void) const;                 // Build a string representing the key.
*   cKey& operator=(const cKey &src)               // Assignment operator.
* };
* @endcode
* Look at RIntKey and RIntsKey for examples of implementation.
*
*
* The class provides high level methods to manage RVectorInt objects:
* @code
* RKeyValueFile<RIntKey> Test("/home/pfrancq/test.bin",10,2,1);  // 10 Kb per blocks, 2 blocks in cache, tolerance of 1 Kb.
* Test.Open();
*
* // Write Vec
* RVectorInt<size_t,false> Vec(30);
* Vec.Insert(3); Vec.Insert(6); Vec.Insert(7); Vec.Insert(10); Vec.Insert(11);
* size_t BlockNb(0); // No block is currently assign
* RIntKey Key(300);
* Test.Write(BlockNb,Key,Vec);
*
* // Read Vec2
* RVectorInt<size_t,false> Vec2(30);
* Test.Read(BlockNb,Key,Vec2);
* for(Vec2.Start();!Vec2.End();Vec2.Next())
*   cout<<"Read "<<Vec2()<<endl;
* @endcode
*
* The class provides also high level methods to manage containers. The class of
* objects contained must provide three methods :
* - A static GetSizeT method returning the size needed to store an object.
* - A static Load method returning a pointer to the object to insert after
*   loading it.
* - A Write method to store an object.
*
* @code
* class MyClass
* {
* public:
* 	size_t Id;
* 	double Nb;
* 	MyClass(size_t id,double nb) : Id(id), Nb(nb) {}
* 	int Compare(const MyClass& c) const {return(CompareIds(Id,c.Id));}
* 	static size_t GetSizeT(void) {return(sizeof(size_t)+sizeof(double));}
* 	template<class K>void Write(RKeyValueFile<K>& file) const
* 	{
* 	  file.Write((char*)&Id,sizeof(size_t));
* 	  file.Write((char*)&Nb,sizeof(double));
* 	}
* 	* 	template<class K> static MyClass* Read(RKeyValueFile<K>& file)
* 	{
* 	  size_t id;
* 	  double nb;
* 	  file.Read((char*)&id,sizeof(size_t));
* 	  file.Read((char*)&nb,sizeof(double));
* 	  return(new MyClass(id,nb));
* 	}
* };
*
*  RIndexFile<RIntKey> Test("/home/pfrancq/test.bin",10,2,1); // 10 Kb per blocks, 2 blocks in cache, tolerance of 1 Kb.
*	Test.Open();
*
*	// Write Cont
*	RContainer<MyClass,true,true> Cont(30);
*	Cont.InsertPtr(new MyClass(2,23.0)); Cont.InsertPtr(new MyClass(1,2.0));
*	size_t BlockNb(0); // Start without any block
*  RIntKey Key(300);
*	Test.Write(BlockNb,Key,Cont);
*
*	// Read Cont2
*	RContainer<MyClass,true,true> Cont2(30);
*	Test.Read(BlockNb,Key,Cont2);
*	RCursor<MyClass> Cur(Cont2);
*	for(Cur.Start();!Cur.End();Cur.Next())
*	  cout<<"Read "<<Cur()->Id<<" "<<Cur()->Nb<<endl;
* @endcode
*
* @tparam K                 Class corresponding the keys.
* @short Generic Key-Value File.
*/
template<class K>
	class RKeyValueFile : protected RBlockFile
{
public:

	using RBlockFile::GetURI;

protected:

	/**
	 * Basic length to store positions, number of records and free spaces.
	 */
	static const size_t cLen=sizeof(size_t);

	/**
	 * Double of the length to store positions, number of records and free
	 * spaces. In practice, it corresponds to the size of the "header" of a block.
	 */
	static const size_t cLen2=2*sizeof(size_t);

	/**
	 * Free spaces in each block.
	 */
	RNumContainer<size_t,false> FreeSpaces;

	/**
	 * Tolerance.
	 */
	size_t Tolerance;

	/**
	 * Number of records in the current block.
	 */
	size_t NbRecs;

public:

	/**
	* Construct an index file.
	* @param uri             URI of the file.
	* @param blocksize       Size of a block (in KBytes).
	* @param nbcaches        Number of blocks managed in memory.
	* @param tolerance       Fix the size of the tolerance, i.e. minimal free
	*                        size to add a new index (in KBytes).
	*/
	RKeyValueFile(const RURI& uri,size_t blocksize,size_t nbcaches,size_t tolerance);

protected:

	/**
	* Open the file.
	* @param mode            The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode);

public:

	/**
	* Open the file in RIO::ReadWrite mode (the only one acceptable).
	*/
	inline void Open(void) {Open(RIO::ReadWrite);}

	/**
	* Close the file.
	*/
	virtual void Close(void);

	/**
	 * Clear the file.
	 */
	void Clear(void);

	/**
	 * Get the number of blocks.
	 */
	size_t GetNbBlocks(void) const {return(NbBlocks);}

	/**
	 * Get the type of the cache.
	 */
	CacheType GetCacheType(void) const {return(Type);}

	/**
	 * Set the type of the cache. If the type is set to WriteThrough, all the
	 * changed caches are saved.
	 * @param type           Type.
	 */
	void SetCacheType(CacheType type) {RBlockFile::SetCacheType(type);}

	/**
	 * Flush the caches : All the blocks in memory that are dirtied are save on
	 * disk.
	 */
	void Flush(void) {RBlockFile::Flush();}

private:

	/**
	 * Get the index of the a identifier in the current block address table.
	 * @param block          Block number.
	 * @param id             Identifier.
	 * @param find           Was it found ?
	 * @return Position in the block address table.
	 */
	size_t GetIndex(size_t block,K& key,bool& find);

	/**
	 * Move all the records of all entries in the table.
	 * @param blockid        Identifier of the block.
	 * @param entry          First entry to move.
	 * @param rel            Number of bytes to increase or decrease the record
	 *                       addresses (increasing the size of record implies
	 *                       decreasing the record addresses).
	 */
	void MoveRecords(size_t blockid,size_t entry,long rel);

	/**
	 * Modify the free space associated with a given block.
	 * @param blockid        Identifier of the block.
	 * @param rel            Number of bytes to increase or decrease.
	 */
	void ModifyFreeSpace(size_t blockid,long rel);

	/**
	 * A new record of a given size is added to the block. Set the block to the
	 * good position to write the content.
	 * @param blockid        Identifier of the block.
	 * @param indexid        Identifier of the index.
	 * @param entry          Entry in the index table.
	 * @param size           Size of the record.
	 */
	void NewRecord(size_t blockid,K& key,size_t entry,size_t size);

	/**
	* Go to a specific position of the file. In fact this method should never be
	* called and generates an error.
	* @param pos            Position to reach.
	*/
	virtual void Seek(off_t pos);

public:

	/**
	* Write a number of bytes of a buffer in the current position of
	* the file. The Seek(size_t&,size_t,size_t) must be called before to ensure
	* the internal integrity of the file.
	* @param buffer         Buffer.
	* @param nb             Number of bytes to write.
	*/
	void Write(const char* buffer,size_t nb);

	/**
	 * Write a vector of integers associated to a given index in a given block.
	 * @tparam I             Type of the numbers contained.
	 * @tparam bOrder        Determine is the container to write is ordered.
	 * @param blockid        Identifier of the block.
	 * @param indexid        Identifier of the index.
	 * @param vec            Vector to write.
	 */
	template<class I,bool bOrder> void Write(size_t& blockid,K& key,const RNumContainer<I,bOrder>& vec)
	{
		Seek(blockid,key,sizeof(size_t)+(vec.GetNb()*sizeof(I)));
		size_t size(vec.GetNb());
		Write((char*)&size,sizeof(size_t));
		const I* ptr(vec.GetList());
		for(size++;--size;ptr++)
		{
			Write((char*)ptr,sizeof(I));
		}
	}

	/**
	 * Write a RContainer of associated to a given index in a given block.
	 * @tparam C             Class of the object contained in the container.
	 * @tparam bAlloc        The container to write is responsible of the deallocation.
	 * @tparam bOrder        The container to write is ordered.
	 * @param blockid        Identifier of the block.
	 * @param indexid        Identifier of the index.
	 * @param cont           Container to write.
	 */
	template<class C,bool bAlloc,bool bOrder> void Write(size_t& blockid,K& key,const RContainer<C,bAlloc,bOrder>& cont)
	{
		Seek(blockid,key,sizeof(size_t)+(cont.GetNb()*C::GetSizeT()));
		size_t nb,size(cont.GetNb());
		Write((char*)&size,sizeof(size_t));
		RCursor<C> Cur(cont);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Write(*this);
	}

	/**
	* Read a given number of bytes at the current position of the file.
	* @param buffer         Buffer (must be allocated).
	* @param nb             Number of bytes to read.
	*/
	void Read(char* buffer,size_t nb);

	/**
	 * Read a vector of integers associated to a given index in a given block.
	 * @tparam I             Type of the numbers contained.
	 * @tparam bOrder        Determine is the container to read is ordered.
	 * @param blockid        Identifier of the block.
	 * @param indexid        Identifier of the index.
	 * @param vec            Read to write.
	 */
	template<class I,bool bOrder> void Read(size_t blockid,K& key,RNumContainer<I,bOrder>& vec)
	{
		size_t nb,size;
		vec.Clear();
		Seek(blockid,key);
		Read((char*)&size,sizeof(size_t));
		vec.Verify(size);
		for(size_t i=0;i<size;i++)
		{
			Read((char*)&nb,sizeof(I));
			vec.InsertAt(nb,i,false);
		}
	}

	/**
	 * Read a RContainer of associated to a given index in a given block.
	 * @tparam C             Class of the object contained in the container.
	 * @tparam bAlloc        The container to read is responsible of the deallocation.
	 * @tparam bOrder        The container to read is ordered.
	 * @param blockid        Identifier of the block.
	 * @param indexid        Identifier of the index.
	 * @param cont           Container to Read.
	 */
	template<class C,bool bAlloc,bool bOrder> void Read(size_t blockid,K& key,RContainer<C,bAlloc,bOrder>& cont)
	{
		Seek(blockid,key);
		size_t size;
		Read((char*)&size,sizeof(size_t));
		cont.Clear(size);
		for(size_t i=0;i<size;i++)
			cont.InsertPtrAt(C::Read(*this),i);
	}

	/**
	 * Erase a given record from the block file.
	 * @param blockid        Block number.
	 * @param indexid        Identifier of the index.
	 */
	void EraseRecord(size_t blockid,K& key);

	/**
	* Go to a specific position of the file.
	* @param blockid        Identifier of the block.
	* @param indexid        Identifier of the index.
	*/
	void Seek(size_t blockid,K& key);

	/**
	* Go to a specific position of the file to write a given number of bytes
	* associated with a given index. This method must be called before any call
	* to Write(const char*,size_t).
	*
	* If the identifier of the block is null, the method find a new block that
	* can contain the given number of bytes. If the index exist but the number
	* of bytes asked is different, internal moves are done to create the
	* necessary space. If the block is full, a new one is searched.
	*
	* @warning This method supposes that the information of the index will be
	* update after the call. In particular, if the amount of bytes needed
	* implies to write in another block, the old content is lost.
	* @param blockid        Identifier of the block. If null, a new block is
	*                       searched and the identifier is updated.
	* @param indexid        Identifier of the index.
	* @param size           Number of bytes to read/write.
	*/
	void Seek(size_t& blockid,K& key,size_t size);

	/**
	* Destruct the file.
	*/
	virtual ~RKeyValueFile(void);

	// Methods than can be used
	using RBlockFile::SeekRel;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rkeyvaluefile.hh>


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
