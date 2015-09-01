/*

	R Project Library

	RObjectHashContainer.h

	Object Hash Container - Header.

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
#ifndef RObjectHashContainer_H
#define RObjectHashContainer_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rhashcontainer.h>
#include <rstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * This class represent a container of objects (class C) managed through a hash
 * index. It is supposed that each object has an unique identifier (managed by
 * this container). The container maintains a list of objects ordered by their
 * identifier and one with a hash index (used for search purposes). The
 * container can be responsible for the deallocation of the elements (bAlloc).
 * @tparam C                  The class of the elements to be contained.
 * @tparam bAlloc             Specify if the elements are deallocated by the
 *                            container.
 *
 * To manage the objects, the container uses methods of the class
 * representing them (class C). These methods have the signatures:
 * @code
 * int Search(const C& c) const;
 * int Search(const TUse& tag) const;
 * size_t HashCode(size_t max) const;
 * size_t GetId(void) const;
 * size_t SetId(size_t id);
 * @endcode
 * In practice, the Search method can be different than the Compare method of
 * the element (class C). While it supposes the allocation of reference pointer
 * for each element inserted, it has the advantage to allow "normal" containers
 * to use another method for sorting elements (typically using identifiers for
 * "normal" containers and a string for this container).
 *
 * @short Object Container.
 */
template<class C,bool bAlloc>
	class RObjectHashContainer
{
protected:

	class ObjectRef;

	/**
	 * The container of objects ordered by identifier.
	 */
	RContainer<C,bAlloc,false> Objects;

	/**
	 * The objects managed through the search criteria.
	 */
	RHashContainer<ObjectRef,false> ObjectsByRef;

	/**
	 * First identifier of the objects.
	 */
	size_t FirstId;

public:

	/**
	* Constructor of a container of objects.
	* @param first           First identifier assigned to an object.
	* @pâram nb              Initial size of the container.
	* @pâram nbi             Incremental size of the container.
	* @param s               Size of the hash index. The hash codes of the
	*                        elements must be in the range [0,s-1].
	* @param m               The initial maximal size of the array for an index.
	* @param i               The value used when increasing the array for an
	*                        index. If null value, the size is set to the half
	*                        of the maximal size.
	 */
	RObjectHashContainer(size_t first,size_t nb,size_t nbi,size_t s,size_t m,size_t i=0);

	/**
	* Verify that a given set of containers can hold a given size of objects.
	* @param max             Maximum number of objects.
   */
	void VerifyTab(size_t max);

	/**
	* Clear the container and destruct the elements if it is responsible for
	* the deallocation. The container can also be extended.
	* @param m               New maximal size of the array. If null, the old
	*                        size remains.
	* @param i               New increasing value. If null, the old value
	*                        remains.
	*/
	void Clear(size_t m=0,size_t i=0);

	/**
	 * Compare method that can be used to construct an unordered container of
	 * containers.
    */
	int Compare(const RObjectHashContainer<C,bAlloc>&) const {return(-1);}

	/**
	 * Get the number of the objects.
	 * @return Maximum position occupied by the elements.
	 */
	inline size_t GetNbObjs(void) const {return(Objects.GetNb());}

	/**
	 * Get the number of the objects sharing a given hash code.
	 * @param hash           Hash code.
	 * @return Maximum position occupied by the elements.
	 */
	inline size_t GetNbObjsHashCode(size_t hash) const {return(ObjectsByRef[hash]->GetNb());}

	/**
	 * Get the highest identifier for the objects.
	 * @return Highest identifier.
	 */
	inline size_t GetMaxObjId(void) const
	{
		if(!Objects.GetNb())
			return(0);
		return(Objects[Objects.GetMaxPos()]->GetId());
	}

	/**
	* This function returns the index of an element represented by tag in a
	* particular array of a hash code.
	* @tparam TUse           The type of tag, the container uses the Compare(TUse &)
	*                        member function of the elements.
	* @param tag             The tag used.
	* @param hash            Hash code of the tag (set by the method)
	* @param find            If the element represented by tag exist, find is set to
	*                        true (set by the method).
	* @return Returns the index of the element if it exists or the index where
	* is has to inserted.
	*/
	template<class TUse> inline size_t GetIndex(const TUse& tag,size_t& hash,bool& find) const;

	/**
	 * Get the maximum position of the objects.
	 * @return Maximum position occupied by the elements.
	 */
	inline size_t GetMaxObjPos(void) const {return(Objects.GetMaxPos());}

	/**
	 * This method get a cursor over the objects.
    * @param min            Minimum position of the elements to iterate.
	 * @param max            Maximum position of the elements to iterate (included max).
	 *                       If SZE_MAX, iterate until the end of the container.
	 * @return a cursor over the objects.
	 */
	R::RCursor<C> GetObjs(size_t min=0,size_t max=SIZE_MAX) const {return(R::RCursor<C>(Objects,min,max));}

	/**
	 * Fill an array of pointers with the objects.
	 * @param tab            Array that will be filled.
	 * @param alloc          Must the array be allocated.
	 * @param min             Starting index of the container's part concerned.
	 * @param max             Ending index of the container's part concerned.
	 * @return number of objects in the array (including null pointers).
	 */
	size_t GetObjs(C** &tab,bool alloc,size_t min=0,size_t max=0);

	/**
	* Get the object with a given identifier (Only read). The
	* operator generates an exception is the identifier is out of range.
	* @param id              Identifier of the object.
	* @return a pointer.
	*/
	const C* operator[](size_t id) const;

	/**
	* Get the object with a given identifier (Read/Write). The
	* operator generates an exception is the identifier is out of range.
	* @param id              Identifier of the object.
	* @return a pointer.
	* @warning If the identifier of the object is modified, this method can
	* disorder the container.
	*/
	C* operator[](size_t id);

	/**
	 * Get a object corresponding to a search criteria.
	 * @tparam TUse          The type of tag, the container uses the Compare(TUse &)
	 *                       member function of the elements.
	 * @param tag            The tag used.
	 * @param sortkey        The tag represents the sorting key. The default value
	 *                       depends if the container is ordered (true) or not
	 *                       (false).
	 * @return a pointer to a object.
	 */
	template<class TUse> C* GetObj(const TUse& tag,bool sortkey=true);

	/**
	* Insert a object.
	* @param obj             Object.
	* @return true if the container has assigned an identifier to the object.
	*/
	bool InsertObj(C* obj);

	/**
	* Insert an element in the container at a given index of a given hash code.
	* If the corresponding index is already used, the previously inserted element
	* is removed from the container (and destroy if the container is
	* responsible for the allocation).
	*
	* This method should be used very carefully (in general with the GetIndex
	* method) because the container can then disfunction.
	* @param obj             Object to insert.
	* @param hash            Hash code of the object to insert.
	* @param index           Index of the object to insert.
	* @param del             Specify if a similar existing element must be
	*                        deleted or shifted.
	* @return true if the container has assigned an identifier to the object.
	*/
	bool InsertObjAt(C* obj,size_t hash,size_t idx,bool del=false);

	/**
	* Delete a object.
	* @param obj             Object.
	*/
	void DeleteObj(C* obj);

	/**
	* Assign an identifier to a new object.
	* @param obj             Object.
	*/
	void AssignId(C* obj);

	/**
	 * Destructor.
	 */
	virtual ~RObjectHashContainer(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <robjecthashcontainer.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
