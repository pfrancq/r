/*

	R Project Library

	RIContainer.h

	Container Interface - Header.

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
#ifndef RIContainerH
#define RIContainerH


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
template<class C> class RConstCursor;
template<class C> class RQuickSort;
template<class C,bool bAlloc,bool bOrder>	class RContainer;


//-----------------------------------------------------------------------------
/**
* This class represent an interface for a container of elements (class C). It is
* useful to manipulate a instance of RContainer without to know if it is
* responsible for the deallocation of the objects or if it is ordered.
* @tparam C                  The class of the elements to be contained.
*
* Here is an example of a generic function:
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
* void Do(R::RIContainer<MyElement>& cont,double c)
* {
*    cont.DoSomething(c);
* }
*
* int main()
* {
*    R::RContainer<MyElement,true,true> c(20,10);
*    R::RContainer<MyElement,true,true> c2(20,10);
*    c.InsertPtr(new MyElement(5));
*    c.InsertPtr(new MyElement(2));
*    c2.InsertPtr(new MyElement(10));
*    Do(c,2);
*	  Do(c2,3);
* }
* @endcode
*
* @short Container Interface.
* @warning Some methods applied on an ordered container can disorder the
*          container.
*/
template<class C>
	class RIContainer
{
	/**
	 * Specify if the container deallocate its elements.
	 */
	bool Dealloc;

	/**
	 * Specify if the elements are ordered in the container.
	 */
	bool Order;

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
	* @param src             Pointer to the source container.
	*/
	void Create(const RIContainer<C>& src);

	/**
	* Copy a container from another one. If the pointer to the container is
	* null, the container is just emptied.
	* @param src             Pointer to the source container.
	*/
	RIContainer& NormalCopy(const RIContainer<C>& src);

	/**
	* Deep copy of a container in another one. If the pointer to the container is
	* null, the container is just emptied.
	* @param src             Pointer to the source container.
	*/
	void DeepCopy(const RIContainer<C>& src);

	/**
	* Add a container (if the pointer is not null) from another one.
	* @param src             Pointer to the source container.
	*/
	RIContainer& Add(const RIContainer<C>& src);

private:

	/**
	* Constructor of the container interface.
	*
	* @tparam dealloc        Specify if the elements are deallocated by the
	*                        container.
	* @tparam order          Specify if the elements are ordered in the
	*                        container.
	* @param m               The initial maximal size of the array.
	* @param i               The value used when increasing the array. If null
	*                        value, the size is set to the half the maximal
	*                        size and at least at 10.
	*/
	RIContainer(bool dealloc,bool order,size_t m,size_t i);

public:

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
	* @warning If applied on an ordered container, this method can disorder it.
	*/
	void Exchange(size_t pos1,size_t pos2);

	/**
	 * Shift the elements in the container. This is done by choosing an element
	 * that will become the first one. Depending from the element chosen, the
	 * elements seems to be shifted to the left or to the right:
	 * - Choosing 2 means that 2→0, 3→1, 4→2, LastPos→LastPos-2,
	 *   0→LastPos-1, etc. It simulates a shift to the left.
	 * - Choosing LastPos means LastPos→0, 0→1, 2→1, etc. It simulates a shift to
	 *   the right.
	 *
	 * While shifted, the elements may also be reverse. Let us suppose a
	 * container with the elements (0,1,2,3,4,5), and that the element 2 becomes
	 * the first one:
	 * - If no reverse, the result is (2,3,4,5,0,1).
	 * - If reverse, the result is (2,1,0,5,4).
	 *
	 * The method does not verify if the shift is compatible with the ordering
	 * of the container.
    * @param idx            Index of the element in the container that must
	 *                       become the first one.
    * @param reverse        Define if the elements are reversed (the element
	 *                       idx-1 becomes the second one) or not (the element
	 *                       idx+1 becomes the second one).
	 * @remark If the shift is not reversed, a temporary container is created
	 * (with a maximal size of NbPtr/2). It makes the shift fast since the method
	 * uses the memcpy and memmove functions. If the shift is reserved, the
	 * method makes NbPtr/2 exchanges.
	 * @warning If applied on an ordered container, this method can disorder it.
    */
	void Shift(size_t idx,bool reverse);

	/**
	 * Transfer a container into another one. The destination container is
	 * cleared before. If the source container isn't ordered and the destination
	 * is, ReOrder is called.
	 * @param src            Source container.
	 */
	void Transfer(RIContainer<C>& src);

	/**
	* The assignment operator.
	* @param src             Container used as source.
	* \warning If the container is not a responsible for the allocation, the
	* elements of src are just copied and not re-created. Use Copy if you want a
	* "deep" copy of src.
	*/
	inline RIContainer& operator=(const RIContainer<C>& src) {return(NormalCopy(src));}

	/**
	* Deep copy of a container.
	* @tparam a              Determine if the source container is responsible for the allocation.
	* @param src             Container used as source.
	*/
	inline void Copy(const RIContainer<C>& src) {DeepCopy(src);}

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	inline RIContainer& operator+=(const RIContainer<C>& src) {return(Add(src));}

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
		return(GetIndex(Order,tag,find,min,max));
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
	template<class TUse> inline bool IsIn(const TUse& tag,bool sortkey,size_t min=0,size_t max=0) const
	{
		bool Find;
		GetIndex(Order&&sortkey,tag,Find,min,max);
		return(Find);
	}

	/**
	* Look if a certain element is in the container. The search can be limited
	* to a part of the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return Return true if the element is in the container.
	*/
	template<class TUse> inline bool IsIn(const TUse& tag,size_t min=0,size_t max=0) const
	{
		bool Find;
		GetIndex(Order,tag,Find,min,max);
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
	* @warning If applied on an ordered container, this method can disorder it.
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
	template<class TUse> C* GetPtr(const TUse& tag,bool sortkey,size_t min=0,size_t max=0) const;

	/**
	* Get a pointer to a certain element in the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> inline C* GetPtr(const TUse& tag,size_t min=0,size_t max=0) const
	{
		return(GetPtr(tag,Order,min,max));
	}

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
	template<class TUse> C* GetInsertPtr(const TUse& tag,bool sortkey,size_t min=0,size_t max=0);

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> inline C* GetInsertPtr(const TUse& tag,size_t min=0,size_t max=0)
	{
		return(GetInsertPtr(tag,Order,min,max));
	}

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param pos             The position where to insert it.
	* @return The function returns a pointer to the element of the container.
	* @warning If applied on an ordered container, this method can disorder it.
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
	* @warning If applied on an ordered container, this method can disorder it.
	*/
	void InsertPtrAt(C* ins,size_t pos,bool del);

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
	* @warning If applied on an ordered container, this method can disorder it.
	*/
	inline void InsertPtrAt(C* ins,size_t pos)
	{
		InsertPtrAt(ins,pos,Dealloc);
	}

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
	* @param del             Specify if the object must deleted or not.
	* @warning If applied on an ordered container, this method can disorder it.
	*/
	void DeletePtrAt(size_t pos,bool shift,bool del);

	/**
	* Delete an element from the container at a given position. The elements are
	* deallocated if the container is responsible for that.
	* @param pos             Position of the element.
	* @param shift           Specify if the the container should be shifted or
	*                        if the position should be left empty.
	* @warning If applied on an ordered container, this method can disorder it.
	*/
	inline void DeletePtrAt(size_t pos,bool shift)
	{
		DeletePtrAt(pos,shift,Dealloc);
	}

	/**
	* Delete an element from the container at a given position. The elements are
	* shifted and deallocated if the container is responsible for that.
	* @param pos             Position of the element.
	* @warning If applied on an ordered container, this method can disorder it.
	*/
	inline void DeletePtrAt(size_t pos)
	{
		DeletePtrAt(pos,true,Dealloc);
	}

	/**
	* Delete an element from the container. This method can be limited to a
	* part of the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @param del             Specify if the object must deleted or not.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	*/
	template<class TUse> void DeletePtr(const TUse& tag,bool sortkey,bool del,size_t min=0,size_t max=0);

	/**
	* Delete an element from the container. This method can be limited to a
	* part of the container. The elements are deallocated if the container is
	* responsible for that.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	*/
	template<class TUse> inline void DeletePtr(const TUse& tag,bool sortkey,size_t min=0,size_t max=0)
	{
		DeletePtr(tag,sortkey,Dealloc,min,max);
	}

	/**
	* Delete an element from the container. This method can be limited to a
	* part of the container. If the elements of the container are ordered, the
	* it is supposed that the tag is a sorting key of it. The elements are
	* deallocated if the container is responsible for that.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param min             Starting index of the container's part concerned.
	* @param max             Ending index of the container's part concerned.
	*/
	template<class TUse> inline void DeletePtr(const TUse& tag,size_t min=0,size_t max=0)
	{
		 DeletePtr(tag,Order,Dealloc,min,max);
	}

	/**
	* Simple destructor.
	*/
	virtual ~RIContainer(void);

	friend class RContainer<C,true,true>;
	friend class RContainer<C,true,false>;
	friend class RContainer<C,false,true>;
	friend class RContainer<C,false,false>;
	friend class RCursor<C>;
	friend class RConstCursor<C>;
	friend class RQuickSort<C>;
};


//-----------------------------------------------------------------------------
// Template implementation
#include <ricontainer.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
