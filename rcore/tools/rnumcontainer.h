/*

	R Project Library

	RNumContainer.h

	Class representing a container of numbers - Header

	Copyright 2001-2009 by Pascal Francq (pascal@francq.info).
	Copyright 2000-2001 by Valery Vandaele.
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
#ifndef RNumContainerH
#define RNumContainerH


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <random.h>
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// forward declaration
template<class I> class RNumCursor;


//------------------------------------------------------------------------------
/**
* The RNumContainer class provides a representation of a container of numbers.
* @tparam I                  Type (int, double, etc.).
* @tparam bOrder             Is the container ordered?
*
* @author Pascal Francq (initial coding from Valery Vandaele).
* @short Container of Numbers.
*/
template<class I,bool bOrder=true>
	class RNumContainer
{
protected:

	/**
	* Number of values in the list.
	*/
	size_t NbInt;

	/**
	* Maximal number of values in the list.
	*/
	size_t MaxInt;

	/**
	* The array representing the values.
	*/
	I* List;

public:

	/**
	* Construct the list of values.
	* @param max             Maximum number of values.
	*/
	RNumContainer(size_t max);

	/**
	* Copy constructor.
	* @param vector          Vector to copy.
	*/
	RNumContainer(const RNumContainer& vector);

	/**
	* Verify if the container can hold a certain number of elements. If not,
	* the container is extended.
    * @param max             Number of elements that must be contained.
	*/
	void Verify(size_t max);

private:

	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered.
	* @param nb              Number to find.
	* @param find            If the element represented by tag exist, find is set to
	*                        true.
	* @return Returns the index of the element if it exists or the index where
	* is has to inserted.
	*/
	size_t GetId(I nb,bool& find) const;

public:

	/**
	* Test if two lists have exactly the same values.
	* @param vi              The list used for the comparison.
	* @return True if the lists are the same.
	*/
	bool IsSame(const RNumContainer& vi) const;

	/**
	* Verify if a value is in the list.
	* @param value           The value to test.
	* @return True if the value is in the list, false else.
	*/
	bool IsIn(I value) const;

	/**
	* Insert an value in the list. If the vector is ordered, the method
	* verifies that the value is not in before insertion. If the vector is
	* unordered, the value is simply inserted at the end.
	* @param ins             The value to insert.
	*/
	void Insert(I ins);

	/**
	* Insert a value in the list at a given position.
	* @param ins             The value to insert.
	* @param pos             The position where to insert.
	* @param del             Specify if the object that was previously at the
	*                        position should be deleted or shifted.
	*
	*/
	void InsertAt(I ins,size_t pos,bool del=false);

	/**
	* Delete a value in the list.
	* @param del             The value to delete.
	*/
	void Delete(I del);

	/**
	* Delete an element at a given position.
	* @param pos             Position of the element to remove.
	* @param shift           Must the elements be shifted.
	*/
	void DeleteAt(size_t pos,bool shift=true);

	/**
	* Clear the list.
	*/
	void Clear(void);

private:

	/**
	 * Static function needed to order a list.
	 */
	static int ReOrderFunction(const void* num1, const void* num2);

public:

	/**
	* Reorder the current list of values in ascending order.
	*/
	void ReOrder(void);

	/**
	* Randomize the list.
	* @param rand            Random number generator to use.
	* @param nb              Number of first element to randomize. If null,
	*                        all values are randomized.
	*/
	void Randomize(RRandom* rand,size_t nb=0);

	/**
	* Assignment operator.
	* @param src             List used for the assignment.
	*/
	RNumContainer& operator=(const RNumContainer& src);

	/**
	* Insert all the values of a list in the current one. This function
	* supposes that the list to insert don't contain any value already
	* in the current list.
	* @param ins             The list to insert.
	*/
	void Add(const RNumContainer& ins);

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	I operator[](size_t i) const;

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	I& operator[](size_t i);

	/**
	 * Get the list of the values contained in the vector.
	 */
	const I* GetList(void) const {return(List);}

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	size_t GetNb(void) const {return(NbInt);}

	/**
	* Destructor of the list.
	*/
	~RNumContainer(void);

	friend class RNumCursor<I>;
};


//------------------------------------------------------------------------------
// Template implementation
#include <rnumcontainer.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
