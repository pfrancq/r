/*

	R Project Library

	RLowerTriangularMatrix.h

	Matrix class - Header.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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
#ifndef RLowerTriangularMatrix_H
#define RLowerTriangularMatrix_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rvector.h>
#include <rcursor.h>
#include <rmatrix.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RLowerTriangularMatrix class provides a representation of a lower
* triangular matrix.
* @code
* RLowerTriangular a(2);   // Square matrix of (2,2)
* a(0,0)=1.0;
* a(1,0)=2.0;
* a(1,1)=3.0;
*
* RCursor<RVector> Lines(c.GetLines());
* for(Lines.Start();!Lines.End();Lines.Next())
* {
* 	RNumCursor<double> Cols(Lines()->GetCols());
* 	for(Cols.Start();!Cols.End();Cols.Next())
* 		cout<<Cols()<<"\t";
* 	cout<<endl;
* cont RLowerTriangular& b(a); // const version of a.
* cout<<b(0,1)<<endl;  // OK, print 0 (use the const (i,j) operator).
* a(0,1)=45.0;         // !!! Generate an exception since Matrix(i,j)=0 ∀ j>i
* a(0,1)=0.0;          // !!! Generate an exception : In fact, Matrix(i,j) cannot be written ∀ j>i
* @endcode
* @short Lower Triangular Matrix.
*/
class RLowerTriangularMatrix : public RMatrix
{
public:

	/**
	* Construct a square matrix of a given size.
	* @param size            Size of the matrix.
	*/
	RLowerTriangularMatrix(size_t size);

	/**
	* Construct a matrix from another one.
	* @param matrix          Matrix.
	*/
	RLowerTriangularMatrix(const RLowerTriangularMatrix& matrix);

	/**
	 * Compare method used by RContainer.
    * @param matrix          Matrix to compare with.
    * @return always -1.
    */
	int Compare(const RLowerTriangularMatrix& matrix) const;

protected:

	/**
	* Create the matrix. The method should always put Init to false.
	*/
	virtual void Create(void);

public:

	/**
	* Verify if the matrix has a given size, and increase them if necessary.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	virtual void VerifySize(size_t newlines,size_t newcols,bool fill=false,double val=NAN);

	/**
	 * Get the type of the matrix.
	 */
	virtual tType GetType(void) const {return(tLowerTriangular);}

	/**
	* Get the number of lines in the matrix.
	*/
	RCursor<RVector> GetLines(void) const {return(RCursor<RVector>(*this));}

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
	RLowerTriangularMatrix& operator=(const RLowerTriangularMatrix& matrix);

	/**
	* Destruct the matrix.
	*/
	virtual ~RLowerTriangularMatrix(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
