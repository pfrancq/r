/*

	Rainbow Library Project

	RHashContainer.h

	Container with an hash table

	(C) 2000 by P. Francq.

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

/*

  template <class C,class T,T tSize,bool bAlloc> class RHashContainer
    Class C  :  class representing a member (Ex.: string)
    Class T  :  class representing an iterator (Ex.: int)
    tSize		 :	size of the hash table
    bAlloc   :  If true, the container is responsible for the
                desallocation of his member
	

	
  Class C:
    The Compare(TUse tag) function returns an int:
        C==tag : 0
        C<tag  : <0
        C>tag  : >0
    The class C must declare at least one function with the signature
    Compare(C*).
		The HashIndex(void) function returns the hash index of the element
		The HashIndex of TUse function returns the hash index of the tag

*/



//-----------------------------------------------------------------------------
#ifndef RHashContainerH
#define RHashContainerH


//-----------------------------------------------------------------------------
#include "rcontainer.h"
using namespace RStd;


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


template<class C,class T,T tSize,bool bAlloc>
	class RHashContainer
{
public:
	/**
	* This container represents the hash table of the elements.
	*/
	RContainer<C,T,bAlloc,true> **Hash;

	/**
	* Construct a Hash container.
	* @param M              Default maximum number of elements.
	* @param I              Incremental number of elements.
	*/
	RHashContainer(T M,T I) throw(bad_alloc)
	{
		RContainer<C,T,bAlloc,true> **ptr;
		T i;

		Hash= new RContainer<C,T,bAlloc,true>*[tSize];
		for(i=tSize+1,ptr=Hash;--i;ptr++)
			(*ptr)=new RContainer<C,T,bAlloc,true>(M,I);
	}

	/**
	* Clear the hash container.
	*/
	void Clear(void)
	{
		RContainer<C,T,bAlloc,true> **ptr;
		T i;

		for(i=tSize+1,ptr=Hash;--i;ptr++)	(*ptr)->Clear();
	}

	/**
	* Insert an element.
	* @param ins            Pointer to the element to insert.
	*/
	inline void InsertPtr(C *ins) throw(bad_alloc)
	{
		RReturnIfFail(ins);
		Hash[ins->HashIndex()]->InsertPtr(ins);
	}

	/**
	* Verify if an element is in the hash container.
	* @param TUse           The type of tag, the hash container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @returns The function returns true if the element is in the hash
	* container, else false.
	*/
	template<class TUse> inline bool IsIn(const TUse &tag,bool sortkey=true) const
	{
		return(Hash[tag.HashIndex()]->IsIn<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the hash container.
	* @param TUse           The type of tag, the hash container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default value
	*                       depends if the container is ordered (true) or not
	*                       (false).
	* @return Return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> inline C* GetPtr(const TUse &tag,bool sortkey=true) const
	{
		return(Hash[tag.HashIndex()]->GetPtr<TUse>(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container. If the element is
	* not existing, the container creates it by using the constructor with TUse
	* as parameter.
	* @param TUse           The type of tag, the container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default
	*                       value depends if the container is ordered (true) or
	*                       not (false).
	* @return The function returns a pointer to the element of the container.
	*/
	template<class TUse> inline C* GetInsertPtr(const TUse &tag,bool sortkey=true) throw(bad_alloc)
	{
		return(Hash[tag.HashIndex()]->GetInsertPtr<TUse>(tag,sortkey));
	}

	/**
	* Delete an element of the hash container.
	* @param del            Pointer to the element to delete.
	*/
	inline void DeletePtr(C* del)
	{
		RReturnIfFail(del);
		Hash[ins->HashIndex()]->DeletePtr(del);
	}

	/**
	* Delete an element of the hash container.
	* @param TUse           The type of tag, the container uses the
	*                       Compare(TUse &) member function of the elements.
	* @param tag            The tag used.
	* @param sortkey        The tag represents the sorting key. The default
	*                       value depends if the container is ordered (true) or
	*                       not (false).
	*/
	template<class TUse> inline void DeletePtr(const TUse &tag,bool sortkey=true)
	{
		Hash[tag.HashIndex()]->DeletePtr<TUse>(tag,sortkey);
	}

	/**
	* Destruct the hash container.
	*/
	virtual ~RHashContainer(void)
	{
		RContainer<C,T,bAlloc,true> **ptr;
		T i;

		for(i=tSize+1,ptr=Hash;--i;ptr++)	delete (*ptr);
		delete[] Hash;
	}
};


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
#endif
