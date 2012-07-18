/*

	R Project Library

	RContainer.h

	Container - Header.

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
#ifndef RContainerH
#define RContainerH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rexception.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// forward declaration
template<class C> class RCursor;
template<class C> class RQuickSort;


//-----------------------------------------------------------------------------
/**
* This class represent a container of elements (class C). This elements are
* stored in an array of pointers which will be increase when necessary. The
* container can be responsible for the deallocation of the elements
* (bAlloc), and the elements can be ordered (bOrder).
* @tparam C                  The class of the elements to be contained.
* @tparam bAlloc             Specify if the elements are deallocated by the
*                            container.
* @tparam bOrder             Specify if the elements are ordered in the
*                            container.
*
* To make the necessary comparisons, the container uses member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* @endcode
*
* The TUse represent a class or a structure used for the comparisons. These
* functions are working like the strcmp function from the standard C/C++
* library. The result returned specifies if the tag precedes (>0), is the
* same (0) or is after (<0) the element used. The structure used for the
* Comparisons can represent or not the sort key used when the container must
* be ordered. The user must specify if the key used for a search is the
* sorting one or not.
*
* At least, a compare function must be implemented in the class C:
* @code
* int Compare(const C&) const;
* @endcode
*
* To iterate through the container, a RCursor can be used. Here is an example
* of class MyElement that will be contained in the variable c:
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
* int main()
* {
*    R::RContainer<MyElement,true,true> c(20,10);
*
*    c.InsertPtr(new MyElement(5));
*    if(c.IsIn<char*>("5"))
*       cout<<"An element of value 5 is in the container"<<endl;
*    c.InsertPtr(new MyElement(10));
*    R::RCursor<MyElement> cur(c);
*    for(cur.Start();!cur.End();cur.Next())
*       cur()->DoSomething(2.3);
* }
* @endcode
*
* @author Pascal Francq
* @short Container template.
*/
template<class C,bool bAlloc,bool bOrder=false>
	class RContainer
{
protected:

	/**
	* The array of pointers for the elements.
	*/
	C** Tab;

	/**
	* The number of elements in the container.
	*/
	size_t NbPtr;

	/**
	* The maximal number of elements that can be hold by the container
	* actually.
	*/
	size_t MaxPtr;

	/**
	* The last position in the array used by an object.
	*/
	size_t LastPtr;

	/**
	* When the array is increased, this value is used.
	*/
	size_t IncPtr;

	/**
	* Generic static template function to sort a container.
	*/
	static int SortOrder(const void* a,const void* b)
	{
		return((*((C**)a))->Compare(**((C**)b)));
	}

	/**
	* Create a container from another one. If the pointer to the container is
	* null, an empty container is created.
	* @tparam b              Determine if the source container is responsible for
	*                        the allocation.
	* @param src             Pointer to the source container.
	*/
	template<bool b> void Create(const RContainer<C,b,bOrder>& src);

	/**
	* Copy a container from another one. If the pointer to the container is
	* null, the container is just emptied.
	* @tparam b              Determine if the source container is responsible for
	*                        the allocation.
	* @param src             Pointer to the source container.
	*/
	template<bool b> RContainer& NormalCopy(const RContainer<C,b,bOrder>& src);

	/**
	* Deep copy of a container in another one. If the pointer to the container is
	* null, the container is just emptied.
	* @tparam b              Determine if the source container is responsible for
	*                        the allocation.
	* @param src             Pointer to the source container.
	*/
	template<bool b> void DeepCopy(const RContainer<C,b,bOrder>& src);

	/**
	* Add a container (if the pointer is not null) from another one.
	* @tparam b              Determine if the source container is responsible for
	*                        the allocation.
	* @tparam o              Determine if the source container is ordered.
	* @param src             Pointer to the source container.
	*/
	template<bool b,bool o> RContainer& Add(const RContainer<C,b,o>& src);

public:

