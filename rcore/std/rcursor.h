/*

	R Project Library

	RCursor.h

	Cursor for Containers - Header.

	(C) 1999-2001 by P. Francq.

	Version $Revision$

	Last Modify: $Date$

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
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* @param C                  The class of the elements that are contained.
* @param T                  The type of the iterator used.
*
* This class represent a cursor to iterate a RContainer. The parameters are
* those of the container.
*
* The difference with RContainerCursor is that if the array changed (elements
* are added or deleted), RCursor wouldn't be updated while RContainerCursor would.
*
* Here is an example of a cursor used:
* <pre>
* int example(RContainer<MyElement,unsigned int> *c)
* {
* 	RCursor<MyElement,unsigned int> Cur();
*
* 	for(Cur.Start(c);!Cur.End();Cur.Next())
* 		Cur()->DoSomething(2.3);
* }
* </pre>
*
* @author Pascal Francq
* @short Cursor for Elements.
*/
template<class C,class T>
	class RCursor
{
	/**
	* This variable is used to go through the container.
	*/
	C** Current;

	/**
	* This variable is used to see if the end of the container is reached.
	*/
	T ActPtr;

	/**
	* The array of pointers for the elements.
	*/
	C **Tab;

	/**
	* The number of elements in the container.
	*/
	T NbPtr;

	/**
	* The last position in the array used by an object.
	*/
	T LastPtr;

public:

	/**
	* Construct the cursor.
	*/
	RCursor(void);

private:

	/**
	* Start the iterator to go trough the container.
	*/
	inline void Start(void);

public:

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,true,true>& c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,true,true>* c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,false,true>& c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,false,true>* c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,true,false>& c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,true,false>* c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,false,false>& c);

	/**
	* Start the cursor.
	* param c               Container to iterate.
	*/
	void Start(RContainer<C,T,false,false>* c);

	/**
	* Test if the end of the container is reached.
	*/
	inline bool End(void) const;

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	*/
	inline void Next(void);

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const;
};


#include "rcursor.hh" // implementation


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
/**
* The CLASSCURSOR macro defines a way to create a cursor for a given class with
* a name. It is sometimes easier to create a specific class rather than using
* directly the template class RContainerCursor.
* @param name               Nmae of the class representing the cursor.
* @param C                  The class of the elements that are contained.
* @param T                  The type of the iterator used.
*/
#define CLASSCURSOR(name,C,T)                                       \
class name : public RStd::RCursor<C,T)        \
{                                                                    \
public:                                                              \
	name(void) : RCursor<C,T( {}                   \
};


//-----------------------------------------------------------------------------
#endif
