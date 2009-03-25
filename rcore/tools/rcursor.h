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
#include <rcontainer.h>
#include <basiccursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a cursor iterating a RContainer.
* @param C                   Class of the elements of the container.
* \attention When an element is added or removed from the container parsed by
* the cursor, the cursor is not valid anymore.
*
* Here is an example of a cursor used:
* @code
* #include <rcontainer.h>
* #include <rcursor.h>
* using namespace R;
*
* class MyElement
* {
*    size_t Id;
* public:
*    MyElement(size_t id) : Id(id) {}
*    MyElement(const MyElement& e) : Id(e.Id) {}
*    void DoSomething(double d) {cout<<d<<endl;}
*    int Compare(const MyElement& e) const {return(Id-e.Id);}
*    int Compare(const size_t id) const {return(Id-id);}
*    int Compare(const char* text) const
*    {
*       size_t id=atoi(text);
*       return(Compare(id));
*    }
* };
*
* int example(RContainer<MyElement,true,true> *c)
* {
*    RCursor<MyElement> Cur;
*
*    Cur.Set(c);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       Cur()->DoSomething(2.3);
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
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	template<bool a,bool o> RCursor(const RContainer<C,a,o>& c,size_t min=0,size_t max=SIZE_MAX) : BasicCursor(c,min,max) {}

	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RCursor<C>& operator=(const RCursor<C>& src) { return(static_cast<RCursor<C>&>(BasicCursor::operator=(src)));}

	/**
	* Set the container.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	template<bool a,bool o> void Set(const RContainer<C,a,o>& c,size_t min=0,size_t max=SIZE_MAX) {BasicCursor::Set(c,min,max);}

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const {return(static_cast<C*>(*Current));}
};


//-----------------------------------------------------------------------------
/**
* This class represent a cursor iterating a RContainer and performing cast of
* the pointers stored. It is used when the container to iterate is supposed to
* handle objects from a given class but the objects allocated are of a child
* class.
* @param C                   Class of the elements of the container. It must
*                            inherit from the class declared in the container.
* \attention When an element is added or removed from the container parsed by
* the cursor, the cursor is not valid anymore.
* 
* Here is an example of a cursor used:
* @code
* #include <rcontainer.h>
* #include <rcursor.h>
* using namespace R;
*
* class O
* {
* public:
*    RString Name;
*    O(const RString& name) : Name(name) {}
*    int Compare(const O& o) const {return(Name.Compare(o.Name));}
* };
* 
* class O2 : public O
* {
* public:
*    int Id;
*    O2(const RString& name,int id) : O(name), Id(id) {}
* };
* 
* void Test(void)
* {
*    RContainer<O,true,true> Cont(20);
*    Cont.InsertPtr(new O2("Obj1",1));
*    Cont.InsertPtr(new O2("Obj2",2));
*    Cont.InsertPtr(new O2("Obj3",3));
*    RCastCursor<O2> Cur(Cont);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       cout<<Cur()->Name<<" - "<<Cur()->Id<<endl;
* }
* @endcode
*
* @author Pascal Francq
* @short Container Child Cursor.
*/
template<class C>
	class RCastCursor : public BasicCursor
{
public:

	/**
	* Construct the cursor.
	*/
	RCastCursor(void) : BasicCursor() {}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCastCursor(const BasicCursor& src) : BasicCursor(src) {}
	
	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCastCursor(const RCastCursor<C>& src) : BasicCursor(src) {}
	
	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCastCursor(const RCursor<C>& src) : BasicCursor(src) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RCastCursor(const BasicContainer& c,size_t min=0,size_t max=SIZE_MAX) : BasicCursor(c,min,max) {}

	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RCastCursor<C>& operator=(const RCastCursor<C>& src) { return(static_cast<RCastCursor<C>&>(BasicCursor::operator=(src)));}

	/**
	* Set the container.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	void Set(const BasicContainer& c,size_t min=0,size_t max=SIZE_MAX) {BasicCursor::Set(c,min,max);}

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const {return(static_cast<C*>(*Current));}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif