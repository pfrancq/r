/*

	R Project Library

	RContainer.h

	Container - Header.

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



//------------------------------------------------------------------------------
#ifndef RContainerH
#define RContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#undef PRIVATE

//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration of the cursor template
template<class C> class RCursor;


//------------------------------------------------------------------------------
/**
* @param C                  The class of the elements to be contained.
* @param bAlloc             Specify if the elements are desallocated by the
*                           container.
* @param bOrder             Specify if the elements are ordered in the
*                           container.
* This class represent a container of elements (class C). This elements are
* store in an array of pointers which will be increase when necessary. The
* container can be responsible for the desallocation of the elements
* (bAlloc), and the elements can be ordered (bOrder).
*
* To make the necessary comparaisons, the container used member functions of
* the class representing the elements (class C). These functions have the
* signature:
* @code
* int Compare(const TUse& tag) const;
* int Compare(const TUse* tag) const;
* @endcode
*
* The TUse represent a class or a structure used for the comparaisons. These
* functions are working like the strcmp function from the standard C/C++
* library. The result returned specifies if the tag preceeds (>0), is the
* same (0) or is after (<0) the element used. The structure used for the
* comparaisons can represent or not the sort key used when the container must
* be ordered. The user must specify if the key used for a search is the
* sorting one or not.
*
* At least, a compare function must be implemented in the class C:
* @code
* int Compare(const C*) const;
* @endcode
*
* To iterate through the container, a RCursor can be used. Here is an example
* of class MyElement that will be contained in the variable c:
* @code
* class MyElement
* {
* 	unsigned int Id;
* public:
* 	MyElement(unsigned int id) : Id(id) {}
* 	MyElement(MyElement* e) : Id(e->Id) {}
* 	void DoSomething(double d) {cout<<d<<endl;}
* 	int Compare(const MyElement* e) const {return(Id-e->Id);}
* 	int Compare(const unsigned int id) const {return(Id-id);}
* 	int Compare(const char* text) const
* 	{
* 		unsigned int id=atoi(text);
* 		return(Compare(id));
* 	}
* };
*
*
* int main()
* {
* 	R::RContainer<MyElement,true,true> c(20,10);
*
* 	c.InsertPtr(new MyElement(5));
* 	if(c.IsIn<char*>("5"))
* 		cout<<"An element of value 5 is in the container"<<endl;
* 	c.InsertPtr(new MyElement(10));
* 	R::RCursor<MyElement> cur(c);
* 	for(cur.Start();!cur.End();cur.Next())
* 		cur()->DoSomething(2.3);
* }
* @endcode
*
* @author Pascal Francq
* @short Container template.
*/
template<class C,bool bAlloc,bool bOrder=false>
	class RContainer
{
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

public:

	/**
	* @name Constructors.
	*/
	//@{

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

	//@}

	/**
	* @name Assignement operators.
	*/
	//@{

	/**
	* The assignement operator.
	* @param src             Container used as source.
	* \warning If the container is not a responsible for the allocation, the
	* elements of src are just copied and not re-created. Use Copy if you want a
	* "deep" copy of src.
	*/
	RContainer& operator=(const RContainer<C,true,bOrder>& src);

	/**
	* The assignement operator.
	* @param src             Container used as source.
	* \warning The elements of src are just copied and not re-created. Use
	* Copy if you want a "deep" copy of src.
	*/
	RContainer& operator=(const RContainer<C,false,bOrder>& src);

	/**
	* Deep copy of a container.
	* @param src             Container used as source.
	*/
	void Copy(const RContainer<C,false,bOrder>& src);

	/**
	* Deep copy of a container.
	* @param src             Container used as source.
	*/
	void Copy(const RContainer<C,true,bOrder>& src);

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	RContainer& operator+=(const RContainer<C,true,true>& src);

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	RContainer& operator+=(const RContainer<C,true,false>& src);

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	RContainer& operator+=(const RContainer<C,false,true>& src);

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	RContainer& operator+=(const RContainer<C,false,false>& src);
	//@}

	/**
	* @name Size verification methods.
	*/
	//@{

	/**
	* Verify if the container can hold the next element to be inserted. If not,
	* the container is extended.
	*/
	void VerifyTab(void);

	/**
	* Verify if the container can hold a certain nujmber of elements. If not,
	* the container is extended.
    * @param max             Maximal number of elements that must be contained.
	*/
	void VerifyTab(size_t max);
	//@}

	/**
	* @name Clear methods.
	*/
	//@{

	/**
	* Clear the container and destruct the elements if he is responsible for
	* the desallocation.
	*/
	void Clear(void);

	/**
	* Clear the container and destruct the elements if he is responsible for
	* the desallocation.
	* @param m               The initial maximal size of the array.
	* @param i               The value used when increasing the array.
	*/
	void Clear(size_t m,size_t i=0);
	//@}

	/**
	* @name Read methods.
	*/
	//@{

	/**
	* Get the number of elements in the container.
	* @return unsigned int.
	*/
	size_t GetNb(void) const {return(NbPtr);}

	/**
	* Get the maximal position occupied by an elements in the container.
	* @return unsigned int.
	*/
	size_t GetMaxPos(void) const {return(LastPtr);}

	/**
	* Get the maximum number of elements in the container.
	* @return unsigned int.
	*/
	size_t GetMaxNb(void) const {return(MaxPtr);}

