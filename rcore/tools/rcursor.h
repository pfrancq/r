/*

	R Project Library

	RCursor.h

	Container Cursor - Header.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
* int example(RContainer<RString,true,true>& c)
* {
*    RCursor<MyElement> Cur(c);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       Cur()->DoSomething(2.3);
*
*    RCursor<MyElement> Cur2(0,1);  // Treat the first two elements
*    for(Cur2.Start();!Cur2.End();Cur2.Next())
*       Cur2()->DoSomething(2.3);
* }
* @endcode
*
* @author Pascal Francq
* @short Container Cursor.
*/
template<class C>
	class RCursor
{
protected:

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
	C** Tab;

	/**
	* The number of elements in the container.
	*/
	size_t NbPtr;

	/**
	* The first position in the array handled by the cursor.
	*/
	size_t FirstPtr;

	/**
	* The last position in the array handled by the cursor.
	*/
	size_t LastPtr;


public:

	/**
	* Construct the cursor.
	*/
	RCursor(void)
	{
		ActPtr=0;
		Current=Tab=0;
		ActPtr=FirstPtr=LastPtr=NbPtr=0;
	}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCursor(const RCursor<C>& src)
	{
		ActPtr=src.ActPtr;
		Current=src.Current;
		Tab=src.Tab;
		FirstPtr=src.FirstPtr;
		LastPtr=src.LastPtr;
		NbPtr=src.NbPtr;
	}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate (included max).
	*                        If SIZE_MAX, iterate until the end of the container.
	*/
	template<bool a,bool o> RCursor(const RContainer<C,a,o>& c,size_t min=0,size_t max=SIZE_MAX)
	{
		Set(c,min,max);
	}


	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RCursor<C>& operator=(const RCursor<C>& src)
	{
		ActPtr=src.ActPtr;
		Current=src.Current;
		Tab=src.Tab;
		FirstPtr=src.FirstPtr;
		LastPtr=src.LastPtr;
		NbPtr=src.NbPtr;
		return(*this);
	}

	/**
	* Set the container.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate (included max).
	*                        If SZE_MAX, iterate until the end of the container.
	*/
	template<bool a,bool o> void Set(const RContainer<C,a,o>& c,size_t min=0,size_t max=SIZE_MAX)
	{
		RAssert(min<=max);
		NbPtr=c.NbPtr;
		Tab=c.Tab;
		if((max!=SIZE_MAX)&&(max<c.LastPtr))
			LastPtr=max+1;
		else
			LastPtr=c.LastPtr;
		if((min<=max)&&(min<c.LastPtr))
			FirstPtr=min;
		else
			FirstPtr=0;
		Current=0;
		ActPtr=0;
	}

	/**
	 * Clear the cursor.
	 */
	void Clear(void)
	{
		Current=Tab=0;
		ActPtr=FirstPtr=LastPtr=NbPtr=0;
	}

	/**
	* Start the iterator at the beginning of the container.
	*/
	void Start(void)
	{
		ActPtr=FirstPtr;
		if(!NbPtr)
		{
			Current=0;
			return;
		}
		Current=&Tab[FirstPtr];
		while((!(*Current))&&(ActPtr<LastPtr))
			Next();
	}

	/**
	 * Start the iterator at the end of the container.
	 */
	void StartFromEnd(void)
	{
		ActPtr=LastPtr-1;
		if(!NbPtr)
		{
			Current=0;
			return;
		}
		Current=&Tab[LastPtr-1];
		while((!(*Current))&&(ActPtr))
			Prev();
	}

	/**
	* Go to the i-th element of the cursor.
	* @param idx             Index of the element to get.
	*/
	void GoTo(size_t idx)
	{
		idx+=FirstPtr;
		if(idx>=LastPtr)
			throw std::range_error("void RCursor::GoTo(size_t) : column "+RString::Number(idx)+" outside range ["+RString::Number(FirstPtr)+","+RString::Number(LastPtr-1)+"]");
		Current=&Tab[idx];
		ActPtr=idx;
	}

