/*

	R Project Library

	RKeys.h

	Keys for RKeyValueFile - Header.

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
#ifndef RKeys_H
#define RKeys_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>
#include <rkeyvaluefile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RIntKey provides a representation for a key that is composed from a single
* integer. It can be used with RKeyValueFile to manage pairs of (key,value).
* @short Integer-based Key.
*/
class RIntKey
{
	/**
	 * Integer representing the key.
    * @return
    */
	size_t Id;

public:

	/**
	 * Defaul construct.
    */
	inline RIntKey(void) {}

	/**
	 * Constructor.
	 * @param id             Integer composing the key.
	 */
	inline RIntKey(size_t id) : Id(id) {}

	/**
	 * Copy constructor.
	 * @param key            Source key.
	 */
	inline RIntKey(const RIntKey& key) : Id(key.Id) {}

	/**
	 * Get the size of a key.
	 * @eturn a size_t.
	 */
	inline size_t GetSize(void) const {return(sizeof(size_t));}

	/**
	 * Compare the key with the one pointed by a given pointer.
	 * @param                Data containing the key to compare with.
	 * @return +1, 0 or -1 depending if the current key is greater, identical
	 * or smaller than the key to compare with.
	 */
	inline int Compare(const char* data) const
	{
		size_t Cur(*reinterpret_cast<const size_t*>(data));
		return(Cur-Id);
	}

	/**
	 * Read the key from a key-value file at the current position.
	 * @param file           File to read from.
	 */
	inline void Read(R::RKeyValueFile<RIntKey>& file)
	{
		file.Read((char*)(&Id),sizeof(size_t));
	}

	/**
	 * Write the key in a key-value file at the current position.
	 * @param file           File to write in.
	 */
	inline void Write(R::RKeyValueFile<RIntKey>& file)
	{
		file.Write((char*)(&Id),sizeof(size_t));
	}

	/**
	 * Get a string version of a key.
	 * @return a RString.
	 */
	inline R::RString GetKey(void) const
	{
		return("("+RString::Number(Id)+")");
	}

	/**
	 * Assignment operator.
	 * @param key            Source key.
	 * @return the key assigned.
	 */
	inline RIntKey& operator=(const RIntKey& src)
	{
		Id=src.Id;
		return(*this);
	}
};


//------------------------------------------------------------------------------
/**
* The RIntsKey provides a representation for a key that is composed from two
* integers. It can be used with RKeyValueFile to manage pairs of (key,value).
* @short Integers-based Key.
*/
class RIntsKey
{
	/**
	 * First integer composing the key.
	 */
	size_t Id1;

	/**
	 * Second integer composing the key.
	 */
	size_t Id2;

public:

	/**
	 * Default constructor.
	 */
	inline RIntsKey(void) {}

	/**
	 * Constructor.
	 * @param id1            First integer composing the key.
	 * @param id2            Second integer composing the key.
	 */
	inline RIntsKey(size_t id1,size_t id2) : Id1(id1), Id2(id2) {}

	/**
	 * Copy constructor.
	 * @param key            Source key.
	 */
	inline RIntsKey(const RIntsKey& key) 	: Id1(key.Id1), Id2(key.Id2) {}

	/**
	 * Get the size of a key.
	 * @eturn a size_t.
	 */
	inline size_t GetSize(void) const {return(2*sizeof(size_t));}

	/**
	 * Compare the key with the one pointed by a given pointer.
	 * @param                Data containing the key to compare with.
	 * @return +1, 0 or -1 depending if the current key is greater, identical
	 * or smaller than the key to compare with.
	 */
	inline int Compare(const char* data) const
	{
		size_t Cur1(*reinterpret_cast<const size_t*>(data));
		if(Id1==Cur1)
		{
			size_t Cur2(*reinterpret_cast<const size_t*>(data+sizeof(size_t)));
			return(Cur2-Id2);
		}
		else
			return(Cur1-Id1);
	}

	/**
	 * Read the key from a key-value file at the current position.
	 * @param file           File to read from.
	 */
	inline void Read(R::RKeyValueFile<RIntsKey>& file)
	{
		file.Read((char*)(&Id1),sizeof(size_t));
		file.Read((char*)(&Id2),sizeof(size_t));
	}

	/**
	 * Write the key in a key-value file at the current position.
	 * @param file           File to write in.
	 */
	inline void Write(R::RKeyValueFile<RIntsKey>& file)
	{
		file.Write((char*)(&Id1),sizeof(size_t));
		file.Write((char*)(&Id2),sizeof(size_t));
	}

	/**
	 * Get a string version of a key.
	 * @return a RString.
	 */
	inline R::RString GetKey(void) const
	{
		return("("+RString::Number(Id1)+","+RString::Number(Id2)+")");
	}

	/**
	 * Assignment operator.
	 * @param key            Source key.
	 * @return the key assigned.
	 */
	inline RIntsKey& operator=(const RIntsKey& src)
	{
		Id1=src.Id1;
		Id2=src.Id2;
		return(*this);
	}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
