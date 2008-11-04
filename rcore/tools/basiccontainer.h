/*

	R Project Library

	BasicContainer.h

	Basic Container - Header.

	Copyright 2005-2008 by the Université Libre de Bruxelles.

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



//-----------------------------------------------------------------------------
#ifndef BasicContainerH
#define BasicContainerH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration of the cursor template
class BasicCursor;


//-----------------------------------------------------------------------------
/**
 * This class represent a basic container. This class cannot be instantiated,
 * use R::RContainer instead.
 * @author Pascal Francq
 * @short Basic Container.
 */
class BasicContainer
{
protected:

	/**
	* The array of pointers for the elements.
	*/
	void** Tab;

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
	* Method to delete a given element of the container.
	*/
	virtual void Delete(void*)=0;

	/**
	* Copy constructor of a container.
	* @param src             Container used as source.
	*/
	BasicContainer(void);

	/**
	* Constructor of a container.
	*/
	explicit BasicContainer(size_t m,size_t i=0);

	/**
	* Verify if the container can hold a certain number of elements. If not,
	* the container is extended.
	*/
	void VerifyTab(size_t max);

	/**
	* Clear the container and destruct the elements if he is responsible for
	* the deallocation.
	*/
	void Clear(bool bAlloc,size_t m,size_t i);

	/**
	* ReOrder the container.
	*/
	void ReOrder(int sortOrder(const void*,const void*));

	/**
	* Exchange two elements in the container. The method does not verify if the
	* exchange is compatible with the ordering of the container.
	*/
	void Exchange(size_t pos1,size_t pos2);

	/**
	 * Transfer a container into another one. The destination container is
	 * cleared before.
	 */
	void Transfer(bool bAlloc,BasicContainer& src);

	/*
	* This function returns the index of an given element.
	*/
	size_t GetIndex(bool bOrder,const void* tag,bool& find,size_t min, size_t max,int compare(const void*,const void*)) const;

	/**
	* Verify that a element is in the container.
	*/
	bool IsIn(bool bOrder,const void* tag,bool sortkey,size_t min, size_t max,int compare(const void*,const void*)) const;

	/**
	* Get a pointer to the ith element in the container (Only read).
	*/
	const void* operator[](size_t idx) const;

	/**
	* Get a pointer to the ith element in the container (Read/Write).
	*/
	void* operator[](size_t idx);

	/**
	* Get a given element of the container.
	*/
	void* GetPtr(bool bOrder,const void* tag,bool sortkey,size_t min, size_t max,int compare(const void*,const void*)) const;

	/**
	* Copy the array of pointers into a temporary array. This array must have
	* the right size (Read only).
	*/
	size_t GetTab(const void** tab,size_t min, size_t max) const;

	/**
	* Copy the array of pointers into a temporary array. This array must have
	* the right size (Read/Write).
	*/
	size_t GetTab(void** tab,size_t min, size_t max);

	/**
	* Insert an element.
	*/
	void InsertPtr(bool bAlloc,bool bOrder,const void* ins,bool del,size_t min, size_t max,int compare(const void*,const void*));

	/**
	* Insert an element in at a given position.
	*/
	void InsertPtrAt(bool bAlloc,const void* ins,size_t pos,bool del);

	/**
	* Delete an element.
	*/
	void DeletePtr(bool bOrder,bool bAlloc,const void* tag,bool sortkey,size_t min, size_t max,int compare(const void*,const void*));

	/**
	* Delete an element at a given position.
	*/
	void DeletePtrAt(bool bAlloc,size_t pos,bool del);

	/**
	* Destruct the container.
	*/
	virtual ~BasicContainer(void);

	friend class BasicCursor;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
