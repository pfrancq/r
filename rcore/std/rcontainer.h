/*

	R Project Library

	RContainer.h

	Container - Header.

	(C) 1999-2001 by P. Francq.

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



//---------------------------------------------------------------------------
#ifndef RContainerH
#define RContainerH


//---------------------------------------------------------------------------
// include files for R Project
#include <new.h>
#include <rstd/rstd.h>
using namespace RStd;


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/**
* @param C                  The class of the elements to be contained.
* @param T                  The type of the iterator used.
* @param bAlloc             Specify if the elements are desallocated by the
*                           container.
* @param bOrder             Specify if the elements are ordered in the
*                           container.
* This class represent a container of elements (class C). This elements are
* store in an array of pointers which will be increase when necessary. The
* container can be responsible for the desallocation of the elements
* (bAlloc), and the elements can be ordered (bOrder). It is also possible to
* indicate an iterator (class T) for the container.
*
* To make the necessary comparaisons, the container used member functions of
* the class representing the elements (class C). These functions have the
* signature:
* <pre>
* int Compare(const TUse &tag);
* int Compare(const TUse *tag);
* </pre>
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
* <pre>
* int Compare(C*);
* </pre>
*
* The functions Start, End, Next and the operator () are used to go through
* the container.
*
* Here is an example of class MyElement that will be contained in the
* variable c:
* <pre>
* #include<rstd/rcontainer.h>
* #include<rstd/rcontainercursor.h>
* using namespace RStd;
*
*
* class MyElement
* {
* 	unsigned int Id;
* public:
* 	MyElement(unsigned int id) : Id(id) {}
* 	MyElement(MyElement *e) : Id(e->Id) {}
* 	void DoSomething(double d);
* 	int Compare(MyElement *e) {return(Id-e->Id);}
* 	int Compare(unsigned int id) {return(Id-id);}
* 	int Compare(char *text)
* 	{
* 		unsigned int id=atoi(text);
* 		return(Compare(id));
* 	}
* };
*
*
* int main()
* {
* 	RContainer<MyElement,unsigned int,true,true> c(20,10);
* 	RContainerCursor<MyElement,unsigned int,true,true> cur(c);
*
* 	c.InsertPtr(new MyElement(5));
* 	if(c.IsIn<char*>("5"))
* 		cout<<"An element of value 5 is in the container"<<endl;
* 	c.InsertPtr(new MyElement(10));
* 	for(cur.Start();!cur.End();cur.Next())
* 		cur()->DoSomething(2.3);
* }
* </pre>
*
* @author Pascal Francq
* @short Container template.
*/
template<class C,class T,bool bAlloc,bool bOrder=false>
	class RContainer
{
	/**
	* This variable is used to go through the container.
	*/
	C **Current;

	/**
	* This variable is used to see if the end of the container is reached.
	*/
	T ActPtr;

public:

	/**
	* The array of pointers for the elements.
	*/
	C **Tab;

	/**
	* The number of elements in the container.
	*/
	T NbPtr;

	/**
	* The maximal number of elements that can be hold by the container
	* actually.*/
	T MaxPtr;

	/**
	* The last position in the array used by an object.
	*/
	T LastPtr;

	/**
	* When the array is increased, this value is used.
	*/
	T IncPtr;

public:

	/**
	* @name Constructors.
	*/
	//@{

	/**
	* Construct the container.
	* @param M              The initial maximal size of the array.
	* @param I              The value used when increasing the array.
	*/
	RContainer(T M,T I=0) throw(bad_alloc);

	/**
	* Construct the container from another container.
	* @param src            Container used as source.
	*/
	RContainer(const RContainer<C,T,bAlloc,bOrder>* src) throw(bad_alloc);

	/**
	* Construct the container from another container.
	* @param src            Container used as source.
	*/
	RContainer(const RContainer<C,T,!bAlloc,bOrder>* src) throw(bad_alloc);

	/**
	* Construct the container from another container.
	* @param src            Container used as source.
	*/
	RContainer(const RContainer<C,T,bAlloc,bOrder>& src) throw(bad_alloc);

	/**
	* Construct the container from another container.
	* @param src            Container used as source.
	*/
	RContainer(const RContainer<C,T,!bAlloc,bOrder>& src) throw(bad_alloc);
	//@}

	/**
	* @name Assignement operators.
	*/
	//@{

	/**
	* The assignement operator.
	* @param src            Container used as source.
	*/
	RContainer& operator=(const RContainer<C,T,bAlloc,bOrder>& src) throw(bad_alloc);

	/**
	* The assignement operator.
	* @param src            Container used as source.
	*/
	RContainer& operator=(const RContainer<C,T,!bAlloc,bOrder>& src) throw(bad_alloc);

