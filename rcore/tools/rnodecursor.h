/*

	R Project Library

	RNodeCursor.h

	Node Cursor - Header.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RNodeCursorH
#define RNodeCursorH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rnode.h>
#include <rtree.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class represent a cursor iterating childs of a given RNode of a RTree.
* @param T                   Class of the tree.
* @param N                   Class of the node.
* \attention When an element is added or removed from the node parsed by
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
* @short Node Cursor.
*/
template<class T,class N>
	class RNodeCursor
{
protected:

	/**
	 * Root node iterated;
	 */
	const T* Root;

	/**
	 * Node iterated;
	 */
	const N* Node;

	/**
	* This variable is used to go through the node.
	*/
	N* Current;

	/**
	* This variable is used to see if the end of the node is reached.
	*/
	size_t ActPtr;

	/**
	* The number of elements in the node.
	*/
	size_t NbPtr;

	/**
	* The first position handled by the cursor.
	*/
	size_t FirstPtr;

	/**
	* The last position handled by the cursor.
	*/
	size_t LastPtr;

public:

	/**
	* Construct the cursor.
	*/
	RNodeCursor(void)
	{
		Root=0;
		Node=Current=0;
		ActPtr=ActPtr=FirstPtr=LastPtr=NbPtr=0;
	}

	/**
	* Construct the cursor.
	* @param src             Source container.
	*/
	RNodeCursor(const RNodeCursor<T,N>& src)
	{
		Root=src.Root;
		ActPtr=src.ActPtr;
		Node=src.Node;
		Current=src.Current;
		FirstPtr=src.FirstPtr;
		LastPtr=src.LastPtr;
		NbPtr=src.NbPtr;
	}

	/**
	* Construct the cursor.
	* @param node            Node to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate (included max).
	*                        If SIZE_MAX, iterate until the end of the container.
	*/
	RNodeCursor(const N* node,size_t min=0,size_t max=SIZE_MAX)
	{
		Set(node,min,max);
	}

	/**
	* Construct the cursor.
	* @param root            Root node to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate (included max).
	*                        If SIZE_MAX, iterate until the end of the container.
	*/
	RNodeCursor(const T& root,size_t min=0,size_t max=SIZE_MAX)
	{
		Set(root,min,max);
	}


	/**
	* Assignment operator using a "Cursor".
	* @param src             Source container.
	*/
	RNodeCursor<T,N>& operator=(const RNodeCursor<T,N>& src)
	{
		Root=src.Root;
		ActPtr=src.ActPtr;
		Node=src.Node;
		Current=src.Current;
		FirstPtr=src.FirstPtr;
		LastPtr=src.LastPtr;
		NbPtr=src.NbPtr;
		return(*this);
	}

	/**
	* Set the container.
	* @param root            Top node to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate (included max).
	*                        If SZE_MAX, iterate until the end of the container.
	*/
	void Set(const T& root,size_t min=0,size_t max=SIZE_MAX)
	{
		mAssert(min<=max);
		NbPtr=root.NbTopNodes;
		if((max!=SIZE_MAX)&&(max<root.NbTopNodes))
			LastPtr=max+1;
		else
			LastPtr=root.NbTopNodes;
		if((min<=max)&&(min<root.NbTopNodes))
			FirstPtr=min;
		else
			FirstPtr=0;
		Current=0;
		ActPtr=0;
		Root=&root;
		Node=0;
	}

	/**
	* Set the container.
	* @param node            Node to iterate.
	* @param min             Minimum position of the elements to iterate.
	* @param max             Maximum position of the elements to iterate (included max).
	*                        If SZE_MAX, iterate until the end of the container.
	*/
	void Set(const N* node,size_t min=0,size_t max=SIZE_MAX)
	{
		mAssert(min<=max);
		NbPtr=node->NbSubNodes;
		if((max!=SIZE_MAX)&&(max<node->NbSubNodes))
			LastPtr=max+1;
		else
			LastPtr=node->NbSubNodes;
		if((min<=max)&&(min<node->NbSubNodes))
			FirstPtr=min;
		else
			FirstPtr=0;
		Current=0;
		ActPtr=0;
		Root=0;
		Node=node;
	}

	/**
	 * Clear the cursor.
	 */
	void Clear(void)
	{
		Root=0;
		Node=Current=0;
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
		if(Root)
			Current=Root->First;
		else
			Current=Node->First;
		for(size_t i=0;i<ActPtr;i++)
			Current=Current->Next;
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
		if(Root)
			Current=Root->Last;
		else
			Current=Node->Last;
		for(size_t i=ActPtr;i<LastPtr-1;i++)
			Current=Current->Prev;
	}

	/**
	* Go to the i-th element of the cursor.
	* @param idx             Index of the element to get.
	*/
	void GoTo(size_t idx)
	{
		idx+=FirstPtr;
		if(idx>=LastPtr)
			throw std::range_error("void RNodeCursor::GoTo(size_t) : column "+RString::Number(idx)+" outside range ["+RString::Number(FirstPtr)+","+RString::Number(LastPtr-1)+"]");
		if(Root)
			Current=Root->First;
		else
			Current=Node->First;
		for(size_t i=0;i<idx;i++)
			Current=Current->Next;
		ActPtr=idx;
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
			while((ActPtr<LastPtr)&&inc--)
			{
				ActPtr++;
				Current=Current->Next;
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
				while((ActPtr<LastPtr)&&inc--)
				{
					ActPtr++;
					Current=Current->Prev;
				}
			}
		}
	}

	/**
	* @return the current element.
	*/
	inline N* operator()(void) const {return(Current);}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
