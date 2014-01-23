/*

	R Project Library

	RMatrixStrorage.h

	Matrix Storage - Header.

	Copyright 2009-2012 by Pascal Francq (pascal@francq.info).

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
 * For a full matrix (all elements must be stored), maximum 3 files are needed:
 * -# File ".lower" contains the lower triangular part of the matrix. For a
 * \f$4\cdot 4\f$ matrix, the values stored are (0,0), (1,0), (1,1), (2,0),
 * (2,1), (2,2), (3,0), (3,1), (3,2) and (3,3).
 * -# File ".upper" contains the upper triangular part of the matrix, For a
 * \f$4\cdot 4\f$ matrix, the values stored are (0,1), (0,2), (1,2), (0,3),
 * (1,3) and (2,3).
 * -# File ".info" contains the size of the matrix and other information that
 *    the developer may add.
 *
 * For a sparse matrix, the storage adapts a method proposed by Knuth. Two files
 * are needed:
 * -# File ".rec" contains the non-null elements. In fact, it stores a series
 *    of records composed from (value,line,column,next line,next column).
 * -# File ".info" contains the size of the matrix, the positions of the
 *    first and the last elements of each line and column and other information
 *    that the developer may add. In the case of an ascending ordered sparse
 *    matrix, the file contains also the number of values associated with each
 *    line.
 *
 * Of course, once the matrix storage is created, it is not possible to change
 * its type (full, sparse or ascending ordered sparse). If done, at best, the
 * values manipulated are wrong, but mostly a generation will be generated after
 * a few calls.
 * @remark The information of ".info" are removed if the size of the matrix is
 * changed and the matrix is sparse. Therefore, in this case, the information
 * should be loaded first, and store again before the file is closed after the
 * modification.
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
	 * Line of the last element accessed.
	 */
	size_t LastLine;

	/**
	 * Column of the last element accessed.
	 */
	size_t LasCol;

	/**
	 * Last element accessed was write (true) or read (false).
	 */
	bool LastWrite;

	/**
	 * Type of the matrix stored.
	 */
	RGenericMatrix::tType Type;

	/**
	 * Sparse version of the storage ?
	 */
	bool Sparse;

	/**
	 * Max version of the storage ?
	 */
	bool Max;

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
	 * Compute the position if a given element in the lower part of a full matrix.
	 * @param i              Line.
	 * @param j              Column.
	 * @return Position of the element.
	 *
	 * To compute ther size of the lower part of a full matrix \f$n \cdot m\f$,
	 * you must use:
	 * @code
	 *	size_t Size=(GetLowerPos(n-1,m-1)+1)*sizeof(double).
	 * @endcode
	 */
	inline size_t GetLowerPos(size_t i,size_t j);

	/**
	 * Compute the position if a given element in the upper part of a full matrix.
	 * @param i              Line.
	 * @param j              Column.
	 * @return Position of the element.
	 *
	 * To compute ther size of the upper part of a full matrix \f$n \cdot m\f$,
	 * you must use:
	 * @code
	 *	size_t Size=(GetUpperPos(n-1,m-1)+1)*sizeof(double).
	 * @endcode
	 */
	inline size_t GetUpperPos(size_t i,size_t j);

	/**
	 * Get the maximal dimension of the matrix.
	 */
	inline size_t GetMaxDim(void) const {return(NbLines>NbCols?NbLines:NbCols);}

public:

	/**
	* Get the number of lines in the matrix.
	*/
	inline size_t GetNbLines(void) const {return(NbLines);}

	/**
	* Get the number of columns in the matrix.
	*/
	inline size_t GetNbCols(void) const {return(NbCols);}

	/**
	* For full and sparse matrices, the method returns the number of column of
	* the matrix. For an ascending ordered sparse matrix, the method returns the
	* number of elements associated with that line (which may be smaller that the
	* number of columns of the matrix).
	* @param line            Line.
	* @returns the number of columns in the matrix at a given line.
	*/
	size_t GetNbCols(size_t line) const;

	/**
	 * Clear the files. In practice, when the files correspond to full matrices,
	 * a default value is assigned.
	 * @param val            Default value.
	 */
	void Clear(double val=NAN);

	/**
	 * Set the elements to a given value. In the case of sparse matrices, this
	 * may lead to the creation of a high number of records when the value is
	 * not null.
	 * @param val            Value to set.
	 */
	void Init(double val);

	/**
	 * Set the elements corresponding to a line to a given value. In the case of
	 * sparse matrices, this may lead to the creation of a high number of records
	 * when the value is not null.
	 * @param line           Line of the elements.
	 * @param val            Value to set.
	 */
	void InitLine(size_t line,double val);

	/**
	 * Set the elements corresponding to a column to a given value. In the case
	 * of sparse matrices, this may lead to the creation of a high number of
	 * records when the value is not null.
	 * @param col            Columns of the elements.
	 * @param val            Value to set.
	 */
	void InitCol(size_t col,double val);

