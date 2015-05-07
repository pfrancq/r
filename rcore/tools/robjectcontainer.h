/*

	R Project Library

	RObjectContainer.h

	Object Container - Header.

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
#ifndef RObjContainer_H
#define RObjContainer_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rcontainer.h>
#include <rstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
 * This class represent a container of objects (class C). It is supposed that
 * each object has an unique identifier (managed by this container). The
 * container maintains a list of objects ordered by their identifier and one
 * ordered by a search criteria (used for search purposes). The container can
 * be responsible for the deallocation of the elements (bAlloc).
 * @tparam C                  The class of the elements to be contained.
 * @tparam bAlloc             Specify if the elements are deallocated by the
 *                            container.
 *
 * To manage the objects, the container uses member functions of the class
 * representing them (class C). These functions have the signatures:
 * @code
 * int Search(const TUse& tag) const;
 * int GetId(void) const;
 * int SetId(size_t id);
 * @endcode
 *
 *
 * @author Pascal Francq
 * @short Object Container.
 */
template<class C,bool bAlloc>
	class RObjectContainer
{
protected:

	class ObjectRef;

	/**
	 * The container of objects ordered by identifier.
	 */
	RContainer<C,bAlloc,false> Objects;

	/**
	 * The objects of the list ordered by the search criteria.
	 */
	R::RContainer<ObjectRef,true,true> ObjectsByRef;

	/**
	 * First identifier of the objects.
	 */
	size_t FirstId;

public:

	/**
	 * Constructor of a container of objects.
	 * @param first           First identifier assigned to an object.
	 * @param m               The initial maximal size of the array.
	 * @param i               The value used when increasing the array. If null
	 *                        value, the size is set to the half the maximal
	 *                        size or at least to 10.
	 */
	RObjectContainer(size_t first,size_t m,size_t i=0);

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
	int Compare(const RObjectContainer<C,bAlloc>&) const {return(-1);}

	/**
	 * Get the number of the objects.
	 * @return Maximum position occupied by the elements.
	 */
	inline size_t GetNbObjs(void) const {return(Objects.GetNb());}

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
	 * Get a object corresponding to a given identifier.
	 * @param id             Identifier of the object.
	 * @return a pointer to a object.
	 */
	C* GetObj(size_t id);

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
	virtual ~RObjectContainer(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <robjectcontainer.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
