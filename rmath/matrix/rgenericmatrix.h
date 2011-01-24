/*

	R Project Library

	RGenericMatrix.h

	Generic Matrix class - Header.

	Copyright 2008-2011 by Pascal Francq (pascal@francq.info).

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
#ifndef RGenericMatrix_H
#define RGenericMatrix_H


//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <math.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rtextencoding.h>
#include <rcontainer.h>
#include <rvector.h>
#include <rcursor.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
class RMatrix;

//------------------------------------------------------------------------------
/**
* The RGenericMatrix class provides a representation for a generic matrix. It
* provides several operator acting on generic matrices. The operators are based
* on the operator(i,j) and are not optimized. If possible, these operators
* should be specialized in inheriting classes for performance purposes.
* @short Generic Matrix.
* @author Pascal Francq
*/
class RGenericMatrix
{
public:

	/**
	 * Type of a matrix.
	 */
	enum tType
	{
		tNormal              /** Normal matrix.*/,
		tDiagonal            /** Diagonal matrix.*/,
		tLowerTriangular     /** Lower triangular matrix.*/,
		tUpperTriangular     /** Upper triangular matrix.*/,
		tSymmetric           /** Symmetric matrix.*/,
		tSparse              /** Sparse matrix.*/,
		tSparseSymmetric     /** Sparse symmetric matrix.*/,
		tMax                 /** Sparse ordered matrix.*/
	};

protected:

	/**
	 * Number of lines.
	 */
	size_t NbLines;

	/**
	 * Number of columns.
	 */
	size_t NbCols;

public:

	/**
	* Construct a square matrix of a given size.
	* @param size            Size of the matrix.
	*/
	RGenericMatrix(size_t size);

	/**
	* Construct a matrix (L,C).
	* @param lines           Number of lines.
	* @param cols            Number of columns.
	*/
	RGenericMatrix(size_t lines,size_t cols);

	/**
	* Construct a matrix from another one.
	* @param matrix          Matrix.
	*/
	RGenericMatrix(const RGenericMatrix& matrix);

public:

	/**
	 * Clear the matrix. By default, it initialize the elements with Init.
	 * @param val            Value eventually used to assign to the elements.
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
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	virtual void VerifySize(size_t newlines,size_t newcols,bool fill=false,double val=NAN)=0;

	/**
	 * Get the type of the matrix.
	 */
	virtual tType GetType(void) const=0;

	/**
	* Get the number of lines in the matrix.
	*/
	size_t GetNbLines(void) const {return(NbLines);}

	/**
	* Get the number of columns in the matrix.
	*/
	size_t GetNbCols(void) const {return(NbCols);}

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	virtual double operator()(size_t i,size_t j) const=0;

	/**
	* Return a specific element of the matrix (const version).
	* @param i               Line number of the element.
	* @param j               Column number of the element.
	*/
 	virtual double& operator()(size_t i,size_t j)=0;

	/**
	 * Add a value associated to a given index into the matrix. By default, it
	 * calles the operator(size_t,size_t) method
	 * @param i              Line number of the element.
	 * @param j              Column number of the element.
	 * @param val            Value to add.
	 */
	virtual void Add(size_t i,size_t j,double val);

	/**
	* Assign operator.
	* @param matrix          Matrix.
	*/
	RGenericMatrix& operator=(const RGenericMatrix& matrix);

	/**
	 * Print the content of the matrix.
	 * @tparam S             Stream class that implements the << operator.
	 * @param stream         Stream.
	 * @param name           Name of the matrix.
	 */
	template<class S> void Print(S& stream,const RString& name)
	{
	 	RTextEncoding* Trans(RTextEncoding::GetTextEncoding("UTF-8"));
		RString Spaces;
		for(size_t i=0;i<name.GetLen()+1;i++)
			Spaces+=" ";
		size_t NamePos(NbLines/2);

		for(size_t i=0;i<NbLines;i++)
		{
			if(NamePos==i)
				stream<<name<<Trans->ToUnicode("=│",strlen("=│"));
			else
			{
				stream<<Spaces;
				if(!i)
					stream<<Trans->ToUnicode("┌",strlen("┌"));
				else if(i==NbLines-1)
					stream<<Trans->ToUnicode("└",strlen("└"));
				else
					stream<<Trans->ToUnicode("│",strlen("│"));
			}
			for(size_t j=0;j<NbCols;j++)
			{
				RString Str(RString::Number((*this)(i,j)));
				Str.SetLen(14," ");
				stream<<Str;
			}
			if(!i)
				stream<<Trans->ToUnicode("┐",strlen("┐"));
			else if(i==NbLines-1)
				stream<<Trans->ToUnicode("┘",strlen("┘"));
			else
				stream<<Trans->ToUnicode("│",strlen("│"));
			stream<<std::endl;
		}
	}


	/**
	* Destruct the matrix.
	*/
	virtual ~RGenericMatrix(void);
};


//------------------------------------------------------------------------------
/**
* Addition of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator+(const RGenericMatrix& arg1,const RGenericMatrix& arg2);


//------------------------------------------------------------------------------
/**
* Subtraction of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator-(const RGenericMatrix& arg1,const RGenericMatrix& arg2);


//------------------------------------------------------------------------------
/**
* Multiplication of a matrix and a number.
* @param arg1                Matrix.
* @param arg2                Number.
*/
RMatrix operator*(const RGenericMatrix& arg1,const double arg2);


//------------------------------------------------------------------------------
/**
* Multiplication of a matrix and a number.
* @param arg1                Number.
* @param arg2                Matrix.
*/
RMatrix operator*(const double arg1,const RGenericMatrix& arg2);


//------------------------------------------------------------------------------
/**
* Multiplication of two matrixes.
* @param arg1                Matrix.
* @param arg2                Matrix.
*/
RMatrix operator*(const RGenericMatrix& arg1,const RGenericMatrix& arg2);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
