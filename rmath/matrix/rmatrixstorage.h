/*

	R Project Library

	RMatrixStrorage.h

	Matrix Storage - Header.

	Copyright 2009-2010 by Pascal Francq (pascal@francq.info).

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
#ifndef RMatrixStorage_H
#define RMatrixStorage_H



//------------------------------------------------------------------------------
// include files for R Project
#include <rbinaryfile.h>
#include <rgenericmatrix.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RMatrixStorage provides a class to store a matrix and retrieve a
 * particular element.
 *
 * If the matrix is not sparse (all elements must be stored), maximum 3 files
 * are needed:
 * -# File ".lower" contains the lower triangular part of the matrix, i.e. the
 *    values (0,0), (1,0), (1,1), (2,0), (2,1), (2,2)...
 * -# File ".upper" contains the upper triangular part of the matrix, i.e. the
 *    values (0,1), (0,2), (1,2), (0,3), (1,3), (2,3)...
 * -# File ".info" contains the size of the matrix and other information that
 *    the developer may add.
 *
 * If the matrix is sparse, the storage adapts a method proposed by Knuth. Two
 * files are needed:
 * -# File ".rec" contains the non-null elements. In fact, it stores a series
 *    of records composed from (value,line,column,next line,next column).
 * -# File ".info" contains the size of the matrix, the positions of the
 *    first and the last elements of each line and column and other information
 *    that the developer may add.
 *
 *
 * The information of ".info" are removed if the size of the matrix is changed
 * and the matrix is sparse. The information must be load when the matrix is
 * used for the first time and store again before the file is closed.
 * @short Matrix Storage
 * @author Pascal Francq
 */
class RMatrixStorage
{
	/**
	 * File containing the ".lower" or the ".rec" part.
	 */
	RBinaryFile File1;

	/**
	 * File containing the ".upper" part if necessary.
	 */
	RBinaryFile File2;

	/**
	 * An upper part of the matrix is managed.
	 */
	bool Upper;

	/**
	 * Index file.
	 */
	RBinaryFile Index;

	/**
	 * Base URI.
	 */
	RString BaseURI;

	/**
	 * Number of lines of the matrix.
	 */
	size_t NbLines;

	/**
	 * Number of columns of the matrix.
	 */
	size_t NbCols;

	/**
	 * Type of the matrix stored.
	 */
	RGenericMatrix::tType Type;

	/**
	 * Sparse version of the storage.
	 */
	bool Sparse;

	/**
	 * Max version of the storage.
	 */
	bool Max;

	/**
	 * Position of the (0,1) element in the upper part of the matrix (0 or 1).
	 */
	size_t Pos01;

public:

	/**
	 * Construct the storage.
	 */
	RMatrixStorage(void);

	/**
	 * Open the storage.
	 * @param baseuri        Base URI.
	 * @param type           Type of the matrix.
	 */
	void Open(const RString& baseuri,RGenericMatrix::tType type);

	/**
	 * Close the storage.
	 */
	void Close(void);

private:

	/**
	 * Compute the position if a given element in the lower part of the matrix.
	 * @param i              Line.
	 * @param j              Column.
	 * @return Position of the element.
	 */
	inline size_t GetLowerPos(size_t i,size_t j);

	/**
	 * Compute the position if a given element in the upper part of the matrix.
	 * @param i              Line.
	 * @param j              Column.
	 * @return Position of the element.
	 */
	inline size_t GetUpperPos(size_t i,size_t j);

	/**
	 * Get the maximal dimension of the matrix
	 */
	inline size_t GetMaxDim(void) const {return(NbLines>NbCols?NbLines:NbCols);}

public:

	/**
	* Get the number of lines in the matrix.
	*/
	size_t GetNbLines(void) const {return(NbLines);}

	/**
	* Get the number of columns in the matrix.
	*/
	size_t GetNbCols(void) const {return(NbCols);}

