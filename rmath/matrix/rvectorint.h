/*

	R Project Library

	RVectorInt.h

	Class representing a list of Integer values - Header

	Copyright 1998-2003 by the Université Libre de Bruxelles.

	Authors:
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
// include files for RProject
#include <rstd/random.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
/**
* The RVectorInt class provides a representation of vector of integers, each
* integer is represented by a number (int).

* @author Vandaele Valery
* @short List of Integers.
*/
class RVectorInt
{
protected:
	/**
	* Number of Integer value in the list.
	*/
	unsigned int NbInt;

	/**
	* Maximal Number of Integer value in the list.
	*/
	unsigned int MaxInt;

	/**
	* The array representing the Integer values.
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
	* @param MaxSize        The maximum number of Integer value.
	*/
	RVectorInt(const unsigned int MaxSize) throw(std::bad_alloc);

	/**
	* Copy constructor.
	* @param lst            List to copy.
	*/
	RVectorInt(const RVectorInt* lst) throw(std::bad_alloc);

	/**
	* Test if two lists have exactly the same Integers.
	* @param vi             The list used for the comparaison.
	* @return True if the lists are the same.
	*/
	bool IsSame(const RVectorInt& vi) const;

	/**
	* Verify if a Integer value is in the list.
	* @param In             The Integer value to test.
	* @return True if the Integer is in the list, false else.
	*/
	bool IsIn(const unsigned int In) const;

	/**
	* Insert an Integer value in the list.
	* @param Ins            The Integer value to insert.
	*/
	void Insert(const unsigned int Ins);

	/**
	* Insert all the Integer of a list in the current one. This function
	* supposes that the list to insert don't contain any Integers already
	* in the current list.
	* @param Ins            The list to insert.
	*/
	void Insert(const RVectorInt& Ins);

	/**
	* Delete an Integer value in the list.
	* @param Del            The Integer value to delete.
	*/
	void Delete(const unsigned int Del);

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


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