	/**
	* Constructor of a container.
	* @param m               The initial maximal size of the array.
	* @param i               The value used when increasing the array. If null
	*                        value, the size is set to the half the maximal
	*                        size.
	*/
	explicit RContainer(size_t m,size_t i=0);

	/**
	* Copy constructor of a container.
	* @param src             Container used as source.
	*/
	RContainer(const RContainer<C,true,bOrder>& src);

	/**
	* Copy constructor of a container.
	* @param src             Container used as source.
	*/
	RContainer(const RContainer<C,false,bOrder>& src);

	/**
	* @return the number of elements in the container.
	*/
	inline size_t GetNb(void) const {return(NbPtr);}

	/**
	* @return  the maximal position occupied by an elements in the container.
	*/
	inline size_t GetMaxPos(void) const {if(NbPtr) return(LastPtr-1); else return(0);}

	/**
	* @return the maximum number of elements in the container.
	*/
	inline size_t GetMaxNb(void) const {return(MaxPtr);}

	/**
	* @return the increment used to resize the container.
	*/
	inline size_t GetIncNb(void) const {return(IncPtr);}

	/**
	 * Verify if an index is a valid one in the container.
	 * @param idx            Index.
	 * @return true if the index is valid.
	 */
	inline bool VerifyIndex(size_t idx) const {if(!NbPtr) return(0); return(idx<LastPtr);}

	/**
	* Verify if the container can hold a certain number of elements. If not,
	* the container is extended.
    * @param max             Maximal number of elements that must be contained.
	*                        If null, the method verifies that one element can
	*                        be added.
	*/
	void VerifyTab(size_t max=0);

	/**
	* Clear the container and destruct the elements if it is responsible for
	* the deallocation.The container can also be extended.
	* @param m              New maximal size of the array. If null, the old
	*                       size remains.
	* @param i              New increasing value. If null, the old value
	*                       remains.
	*/
	void Clear(size_t m=0,size_t i=0);

	/**
	 * Compare method that can be used to construct an unordered container of
	 * containers.
    */
	int Compare(const RContainer<C,bAlloc,bOrder>&) const {return(-1);}

	/**
	* ReOrder the container.
	*
	* @param sortOrder       Pointer to a (static) function used for the ordering.
   * @param min             Starting index of the container part concerned.
	* @param max             Ending index of the container part concerned.
   * @warning This method must be used with caution, because it can crash the
   * container if:
	* -# The container contains null pointers.
	* -# The container is ordered and the method does not use the same criterion
	*    for the ordering.
	*/
	inline void ReOrder(int sortOrder(const void*,const void*),size_t min=0,size_t max=0)
	{
       if(!NbPtr)
           return;  // No elements -> no sorting
       size_t NbMin,NbMax;
       if(min<LastPtr-1)
          NbMin=min;
       else
          NbMin=0;
       if((!max)||(max>=LastPtr)||(max<min))
          NbMax=LastPtr;
       else
          NbMax=max;
		if(NbMax)
			qsort(static_cast<void*>(&Tab[NbMin]),NbMax,sizeof(void*),sortOrder);
	}

	/**
	* ReOrder the container based on the 'Compare' method of the objects
	* contained.
	* @param min             Starting index of the container part concerned.
	* @param max             Ending index of the container part concerned.
   * @warning This method must be used with caution, because it can crash the
   * container if the container contains null pointers.
	*/
	inline void ReOrder(size_t min=0,size_t max=0) {ReOrder(SortOrder,min,max);}

	/**
	* Exchange two elements in the container. The method does not verify if the
	* exchange is compatible with the ordering of the container.
	* @param pos1            Position of the first element.
	* @param pos2            Position of the second element.
	*/
	void Exchange(size_t pos1,size_t pos2);

