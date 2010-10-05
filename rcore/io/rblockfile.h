/*

	R Project Library

	RBlockFile.h

	Blocks File - Header.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef RBlockFile_H
#define RBlockFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <riofile.h>
#include <rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RBlockFile class represents a file composed from several blocks of a
* given size. It maintains a cache in memory with a given number of blocks. The
* class supposes that the first block has an identifier of 1 (0 points to a
* null block).
*
* Each block have a fixed size and defines an upper limit of the amount of data
* continuously stored.
*
* @code
*	char Buffer[80];
*
*	RBlockFile Test("~/test.bin",1,2);  // 1 Kb per block, 2 block in cache
*	Test.Open();
*
*	// Write information
*	strcpy(Buffer,"coucou");
*	Test.Seek(2,20);  // Go to position 20 in the second block
*	Test.Write(Buffer,6);
*	strcpy(Buffer,"coucou2");
*	Test.Seek(3,10);  // Go to position 10 in the third block
*	Test.Write(Buffer,7);
*
*	// Read information
*	Test.Seek(2,20);  // Go to position 20 in the second block
*	Test.Read(Buffer,6); Buffer[6]=0;
*	cout<<"Read "<<Buffer<<endl;
*	Test.Seek(3,10);  // Go to position 10 in the third block
*	Test.Read(Buffer,7); Buffer[7]=0;
*	cout<<"Read "<<Buffer<<endl;
* @endcode
* @warning When a file created with a given block size is opened with another
*          size, it does not work.
* @author Pascal Francq
* @short Block File.
*/
class RBlockFile : protected RIOFile
{
	class Block;

public:

	/**
	 * Type of the cache.
	 */
	enum CacheType
	{
		WriteThrough       /** All write operations are also done on disk. */,
		WriteBack          /** Write operations are done in cache but not
		                       directly saved on disk. */
	};

protected:

	/**
	 * Type of the cache.
	 */
	CacheType Type;

	/**
	 * Size of the blocks.
	 */
	size_t BlockSize;

	/**
	 * Cache managed.
	 */
	RContainer<Block,true,true> Cache;

	/**
	 * Current block.
	 */
	Block* Current;

	/**
	 * Current position.
	 */
	size_t CurrentPos;

	/**
	 * Pointer to the current data.
	 */
	char* CurrentData;

	/**
	 * Number of blocks contained in the file.
	 */
	size_t NbBlocks;

public:

	/**
	* Construct a file.
	* @param uri             URI of the file.
	* @param blocksize       Size of a block (in KBytes).
	* @param nbcaches        Number of blocks managed in memory.
	*/
	RBlockFile(const RURI& uri,size_t blocksize,size_t nbcaches);

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
	void SetCacheType(CacheType type);

	/**
	 * Flush the caches : All the blocks in memory that are dirtied are save on
	 * disk.
	 */
	void Flush(void);

private:

	/**
	 * Method used to ordered the blocks by descending order of accesses.
	 * @param a              Pointer to a block.
	 * @param b              Pointer to the second block.
	 */
	static int sortOrderAccess(const void* a,const void* b);

	/**
	 * Load a given block in to memory.
	 * @param id             Identifier.
	 * @return Pointer to the block.
	 */
	Block* LoadBlock(size_t id);

public:

	/**
	* Read a given number of bytes at the current position of the file.
	* @param buffer         Buffer (must be allocated).
	* @param nb             Number of bytes to read.
	*/
	void Read(char* buffer,size_t nb);

	/**
	* Write a number of bytes of a buffer in the current position of
	* the file.
	* @param buffer         Buffer.
	* @param nb             Number of bytes to read.
	*/
	void Write(const char* buffer,size_t nb);

	/**
	 * Move a given number of bytes of a given block. The existing data are
	 * overwritten. After the call, the file is positioned at the last byte
	 * moved.
	 * @param blockid        Identifier of the block.
	 * @param start          Initial position.
	 * @param end            Final position.
	 * @param size           Size of the data to move.
	 */
	void MoveBlock(size_t blockid,size_t start,size_t end,size_t size);

	/**
	 * Get a pointer to the memory at the current position of the file. The
	 * internal cursor is moved.
	 * @param size           Number of bytes to manipulate from that position.
	 */
	const char* GetPtr(size_t size);

	/**
	* Go to a specific position of the file.
	* @param blockid        Identifier of the block.
	* @param pos            Position to reach in the block.
	*/
	void Seek(size_t blockid,size_t pos);

	/**
	* Go to a specific position in the current block.
	* @param pos            Relative position to reach in the block.
	*/
	void SeekRel(long pos);

	/**
	* Destruct the file.
	*/
	virtual ~RBlockFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
