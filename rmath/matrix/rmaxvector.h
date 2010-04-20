/*
	R Project Library

	RMaxVector.h

	Ascending Ordered Vector - Header.

	Copyright 2005-2010 by Pascal Francq (pascal@francq.info).
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
#ifndef RMaxVector_H
#define RMaxVector_H


//-----------------------------------------------------------------------------
// include file for R Project
#include <rmaxvalue.h>
#include <rcontainer.h>
#include <rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RMaxVector provides a representation for a sparse vector where the values
* are ascending ordered (a[0]>a[1]>...>a[n]). The vector is coded as a
* container of RMaxValue. An identifier can be associate to the
* vector (this feature is used by RMaxMatrix).
*
* Here is an example of code:
* @code
* RMaxVector a(3);
* a.Add(1,3.0); // Identifier 1 is associated with value 3.0.
* a.Add(2,2.0); // Identifier 2 is associated with value 2.0.
* a.Add(3,1.0); // Identifier 3 is associated with value 1.0.
* for(size_t i=0;i<3;i++)
* 	cout<<static_cast<const RMaxVector&>(b)[i]<<endl;
* @endcode
* An important aspect is the use of static_cast<const RMaxVector&> to ensure
* the call of the const version of the operator(). If static_cast<const RMaxVector&>
* is not used, the different elements are created with uninitialized values.
* @author Pascal Francq.
* @short Ascending Ordered Sparse Vector.
*/
class RMaxVector : public RContainer<RMaxValue,true,true>
{
private:

	/**
	* Identifier of the vector (used by RSMaxMatrix).
	*/
	size_t Id;

public:

	/**
	* Construct a sparse ordered vector.
	* @param size            Initial maximal size of the vector.
	* @param id              Identifier of the vector (cNoRef by default).
	*/
	RMaxVector(size_t size,size_t id=cNoRef);

	/**
	* Copy constructor.
	* @param vec             Sparse ordered vector used as source.
	*/
	RMaxVector(const RMaxVector& vec);

	/**
	* The assignment operator.
	* @param vec             Sparse ordered vector used as source.
	*/
	RMaxVector& operator=(const RMaxVector& vec);

	/**
	* Compare the identifiers of two sparse ordered vectors.
	* param vec              Sparse ordered vector to compared with.
	*/
	int Compare(const RMaxVector& vec) const;

	/**
	* Compare the identifier of the sparse ordered vector with a given identifier.
	* param id               Identifier to compare with.
	*/
	int Compare(size_t id) const;

	/**
	* Return the ith maximal value. The maximal value is at position 0.
	* @param i               Index.
	*/
	double operator[](size_t i) const;

	/**
	 * Add a value associated to a given index into the vector.
	 * @param id             Identifier to associate.
	 * @param val            Value to add.
	 */
	void Add(size_t id,double val);

	/**
	 * Verify if a given identifier has a value defined in the vector.
	 * @param id             Identifier.
	 * @return true or false.
	 */
	bool IsIn(size_t id) const {return(RContainer<RMaxValue,true,true>::IsIn(id,false));}

	/**
	 * Get a pointer over the value at a given index.
	 * @param i              Index.
	 * @return Pointer to the value.
	 */
	const RMaxValue* GetValue(size_t i) const {return(RContainer<RMaxValue,true,true>::operator[](i));}

	/**
	 * Get a pointer over the value associated to a given identifier.
	 * @param id             Identifier.
	 * @return Pointer or null if the index hasn't no value.
	 */
	const RMaxValue* GetValueId(size_t id) const {return(RContainer<RMaxValue,true,true>::GetPtr(id,false));}

	/**
	* @return Identifier of the cell.
	*/
	size_t GetId(void) const {return(Id);}

	/**
	 * The method looks if a given value is better than the worst one, and
	 * replace it if necessary.
	 * @param val            Value to verify (it contains the value replaced).
	 * @param id             Corresponding identifier (it contains the
	 *                       identifier replaced).
	 * @return the position of the value inserted or cNoRef if the value is
	 * worser than all the existing one.
	 */
	size_t Replace(double& val,size_t& id);

	/**
	* Destruct the sparse ordered vector.
	*/
	~RMaxVector(void);

	friend class RMaxMatrix;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