	/**
	 * Transfer a container into another one. The destination container is
	 * cleared before.
	 * @tparam b             Determine if the source container is responsible for the allocation.
	 * @tparam o             Determine if the source container is ordered.
	 * @param src            Source container.
	 */
	template<bool a,bool o> void Transfer(RContainer<C,a,o>& src);

	/**
	* The assignment operator.
	* @param src             Container used as source.
	* \warning If the container is not a responsible for the allocation, the
	* elements of src are just copied and not re-created. Use Copy if you want a
	* "deep" copy of src.
	*/
	inline RContainer& operator=(const RContainer<C,true,bOrder>& src) {return(NormalCopy<true>(src));}

	/**
	* The assignment operator.
	* @param src             Container used as source.
	* \warning If the container is not a responsible for the allocation, the
	* elements of src are just copied and not re-created. Use Copy if you want a
	* "deep" copy of src.
	*/
	inline RContainer& operator=(const RContainer<C,false,bOrder>& src) {return(NormalCopy<false>(src));}

	/**
	* Deep copy of a container.
	* @tparam a              Determine if the source container is responsible for the allocation.
	* @param src             Container used as source.
	*/
	template<bool a> inline void Copy(const RContainer<C,a,bOrder>& src) {DeepCopy<a>(src);}

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	inline RContainer& operator+=(const RContainer<C,true,true>& src) {return(Add<true,true>(src));}

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	inline RContainer& operator+=(const RContainer<C,true,false>& src) {return(Add<true,false>(src));}

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	inline RContainer& operator+=(const RContainer<C,false,true>& src) {return(Add<false,true>(src));}

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	inline RContainer& operator+=(const RContainer<C,false,false>& src) {return(Add<false,false>(src));}

	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered. The search can be limited to
	* a part of the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param order           Must the search be done on an ordered basis ?
	* @param tag             The tag used.
	* @param find            If the element represented by tag exist, find is set to
	*                        true.
	* @param min             Starting index of the container part concerned.
	* @param max             Ending index of the container part concerned.
	* @return Returns the index of the element if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> size_t GetIndex(bool order,const TUse& tag,bool& find,size_t min=0,size_t max=0) const;

	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered. The search can be limited to
	* a part of the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param find            If the element represented by tag exist, find is set to
	*                        true.
	* @param min             Starting index of the container part concerned.
	* @param max             Ending index of the container part concerned.
	* @return Returns the index of the element if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> inline size_t GetIndex(const TUse& tag,bool& find,size_t min=0,size_t max=0) const
	{
		return(GetIndex(bOrder,tag,find,min,max));
	}

	/**
	* Look if a certain element is in the container. The search can be limited
	* to a part of the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return Return true if the element is in the container.
	*/
	template<class TUse> inline bool IsIn(const TUse& tag,bool sortkey=bOrder,size_t min=0,size_t max=0) const
	{
		bool Find;
		GetIndex(bOrder&&sortkey,tag,Find,min,max);
		return(Find);
	}

	/**
	* Get a pointer to the ith element in the container (Only read). The
	* operator generates an exception is the index is out of range.
	* @param idx             Index of the element to get.
	* @return Return the pointer.
	*/
	const C* operator[](size_t idx) const;

	/**
	* Get a pointer to the ith element in the container (Read/Write). The
	* operator generates an exception is the index is out of range.
	* @param idx             Index of the element to get.
	* @return the pointer.
	*/
	C* operator[](size_t idx);

	/**
	 * Get a pointer of the ith element in the container (Only read).
	 * @param idx            Index of the element to get.
	 * @return the pointer of null if the index is out of range.
	 */
	inline const C* GetPtrAt(size_t idx) const
	{
		if(idx>=LastPtr)
			return(0);
		return(Tab[idx]);
	}

	/**
	 * Get a pointer of the ith element in the container (Read/Write).
	 * @param idx            Index of the element to get.
	 * @return the pointer of null if the index is out of range.
	 */
	inline C* GetPtrAt(size_t idx)
	{
		if(idx>=LastPtr)
			return(0);
		return(Tab[idx]);
	}

