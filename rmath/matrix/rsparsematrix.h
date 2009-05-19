/*
	R Project Library

	RSparseMatrix.h

	Sparse Matrix - Header.

	Copyright 2005-2009 by Pascal Francq (pascal@francq.info).
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


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RSparseMatrix provides a representation for a sparse matrix. The matrix
* is coded as a container of RSparseVector. The matrix can be created so that
* to each line a vector is always build to speed up the access.
* @author Pascal Francq (initial coding from Valery Vandaele).
* @short Sparse Matrix.
*/
class RSparseMatrix : private RContainer<RSparseVector,true,true>
{
	/**
	 * All lines have an entry.
	 */
	bool AllLines;

	/**
	 * Initial maximum number of columns.
	 */
	size_t NbCols;

public:

	/**
	* Construct a sparse matrix.
	* @param nblines           Initial maximum number of lines.
	* @param nbcols            Initial maximum number of columns.
	* @param alllines          All lines have a vector.
	*/
	RSparseMatrix(size_t nblines,size_t nbcols,bool alllines=true);

	/**
	* Construct the generic cell from another one.
	* @param src            Generic Sparse Matrix used as source.
	*/
	RSparseMatrix(const RSparseMatrix& src);

	/**
	 * Clear the matrix. In practice, each line is emptied. By default, the
	 * lines are not removed from the matrix.
	 * @param dellines       Delete the lines.
	 */
	void Clear(bool dellines=false);

	/**
	 * Get the number of vectors contained in the matrix. If the matrix does not
	 * create a vector for each line, the number of vectors does not correspond
	 * to the number of lines of the matrix.
	 */
	size_t GetNbVectors(void) const {return(GetNb());}

	/**
	 * Get a cursor over the vectors of the matrix.
	 */
	RCursor<RSparseVector> GetVectors(void) const {return(RCursor<RSparseVector>(*this));}

	/**
	* The assignment operator.
	* @param src            Generic Sparse Matrix used as source.
	*/
	RSparseMatrix& operator=(const RSparseMatrix& src);

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	const RValue& operator()(size_t i,size_t j) const;

	/**
	* Return a specific element of the matrix.
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	RValue& operator()(size_t i,size_t j);

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
	* Destruct the sparse matrix.
	*/
	virtual ~RSparseMatrix(void);
};



}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
