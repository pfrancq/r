/*

	R Project Library

	BasicCursor.h

	Basic Container Cursor - Header.

	Copyright 2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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



//-----------------------------------------------------------------------------
#ifndef BasicCursorH
#define BasicCursorH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <stdint.h>
#if __WORDSIZE == 64
#define SIZE_MAX		(18446744073709551615UL)
#else
#define SIZE_MAX		(4294967295U)
#endif


//-----------------------------------------------------------------------------
// include files for R Project
#include <basiccontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*
* @author Pascal Francq
* @short Container Cursor.
*/
class BasicCursor
{
protected:

	/*
	* This variable is used to go through the container.
	*/
	void** Current;

	/*
	* This variable is used to see if the end of the container is reached.
	*/
	size_t ActPtr;

	/*
	* The array of pointers for the elements.
	*/
	void** Tab;

	/**
	* The number of elements in the container.
	*/
	size_t NbPtr;

	/*
	* The first position in the array handled by the cursor.
	*/
	size_t FirstPtr;

	/*
	* The last position in the array handled by the cursor.
	*/
	size_t LastPtr;

protected:

	/*
	* Construct the cursor.
	*/
	BasicCursor(void);

	/*
	* Construct the cursor.
	* @param src             Source container.
	*/
	BasicCursor(const BasicCursor& src);

	/*
	* Construct the cursor.
	* param c                Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	BasicCursor(const BasicContainer& c,size_t min,size_t max);

	/*
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	BasicCursor& operator=(const BasicCursor& src);

public:

	/**
	* Set the container.
	* param c                Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	void Set(const BasicContainer& c,size_t min,size_t max);

	/**
	* Clear the cursor.
	*/
	void Clear(void);

	/**
	* Start the iterator to go trough the container.
	*/
	void Start(void);

	/**
	* Go to the i-th element of the container.
	* @param idx             Index of the element to get.
	*/
	void GoTo(size_t idx);

	/**
	* Return the actual position in the cursor.
	*/
	inline size_t GetPos(void) const {return(ActPtr);}

	/**
	* Return the number of elements in the cursor.
	*/
	inline size_t GetNb(void) const {return(LastPtr-FirstPtr);}

	/**
	* Test if the end of the cursor is reached.
	*/
	inline bool End(void) const {return(ActPtr==LastPtr);}

	/**
	* Test if the cursor is at a given index.
	* @param idx             Index of the element.
	*/
	inline bool IsAt(size_t idx) const {return(ActPtr==idx);}

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	void Next(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
