/*

	R Project Library

	RCursor.h

	Container Cursor - Header.

	Copyright 1999-2005 by the Universit�Libre de Bruxelles.

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



//------------------------------------------------------------------------------
#ifndef RCursorH
#define RCursorH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @param C                  The class of the elements that are contained.
*
* This class represent a cursor to iterate a RContainer. The parameters are
* those of the container. When an element is added or removed from the container
* parsed by the cursor, the cursor is not valid anymore.
*
* Here is an example of a cursor used:
* @code
* int example(RContainer<MyElement,true,true> *c)
* {
* 	RCursor<MyElement> Cur;
*
* 	Cur.Set(c);
* 	for(Cur.Start();!Cur.End();Cur.Next())
* 		Cur()->DoSomething(2.3);
* }
* @endcode
*
* @author Pascal Francq
* @short Container Cursor.
*/
template<class C>
	class RCursor
{
	/**
	* This variable is used to go through the container.
	*/
	C** Current;

	/**
	* This variable is used to see if the end of the container is reached.
	*/
	size_t ActPtr;

	/**
	* The array of pointers for the elements.
	*/
	C **Tab;

	/**
	* The number of elements in the container.
	*/
	size_t NbPtr;

	/**
	* The last position in the array used by an object.
	*/
	size_t LastPtr;

public:

	/**
	* Construct the cursor.
	*/
	RCursor(void);

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCursor(const RCursor<C>& src);

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,true,true>& c,size_t max=0);

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,false,true>& c,size_t max=0);

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,true,false>& c,size_t max=0);

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,false,false>& c,size_t max=0);

	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RCursor<C>& operator=(const RCursor<C>& src);

	/**
	* Set the container.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	template<bool b,bool o> void Set(const RContainer<C,b,o>& c,size_t max=0);

	/**
	* Clear the cursor.
	*/
	void Clear(void);

	/**
	* Start the iterator to go trough the container.
	*/
	inline void Start(void);

	/**
	* Go to the i-th element of the container.
	* @param idx             Index of the element to get.
	*/
	void GoTo(size_t idx);

	/**
	* Return the actual position in the container.
	*/
	inline size_t GetPos(void) {return(ActPtr);}

	/**
	* Return the number of elements in the container.
	*/
	inline size_t GetNb(void);

	/**
	* Test if the end of the container is reached.
	*/
	inline bool End(void) const;

	/**
	* Test if the cursor is at a given index.
	* @param idx             Index of the element.
	*/
	inline bool IsAt(size_t idx) const;

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void Next(void);

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const;
};


#include <rstd/rcursor.hh> // implementation


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
