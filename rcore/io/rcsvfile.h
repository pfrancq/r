/*

	R Project Library

	RCSVFile.h

	CSV File - Header.

	Copyright 2009-2014 by Pascal Francq (pascal@francq.info).

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
#ifndef RCSVFileH
#define RCSVFileH


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RCSVFile provides a class for manipulating CSV text files. Currently,
 * these files can only be opened. When the file is opened, the fist "CSV line"
 * is read. An end of line character inside a field delimited by quotes are
 * part of the value and not the end of a "CSV line"
 *
 * Here is an example of use:
 * @code
 * RCSVFile File("test.csv",';',"utf-8");
 * File.Open();
 * cout<<"Field1"<<"\t"<<"Field2"<<endl;  // Print the header
 * while(!File.End())
 * {
 *    File.Read();
 *    cout<<File.Get(0)<<"\t"<<File.Get(1)<<endl;
 * }
 * @endcode
 * @remark If this first line contains the headers, the programmer should
 * called the method 'Read' before reading the first values.
 * @author Pascal Francq.
 * @short CVS File.
 */
class RCSVFile : private RTextFile
{
	/**
	 * Separator of the values.
	 */
	RChar Sep;

	/**
	 * Can the columns be delimited by quotes.
	 */
	bool Quotes;

	/**
	 * Escape character
	 */
	RChar Escape;

	/**
	 * Current values.
	 */
	RContainer<RString,true,false> Values;

	/**
	 * Number of values on a line.
	 */
	size_t NbValues;

	/**
	 * Internal Buffer.
	 */
	RChar* Internal;

	/**
	 * Number of characters in the buffer.
	 */
	size_t SizeBuffer;

	/**
	 * Current position in the buffer.
	 */
	RChar* Buffer;

	/**
	 * Current value read.
    */
	RString* CurValue;

	/**
	 * Current column.
    */
	int CurCol;

	/**
	 * Current character.
    */
	RChar CurChar;

	/**
	 * Is a value currently read?
	 */
	bool ReadValue;

	/**
	 * Character that will represent the end of a value.
	 */
	RChar EndValueChar;

public:

	/**
	 * Constructor of the CSV file.
	 * @param uri            URI of the file.
	 * @param sep            Separator (comma by default).
	 * @param quotes         Can columns be delimited by quotes ?
	 * @param encoding       The encoding scheme of the file.
	 * @return
	 */
	RCSVFile(const RURI& uri,RChar sep=',',bool quotes=true,const RCString& encoding="utf-8");

	/**
	* Return the URI of the file including.
	*/
	inline const RURI GetURI(void) const {return(RTextFile::GetURI());}

	/**
	 * Open the file.
	 * @param mode           The open mode for the file.
	 */
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	 * Open the file.
	 * @param uri            URI of the file.
	 * @param sep            Separator (comma by default).
	 * @param quotes         Can columns be delimited by quotes ?
	 * @param mode           The open mode for the file.
	 * @param encoding       The encoding scheme of the file.
	 */
	virtual void Open(const RURI& uri,RChar sep=',',bool quotes=true,RIO::ModeType mode=RIO::Read,const RCString& encoding="utf-8");

	/**
	 * Close the file.
	 */
	virtual void Close(void);

	/**
	 * @return true if the end of the file is reached.
	 */
	inline bool End(void) const {return(RTextFile::End());}

	/**
	 * Get the number of the current line.
    * @return current line.
    */
	unsigned int GetLineNb(void) const {return(RTextFile::GetLineNb());}

private:

	/**
	 * Create a new value to read and assign it the CurValue.
	 */
	inline void NewValue(void);

	/**
	 * Add the content of the buffer to CurValue and reset the buffer.
    */
	inline void AddBuffer(void);

	/**
	 * Add the current character to the buffer.
    */
	inline void AddToBuffer(void);

	/**
	 * Read the current character in CurChar and increment CurCol.
    */
	inline void NextChar(void);

	/**
	 * Look if there is some values to read at thr current column. In practice,
	 * the method is called at the beginning and after a separation.
	 * @return true if the end of line is reached.
    */
	inline bool IsEol(void);

public:

	/**
	 * Read and analyze a line.
	 */
	void Read(void);

	/**
	 * @return a RString corresponding to a given value of the current line.
	 * @param idx            Index of the value.
	 */
	RString Get(size_t idx) const;

	/**
	 * @return a size_t corresponding to a given value of the current line. if
	 * the value is not an integer, an exception is generated.
	 * @param idx            Index of the value.
	 * @param zero           Specify if an empty field must be considered as a
	 *                       zero (true) or generate an exception (false).
	 */
	size_t GetSizeT(size_t idx,bool zero=true) const;

	/**
	 * @return a double corresponding to a given value of the current line. if
	 * the value is not an number, an exception is generated.
	 * @param idx            Index of the value.
	 * @param zero           Specify if an empty field must be considered as a
	 *                       zero (true) or generate an exception (false).
	 * 	 */
	double GetDouble(size_t idx,bool zero=true) const;

	/**
	 * @return the number of values at the last line read.
	 */
	size_t GetNbValues(void) const {return(NbValues);}

	/**
	 * Verify if the current line has a value for a given index.
    * @param idx            Index to verify.
    * @return true if a value exists.
    */
	bool HasValue(size_t idx) const {return(idx<NbValues);}

	/**
	 * Destruct the file.
	 */
	virtual ~RCSVFile(void);
};


}  //-------- End of namespace R ----------------------------------------------


//------------------------------------------------------------------------------
#endif
