/*

	R Project Library

	RMatrix.h

	Matrix classes - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision: 1.4 $

	Last Modify: $Date: 2000/11/29 09:23:50 $

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
#include <rstd/rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Typedef
typedef int tSize;
typedef float tNumber;


//------------------------------------------------------------------------------
/**
* The RMatrix class provides a matrix that can be used to do calculations.
*
* When a matrix is not bigb enough to hold the result of some operations,
* its sizes are incerased correctly.
*
* Most of the operators where defined for the matrixes. Here are some
* examples:
* <pre>
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
* </pre>
* @short Matrix class.
* @author Pascal Francq
*/
class RMatrix
{
	/**
	* Initialise the matrix.
	*/
	void Init(void) throw(bad_alloc);

public:

	/**
	* Actual number of columns in the matrix.
	*/
 	tSize Col;

	/**
	* Actual number of lines in the matrix.
	*/
	tSize Lin;

	/**
	* Maximal number of columns in the matrix.
	*/
	tSize MaxCol;

	/**
	* Maximal number of lines in the matrix.
	*/
	tSize MaxLin;

	/**
	* The matrix itself.
	*/
	tNumber **M;

    /**
	* Basic Exception for matrixes.
	*/
    class errMatrix {};

	/**
	* Two matrixes evolved in an operation, haven't compatible sizes.
	*/
    class errNotCompatibleSize : public errMatrix {};

	/**
	* The matrix is not a quadratic one.
	*/
    class errNotQuadraticMatrix : public errMatrix {};

	/**
	* Bad indexes where passed to the matrix.
	*/
    class errBadIndex : public errMatrix {};

	/**
	* Construct a empty matrix.
	*/
	RMatrix(void);

	/**
	* Construct a square matrix(Size,Size).
	* @param Size		Initial size of the matrix.
	*/
	RMatrix(tSize Size) throw(bad_alloc);

	/**
	* Construct a matrix (L,C).
	* @param L	Initial number of lines.
	* @param C  Initial number of columns.
	*/
	RMatrix(tSize L,tSize C) throw(bad_alloc);

	/**
	* Construct a matrix from another one.
	*/
	RMatrix(const RMatrix &Matrix) throw(bad_alloc);

	/**
	* Construct a matrix from another one.
	*/
	RMatrix(const RMatrix *Matrix) throw(bad_alloc);

	/**
	* Verify if the matrix has a given size, and increase them if necessary.
	* @param NewLin		New line number.
	* @param NewCol		New column number.*/
	inline void VerifySize(tSize NewLin,tSize NewCol) throw(bad_alloc);

	/**
	* Symetrize the matrix.
	*/
	void Symetrize(void);

	/**
	* Return a specific element of the matrix.
	* @param m	Line number of the element.
	* @param n	Column number of the element.
	*/
 	tNumber& operator()(int m, int n) const throw(errMatrix,bad_alloc);

	/**
	* Make the transitive closure for the matrix.
	*/
	char TransitiveClosure(RMatrix *Matrix) throw(errMatrix,bad_alloc);

	/**
	* Assign operator.
	*/
	RMatrix& operator=(const RMatrix) throw(errMatrix,bad_alloc);

	/**
	* Add a matrix to the current one.
	*/
	RMatrix& operator+=(const RMatrix) throw(errMatrix,bad_alloc);

	/**
	* Substract a matrix from the current one.
	*/
  	RMatrix& operator-=(const RMatrix) throw(errMatrix,bad_alloc);

	/**
	* Multiply a matrix with the current one. It is important to remember that
	* the matrix multiplication is not communitativ. So the next code defines
	* two different matrixes TempA et TempB.
	* <pre>
	* void Test(RMatrix &A,RMatrix &B)
	* {
	* 	RMatrix TempA(A);
	* 	RMatrix TempB(B);
	*
	* 	TempA*=B;
	* 	TempB*=A;
	* }
	* </pre>
	*/
	RMatrix& operator*=(const RMatrix) throw(errMatrix,bad_alloc);

	/**
	* Multiply a matrix with a given number.
	*/
	RMatrix& operator*=(const tNumber) throw(errMatrix,bad_alloc);

	/**
	* Return a temporary matrix. This function is needed for the operators.
	*/
	static RMatrix* GetMatrix(void);

	/**
	* Destruct the matrix.
	*/
	~RMatrix(void);
};


//------------------------------------------------------------------------------
// Operators

//------------------------------------------------------------------------------
/**
* Multiplication of a matrix and a number.
*/
RMatrix& operator*(const RMatrix&,const tNumber) throw(bad_alloc);

//------------------------------------------------------------------------------
/**
* Multiplication of a matrix and a number.
*/
RMatrix& operator*(const tNumber,const RMatrix&) throw(bad_alloc);

//------------------------------------------------------------------------------
/**
* Addition of two matrixes.
*/
RMatrix& operator+(const RMatrix& ,const RMatrix&) throw(RMatrix::errMatrix,bad_alloc);

//------------------------------------------------------------------------------
/**
* Substraction of two matrixes.
*/
RMatrix& operator-(const RMatrix& ,const RMatrix&) throw(RMatrix::errMatrix,bad_alloc);

//------------------------------------------------------------------------------
/**
* Multiplication of two matrixes.
*/
RMatrix& operator*(const RMatrix& ,const RMatrix&) throw(RMatrix::errMatrix,bad_alloc);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
