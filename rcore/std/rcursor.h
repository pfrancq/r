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
* This class represent a cursor to iterate a RContainer.
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
*    unsigned int Id;
* public:
*    MyElement(unsigned int id) : Id(id) {}
*    MyElement(const MyElement& e) : Id(e.Id) {}
*    void DoSomething(double d) {cout<<d<<endl;}
*    int Compare(const MyElement& e) const {return(Id-e.Id);}
*    int Compare(const unsigned int id) const {return(Id-id);}
*    int Compare(const char* text) const
*    {
*       unsigned int id=atoi(text);
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
	RCursor(const RContainer<C,true,true>& c,size_t min=0,size_t max=SIZE_MAX) : BasicCursor(c,min,max) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RCursor(const RContainer<C,false,true>& c,size_t min=0,size_t max=SIZE_MAX) : BasicCursor(c,min,max) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RCursor(const RContainer<C,true,false>& c,size_t min=0,size_t max=SIZE_MAX) : BasicCursor(c,min,max) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RCursor(const RContainer<C,false,false>& c,size_t min=0,size_t max=SIZE_MAX) : BasicCursor(c,min,max) {}

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
	template<bool b,bool o> void Set(const RContainer<C,b,o>& c,size_t min=0,size_t max=SIZE_MAX) {BasicCursor::Set(c,min,max);}

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const {return(reinterpret_cast<C*>(*Current));}

};


//-----------------------------------------------------------------------------
/**
* This class represent a child cursor to iterate a RContainer. It is used when
* the container to iterate is supposed to handle objects from a given class but
* the objects allocated are of a child class.
* @param master              Class originally declared in the container. This
*                            class must have at least one virtual method (for
*                            example the destructor).
* @param child               Class of the elements of the container. It must be
*                            a child class of the master class.
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
*    virtual ~O(void) {}    // Virtual destructor is necessary
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
*    RChildCursor<O,O2> Cur(Cont);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       cout<<Cur()->Name<<" - "<<Cur()->Id<<endl;
* }
* @endcode
*
* @author Pascal Francq
* @short Container Child Cursor.
*/
template<class master,class child>
	class RChildCursor : public RCursor<master>
{
public:
	
	/**
	* Construct the cursor.
	*/
	RChildCursor(void) : RCursor<master>() {}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RChildCursor(const RCursor<master>& src) : RCursor<master>(src) {}
	
	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RChildCursor(const RChildCursor<master,child>& src) : RCursor<master>(src) {}
	
	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RChildCursor(const RContainer<master,true,true>& c,size_t min=0,size_t max=SIZE_MAX) : RCursor<master>(c,min,max) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RChildCursor(const RContainer<master,false,true>& c,size_t min=0,size_t max=SIZE_MAX) : RCursor<master>(c,min,max) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RChildCursor(const RContainer<master,true,false>& c,size_t min=0,size_t max=SIZE_MAX) : RCursor<master>(c,min,max) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	RChildCursor(const RContainer<master,false,false>& c,size_t min=0,size_t max=SIZE_MAX) : RCursor<master>(c,min,max) {}

	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RChildCursor<master,child>& operator=(const RChildCursor<master,child>& src) { return(static_cast<RChildCursor<master,child>&>(RCursor<master>::operator=(src)));}

	/**
	* Set the container.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	template<bool b,bool o> void Set(const RContainer<master,b,o>& c,size_t min=0,size_t max=SIZE_MAX) {RCursor<master>::Set(c,min,max);}

	/**
	* Return the current element.
	*/
	inline child* operator()(void) const {return(dynamic_cast<child*>(RCursor<master>::operator()()));}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