	/**
	* Add the elements of a container.
	* @param src            Container used as source.
	*/
	RContainer& operator+=(const RContainer<C,T,bAlloc,bOrder>& src) throw(bad_alloc);

	/**
	* Add the elements of a container.
	* @param src            Container used as source.
	*/
	RContainer& operator+=(const RContainer<C,T,!bAlloc,bOrder>& src) throw(bad_alloc);
	//@}

	/**
	* @name Size verification methods.
	*/
	//@{

	/**
	* Verify if the container can hold the next element to be inserted. If not,
	* the container is extended.
	*/
	void VerifyTab(void) throw(bad_alloc);

	/**
	* Verify if the container can hold a certain nujmber of elements. If not,
	* the container is extended.
    * @param MaxSize        The number of elements that must be contained.
	*/
	void VerifyTab(T MaxSize) throw(bad_alloc);
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
	* @param M              The initial maximal size of the array.
	* @param I              The value used when increasing the array.
	*/
	void Clear(T M,T I);
	//@}

protected:

	/**
	* @name Accessing elements.
	*/
	//@{

	/**
	* Insert an element in an ordered container. Used by InsertPtr and
	* GetInsertPtr.
	* @param ins            A pointer to the element to insert.
	* @param Pos            The position where to insert it.
	*/
	void InsertPtrOrderAt(const C *ins,T Pos) throw(bad_alloc);
	//@}

public:

	/**
	* @name Accessing elements.
	*/

	//@{
	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	* @param Find           If the element represented by tag exist, bFind is set to
	*                       true.
	* @return Returns the index of the element if it exists orthe index where
	* is has to inserted.
	*/
	template<class TUse> T GetId(const TUse &tag,bool &Find) const;

	/**
	* Insert an element at a certain position. Two remarks must be made :
	* -#    The function verify not if the index used is compatible with the order
	*       in case of the elements are treated in ascending order.
	* -#    By using this function, the user can leave "blanks" in the container,
	*       in other words, some "valid" pointers could be null. This situation is
	*       not handle by the other functions of the container.
	* @param ins            A pointer to the element to insert.
	* @param Pos            The position where to insert it.
	* @param del            Specify if the object that was previously at Pos
	*                       must be shift or deleted.
	*/
	void InsertPtrAt(const C *ins,T Pos,bool del=true) throw(bad_alloc);

	/**
	* Insert an element in the container.
	* @param ins            A pointer to the element to insert.
	*/
	void InsertPtr(const C* ins) throw(bad_alloc);

	/**
	* Look if a certain element is in the container.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @return Return true if the element is in the container.
	*/
	template<class TUse> bool IsIn(const TUse &tag,bool sortkey=bOrder) const;

	/**
	* Get a pointer to a certain element in the container.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> C* GetPtr(const TUse &tag,bool sortkey=bOrder) const;

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> C* GetInsertPtr(const TUse &tag,bool sortkey=bOrder) throw(bad_alloc);

	/**
	* This function returns a container of all the elements that are responding
	* to the given criteria.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	* @return The function returns a pointer to the result container.
	*/
	template<class TUse> RContainer<C,T,false,bOrder>* GetPtrs(const TUse &tag) const throw(bad_alloc);
	//@}

	/**
	* @name Deleting elements.
	*/
	//@{

	/**
	* Delete an element from the container. The element is destruct if the
	* container is responsible of the desallocation.
	* @param del            A pointer to the element to delete.
	*/
	void DeletePtr(C*& del) throw(bad_alloc);

	/**
	* Delete an element from the container. The element is destruct if the
	* container is responsible of the desallocation.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	*/
	template<class TUse> void DeletePtr(const TUse &tag,bool sortkey=bOrder) throw(bad_alloc);
	//@}

	/**
	* @name Iterator functions.
	* \deprecated
	* Use the RContainerCursor class instead of this methods.
	*/
	//@{

	/**
	* Start the iterator to go trough the container.
	* \deprecated
	* Use the RContainerCursor class instead of this methods.
	*/
	inline void Start(void);

	/**
	* Test if the end of the container is reached.
	* \deprecated
	* Use the RContainerCursor class instead of this methods.
	*/
	inline bool End(void) const;

	/**
	* Goto the next element, if the end is reached, go to the beginning.
	* \deprecated
	* Use the RContainerCursor class instead of this methods.
	*/
	inline void Next(void);

	/**
	* Return the current element.
	* \deprecated
	* Use the RContainerCursor class instead of this methods.
	*/
	inline C* operator()(void) const;
	//@}

	/**
	* @name Destructor.
	*/
	//@{
	/**
	* Destructs the container.
	*/
	virtual ~RContainer(void);
	//@}
};


//---------------------------------------------------------------------------
// inline implementation
#include "rcontainer.hh"


}  //-------- End of namespace RStd -----------------------------------------


//---------------------------------------------------------------------------
#endif


