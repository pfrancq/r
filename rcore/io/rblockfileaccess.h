/*

	R Project Library

	RBlockFileAccess.h

	Block File Access - Header.

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
#ifndef RBlockFileAccess_H
#define RBlockFileAccess_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rblockfiledata.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration
class RBlockFile;


//------------------------------------------------------------------------------
/**
 * The RBlockFileAccess class provides an access to a given block file to read
 * and write data in a particular bloc.
 *
 * When it is necessary to ensure the coherence of a serie of reads or writes
 * (for example to store data stored in multiple variables), we must first lock
 * the access in read or write mode. Don't forget to unlock at the end of the
 * reads and writes!
 * @warning While the RBlockFileAccess allows a multithread access to a block
 * file, it is itself not reentrant.
 * @see RBlockFile
 * @short Block File Access
 */
class RBlockFileAccess
{
	/**
	 * Block file to access to.
	 */
	RBlockFile* File;

	/**
	 * Block identifier.
	 */
	size_t BlockId;

	/**
	 * Pointer to the data block. Can be null if the block is not in memory
	 * anymore.
	 */
	RBlockFileData* Block;

	/**
	 * Current position in the data block.
	 */
	size_t CurrentPos;

	/**
	 * Pointer to the current data block.
	 */
	char* CurrentData;

	/**
	 *  Is the access currently in lock mode ?
	 */
	bool Lock;

public:

	/**
	 * Constructor of an access.
	 * @param file           File to access to.
	 */
	RBlockFileAccess(RBlockFile* file);

	/**
	 * Constructor of an access for a given block and at a given position.
	 * @param file           File to access to.
	 * @param blockid        Identifier of the block to access.
	 * @param pos            Position in the block.
	 */
	RBlockFileAccess(RBlockFile* file,size_t blockid,size_t pos=0);

	/**
	 * Constructor of an access.
	 * @param file           File to access to.
	 */
	RBlockFileAccess(RBlockFile& file);

	/**
	 * Constructor of an access for a given block and at a given position.
	 * @param file           File to access to.
	 * @param blockid        Identifier of the block to access.
	 * @param pos            Position in the block.
	 */
	RBlockFileAccess(RBlockFile& file,size_t blockid,size_t pos=0);

	/**
	* Read locking.
	*/
	void ReadLock(void);

	/**
	* Read unlocking.
	*/
	void ReadUnLock(void);

	/**
	* Write locking.
	*/
	void WriteLock(void);

	/**
	* Write unlocking.
	*/
	void WriteUnLock(void);

	/**
	 * Destructor.
	 */
	~RBlockFileAccess(void);

	friend class RBlockFile;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
