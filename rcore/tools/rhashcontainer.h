/*

	Rainbow Library Project

	RHashContainer.h

	Hash Table Container - Header

	Copyright 2000-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RHashContainerH
#define RHashContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rcursor.h>
#include <rconstcursor.h>

//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RHashContainer provides a class to manage a container of elements (class
* C) with a hash table of a given fixed size. Each element must have a
* key (such an unique name), but the hash code must not be unique. The container
* can be responsible for the deallocation of the elements (bAlloc).
*
* In practice, a table of the given size is allocated, each row representing a
* possible hash code. For each hash code, a container is maintained to manage
* elements sharing the same hash code.
*
* @tparam C                 The class of the element to be contained.
* @tparam bAlloc            Specify if the elements are deallocated by the
*                           container.
*
* To make the necessary comparisons, the container uses methods of the class
* representing the elements (class C). These methods have the signature:
* @code
* int Compare(const TUse& tag) const;
* size_t HashCode(size_t max) const;
* @endcode
* Look at R::RString and R::RCString for example of implementations.
*
* The TUse represent a class or a structure used for the comparisons. The
* Compare method are working like the strcmp function from the standard C/C++
* library. The result returned specifies if the tag precedes (>0), is the
* same (0) or is after (<0) the element used. The HashCode methods return the
* hash index of the given argument.
*
* Here is an example of a container with a hash table managing instances of the
* class MyElement:
* @code
* #include <string.h>
* #include <rhashcontainer.h>
* using namespace R;
*
*
* class MyElement
* {
*    RString Text;
*    char* AdditionalData;
* public:
*    MyElement(RString text) : Text(text) {}
*    int Compare(const MyElement& e) const {return(Text.Compare(e.Text));}
*    size_t HashCode(size_t max) const {return(Text.HashCode(max));}
*    void DoSomething(void) {cout<<Text<<endl;}
*    ~MyElement(void) {}
* };
*
*
* int main()
* {
*    RHashContainer<MyElement,true> c(1000,20,10);
*
*    c.InsertPtr(new MyElement("Hello World"));
*    if(c.IsIn("Hello World"))
*       cout<<"An element of value \"Hello World\" is in the container"<<endl;
*    c.InsertPtr(new MyElement("Other"));
*
*    // Parse the hash table
*    RCursor<RContainer<MyElement,true,true> > Cur(c.GetCursor());
*    for(Cur.Start();!Cur.End();Cur.Next())
*    {
*       RCursor<MyElement> Cur2(*Cur());
*       for(Cur2.Start();!Cur2.End();Cur2.Next())
*          Cur2()->DoSomething();
*    }
* }
* @endcode
*
* @short Hash Table Container.
*/
template<class C,bool bAlloc>
	class RHashContainer
{
	typedef RContainer<C,bAlloc,true> cHash;

	/**
	* This container represents the hash table of the elements.
	*/
	RContainer<cHash,true,false> Hash;

public:

	/**
	* Constructor of the container. In practice, the sizes should be used
	* with caution. If the size of the hash table is small in comparison with the
	* number of elements, the initial sizes of the individual array should be
	* larger.
	* @param s               Size of the hash table. The hash codes of the
	*                        elements must be in the range [0,s-1].
	* @param m               Initial size of the array for a given hash code.
	* @param i               Incremental size of the array for a given hash code.
	*                        If null value, the size is set to the half of the
	*                        initial size.
	*/
	RHashContainer(size_t s,size_t m,size_t i=0)
		: Hash(s)
	{
		for(size_t i=0;i<s;i++)
			Hash.InsertPtr(new cHash(m,i));
	}

	/**
	* Get the number of elements in the container.
	*/
	inline size_t GetNb(void) const
	{
		RCursor<cHash> Cur(Hash);
		size_t nb(0);
		for(Cur.Start();!Cur.End();Cur.Next())
			nb+=Cur()->GetNb();
		return(nb);
	}

	/**
	 * Get a cursor over the hash table.
	 * @return a R::RCursor.
	 */
	inline RCursor<cHash> GetCursor(void) const
	{
		return(RCursor<cHash>(Hash));
	}

