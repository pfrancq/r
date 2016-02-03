/*

	R Project Library

	RBlockFileData.h

	File Data Block - Header.

	Copyright 2015-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RBlockFileData_H
#define RBlockFileData_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rrwlock.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration
class RBlockFileAccess;


//------------------------------------------------------------------------------
/**
 * The RBlockFileData class represents a block of data of a given file loaded
 * in memory. In practice, it is only useful for internal purposes.
 * @see RBlockFile
 * @short File Data Block
 */
class RBlockFileData
{
	/**
	 * Identifier of the block.
	 */
	size_t Id;

	/**
	 * Number of access.
	 */
	size_t NbAccess;

	/**
	 * Was the block modified ?
	 */
	bool Dirty;

	/**
	 * Data contained in the block.
	 */
	char* Data;

	/**
	 * Current access to the block.
	 */
	RContainer<RBlockFileAccess,false,false> Access;

	/**
	 * Read/Write Lock.
	 */
	RRWLock lData;

	/**
	 * Initiliaz a block.
	 * @param id             Identifier.
	 * @param size           Block size.
	 */
	RBlockFileData(size_t id,size_t size);

public:

	/**
	 * Compare method based on the identifier.
	 * @param block          Block to compare with.
	 * @return a value depending if the current block has a higher identifier
	 * (1), an equal one (0) or a smaller one (-1) than the one used to compare
	 * with.
	 */
	inline int Compare(const RBlockFileData& block) const {return(CompareIds(Id,block.Id));}

	/**
	 * Compare method based on the identifier.
	 * @param id             Identifier to compare with.
	 * @return a value depending if the current block has a higher identifier
	 * (1), an equal one (0) or a smaller one (-1) than the one used to compare
	 * with.
	 */
	inline int Compare(size_t id) const {return(CompareIds(Id,id));}

	/**
	 * Destructor.
	 */
	~RBlockFileData(void);

	friend class RBlockFile;
	friend class RBlockFileAccess;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