	/**
	* Get the number of lines in the matrix at a given column.
	* @param col             Column.
	*/
	size_t GetNbLines(size_t col) const;

	/**
	* Get the number of columns in the matrix at a given line.
	* @param line            Line.
	*/
	size_t GetNbCols(size_t line) const;

	/**
	 * Clear the files. In practice, the files are re-created with, eventually,
	 * a new value.
	 */
	void Clear(double val=NAN);

private:

	/**
	 * Method called to re-created the upper file.
	 * @param newlines       New lines.
	 * @param newcols        New cols.
	 * @param val            Value used eventually to fill the elements created.
	 */
	void ReCreateUpperFile(size_t newlines,size_t newcols,double val);

	/**
	 * Method called to truncate or extend the upper file.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	 */
	void ReModifyUpperFile(size_t newlines,size_t newcols,bool fill,double val);

public:

	/**
	* Verify if the matrix has a given size, and increase and reduce the file
	* if necessary. The method must be used with caution since it may be costly
	* sometimes:
	* - When a non sparse matrix is reduced, some files are always re-created.
	* - When a non sparse matrix is extended and is not leave square, some
	*   files are re-created.
	* - When a sparse matrix is reduced, a new file is re-created and the
	*   unnecessary elements are removed.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	void VerifySize(size_t newlines,size_t newcols,bool fill=false,double val=NAN);

	/**
	 * Load a matrix.
	 * \warning If the matrix is a sparse one, the values read are added to the
	 * matrix.
	 * @param matrix         matrix.
	 */
	void Load(RGenericMatrix& matrix);

private:

	/**
	 * Save a full matrix.
	 * @param matrix         matrix.
	 */
	void SaveFull(const RGenericMatrix& matrix);

	/**
	 * Save a sparse matrix.
	 * @param matrix         matrix.
	 */
	void SaveSparse(const RGenericMatrix& matrix);

	/**
	 * Save a matrix storing maximal values.
	 * @param matrix         matrix.
	 */
	void SaveMax(const RGenericMatrix& matrix);

public:

	/**
	 * Save a matrix.
	 * @param matrix         matrix.
	 */
	void Save(const RGenericMatrix& matrix);

	/**
	 * Read a value of a given element.
	 * @param i              Line of the element.
	 * @param j              Column of the element.
	 */
	double Read(size_t i,size_t j);

	/**
	 * Write a value of a given element. If the matrix is sparse, the method
	 * first verify if the value is already stored.
	 *
	 * If the matrix is sparse and an old value is already present, a null
	 * value replaces the old value in the file.
	 * @param i              Line of the element.
	 * @param j              Column of the element.
	 * @param val            Value to write.
	 */
	void Write(size_t i,size_t j,double val);

	/**
	* Read a given information.
	* @param posinfo        Identifier of the information.
	* @param buffer         Buffer that will hold the information (must be allocated).
	* @param nb             Number of bytes of the information.
	*
	* Here is an example of reading information an information starting at 10
	* which is a double:
	* @code
	* RMatrixStorage Storage;
	* ...
	* double nb;    // Information to read
	* Storage.ReadInfo(10,static_cast<char*>(&nb),sizeof(double));
	* @endcode
	*/
	void ReadInfo(size_t posinfo,char* buffer,size_t nb);

	/**
	* Write a given information.
	* @param posinfo        Position of the information.
	* @param buffer         Buffer holding the information.
	* @param nb             Number of bytes of the information.
	*
	* Here is an example of writing information an information starting at 10
	* which is a double:
	* @code
	* RMatrixStorage Storage;
	* ...
	* double nb(3.5);    // Information to write
	* Storage.WriteInfo(10,static_cast<char*>(&nb),sizeof(double));
	* @endcode
	*/
	void WriteInfo(size_t posinfo,const char* buffer,size_t nb);

	/**
	 * See if there are some information contained in the ".info" file.
	 */
	bool HasInfo(void) const;
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
