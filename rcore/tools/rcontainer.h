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
#include <ricontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This class represent a container of elements (class C). It inherits all the
* methods provided by the interface defined by RIContainer. Its elements are
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
* @author Pascal Francq
* @short Container.
*/
template<class C,bool bAlloc,bool bOrder=false>
	class RContainer : public RIContainer<C>
{
protected:

	using RIContainer<C>::NbPtr;
	using RIContainer<C>::LastPtr;
	using RIContainer<C>::MaxPtr;
	using RIContainer<C>::IncPtr;
	using RIContainer<C>::Tab;
	using RIContainer<C>::SortOrder;

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
	 * Compare method that can be used to construct an unordered container of
	 * containers.
    */
	int Compare(const RContainer<C,bAlloc,bOrder>&) const {return(-1);}

	/**
	 * Transfer a container into another one. The destination container is
	 * cleared before.
	 * @tparam b             Determine if the source container is responsible for the allocation.
	 * @tparam o             Determine if the source container is ordered.
	 * @param src            Source container.
	 */
	template<bool a,bool o> inline void Transfer(RContainer<C,a,o>& src) {RIContainer<C>::Transfer(src);}

	/**
	* The assignment operator.
	* @param src             Container used as source.
	* \warning If the container is not a responsible for the allocation, the
	* elements of src are just copied and not re-created. Use Copy if you want a
	* "deep" copy of src.
	*/
	inline RContainer& operator=(const RContainer<C,true,bOrder>& src) {RIContainer<C>::NormalCopy(src); return(*this);}

	/**
	* The assignment operator.
	* @param src             Container used as source.
	* \warning If the container is not a responsible for the allocation, the
	* elements of src are just copied and not re-created. Use Copy if you want a
	* "deep" copy of src.
	*/
	inline RContainer& operator=(const RContainer<C,false,bOrder>& src) {RIContainer<C>::NormalCopy(src); return(*this);}

	/**
	* Deep copy of a container.
	* @tparam a              Determine if the source container is responsible for the allocation.
	* @param src             Container used as source.
	*/
	template<bool a> inline void Copy(const RContainer<C,a,bOrder>& src) {RIContainer<C>::DeepCopy(src);}

	/**
	* Add the elements of a container. If the source container contains null
	* elements, these elements are not copied.
	* @param src             Container used as source.
	*/
	template<bool a,bool o> inline RContainer& operator+=(const RContainer<C,a,o>& src) {RIContainer<C>::Add(src); return(*this);}

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
