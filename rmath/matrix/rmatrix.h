/*

	R Project Library

	RMatrix.h

	Matrix class - Header.

	Copyright 1999-2011 by Pascal Francq (pascal@francq.info).
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
#include <rcontainer.h>
#include <rvector.h>
#include <rcursor.h>
#include <rgenericmatrix.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RMatrix class provides a representation of a matrix as a given number of
* vector, each vector representing a line
*
* Here are some examples:
* @code
* RMatrix a(2,2);
* RMatrix b(2,2);
* RMatrix c;
*
* a(0,0)=1; a(0,1)=2;
* a(1,0)=2; a(0,1)=2;
* b(0,0)=3; b(0,1)=2;
* b(1,1)=1; b(1,1)=4;
* c=a+b;
* c=a-b;
* c=a*b;
* c=(2*a)+(b*a);
*
* RCursor<RMatrixLine> Lines(c.GetLines());
* for(Lines.Start();!Lines.End();Lines.Next())
* {
* 	RNumCursor<double> Cols(Lines()->GetCols());
* 	for(Cols.Start();!Cols.End();Cols.Next())
* 		cout<<Cols()<<"\t";
* 	cout<<endl;
* }
* @endcode
* @short Matrix.
* @author Pascal Francq
*/
class RMatrix : public RGenericMatrix, protected RContainer<RMatrixLine,true,false>
{
protected:

	/**
	 * Matrix must be initialized. If MustCreate is false, Create() must be
	 * called. It must be verify in each method.
	 */
	bool MustCreate;

public:

	/**
	* Construct a square matrix of a given size.
	* @param size            Size of the matrix.
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

protected:

	/**
	* Create the matrix. The method should always put MustCreate to false.
	*/
	virtual void Create(void);

	/**
	 * Test if the matrix must be created.
	 */
	inline void TestThis(void) const {if(MustCreate) const_cast<RMatrix*>(this)->Create();}

public:

	/**
	 * Initialize the matrix with a given value.
	 * @param val            Value to assign.
	 */
	virtual void Init(double val);

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
	virtual tType GetType(void) const {return(tNormal);}

	/**
	* Get the number of lines in the matrix.
	*/
	RCursor<RMatrixLine> GetLines(void) const {return(RCursor<RMatrixLine>(*this));}

	/**
	* Make the matrix symmetric by copying the "left-upper" part in the
	* "right-bottom" part.
	*/
	void Symetrize(void);

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
 	 * Return the vector at a given line from the matrix (const version).
 	 * @param i               Line number of the vector.
 	 */
 	const RMatrixLine* operator[](size_t i) const;

 	/**
 	 * Return the vector at a given line from the matrix.
 	 * @param i               Line number of the vector.
 	 */
 	RMatrixLine* operator[](size_t i);

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
	* Subtract a matrix from the current one.
	* @param matrix          Matrix to subtract.
	*/
  	RMatrix& operator-=(const RMatrix& matrix);

	/**
	* Multiply a matrix with a given number.
	* @param arg             Number.
	*/
	RMatrix& operator*=(const double arg);

	/**
	* Multiply a matrix with the current one. It is important to remember that
	* the matrix multiplication is not communitative. So the next code defines
	* two different matrixes TempA and TempB.
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
	* Destruct the matrix.
	*/
	virtual ~RMatrix(void);
};


//------------------------------------------------------------------------------
/**
* Addition of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator+(const RMatrix& arg1,const RMatrix& arg2);


//------------------------------------------------------------------------------
/**
* Subtraction of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator-(const RMatrix& arg1,const RMatrix& arg2);


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
* Multiplication of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator*(const RMatrix& arg1,const RMatrix& arg2);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
