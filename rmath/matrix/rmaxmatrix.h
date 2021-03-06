/*
	R Project Library

	RMaxMatrix.h

	Ascending Ordered Sparse Matrix - Header.

	Copyright 2005-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by Vandaele Valery.
	Copyright 2003-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef RMaxMatrix_H
#define RMaxMatrix_H


//-----------------------------------------------------------------------------
// include file for R Project
#include <rcursor.h>
#include <rmaxvector.h>
#include <rgenericmatrix.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RMaxMatrix provides a representation for a sparse ordered matrix. The
* matrix is coded as a container of RMaxVector. To speed up the access, a vector
* can be build for every line of the matrix.
*
* Here is an example of code:
* @code
* RMaxMatrix a(2,2); // Matrix initially contains 2 lines and 2 values per line.
* a.Add(0,1,0.2);    // First element of line 0 is (0,1)
* a.Add(0,2,1.5);    // Second element of line 0 is (0,2)
* a.Add(1,2,1.0);    // First element of line 1 is (1,2)
* a.Add(1,3,5.1);    // Second element of line 1 is (1,3)
* for(size_t i=0;i<2;i++)
* {
* 	 for(size_t j=0;j<a[i]->GetNb();j++)
* 		cout<<static_cast<const RMaxMatrix&>(a)(i,j)<<"\t";
* 	 cout<<endl;
* }
* @endcode
*
* The number of of the columns of the matrix is fixed (but can be changed). It
* represents the maximal number of elements per line. Of course, an identifier
* can be greater than a column index (see example). The following code will
* generate an exception since the matrix a is of size \f$2 \cdot 2\f$.
* @code
* cout<<static_cast<const RMaxMatrix&>(a)(1,3)<<endl; // There are already two elements in the matrix at line 1
* @endcode
*
* An important aspect is the use of static_cast<const RMaxMatrix&> to ensure
* the call of the const version of the operator(). If static_cast<const RMaxMatrix&>
* is not used, the different elements are created with uninitialized values.
* @short Ascending Ordered Sparse Matrix.
*/
class RMaxMatrix : public RGenericMatrix, protected RContainer<RMaxVector,true,true>
{
protected:

	/**
	 * All lines have an entry.
	 */
	bool AllLines;

	/**
	 * Initial number of values to reserve for each line.
	 */
	size_t InitNbCols;

public:

	/**
	* Construct a sparse matrix.
	* @param nblines         Number of lines (not the number of vectors).
	* @param nbcols          Maximal number of values for each line.
	* @param alllines        All lines have a vector.
	* @param init            Initial number of elements to reserve for each line.
	*/
	RMaxMatrix(size_t nblines,size_t nbcols,bool alllines=true,size_t init=20);

	/**
	* Copy constructor of a sparse matrix.
	* @param src             Sparse matrix used as source.
	*/
	RMaxMatrix(const RMaxMatrix& src);

	/**
	 * Compare method used by RContainer.
    * @param matrix          Matrix to compare with.
    * @return always -1.
    */
	int Compare(const RMaxMatrix& matrix) const;

	/**
	 * Clear the matrix. All the elements are removed.
	 * @param val            Value eventually used to assign to the elements
	 *                       (not used for sparse matrix).
	 * @param clean          Clean eventually only the memory.
	 */
	virtual void Clear(double val=NAN,bool clean=false);

	/**
	 * Initialize the matrix with a given value. It uses the operator(i,j) to
	 * initialize the different elements.
	 * @param val            Value to assign.
	 */
	virtual void Init(double val);

	/**
	* Verify if the matrix has a given size, and increase them if necessary.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value (Not used for sparse matrix).
	* @param val             Value used eventually to fill the elements created  (Not used for sparse matrix).
	*/
	virtual void VerifySize(size_t newlines,size_t newcols,bool fill=false,double val=NAN);

	/**
	 * Get the type of the matrix.
	 */
	virtual tType GetType(void) const {return(tMax);}

	/**
	 * Get the number of vectors contained in the matrix. If the matrix does not
	 * create a vector for each line, the number of vectors does not correspond
	 * to the number of lines of the matrix.
	 */
	size_t GetNbVectors(void) const {return(GetNb());}

	/**
	 * Get a cursor over the vectors of the matrix.
	 */
	RCursor<RMaxVector> GetLines(void) const {return(RCursor<RMaxVector>(*this));}

	/**
	* The assignment operator.
	* @param matrix          Sparse Matrix used as source.
	*/
	RMaxMatrix& operator=(const RMaxMatrix& matrix);

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Index of the element.
	*/
 	virtual double operator()(size_t i,size_t j) const;

	/**
	* Return a specific element of the matrix. This function cannot be used
	* with RMaxMatrix.
	* @param i               Line number of the element.
	* @param j               Index of the element.
	*/
 	virtual double& operator()(size_t i,size_t j);

	/**
	* Add a value associated to a given line and identifier into the matrix.
	* If the number of elements for that line equals the size of the matrix, the
	* value is only inserted if another value of that line is smaller (this
	* latest value will then be removed).
	* @param i               Line number of the element.
	* @param j               Identifier of the element.
	* @param val             Value to add.
	*/
	virtual void Add(size_t i,size_t j,double val);

 	/**
 	 * Return the vector at a given line from the matrix (const version).
 	 * @param i               Line number of the vector.
 	 */
 	const RMaxVector* operator[](size_t i) const;

 	/**
 	 * Return the vector at a given line from the matrix.
 	 * @param i               Line number of the vector.
 	 */
 	RMaxVector* operator[](size_t i);

	/**
	 * Verify if a given identifier has a vector defined in the matrix.
	 * @param i              Identifier.
	 * @return true or false.
	 */
	bool IsIn(size_t i) const {return(RContainer<RMaxVector,true,true>::IsIn(i));}

	/**
	 * Get a pointer over the vector associated with a given identifier.
	 * @param i              Identifier.
	 * @return Pointer or null if the identifier hasn't no vector.
	 */
	RMaxVector* GetValue(size_t i) const {return(RContainer<RMaxVector,true,true>::GetPtr(i));}

	/**
	* Destruct the sparse matrix.
	*/
	virtual ~RMaxMatrix(void);
};



}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
