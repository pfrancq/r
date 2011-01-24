/*
	R Project Library

	RSparseVector.h

	Sparse Vector - Header.

	Copyright 2005-2011 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by Valery Vandaele.
	Copyright 2003-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RSparseVector_H
#define RSparseVector_H


//-----------------------------------------------------------------------------
// include file for R Project
#include <rvalue.h>
#include <rcontainer.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RSparseVector provides a representation for a sparse vector. The vector
* is coded as a container of RValue. An identifier can be associate to the
* vector (this feature is used by RSparseMatrix).
*
* Here is an example of code:
* @code
* RSparseVector a(3);
* a[0]=1.0;
* a[5]=2.0;
* a[15]=3.0;
* for(size_t i=0;i<16;i++)
* 	cout<<static_cast<const RSparseVector&>(b)[i]<<endl;
* @endcode
* An important aspect is the use of static_cast<const RSparseVector&> to ensure
* the call of the const version of the operator(). If static_cast<const RSparseVector&>
* is not used, the different elements are created with uninitialized values.
* @author Pascal Francq (initial coding from Valery Vandaele).
* @short Sparse Vector.
*/
class RSparseVector : public RContainer<RValue,true,true>
{
private:

	/**
	* Identifier of the vector (used by RSparseMatrix).
	*/
	size_t Id;

public:

	/**
	* Construct a sparse vector.
	* @param size            Initial maximal size of the vector.
	* @param id              Identifier of the vector (cNoRef by default).
	*/
	RSparseVector(size_t size,size_t id=cNoRef);

	/**
	* Copy constructor.
	* @param vec             Sparse Vector used as source.
	*/
	RSparseVector(const RSparseVector& vec);

	/**
	* The assignment operator.
	* @param vec             Sparse Vector used as source.
	*/
	RSparseVector& operator=(const RSparseVector& vec);

	/**
	* Compare the identifiers of two sparse vectors.
	* param vec              Sparse vector to compared with.
	*/
	int Compare(const RSparseVector& vec) const;

	/**
	* Compare the identifier of the sparse vector with a given identifier.
	* param id               Identifier to compare with.
	*/
	int Compare(size_t id) const;

	/**
	 * Set a given value to all the existing elements.
	 * @param val            Value to set.
	 */
	void SetExistingElements(double val);

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	double operator[](size_t i) const;

	/**
	* Return the value at position i. The first value is at position 0.
	* @param i               Index.
	*/
	double& operator[](size_t i);

	/**
	 * Verify if a given index has a value defined in the vector.
	 * @param i              Index.
	 * @return true or false.
	 */
	bool IsIn(size_t i) const {return(RContainer<RValue,true,true>::IsIn(i));}

	/**
	 * Get a pointer over the value at a given index.
	 * @param i              Index.
	 * @return Pointer or null if the index hasn't no value.
	 */
	const RValue* GetValue(size_t i) const {return(RContainer<RValue,true,true>::GetPtr(i));}

	/**
	* Get the identifier of the cell.
	* @return Identifier of the cell.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	* Destruct the sparse vector.
	*/
	~RSparseVector(void);

	friend class RSparseMatrix;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