private:

	/**
	 * Method called to re-created the lower file of a full matrix.
	 * @param newlines       New lines.
	 * @param newcols        New cols.
	 * @param val            Value used eventually to fill the elements created.
	 */
	void ReCreateLowerFile(size_t newlines,size_t newcols,double val);

	/**
	 * Method called to re-created the upper file of a full matrix.
	 * @param newlines       New lines.
	 * @param newcols        New cols.
	 * @param val            Value used eventually to fill the elements created.
	 */
	void ReCreateUpperFile(size_t newlines,size_t newcols,double val);

	/**
	* Method called to truncate or extend the upper file of a full matrix.
	* @param newlines        New line number.
	* @param newcols         New column number.
	*/
	void TruncLowerFile(size_t newlines,size_t newcols);

	/**
	 * Method called to truncate or extend the upper file of a full matrix.
	* @param newlines        New line number.
	* @param newcols         New column number.
	*/
	void TruncUpperFile(size_t newlines,size_t newcols);

	/**
	* Method called to truncate or extend the upper file of a full matrix.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	void ExtendLowerFile(size_t newlines,size_t newcols,bool fill,double val);

	/**
	 * Method called to truncate or extend the upper file of a full matrix.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	void ExtendUpperFile(size_t newlines,size_t newcols,bool fill,double val);

	/**
	* Modify the files of a full matrix of a given size. Depending of the cases,
	* the files are truncated, extended or recreated.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	void ModifyFull(size_t newlines,size_t newcols,bool fill,double val);

	/**
	* Recreate the file of a sparse matrix of a given size.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	* @param keepnull        Keep null elements ?
	*/
	void ReCreateSparse(size_t newlines,size_t newcols,bool keepnull);

	/**
	* Extend the files of a sparse matrix of a given size. In practice, the
	* method updates the indexes in the '.info' files.
	* @param newlines        New line number.
	* @param newcols         New column number.
	* @param fill            Elements must be filled with a value.
	* @param val             Value used eventually to fill the elements created.
	*/
	void ExtendSparse(size_t newlines,size_t newcols);

public:

	/**
	* Verify if the matrix has a given size, and modify the files if necessary:
	* - When a full matrix is reduced, the files are either recreated or reduced.
	* - When a full matrix is extended, the files are either recreated or
	* extended.
	* - When a sparse matrix is reduced, a new file is re-created and the
	*   unnecessary elements are removed.
	* - When a sparse matrix is extended, no values are filled.
	*
	* Since the operation of modifying the size of a matrix can be costly, this
	* method should be use carefully. The ideal solution is to call this method
	* before a read or write call, or at the end of a program.
	* @remark Actually, a very conservative approach is implemented that may lead
	* to unnecessary recreation of files for full matrices.
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
	inline void SaveFull(const RGenericMatrix& matrix);

	/**
	 * Save a sparse matrix.
	 * @tparam cMatrix       Class of the sparse matrix.
	 * @tparam cVector       Class of the sparse vector.
	 * @tparam cValue        Class of a value of sparse vectors and matrices.
	 * @param matrix         matrix.
	 */
	template<class cMatrix,class cVector,class cValue>
		inline void SaveSparse(const RGenericMatrix& matrix);

public:

	/**
	 * Save a matrix.
	 * @param matrix         matrix.
	 */
	void Save(const RGenericMatrix& matrix);

private:

	/**
	 * Read a value of a given element in a sparse matrix.
	 * @param i              Line of the element.
	 * @param j              Column of the element.
	 */
	double ReadSparse(size_t i,size_t j);

	/**
	 * Read a value of a given element in a sparse matrix.
	 * @param i              Line of the element.
	 * @param j              Column of the element.
	 */
	double ReadMax(size_t i,size_t j);

public:

	/**
	 * Read a value of a given element.
	 *
	 * In the case of an ascending ordered sparse matrix, \f$j\f$ represents the
	 * \f$jth\f$ greater elements of line \f$i\f$.
	 * @param i              Line of the element.
	 * @param j              Column of the element. For an ascending ordered
	 *                       sparse matrix, this parameter is the index at that
	 *                       line (and not the identifier).
	 */
	double Read(size_t i,size_t j);

private:

	/**
	 * Write a value of a given element in a sparse matrix. If an old value is
	 * already present, a null value replaces the old value in the file.
	 *
	 * In the case of an ascending ordered sparse matrix, \$j\$ doesn't represent
	 * the column of the element in the matrix, but the identifier associated to
	 * a particular value of line \f$$i$\f$.
	 * @param i              Line of the element.
	 * @param j              Column of the element.
	 * @param val            Value to write.
	 */
	void WriteSparse(size_t i,size_t j,double val);

	/**
	 * Write a value of a given element in a sparse matrix. If an old value is
	 * already present, a null value replaces the old value in the file.
	 * @param i              Line of the element.
	 * @param j              Column of the element.
	 * @param val            Value to write.
	 */
	void WriteMax(size_t i,size_t j,double val);

public:

	/**
	 * Write a value of a given element. If the matrix is sparse, the method
	 * first verify if the value is already stored.
	 *
	 * If the matrix is sparse and an old value is already present, a null
	 * value replaces the old value in the file.
	 *
	 * For an ascending ordered sparse matrix, if the number of elements for that
	 * line equals the size of the matrix, the value is only inserted if another
	 * value of that line is smaller (this latest value will then be removed).
	 * @param i              Line of the element.
	 * @param j              Column of the element. For an ascending ordered
	 *                       sparse matrix, this parameter is the identifier of
	 *                       element (and not its index in the line).
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

	/**
	 * Print the content of the file representing a sparse matrix in a
	 * human-comprehensible format. In practice, the method prints :
	 * - The index for the lines and the columns.
	 * - All the records representing the value of the matrix.
	 *
	 * It is useful for debugging purposes.
	 * @tparam S             Stream class that implements the << operator.
	 * @param stream         Stream.
	 * @param format         Format used to print the number. By default, it is "%E".
	 */
	template<class S> void PrintSparse(S& stream,const char* format="%E")
	{
		if((!Sparse)&&(!Max))
			mThrowRException(BaseURI+" is not a sparse matrix");

		off_t Pos(0);
		stream<<" index : idx, (first element in the line,last element in the line) (first element in the column,last element in the column)";
		if(Max)
			stream<<" number of elements in the column";
		stream<<std::endl;
		Index.Seek(2*sizeof(double));
		for(Pos=0;!Index.End();Pos++)
		{
			off_t firsti,lasti,firstj,lastj;
			Index>>firsti>>lasti>>firstj>>lastj;
			stream<<"  "<<Pos<<" ("<<firsti<<","<<lasti<<") ("<<firstj<<","<<lastj<<")";
			if(Max)
			{
				Index>>lastj;
				stream<<" "<<lastj;
			}
			stream<<std::endl;

		}

		stream<<"records : position (i,j) ";
		if(Max)
			stream<<"idx ";
		stream<<"value (next element in the line,next element in the column)"<<std::endl;
		File1.Seek(0);
		size_t Col(0);
		for(Pos=0;!File1.End();)
		{
			size_t i,j;
			double val;
			off_t nexti,nextj;

			File1>>val>>i>>j>>nexti>>nextj;
			RString Str(RString::Number(val,format));
			stream<<"  "<<Pos<<" ("<<i<<",";
			if(Max)
				stream<<Col<<") "<<j<<" ";
			else
				stream<<j<<") ";
			stream<<Str<<" ("<<nexti<<","<<nextj<<")"<<std::endl;
			Pos+=2*sizeof(size_t)+sizeof(double)+2*sizeof(off_t);
			if(nexti==MaxOffT)
				Col=0;
			else
				Col++;
		}
	}

	/**
	 * Destructor.
	 */
	~RMatrixStorage(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
