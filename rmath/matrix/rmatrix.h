/*

	R Project Library

	RMatrix.h

	Matrix class - Header.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
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
#ifndef RMatrix_H
#define RMatrix_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RMatrix class provides a matrix that can be used to do calculations.
*
* When a matrix is not bigb enough to hold the result of some operations,
* its sizes are incerased correctly.
*
* Most of the operators where defined for the matrixes. Here are some
* examples:
* @code
* int main()
* {
* 	RMatrix a(2,2);
* 	RMatrix b(2,2);
* 	RMatrix c;
*
* 	a.M[0][0]=1; a.M[0][1]=2;
* 	a.M[1][0]=2; a.M[0][1]=2;
* 	b.M[0][0]=3; b.M[0][1]=2;
* 	b.M[1][1]=1; b.M[1][1]=4;
* 	c=a+b;
* 	c=a-b;
*	c=a*b;
*	  c=(2*a)+(b*a);
* }
* @endcode
* @short Matrix class.
* @author Pascal Francq
*/
class RMatrix
{
	/**
	* Actual number of lines in the matrix.
	*/
	size_t Lines;

	/**
	* Maximal number of lines in the matrix.
	*/
	size_t MaxLines;

	/**
	* Actual number of columns in the matrix.
	*/
 	size_t Cols;

	/**
	* Maximal number of columns in the matrix.
	*/
	size_t MaxCols;

	/**
	* The matrix itself.
	*/
	double** M;

public:

	/**
	* Construct a empty matrix.
	*/
	RMatrix(void);

	/**
	* Construct a square matrix of a given size.
	* @param size            Size of the matrix.
	* @param sym             Symetric matrix.
	*/
	RMatrix(size_t size);

	/**
	* Construct a matrix (L,C).
	* @param lines           Initial number of lines.
	* @param cols            Initial number of columns.
	*/
	RMatrix(size_t lines,size_t cols);

	/**
	* Construct a matrix from another one.
	* @param matrix          Matrix.
	*/
	RMatrix(const RMatrix& matrix);

private:

	/**
	* Initialise the matrix.
	*/
	void Init(void);

public:

	/**
	* Get the number of lines in the matrix.
	*/
	size_t GetNbLines(void) const {return(Lines);}

	/**
	* Get the number of columns in the matrix.
	*/
	size_t GetNbCols(void) const {return(Cols);}

	/**
	* Verify if the matrix has a given size, and increase them if necessary.
	* @param nwlines         New line number.
	* @param newcols         New column number.
	*/
	void VerifySize(size_t newlines,size_t newcols);

	/**
	* Symetrize the matrix.
	*/
	void Symetrize(void);

	/**
	* Return a specific element of the matrix.
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	double& operator()(size_t i,size_t j) const;

 	/**
 	 * Return a given lines from the matrix.
 	 * @param i               Line number of the element.
 	 */
 	const double* operator[](size_t i) const;

 	/**
 	 * Return a given lines from the matrix.
 	 * @param i               Line number of the element.
 	 */
 	double* operator[](size_t i);

	/**
	* Assign operator.
	* @param matrix          Matrix.
	*/
	RMatrix& operator=(const RMatrix& matrix);

	/**
	* Add a matrix to the current one.
	* @param matrix          Matrix to add.
	*/
	RMatrix& operator+=(const RMatrix& matrix);

	/**
	* Substract a matrix from the current one.
	* @param matrix          Matrix to substract.
	*/
  	RMatrix& operator-=(const RMatrix& matrix);

	/**
	* Multiply a matrix with the current one. It is important to remember that
	* the matrix multiplication is not communitativ. So the next code defines
	* two different matrixes TempA et TempB.
	* @code
	* void Test(RMatrix &A,RMatrix &B)
	* {
	* 	RMatrix TempA(A);
	* 	RMatrix TempB(B);
	*
	* 	TempA*=B;
	* 	TempB*=A;
	* }
	* @endcode
	* @param matrix          Matrix to multiply.
	*/
	RMatrix& operator*=(const RMatrix& matrix);

	/**
	* Multiply a matrix with a given number.
	* @param arg             Number.
	*/
	RMatrix& operator*=(const double arg);

	/**
	* Destruct the matrix.
	*/
	~RMatrix(void);
};


//------------------------------------------------------------------------------
// Operators
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/**
* Multiplication of a matrix and a number.
* @param arg1                Matrix.
* @param arg2                Number.
*/
RMatrix operator*(const RMatrix& arg1,const double arg2);

//------------------------------------------------------------------------------
/**
* Multiplication of a matrix and a number.
* @param arg1                Number.
* @param arg2                Matrix.
*/
RMatrix operator*(const double arg1,const RMatrix& arg2);

//------------------------------------------------------------------------------
/**
* Addition of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator+(const RMatrix& arg1,const RMatrix& arg2);

//------------------------------------------------------------------------------
/**
* Substraction of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator-(const RMatrix& arg1,const RMatrix& arg2);

//------------------------------------------------------------------------------
/**
* Multiplication of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator*(const RMatrix& arg1,const RMatrix& arg2);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
