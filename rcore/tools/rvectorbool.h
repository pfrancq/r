/*

	R Project Library

	RVectorBool.h

	Class representing a list of boolean values - Header

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef RVectorBoolH
#define RVectorBoolH


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
* The RVectorBool class provides a representation of a vector of boolean
* values. The elements are set to -1 if undefined.
* @author Pascal Francq.
* @short Boolean Vector.
*/
class RVectorBool
{
	/**
	* Number of values in the list.
	*/
	size_t NbBool;

	/**
	* Maximal number of values in the list.
	*/
	size_t MaxBool;

	/**
	* The array representing the boolean values.
	*/
	char* List;

private:

	/**
	* Pointer used to parse the list.
	*/
	char* Parse;

	/**
	* Current position parsed.
	*/
	size_t Pos;

public:

	/**
	* Construct the list of boolean values.
	* @param max             Maximum number of values.
	*/
	RVectorBool(size_t max);

	/**
	* Copy constructor.
	* @param vec             Vector to copy.
	*/
	RVectorBool(const RVectorBool& vec);

private:

	/**
	* Verify if the container can hold a certain number of elements. If not,
	* the container is extended.
    * @param max             Number of elements that must be contained.
	*/
	void Verify(size_t max);

public:

	/**
	 * Initialize the vector with a given values.
	 * @param nb             Number of elements.
	 * @param val            Value used to initialize.
	 */
	void Init(size_t nb,bool val);

	/**
	* Test if two lists have exactly the same values.
	* @param vi              The list used for the comparison.
	* @return True if the lists are the same.
	*/
	bool IsSame(const RVectorBool& vi) const;

	/**
	* Set the boolean value at a given position.
	* @param value           Value to insert.
	* @param pos             The position where to insert.
	*/
	void Set(bool value,size_t pos);

	/**
	* Clear the list (set all values to -1).
	*/
	void Clear(void);

	/**
	* Assignment operator.
	* @param src             List used for the assignment.
	*/
	RVectorBool& operator=(const RVectorBool& src);

	/**
	* Return the Integer value at position i. The first Integer value is at position 0.
	* @param i               Index.
	*/
	bool operator[](size_t i) const;

	/**
	* Return the Integer value at position i. The first Integer value is at position 0.
	* @param i               Index.
	*/
	char& operator[](size_t i);

	/**
	 * @return true if the element is defined (true or false) at a given
	 * position.
	 * @param i               Index.
	 */
	bool IsDefined(size_t i) const {return(List[i]!=-1);}

	/**
	 * Get the list of the integers contained in the vector.
	 */
	const char* GetList(void) const {return(List);}

	/**
	* Get the number of values in the list.
	* @return size_t
	*/
	size_t GetNb(void) const {return(NbBool);}

	/**
	* Get the actual maximal number of values in the list.
	* @return size_t
	*/
	size_t GetMax(void) const {return(MaxBool);}

	/**
	* Start the iterator to go trough the list.
	*/
	void Start(void) {Pos=0; Parse=List;}

	/**
	* Get the current position parsed in the list.
	* @return size_t
	*/
	size_t GetPos(void) const {return(Pos);}

	/**
	* Test if the end of the list is reached.
	*/
	bool End(void) const {return(Pos==NbBool);}

	/**
	* Goto the next element of the list.
	*/
	void Next(void);

	/**
	* Return the current element.
	*/
	bool operator()(void) const {return(*Parse);}

	/**
	* Destruct the list.
	*/
	virtual ~RVectorBool(void);
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
