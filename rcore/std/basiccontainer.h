/*

	R Project Library

	BasicContainer.h

	Basic Container - Header.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#include <rstd/rstd.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration of the cursor template
class BasicCursor;


//-----------------------------------------------------------------------------
/*
* This class represent a container of elements. This elements are
* store in an array of pointers which will be increase when necessary.
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

	/*
	* Constructor.
	*/
	BasicContainer(void) {}

	/*
	* Constructor of a container.
	* @param m               The initial maximal size of the array.
	* @param i               The value used when increasing the array. If null
	*                        value, the size is set to the half the maximal
	*                        size.
	*/
	explicit BasicContainer(size_t m,size_t i=0);

	/*
	* Copy constructor of a container.
	* @param src             Container used as source.
	*/
	BasicContainer(const BasicContainer& src);

public:

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

protected:

	/*
	* Insert an element in an ordered container. Used by InsertPtr and
	* GetInsertPtr.
	* @param ins             A pointer to the element to insert.
	* @param pos             The position where to insert it.
	*/
	void InsertPtrOrderAt(const void* ins,size_t pos);


public:

	/**
	* Exchange two elements in the container. The method does not verify if the
	* exchange is compatible with the ordering of the container.
	* @param pos1            Position of the first element.
	* @param pos2            Position of the second element.
	*/
	void Exchange(size_t pos1,size_t pos2);

protected:

	/*
	* Get a pointer to the ith element in the container (Only read).
	* @param idx             Index of the element to get.
	* @return Return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	const void* operator[](size_t idx) const;

	/*
	* Get a pointer to the ith element in the container (Read/Write).
	* @param idx             Index of the element to get.
	* @return Return the pointer or 0 if the index is outside the scope of the
	*         container.
	*/
	void* operator[](size_t idx);

	/*
	* Copy the array of pointer into a temporary array. This array must have
	* the right size (Read only).
	* @param tab             Array of pointer.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	size_t GetTab(void** tab);

	/*
	* Copy the array of pointer into a temporary array. This array must have
	* the right size (Read only).
	* @param tab             Array of pointer.
	* @return number of elements in the array (including eventually null
	* pointers).
	*/
	size_t GetTab(const void** tab) const;

	friend class BasicCursor;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