	/**
	* Get the increment used to resize the container.
	* @return unsigned int.
	*/
	size_t GetIncNb(void) const {return(IncPtr);}

	/**
	* ReOrder the container. This method must be used with caution, because it
	* can crash the container:
	* -# The container contains null pointers.
	* -# The container is ordered and the method does not use the same criterion
	*    for the ordering.
	* @param sortOrder       Pointer to a (static) funtion used for the ordering.
	*/
	void ReOrder(int sortOrder(const void*,const void*));
	//@}

	/**
	* @name Insert methods.
	*/
	//@{

	/**
	* Insert an element in an ordered container. Used by InsertPtr and
	* GetInsertPtr.
	* @param ins             A pointer to the element to insert.
	* @param pos             The position where to insert it.
	*/
	void InsertPtrOrderAt(const C* ins,size_t pos);

	/**
	* Insert an element at a certain position. Two remarks must be made :
	* -#    The function verify not if the index used is compatible with the order
	*       in case of the elements are treated in ascending order.
	* -#    By using this function, the user can leave "blanks" in the container,
	*       in other words, some "valid" pointers could be null. This situation is
	*       not handle by the other functions of the container.
	* @param ins             A pointer to the element to insert.
	* @param pos             The position where to insert it.
	* @param del             Specify if the object that was previously at Pos
	*                        must be shift or deleted.
	*/
	void InsertPtrAt(const C* ins,size_t pos,bool del=bAlloc);

	/**
	* Insert an element in the container. If the container is ordered and if
	* the corresponding index is already used, the previously inserted element
	* is removed from the container (and destroy if the container is
	* responsible for the allocation)
	* @param ins             A pointer to the element to insert.
	*/
	void InsertPtr(const C* ins);

	/**
	* Exchange two elements in the container. The method does not verify if the
	* exchange is compatible with the ordering of the container.
	* @param pos1            Position of the first element.
	* @param pos2            Position of the second element.
	*/
	void Exchange(size_t pos1,size_t pos2);
	//@}

public:

	/**
	* @name Accessing elements.
	*/
	//@{

	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param find            If the element represented by tag exist, find is set to
	*                        true.
	* @return Returns the index of the element if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> size_t GetId(const TUse& tag,bool& find) const;

	/**
	* Look if a certain element is in the container.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return Return true if the element is in the container.
	*/
	template<class TUse> bool IsIn(const TUse& tag,bool sortkey=bOrder) const;

	/**
	* Get a pointer to the ith element in the container (Only read).
	* @param idx             Index of the element to get.
	* @return Return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	const C* operator[](size_t idx) const;

	/**
	* Get a pointer to the ith element in the container (Read/Write).
	* @param idx             Index of the element to get.
	* @return Return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	C* operator[](size_t idx);

	/**
	* Get a pointer to a certain element in the container.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> C* GetPtr(const TUse& tag,bool sortkey=bOrder) const;

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> C* GetInsertPtr(const TUse& tag,bool sortkey=bOrder);

	/**
	* This function returns a container of all the elements that are responding
	* to the given criteria.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @return The function returns a pointer to the result container.
	*/
	template<class TUse> RContainer<C,false,bOrder>* GetPtrs(const TUse& tag) const;

	/**
	* Copy the array of pointer into a temporary array. This array must have
	* the right size (Read only).
	* @param tab             Array of pointer.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	size_t GetTab(C** tab);

	/**
	* Copy the array of pointer into a temporary array. This array must have
	* the right size (Read only).
	* @param tab             Array of pointer.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	size_t GetTab(const C** tab) const;

	//@}

	/**
	* @name Deleting elements.
	*/
	//@{

	/**
	* Delete an element from the container. The element is destruct if the
	* container is responsible of the desallocation.
	* @param del             A pointer to the element to delete.
	*/
	void DeletePtr(C*& del);

	/**
	* Delete an element from the container. The element is destruct if the
	* container is responsible of the desallocation.
	* @param TUse            The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	*/
	template<class TUse> void DeletePtr(const TUse& tag,bool sortkey=bOrder);
	//@}

private:

	/**
	* @name Internal methods.
	*/
	//@{

	/**
	* Create a container from another one. If the pointer to the container is
	* null, an empty container is created.
	* @param src             Source container.
	*/
	template<bool b> void Create(const RContainer<C,b,bOrder>& src);

	/**
	* Copy a container from another one. If the pointer to the container is
	* null, the container is just emptied.
	* @param src             Source container.
	*/
	template<bool b> RContainer& NormalCopy(const RContainer<C,b,bOrder>& src);

	/**
	* Deep copy of a container in another one. If the pointer to the container is
	* null, the container is just emptied.
	* @param src             Source container.
	*/
	template<bool b> void DeepCopy(const RContainer<C,b,bOrder>& src);

	/**
	* Add a container (if the pointer is not null) from another one.
	* @param src             Source container.
	*/
	template<bool b,bool o> RContainer& Add(const RContainer<C,b,o>& src);

	//@}

public:

	/**
	* @name Destructor.
	*/
	//@{
	/**
	* Destructs the container.
	*/
	virtual ~RContainer(void);
	//@}

	friend class RCursor<C>;
	friend class RContainer<C,true,true>;
	friend class RContainer<C,true,false>;
	friend class RContainer<C,false,true>;
	friend class RContainer<C,false,false>;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rstd/rcontainer.hh>


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif


