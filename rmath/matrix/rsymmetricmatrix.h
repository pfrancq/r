/*

	R Project Library

	RSymmetricMatrix.h

	Matrix class - Header.

	Copyright 1999-2012 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RSymmetricMatrix_H
#define RSymmetricMatrix_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rvector.h>
#include <rcursor.h>
#include <rlowertriangularmatrix.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RSymmetricMatrix class provides a representation of a symmetric matrix.
* @short Symmetric Matrix.
* @author Pascal Francq
*/
class RSymmetricMatrix : public RLowerTriangularMatrix
{
public:

	/**
	* Construct a square symmetric matrix of a given size.
	* @param size            Size of the matrix.
	*/
	RSymmetricMatrix(size_t size);

	/**
	* Construct a matrix from another one.
	* @param matrix          Matrix.
	*/
	RSymmetricMatrix(const RSymmetricMatrix& matrix);

	/**
	 * Compare method used by RContainer.
    * @param matrix          Matrix to compare with.
    * @return always -1.
    */
	int Compare(const RSymmetricMatrix& matrix) const;

	/**
	 * Get the type of the matrix.
	 */
	virtual tType GetType(void) const {return(tSymmetric);}

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	virtual double operator()(size_t i,size_t j) const;

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	virtual double& operator()(size_t i,size_t j);

	/**
	* Assign operator.
	* @param matrix          Matrix.
	*/
	RSymmetricMatrix& operator=(const RSymmetricMatrix& matrix);

	/**
	* Destruct the matrix.
	*/
	virtual ~RSymmetricMatrix(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
