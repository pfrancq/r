/*

	R Project Library

	RCursor.h

	Container Cursor - Header.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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
#ifndef RCursorH
#define RCursorH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rcontainer.h>
#include <rstd/base/basiccursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* @param C                   The class of the elements that are contained.
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
	class RCursor : public BasicCursor
{
public:

	/**
	* Construct the cursor.
	*/
	RCursor(void) : BasicCursor() {}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCursor(const RCursor<C>& src) : BasicCursor(src) {}

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,true,true>& c,size_t max=0) : BasicCursor(c,max) {}

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,false,true>& c,size_t max=0) : BasicCursor(c,max) {}

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,true,false>& c,size_t max=0) : BasicCursor(c,max) {}

	/**
	* Construct the cursor.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	RCursor(const RContainer<C,false,false>& c,size_t max=0) : BasicCursor(c,max) {}

	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RCursor<C>& operator=(const RCursor<C>& src) { return(static_cast<RCursor<C>&>(BasicCursor::operator=(src)));}

	/**
	* Set the container.
	* param c                Container to iterate.
	* @param max             Maximum number of elements to iterate. If null,
	*                        iterate over the whole container.
	*/
	template<bool b,bool o> void Set(const RContainer<C,b,o>& c,size_t max=0) { BasicCursor::Set(c,max);}

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const {return(reinterpret_cast<C*>(*Current));}

};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
