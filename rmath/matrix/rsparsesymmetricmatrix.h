/*
	R Project Library

	RSparseSymmetricMatrix.h

	Sparse Symmetric Matrix - Header.

	Copyright 2008-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef RSparseSymmetricMatrix_H
#define RSparseSymmetricMatrix_H


//-----------------------------------------------------------------------------
// include file for R Project
#include <rsparsematrix.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RSparseSymmetricMatrix provides a representation for a sparse symmetric
* matrix. It is specialized version of RSparseMatrix.

* An important aspect is the use of static_cast<const RSparseSymmetricMatrix&> to ensure
* the call of the const version of the operator(). If static_cast<const RSparseSymmetricMatrix&>
* is not used, the different elements are created with uninitialized values.
* @author Pascal Francq.
* @short Sparse Symmetric Matrix.
*/
class RSparseSymmetricMatrix : public RSparseMatrix
{
public:

	/**
	* Construct a sparse symmetric matrix.
	* @param size            Size of the matrix.
	* @param alllines        All lines have a vector.
	* @param init            Initial number of elements to reserve for each line.
	*/
	RSparseSymmetricMatrix(size_t size,bool alllines=true,size_t init=20);

	/**
	* Copy constructor of a sparse symmetric matrix.
	* @param src            Sparse symmetric matrix used as source.
	*/
	RSparseSymmetricMatrix(const RSparseSymmetricMatrix& src);

	/**
	 * Get the type of the matrix.
	 */
	virtual tType GetType(void) const {return(tSparseSymmetric);}

	/**
	* The assignment operator.
	* @param matrix          Sparse Matrix used as source.
	*/
	RSparseSymmetricMatrix& operator=(const RSparseSymmetricMatrix& matrix);

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
	* Destruct the sparse matrix.
	*/
	virtual ~RSparseSymmetricMatrix(void);
};



}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
