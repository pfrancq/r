/*

	R Project Library

	RVectorInt.h

	Class representing a list of Integer values - Header

	Copyright 1998-2005 by the Université Libre de Bruxelles.

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


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/**
* The RVectorInt class provides a representation of ordered vector of integers.

* @author Vandaele Valery
* @short Ordered Integers Vector.
*/
template<bool bOrder=true>
	class RVectorInt
{
protected:
	/**
	* Number of values in the list.
	*/
	unsigned int NbInt;

	/**
	* Maximal number of values in the list.
	*/
	unsigned int MaxInt;

	/**
	* The array representing the integer values.
	*/
	unsigned int* List;

private:

	/**
	* Pointer used to parse a list.
	*/
	unsigned int* Parse;

	/**
	* Current position parsed.
	*/
	unsigned int Pos;

public:

	/**
	* Construct the list of Integer value.
	* @param Max            The maximum number of values.
	*/
	RVectorInt(const unsigned int max);

	/**
	* Copy constructor.
	* @param lst            List to copy.
	*/
	RVectorInt(const RVectorInt* lst);

private:

	/**
	* Verify if the container can hold the next element to be inserted. If not,
	* the container is extended.
	*/
	void Verify(void);

	/**
	* Verify if the container can hold a certain nujmber of elements. If not,
	* the container is extended.
    * @param max            The number of elements that must be contained.
	*/
	void Verify(unsigned int max);

	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered.
	* @param nb             Number to find.
	* @param find           If the element represented by tag exist, find is set to
	*                       true.
	* @return Returns the index of the element if it exists or the index where
	* is has to inserted.
	*/
	unsigned int GetId(unsigned int nb,bool& find) const;

public:

	/**
	* Test if two lists have exactly the same integers.
	* @param vi             The list used for the comparaison.
	* @return True if the lists are the same.
	*/
	bool IsSame(const RVectorInt& vi) const;

	/**
	* Verify if a integer value is in the list.
	* @param value          The integer value to test.
	* @return True if the integer is in the list, false else.
	*/
	bool IsIn(unsigned int value) const;

	/**
	* Insert an integer value in the list.
	* @param ins            The integer value to insert.
	*/
	void Insert(unsigned int ins);

	/**
	* Insert all the integer of a list in the current one. This function
	* supposes that the list to insert don't contain any Integers already
	* in the current list.
	* @param ins            The list to insert.
	*/
	void Insert(const RVectorInt& ins);

	/**
	* Insert an integer value in the list at a given position.
	* @param ins            The integer value to insert.
	* @param pos            The position where to insert.
	*/
	void InsertAt(unsigned int ins,unsigned int pos);

	/**
	* Delete an integer value in the list.
	* @param del            The integer value to delete.
	*/
	void Delete(const unsigned int del);

	/**
	* Reset the list.
	*/
	void Reset(void);

	/**
	* Assignation operator.
	* @param src            List used for the assignation.
	*/
	RVectorInt& operator=(const RVectorInt& src);

	/**
	* Return the Integer value at position i. The first Integer value is at position 0.
	* @param i              Index.
	*/
	unsigned int operator[](int i) const;

	/**
	* Get the number of Integer value in the list.
	* @return unsigned int
	*/
	unsigned int GetNbInt(void) const;

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
	unsigned int operator()(void) const {return(*Parse);}

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
