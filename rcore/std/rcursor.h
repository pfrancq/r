/*

	R Project Library

	RCursor.h

	Container Cursor - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
	unsigned int ActPtr;

	/**
	* The array of pointers for the elements.
	*/
	C **Tab;

	/**
	* The number of elements in the container.
	*/
	unsigned int NbPtr;

	/**
	* The last position in the array used by an object.
	*/
	unsigned int LastPtr;

public:

	/**
	* Construct the cursor.
	*/
	RCursor(void);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(const RContainer<C,true,true>& c);

	/**
	* Construct the cursor.
	* param c               Smart pointer to the container to iterate.
	*/
	RCursor(const std::auto_ptr<RContainer<C,true,true> >& c);

	/**
	* Construct the cursor. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	RCursor(const RContainer<C,true,true>* c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(const RContainer<C,false,true>& c);

	/**
	* Construct the cursor.
	* param c               Smart pointer to the container to iterate.
	*/
	RCursor(const std::auto_ptr<RContainer<C,false,true> >& c);

	/**
	* Construct the cursor. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	RCursor(const RContainer<C,false,true>* c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(const RContainer<C,true,false>& c);

	/**
	* Construct the cursor.
	* param c               Smart pointer to the container to iterate.
	*/
	RCursor(const std::auto_ptr<RContainer<C,true,false> >& c);

	/**
	* Construct the cursor. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	RCursor(const RContainer<C,true,false>* c);

	/**
	* Construct the cursor.
	* param c               Container to iterate.
	*/
	RCursor(const RContainer<C,false,false>& c);

	/**
	* Construct the cursor.
	* param c               Smart pointer to the container to iterate.
	*/
	RCursor(const std::auto_ptr<RContainer<C,false,false> >& c);

	/**
	* Construct the cursor. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	RCursor(const RContainer<C,false,false>* c);

	/**
	* Assignment operator using a "Cursor".
	*/
	RCursor<C>& operator=(const RCursor<C>& c) throw(std::bad_alloc);

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
	void Set(const RContainer<C,true,true>& c);

	/**
	* Set the container.
	* param c               Smart pointer to the container to iterate.
	*/
	void Set(const std::auto_ptr<RContainer<C,true,true> >& c);

	/**
	* Set the container. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	void Set(const RContainer<C,true,true>* c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(const RContainer<C,false,true>& c);

	/**
	* Set the container.
	* param c               Smart pointer to the container to iterate.
	*/
	void Set(const std::auto_ptr<RContainer<C,false,true> >& c);

	/**
	* Set the container. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	void Set(const RContainer<C,false,true>* c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(const RContainer<C,true,false>& c);

	/**
	* Set the container.
	* param c               Smart pointer to the container to iterate.
	*/
	void Set(const std::auto_ptr<RContainer<C,true,false> >& c);

	/**
	* Set the container. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	void Set(const RContainer<C,true,false>* c);

	/**
	* Set the container.
	* param c               Container to iterate.
	*/
	void Set(const RContainer<C,false,false>& c);

	/**
	* Set the container.
	* param c               Smart pointer to the container to iterate.
	*/
	void Set(const std::auto_ptr<RContainer<C,false,false> >& c);

	/**
	* Set the container. If the pointer passed is null, the cursor will parse
	* zero elements.
	* param c               Pointer to the container to iterate.
	*/
	void Set(const RContainer<C,false,false>* c);

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
* @param name               Name of the class representing the cursor.
* @param C                  The class of the elements that are contained.
*/
#define CLASSCURSOR(name,C)                                                          \
class name : public R::RCursor<C>                                                    \
{                                                                                    \
public:                                                                              \
	name(void) : R::RCursor<C>() {}                                                  \
	name(const R::RContainer<C,true,true>& c) : R::RCursor<C>(c) {}                  \
	name(const std::auto_ptr<R::RContainer<C,true,true> >& c) : R::RCursor<C>(c) {}  \
	name(const R::RContainer<C,true,true>* c) : R::RCursor<C>(c) {}                  \
	name(const R::RContainer<C,true,false>& c) : R::RCursor<C>(c) {}                 \
	name(const std::auto_ptr<R::RContainer<C,true,false> >& c) : R::RCursor<C>(c) {} \
	name(const R::RContainer<C,true,false>* c) : R::RCursor<C>(c) {}                 \
	name(const R::RContainer<C,false,true>& c) : R::RCursor<C>(c) {}                 \
	name(const std::auto_ptr<R::RContainer<C,false,true> >& c) : R::RCursor<C>(c) {} \
	name(const R::RContainer<C,false,true>* c) : R::RCursor<C>(c) {}                 \
	name(const R::RContainer<C,false,false>& c) : R::RCursor<C>(c) {}                \
	name(const std::auto_ptr<R::RContainer<C,false,false> >& c) : R::RCursor<C>(c) {}\
	name(const R::RContainer<C,false,false>* c) : R::RCursor<C>(c) {}                \
	name& operator=(const name& c) throw(std::bad_alloc)                             \
	{                                                                                \
		R::RCursor<C>::operator=(c);                                                 \
		return(*this);                                                               \
	}                                                                                \
	name& operator=(const R::RCursor<C>& c) throw(std::bad_alloc)                    \
	{                                                                                \
		R::RCursor<C>::operator=(c);                                                 \
		return(*this);                                                               \
	}                                                                                \
}


//------------------------------------------------------------------------------
#endif