	/**
	* Get a pointer to the elements of a given hash code (Only read).
	* @param hash            Hash code of the elements to get.
	* @return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	const cHash* operator[](size_t hash) const
	{
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::operator[] const: Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		return(Hash[hash]);
	}

	/**
	* Get a pointer to the elements of a given hash code (Read/Write).
	* @param hash            Hash code of the element to get.
	* @return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	cHash* operator[](size_t hash)
	{
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::operator[]: Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		return(Hash[hash]);
	}

	/**
	* Clear the container.
	*/
	void Clear(void)
	{
		RCursor<cHash> Cur(Hash);
		for(Cur.Start();!Cur.End();Cur.Next())
			Cur()->Clear();
	}

	/**
	* This function returns the index of an element represented by tag in an
	* array corresponding to a particular hash code.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param hash            Hash code of the tag (set by the method)
	* @param find            If the element represented by tag exist, find is set to
	*                        true (set by the method).
	* @return index of the element if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> inline size_t GetIndex(const TUse& tag,size_t& hash,bool& find) const
	{
		hash=tag.HashCode(Hash.GetNb());
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::GetIndex(const TUse&,size_t&,bool&) const: Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		return(Hash[hash]->GetIndex(tag,find));
	}

	/**
	* Look if a certain element is in the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return true if the element is in the container.
	*/
	template<class TUse> inline bool IsIn(const TUse& tag,bool sortkey=true) const
	{
		size_t hash(tag.HashCode(Hash.GetNb()));
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::IsIn(const TUse&,bool): hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		return(Hash[hash]->IsIn(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return the pointer or 0 if the element is not in the container.
	*/
	template<class TUse> inline C* GetPtr(const TUse& tag,bool sortkey=true) const
	{
		size_t hash(tag.HashCode(Hash.GetNb()));
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::GetPtr(const TUse& tag,bool): Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		return(Hash[hash]->GetPtr(tag,sortkey));
	}

	/**
	* Get a pointer to a certain element in the container. If the element doesn't
	* exist, the container creates it by using the constructor with TUse
	* as parameter.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @return a pointer to the element of the container.
	*/
	template<class TUse> inline C* GetInsertPtr(const TUse& tag,bool sortkey=true)
	{
		size_t hash(tag.HashCode(Hash.GetNb()));
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::GetInsertPtr(const TUse&,bool): Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		return(Hash[hash]->GetInsertPtr(tag,sortkey));
	}

	/**
	* Insert an element in the container. If the corresponding index is already
	* used, the previously inserted element is removed from the container (and
	* destroy if the container is responsible for the allocation).
	* @param ins             A pointer to the element to insert.
	* @param del             Specify if a similar existing element must be
	*                        deleted or shifted.
	*/
	inline void InsertPtr(C* ins,bool del=false)
	{
		mReturnIfFail(ins);
		size_t hash(ins->HashCode(Hash.GetNb()));
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::InsertPtr(C*,bool): Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		Hash[hash]->InsertPtr(ins,del);
	}

	/**
	* Insert an element in the container at a given index of a given hash code.
	* If the corresponding index is already used, the previously inserted element
	* is removed from the container (and destroy if the container is
	* responsible for the allocation).
	*
	* This method should be used very carefully (in general with the GetIndex
	* method) because the container can then disfunction.
	* @param ins             A pointer to the element to insert.
	* @param hash            Hash code of the element to insert.
	* @param index           Index of the element to insert.
	* @param del             Specify if a similar existing element must be
	*                        deleted or shifted.
	*/
	inline void InsertPtrAt(C* ins,size_t hash,size_t idx,bool del=false)
	{
		mReturnIfFail(ins);
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::InsertPtr(C*,bool): Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		Hash[hash]->InsertPtrAt(ins,idx,del);
	}

	/**
	* Delete an element from the container.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param sortkey         The tag represents the sorting key. The default value
	*                        depends if the container is ordered (true) or not
	*                        (false).
	* @param del             Specify if the object must deleted or not. By
	*                        default, the element is destruct if the container
	*                        is responsible of the deallocation.
	*/
	template<class TUse> inline void DeletePtr(const TUse& tag,bool sortkey=true,bool del=bAlloc)
	{
		size_t hash(tag.HashCode(Hash.GetNb()));
		if(hash>=Hash.GetNb())
			throw std::range_error(R::RString("RHashContainer<C,bAlloc>::DeletePtr(const TUse&,bool,bool): Hash code "+RString::Number(hash)+" outside range [0,"+RString::Number(Hash.GetNb()-1)+"]").ToString());
		Hash[hash]->DeletePtr(tag,sortkey,del);
	}

	/**
	* Destruct the hash container.
	*/
	virtual ~RHashContainer(void) {}
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
