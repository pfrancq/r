/*

	R Project Library

	RCursor.h

	Container Cursor - Header.

	Copyright 1999-2003 by the Universit� Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
* @param T                  The type of the iterator used.
*
* This class represent a cursor to iterate a RContainer. The parameters are
* those of the container. When an element is added or removed from the container
* parsed by the cursor, the cursor is not valid anymore.
*
* Here is an example of a cursor used:
* <pre>
* int example(RContainer<MyElement,unsigned int> *c)
* {
* 	RCursor<MyElement,unsigned int> Cur();
*
* 	Cur->Set(c);
* 	for(Cur.Start();!Cur.End();Cur.Next())
* 		Cur()->DoSomething(2.3);
* }
* </pre>
*
* @author Pascal Francq
* @short Container Cursor.
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

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,true,true>& c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,true,true>* c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,false,true>& c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,false,true>* c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,true,false>& c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,true,false>* c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,false,false>& c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(RContainer<C,T,false,false>* c);

	/**
	* Assignment operator using a "Cursor".
	*/
	RCursor<C,T>& operator=(const RCursor<C,T>& c) throw(std::bad_alloc);

	/**
	* Clear the cursor.
	*/
	void Clear(void);

	/**
	* Start the iterator to go trough the container.
	*/
	inline void Start(void);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,true,true>& c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,true,true>* c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,false,true>& c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,false,true>* c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,true,false>& c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,true,false>* c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,false,false>& c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(RContainer<C,T,false,false>* c);

	/**
	* Go to the i-th element of the container.
	*/
	void GoTo(const unsigned int i) throw(std::bad_alloc);

	/**
	* Return the number of elements in the container.
	*/
	inline unsigned int GetNb(void);

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


#include <rstd/rcursor.hh> // implementation


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
/**
* The CLASSCURSOR macro defines a way to create a cursor for a given class with
* a name. It is sometimes easier to create a specific class rather than using
* directly the template class RContainerCursor.
* @param name               Nmae of the class representing the cursor.
* @param C                  The class of the elements that are contained.
* @param T                  The type of the iterator used.
*/
#define CLASSCURSOR(name,C,T)                                        \
class name : public R::RCursor<C,T>                                  \
{                                                                    \
public:                                                              \
	name(void) : R::RCursor<C,T>() {}                                \
	static name* GetTmpCursor(void)                                  \
	{                                                                \
		return(R::GetTemporaryObject<name,20>());                    \
	}                                                                \
	name& operator=(const name& c) throw(std::bad_alloc)             \
	{                                                                \
		R::RCursor<C,T>::operator=(c);                               \
		return(*this);                                               \
	}                                                                \
	name& operator=(const R::RCursor<C,T>& c) throw(std::bad_alloc)  \
	{                                                                \
		R::RCursor<C,T>::operator=(c);                               \
		return(*this);                                               \
	}                                                                \
};


//------------------------------------------------------------------------------
#endif
