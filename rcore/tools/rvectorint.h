/*

	R Project Library

	RVectorInt.h

	Class representing a list of integer values - Header

	Copyright 1998-2009 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).
		Vandaele Valery(vvandaele@ulb.ac.be).

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
#ifndef RVectorIntH
#define RVectorIntH


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <limits.h>
#include <new>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <random.h>
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RVectorInt class provides a representation of a vector of integers.
* @param I                   Type of integers.
* @param bOrder              Is the vector of integer ordered?
*
* @author Pascal Francq and Vandaele Valery.
* @short Vector of Integers.
*/
template<class I,bool bOrder=true>
	class RVectorInt
{
	/**
	* Number of values in the list.
	*/
	size_t NbInt;

	/**
	* Maximal number of values in the list.
	*/
	size_t MaxInt;

	/**
	* The array representing the integer values.
	*/
	I* List;

private:

	/**
	* Pointer used to parse the list.
	*/
	I* Parse;

	/**
	* Current position parsed.
	*/
	size_t Pos;

public:

	/**
	* Construct the list of integer values.
	* @param max             Maximum number of values.
	*/
	RVectorInt(size_t max);

	/**
	* Copy constructor.
	* @param vector          Vector to copy.
	*/
	RVectorInt(const RVectorInt& vector);

private:

	/**
	* Verify if the container can hold a certain number of elements. If not,
	* the container is extended.
    * @param max             Number of elements that must be contained.
	*/
	void Verify(size_t max);

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
	* Test if two lists have exactly the same integers.
	* @param vi              The list used for the comparaison.
	* @return True if the lists are the same.
	*/
	bool IsSame(const RVectorInt& vi) const;

	/**
	* Verify if a integer value is in the list.
	* @param value           The integer value to test.
	* @return True if the integer is in the list, false else.
	*/
	bool IsIn(I value) const;

	/**
	* Insert an integer value in the list.
	* @param ins             The integer value to insert.
	*/
	void Insert(I ins);

	/**
	* Insert all the integer of a list in the current one. This function
	* supposes that the list to insert don't contain any Integers already
	* in the current list.
	* @param ins             The list to insert.
	*/
	void Insert(const RVectorInt& ins);

	/**
	* Insert an integer value in the list at a given position.
	* @param ins             The integer value to insert.
	* @param pos             The position where to insert.
	*/
	void InsertAt(I ins,size_t pos);

	/**
	* Delete an integer value in the list.
	* @param del             The integer value to delete.
	*/
	void Delete(I del);

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
	* Reorder the current list of size_tegers in ascending order.
	*/
	void ReOrder(void);

	/**
	* Randomize the list.
	* @param rand            Random number generator to use.
	* @param nb              Number of first element to randomize. If null,
	*                        all integers are randomized.
	*/
	void Randomize(RRandom* rand,size_t nb=0);

	/**
	* Assignment operator.
	* @param src             List used for the assignment.
	*/
	RVectorInt& operator=(const RVectorInt& src);

	/**
	* Return the Integer value at position i. The first Integer value is at position 0.
	* @param i               Index.
	*/
	const I& operator[](size_t i) const;

	/**
	* Return the Integer value at position i. The first Integer value is at position 0.
	* @param i               Index.
	*/
	I& operator[](size_t i);

	/**
	 * Get the list of the integers contained in the vector.
	 */
	const I* GetList(void) const {return(List);}

	/**
	* Get the number of Integer value in the list.
	* @return size_t
	*/
	size_t GetNb(void) const {return(NbInt);}

	/**
	* Start the iterator to go trough the list.
	*/
	void Start(void) {Pos=0; Parse=List;}

	/**
	* Test if the end of the list is reached.
	*/
	bool End(void) const {return(Pos==NbInt);}

	/**
	* Goto the next element of the list.
	*/
	void Next(void) {Pos++; Parse++;}

	/**
	* Return the current element.
	*/
	I operator()(void) const {return(*Parse);}

	/**
	* Destructor of the list.
	*/
	virtual ~RVectorInt(void);
};


//------------------------------------------------------------------------------
// Template implementation
#include <rvectorint.hh>


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