	/**
	* Get a pointer to a certain element in the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> C* GetPtr(const TUse& tag,bool sortkey=bOrder,size_t min=0,size_t max=0) const;

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> C* GetInsertPtr(const TUse& tag,bool sortkey=bOrder,size_t min=0,size_t max=0);

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param pos             The position where to insert it.
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> C* GetInsertPtrAt(const TUse& tag,size_t pos);

	/**
	* Copy the array of pointers into a temporary array. This array must have
	* the right size (Read only). This method can be limited to a part of the
	* container.
	* @param tab             Array of pointers.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	size_t GetTab(const void** tab,size_t min=0,size_t max=0) const;

	/**
	* Copy the array of pointers into a temporary array. This array must have
	* the right size (Read/Write). This method can be limited to a part of the
	* container.
	* @param tab             Array of pointers.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	size_t GetTab(void** tab,size_t min=0,size_t max=0);

	/**
	* Copy the array of pointers into a temporary array. This array must have
	* the right size (Read only). This method can be limited to a part of the
	* container.
	* @param tab             Array of pointers.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	inline size_t GetTab(const C** tab,size_t min=0,size_t max=0) const {return(GetTab(reinterpret_cast<const void**>(tab),min,max));}

	/**
	* Copy the array of pointers into a temporary array. This array must have
	* the right size (Read/Write). This method can be limited to a part of the
	* container.
	* @param tab             Array of pointers.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	inline size_t GetTab(C** tab,size_t min=0,size_t max=0) {return(GetTab(reinterpret_cast<void**>(tab),min,max));}

	/**
	* This method returns a container of all the elements that are responding
	* to the given criteria. This method can be limited to a part of the
	* container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return The function returns a pointer to the result container.
	*/
	template<class TUse> inline RContainer<C,false,bOrder>* GetPtrs(const TUse& tag,size_t min=0,size_t max=0) const;

	/**
	* Insert an element at a certain position. Two remarks must be made :
	* -#    The function verifies not if the index used is compatible with the
	*       order in case of the elements treated in ascending order.
	* -#    By using this function, the user can leave "blanks" in the container.
	*       In other words, some "valid" pointers could be null. This situation is
	*       not handle by the other functions of the container.
	*
	* @param ins             A pointer to the element to insert.
	* @param pos             The position where to insert it.
	* @param del             Specify if the object that was previously at the
	*                        position should be deleted (true) or shifted (false).
	*/
	void InsertPtrAt(C* ins,size_t pos,bool del=bAlloc);

	/**
	* Insert an element in the container. If the container is ordered and if
	* the corresponding index is already used, the previously inserted element
	* is removed from the container (and destroy if the container is
	* responsible for the allocation).  This method can be limited to a part of
	* the container.
	* @param ins             A pointer to the element to insert.
	* @param del             Specify if a similar existing element must be
	*                        deleted.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	*/
	void InsertPtr(C* ins,bool del=false,size_t min=0,size_t max=0);

	/**
	* Delete an element from the container at a given position.
	* @param pos             Position of the element.
	* @param shift           Specify if the the container should be shifted or
	*                        if the position should be left empty.
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	*/
	void DeletePtrAt(size_t pos,bool shift=true,bool del=bAlloc);

	/**
	* Delete an element from the container. This method can be limited to a
	* part of the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	*/
	template<class TUse> void DeletePtr(const TUse& tag,bool sortkey=bOrder,bool del=bAlloc,size_t min=0,size_t max=0);

	/**
	* Destruct the container.
	*/
	virtual ~RContainer(void);

	friend class RContainer<C,true,true>;
	friend class RContainer<C,true,false>;
	friend class RContainer<C,false,true>;
	friend class RContainer<C,false,false>;
	friend class RCursor<C>;
	friend class RQuickSort<C>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rcontainer.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
