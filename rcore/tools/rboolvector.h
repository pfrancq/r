/*

	R Project Library

	RBoolVector.h

	Class representing a list of boolean values - Header

	Copyright 2009-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RBoolVectorH
#define RBoolVectorH


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
* The RBoolVector class provides a representation of a vector of boolean
* values. A default value (that can be modified) is assigned to new elements.
* @short Boolean Vector.
*/
class RBoolVector
{
	/**
	* Number of values in the vector.
	*/
	size_t NbBool;

	/**
	* Maximal number of values in the vector.
	*/
	size_t MaxBool;

	/**
	* The array representing the boolean values.
	*/
	bool* List;

	/**
	 * Default value.
    */
	bool Default;

public:

	/**
	* Construct the vector of boolean values.
	* @param nb              Number of values.
	* @param val             Default value.
	*/
	RBoolVector(size_t nb,bool val=false);

	/**
	* Copy constructor.
	* @param vec             Vector to copy.
	*/
	RBoolVector(const RBoolVector& vec);

	/**
	 * Get the default value.
    */
	bool GetDefault(void) const {return(Default);}

	/**
	 * Set the default value that will be used for the new elements created in
	 * the future.
	 * @param val             Default value.
    */
	void SetDefault(bool val);

private:

	/**
	* Verify if the vector can hold a certain number of elements. If necessary,
	* the vector is extended with undefined values.
   * @param max              Number of elements in the vector.
	*/
	void Verify(size_t max);

public:

	/**
	* Resize the vector. If necessary, the vector is extended and new elements
	* are set to the default value.
   * @param size             Number of elements in the vector.
	*/
	void ReSize(size_t size);

	/**
	 * Initialize the first element of the vector with a given values. If
	 * necessary, the vector is extended.
	 * @param nb             Number of elements.
	 * @param val            Value used to initialize.
	 */
	void Init(size_t nb,bool val);

	/**
	* Test if two vectors have exactly the same values.
	* @param vector          Vector used for the comparison.
	* @return True if the lists are the same.
	*/
	bool IsSame(const RBoolVector& vector) const;

	/**
	* Set the boolean value at a given position. If necessary, the vector is
	* extended and the new elements receive the default value.
	* @param value           Value to insert.
	* @param pos             The position where to insert.
	*/
	void Set(bool value,size_t pos);

	/**
	* Clear the vector (set all values to the default value).
	*/
	void Clear(void);

	/**
	* Assignment operator.
	* @param src             List used for the assignment.
	*/
	RBoolVector& operator=(const RBoolVector& src);

	/**
	* Return the boolean value at position i. The first boolean value is at
	* position 0 (const version).
	* @param i               Index.
	*/
	bool operator[](size_t i) const;

	/**
	* Return the boolean value at position i. The first boolean value is at
	* position 0.
	* @param i               Index.
	*/
	bool& operator[](size_t i);

	/**
	 * Get the list of the booleans contained in the vector.
	 */
	const bool* GetList(void) const {return(List);}

	/**
	* Get the number of values in the vector.
	* @return size_t
	*/
	size_t GetNb(void) const {return(NbBool);}

	/**
	* Get the actual maximal number of values in the vector.
	* @return size_t
	*/
	size_t GetMax(void) const {return(MaxBool);}

	/**
	* Destruct the vector.
	*/
	virtual ~RBoolVector(void);

	friend class RBoolCursor;
};


}  //------- End of namespace R ------------------------------------------------


//------------------------------------------------------------------------------
#endif
