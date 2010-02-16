/*
	R Project Library

	RSparseMatrix.h

	Sparse Matrix - Header.

	Copyright 2005-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2003-2005 by Vandaele Valery.
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
#ifndef RSparseMatrix_H
#define RSparseMatrix_H


//-----------------------------------------------------------------------------
// include file for R Project
#include <rcursor.h>
#include <rsparsevector.h>
#include <rgenericmatrix.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RSparseMatrix provides a representation for a sparse matrix. The matrix
* is coded as a container of RSparseVector. The matrix can be created so that
* to each line a vector is always build to speed up the access.
*
* Here is an example of code:
* @code
* RSparseMatrix a(6,3); // Matrix initially contains 6 lines and 3 values per line (<>3 columns).
* a(0,2)=0.2;
* a(1,5)=1.5;
* a(2,0)=1.0;
* a(5,1)=5.1;
* for(size_t i=0;i<6;i++)
* {
* 	for(size_t j=0;j<6;j++)
* 		cout<<static_cast<const RSparseMatrix&>(a)(i,j)<<"\t";
* 	cout<<endl;
* }
* @endcode
* An important aspect is the use of static_cast<const RSparseMatrix&> to ensure
* the call of the const version of the operator(). If static_cast<const RSparseMatrix&>
* is not used, the different elements are created with uninitialized values.
* @author Pascal Francq (initial coding from Valery Vandaele).
* @short Sparse Matrix.
*/
class RSparseMatrix : public RGenericMatrix, protected RContainer<RSparseVector,true,true>
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
	* @param nbcols          Number of columns (not the number of elements in each vector).
	* @param alllines        All lines have a vector.
	* @param init            Initial number of elements to reserve for each line.
	*/
	RSparseMatrix(size_t nblines,size_t nbcols,bool alllines=true,size_t init=20);

	/**
	* Copy constructor of a sparse matrix.
	* @param src             Sparse matrix used as source.
	*/
	RSparseMatrix(const RSparseMatrix& src);

	/**
	 * Clear the matrix. All the elements are removed.
	 * @param val            Value eventually used to assign to the elements (not used for sparse matrix).
	 * @param clean          Clean eventually the memory.
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
	virtual tType GetType(void) const {return(tSparse);}

	/**
	 * Get the number of vectors contained in the matrix. If the matrix does not
	 * create a vector for each line, the number of vectors does not correspond
	 * to the number of lines of the matrix.
	 */
	size_t GetNbVectors(void) const {return(GetNb());}

	/**
	 * Get a cursor over the vectors of the matrix.
	 */
	RCursor<RSparseVector> GetLines(void) const {return(RCursor<RSparseVector>(*this));}

	/**
	* The assignment operator.
	* @param matrix          Sparse Matrix used as source.
	*/
	RSparseMatrix& operator=(const RSparseMatrix& matrix);

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	virtual double operator()(size_t i,size_t j) const;

	/**
	* Return a specific element of the matrix.
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	virtual double& operator()(size_t i,size_t j);

 	/**
 	 * Return the vector at a given line from the matrix (const version).
 	 * @param i               Line number of the vector.
 	 */
 	const RSparseVector* operator[](size_t i) const;

 	/**
 	 * Return the vector at a given line from the matrix.
 	 * @param i               Line number of the vector.
 	 */
 	RSparseVector* operator[](size_t i);

	/**
	 * Verify if a given index has a vector defined in the matrix.
	 * @param i              Index.
	 * @return true or false.
	 */
	bool IsIn(size_t i) const {return(RContainer<RSparseVector,true,true>::IsIn(i));}

	/**
	 * Get a pointer over the vector at a given index.
	 * @param i              Index.
	 * @return Pointer or null if the index hasn't no vector.
	 */
	RSparseVector* GetValue(size_t i) const {return(RContainer<RSparseVector,true,true>::GetPtr(i));}

	/**
	* Destruct the sparse matrix.
	*/
	virtual ~RSparseMatrix(void);
};



}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
