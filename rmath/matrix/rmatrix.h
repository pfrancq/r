/*

	R Project Library

	RMatrix.h

	Matrix classes - Header.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
	* Actual number of columns in the matrix.
	*/
 	size_t Col;

	/**
	* Actual number of lines in the matrix.
	*/
	size_t Lin;

	/**
	* Maximal number of columns in the matrix.
	*/
	size_t MaxCol;

	/**
	* Maximal number of lines in the matrix.
	*/
	size_t MaxLin;

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
	*/
	RMatrix(size_t size);

	/**
	* Construct a matrix (L,C).
	* @param L	Initial number of lines.
	* @param C  Initial number of columns.
	*/
	RMatrix(size_t L,size_t C);

	/**
	* Construct a matrix from another one.
	*/
	RMatrix(const RMatrix& Matrix);

private:

	/**
	* Initialise the matrix.
	*/
	void Init(void);

public:

	/**
	* Get the number of lines in the matrix.
	*/
	size_t GetLinesNb(void) const {return(Lin);}

	/**
	* Get the number of columns in the matrix.
	*/
	size_t GetColsNb(void) const {return(Col);}

	/**
	* Verify if the matrix has a given size, and increase them if necessary.
	* @param NewLin          New line number.
	* @param NewCol          New column number.
	*/
	void VerifySize(size_t NewLin,size_t NewCol);

	/**
	* Symetrize the matrix.
	*/
	void Symetrize(void);

	/**
	* Return a specific element of the matrix.
	* @param m	Line number of the element.
	* @param n	Column number of the element.
	*/
 	double& operator()(size_t m,size_t n) const;

	/**
	* Assign operator.
	*/
	RMatrix& operator=(const RMatrix);

	/**
	* Add a matrix to the current one.
	* @param arg             Matrix to add.
	*/
	RMatrix& operator+=(const RMatrix arg);

	/**
	* Substract a matrix from the current one.
	* @param arg             Matrix to substract.
	*/
  	RMatrix& operator-=(const RMatrix arg);

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
	* @param arg             Matrix to multiply.
	*/
	RMatrix& operator*=(const RMatrix arg);

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