   /**
    * Delete the current element from the container.
    * @param c              Container to delete from. Must be the one associated
    *                       with the cursor.
    */
   template<bool a,bool o> void DeleteCurrent(RContainer<C,a,o>& c)
   {
       if(Tab!=c.Tab)
           throw std::invalid_argument("template<bool a,bool o> void RCursor::DeleteCur(RContainer<C,a,o>& c) : cursor and container are not synchronized");

       // Remember the position
       size_t where(ActPtr);
       bool IsEnd(ActPtr+1>=LastPtr);

       // Delete the element and reset the container
       c.DeletePtr(**Current);
       Set(c);

       // Put the cursor at the right place
       if(IsEnd)
       {
           if(NbPtr>1)
           {
               ActPtr=where;
               Current=&Tab[where];
               Prev();
           }
           else
               Start(); // Less than two elements -> start again
       }
       else
         GoTo(where);  // Not at the end -> goes to the previous position
   }

	/**
	* Return the actual position in the cursor.
	*/
	inline size_t GetPos(void) const {return(ActPtr-FirstPtr);}

	/**
	* @return the maximal position occupied by an elements in the container.
	*/
	inline size_t GetMaxPos(void) const {if(NbPtr) return(LastPtr-1); else return(0);}

	/**
	* Return the number of elements in the cursor.
	*/
	inline size_t GetNb(void) const {return(LastPtr-FirstPtr);}

	/**
	* Test if the begin of the cursor is reached.
	*/
	inline bool Begin(void) const {return(ActPtr==cNoRef);}

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
	* Go to a given number of next elements. If the end is reached, go to the
	* beginning.
	* @param inc             Number of elements to go to.
	*/
	void Next(size_t inc=1)
	{
		if(!NbPtr) return;
		if(ActPtr==LastPtr)
			Start();
		else
		{
			ActPtr+=inc;
			Current+=inc;
			while((ActPtr<LastPtr)&&(!(*Current)))     // Go to next non-null pointer
			{
				ActPtr++;
				Current++;
			}
		}
	}

	/**
	* Go to a given number of previous elements. If the begin is reached, go to the
	* end.
	* @param inc             Number of elements to go to.
	*/
	void Prev(size_t inc=1)
	{
		if(!NbPtr) return;
		if(ActPtr==cNoRef)
		{
			StartFromEnd();
			if(inc>1)
				Prev(inc);
		}
		else
		{
			if(inc>ActPtr)
				ActPtr=cNoRef;
			else
			{
				ActPtr-=inc;
				Current-=inc;
				while((ActPtr)&&(!(*Current)))     // Go to previous non-null pointer
				{
					ActPtr--;
					Current--;
				}
			}
		}
	}

	/**
	* @return the current element.
	*/
	inline C* operator()(void) const {return(*Current);}
};


//-----------------------------------------------------------------------------
/**
* This class represent a cursor iterating a RContainer and performing cast of
* the pointers stored. It is used when the container to iterate is supposed to
* handle objects from a given class but the objects allocated are of a child
* class.
* @tparam C                  Class of the elements of the container. It must
*                            inherit from the class B.
* @tparam B                  Base class of the elements of the container. The
*                            class must declare at least one virtual method
*                            (for example the destructor).
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
*    RCastCursor<O,O2> Cur(Cont);
*    for(Cur.Start();!Cur.End();Cur.Next())
*       cout<<Cur()->Name<<" - "<<Cur()->Id<<endl;
* }
* @endcode
*
* @author Pascal Francq
* @short Container Child Cursor.
*/
template<class B,class C>
	class RCastCursor : public RCursor<B>
{
public:

	/**
	* Construct the cursor.
	*/
	RCastCursor(void) : RCursor<B>() {}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCastCursor(const RCursor<B>& src) : RCursor<B>(src) {}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RCastCursor(const RCastCursor<B,C>& src) : RCursor<B>(src) {}

	/**
	* Construct the cursor.
	* @param c               Container to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate. If
	*                        SIZE_MAX, iterate until the end of the container.
	*/
	template<bool a,bool o> RCastCursor(const RContainer<B,a,o>& c,size_t min=0,size_t max=SIZE_MAX)  : RCursor<B>(c,min,max) {}

	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RCastCursor<B,C>& operator=(const RCastCursor<B,C>& src) { return(static_cast<RCastCursor<B,C>&>(RCursor<B>::operator=(src)));}

	/**
	* Return the current element.
	*/
	inline C* operator()(void) const {return(dynamic_cast<C*>(*RCursor<B>::Current));}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
