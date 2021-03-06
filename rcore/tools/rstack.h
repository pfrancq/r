/*

	R Project Library

	RStack.h

	Stack - Header.

	Copyright 2007-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2007-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef RStack_H
#define RStack_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a template for a stack of elements. These elements are
* stored in an array of pointers which will be increase when necessary.
* @param C                   The class of the elements.
* @param bAlloc              Specify if the elements are destroyed when removed
*                            from the stack.
* @param bPushTop            Specify if a new element is added at the top of
*                            the stack (true) or at the end (false).
* @param bPopTop             Specify if the next element to remove is taken from
*                            the top of the stack (true) or at the end (false).
*
* To make the necessary comparisons, the stack needs that a compare function
* must be implemented in the class C:
* @code
* int Compare(const C&) const;
* @endcode
*
* Here is an example:
* @code
* #include <rstack.h>
* #include <rcursor.h>
* using namespace R;
*
* class MyElement
* {
*    size_t Id;
* public:
*    MyElement(size_t id) : Id(id) {}
*    MyElement(const MyElement& e) : Id(e.Id) {}
*    int Compare(const MyElement& e) const {return(Id-e.Id);}
*    void DoSomething(double d) {cout<<Id<<":"<<d<<endl;}
* };
*
* int main()
* {
*    // Create a Last-in - First-out stack :
*    // - A new element is push at the end of the stack (false)
*    // - The element to remove is taken from the end of the stack (false)¬≤
*    R::RStack<MyElement,true,false,false> c(20,10);
*
*    c.Push(new MyElement(5));
*    c.Push(new MyElement(10));
*    while(c.GetNb())
*    {
*       c()->DoSomething(2.3);
*       c.Pop();
*	 }
* }
* @endcode
*
* @short Stack template.
*/
template<class C,bool bAlloc,bool bPushTop,bool bPopTop>
	class RStack : private RContainer<C,bAlloc,false>
{
public:

	/**
	* Constructor of a stack.
	* @param m               The initial maximal size of the stack.
	* @param i               The value used when increasing the stack. If null
	*                        value, the size is set to the half the maximal
	*                        size.
	*/
	explicit RStack(size_t m,size_t i=0) : RContainer<C,bAlloc,false>(m,i) {}

	/**
	* Copy constructor of a stack.
	* @param src             Stack used as source.
	*/
	RStack(const RContainer<C,true,false>& src) : RContainer<C,bAlloc,false>(src) {}

	/**
	* Copy constructor of a stack.
	* @param src             Stack used as source.
	*/
	RStack(const RContainer<C,false,false>& src) : RContainer<C,bAlloc,false>(src) {}

	/**
	* Get the number of elements in the stack.
	* @return size_t.
	*/
	inline size_t GetNb(void) const {return(RContainer<C,bAlloc,false>::GetNb());}

	/**
	* Clear the stack and destruct the elements if it is responsible for
	* the desallocation. The stack may also be extended.
	* @param m              New maximal size of the array. If null, the old
	*                       size remains.
	* @param i              New increasing value. If null, the old value
	*                       remains.
	*/
	inline void Clear(size_t m=0,size_t i=0) {RContainer<C,bAlloc,false>::Clear(m,i);}

	/**
	 * Get the current element to treat from the stack (the element is not removed
	 * from the stack.
	 * @return Pointer to the element.
	 */
	inline C* operator()(void)
	{
		if(bPopTop)
			return(RContainer<C,bAlloc,false>::operator[](0));
		else
			return(RContainer<C,bAlloc,false>::operator[](RContainer<C,bAlloc,false>::GetMaxPos()));
	}

	/**
	 * Get the current element to treat from the stack (the element is not removed
	 * from the stack.
	 * @return Pointer to the element.
	 */
	inline const C* operator()(void) const
	{
		if(bPopTop)
			return(RContainer<C,bAlloc,false>::operator[](0));
		else
			return(RContainer<C,bAlloc,false>::operator[](RContainer<C,bAlloc,false>::GetMaxPos()));
	}

	/**
	* Get a pointer to the ith element in the container (Only read).
	* @param idx             Index of the element to get.
	* @return Return the pointer.
	*/
	inline const C* operator[](size_t idx) const {return(static_cast<const C*>(RContainer<C,bAlloc,false>::operator[](idx)));}

	/**
	* Get a pointer to the ith element in the container (Read/Write).
	* @param idx             Index of the element to get.
	* @return Return the pointer.
	*/
	inline C* operator[](size_t idx) {return(static_cast<C*>(RContainer<C,bAlloc,false>::operator[](idx)));}

	/**
	 * Remove the next element from the stack.
	 */
	void Pop(void)
	{
		if(bPopTop)
			RContainer<C,bAlloc,false>::DeletePtrAt(0,true);
		else
			RContainer<C,bAlloc,false>::DeletePtrAt(RContainer<C,bAlloc,false>::GetMaxPos(),true);
	}

	/**
	 * Add an element to the stack.
	 * @return ptr           Element to push.
	 */
	void Push(C* ptr)
	{
		if(bPushTop)
			RContainer<C,bAlloc,false>::InsertPtrAt(ptr,0,false);
		else
			RContainer<C,bAlloc,false>::InsertPtr(ptr);
	}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
