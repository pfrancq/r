/*

	R Project Library

	RIOFile.h

	Generic File for Input/Output - Header.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Universit√© Libre de Bruxelles (ULB).

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
#ifndef RIOFile_H
#define RIOFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rio.h>
#include <rfile.h>
#include <rdownload.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Macros proposed by Christian Biere:
// http://osdir.com/ml/os.netbsd.devel.general/2007-02/msg00000.html
#define MAX_INT_VAL_STEP(t) \
        ((t) 1 << (CHAR_BIT * sizeof(t) - 1 - ((t) -1 < 1)))
#define MAX_INT_VAL(t) \
        ((MAX_INT_VAL_STEP(t) - 1) + MAX_INT_VAL_STEP(t))


//------------------------------------------------------------------------------
/**
 * Constant representing the maximum size of off_t variable.
 */
const off_t MaxOffT=MAX_INT_VAL(off_t);


//------------------------------------------------------------------------------
/**
* The RIOFile class represents a file that can do some input and/or output
* tasks. To read, the class works with an internal buffer of 10 Kb.
*
* Here is an example:
* @code
* #include <riofile.h>
* using namespace R;
*
* char Buffer[81];
* RIOFile In("/home/user/Test.txt");
* In.Open(RIO::Read);
* size_t len=In.Read(Buffer,80);
* Buffer[len]=0;
* cout<<Buffer<<endl;
* @endcode
* \note
* When the file is read, an internal buffer is used to avoid to many accesses to
* the disk. When something must be written, it is first physically written on
* the disk and, if the file can also be read, the internal buffer is updated.
* @short Generic File for Input/Output.
*/
class RIOFile : public RFile
{
	/**
	* Internal Handle of the file.
	*/
	int Handle;

	/**
	* Size of the file.
	*/
	off_t Size;

	/**
	* Current (virtual) position in the file.
	*/
	off_t Pos;

	/**
	 * Downloader for non-local files.
	 */
	static RDownloadFile Get;

	/**
	 * File which is really treated.
	 */
	RURI File;

	/**
	 * Internal Buffer.
	 */
	char* Internal;

	/**
	 * Number of bytes left to read in the internal buffer.
	 */
	size_t InternalToRead;

	/**
	 * Physical (real) position in the file.
	 */
	off_t RealPos;

	/**
	 * Physical (real) position of the first byte of the internal buffer.
	 */
	off_t RealInternalPos;

	/**
	 * Current byte to read.
	 */
	char* CurByte;

	/*
	 * Internal test method.
	 */
	inline void Test(void);

protected:

	/**
	* It is possible to write in the file.
	*/
	bool CanWrite;

	/**
	* It is possible to read from the file.
	*/
	bool CanRead;

public:

	/**
	* Default constructor for a file.
	*/
	RIOFile(void);

	/**
	* Construct a file.
	* @param uri             URI of the file.
	*/
	RIOFile(const RURI& uri);

	/**
	* Copy constructor of a file.
	* @param file           File.
	*/
	RIOFile(RIOFile& file);

	/**
	 * When the URI associated to a RIOFile is not local, it is downloaded and
	 * stored in a temporary file (which becomes the real name of the file).
	 * @return the real name of the file.
    */
	RURI GetRealName(void) const {return(File);}

	/**
	* Open the file
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode);

	/**
	* Open the file
	* @param uri             URI of the file.
	* @param mode            The open mode for the file.
	*/
	void Open(const RURI& uri,RIO::ModeType mode);

	/**
	* Verify if the file is opened or not.
	* @return true if the file is open.
	*/
	bool IsOpen(void) const;

	/**
	* Close the file.
	*/
	virtual void Close(void);

	/**
	* Read a given number of bytes at the current position of the file.
	* @param buffer         Buffer (must be allocated).
	* @param nb             Number of bytes to read.
	* @param move           The file position is moved (default).
	* @return Number of bytes read.
	*/
	size_t Read(char* buffer,size_t nb,bool move=true);

	/**
	* Write the first number of bytes of a buffer in the current position of
	* the file.
	* @param buffer         Buffer.
	* @param nb             Number of bytes to read.
	*/
	void Write(const char* buffer,size_t nb);

	/**
	* Go to a specific position of the file.
	* @param pos            Position to reach.
	*/
	virtual void Seek(off_t pos);

	/**
	* Move for a given number of bytes from the current position of the file.
	* @param pos            Relative position.
	*/
	virtual void SeekRel(off_t pos);

	/**
	* Go to the end of the file.
	*/
	virtual void SeekToEnd(void);

	/**
	 * Truncate the file.
	 * @param newsize        New size.
	 */
	void Truncate(off_t newsize);

	/**
	* Return true if the file is at the end.
	*/
	inline bool End(void) const {return(Pos>=Size);}

	/**
	* Return the size of the file.
	*/
	inline off_t GetSize(void) const {return(Size);}

	/**
	* Return the current position in the file.
	*/
	inline off_t GetPos(void) const {return(Pos);}

	/**
	* Destruct the file.
	*/
	virtual ~RIOFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
