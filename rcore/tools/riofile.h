/*

	R Project Library

	RIOFile.h

	Generic File for Input/Output - Header.

	Copyright 1999-2008 by the Université Libre de Bruxelles.

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
#ifndef RIOFile_H
#define RIOFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rfile.h>
#include <rdownload.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RIOFile class represents a file that can do some input and/or output
* tasks. Here is an example:
* @code
* #include <riofile.h>
* using namespace R;
*
* char Buffer[81];
* RIOFile In("/home/user/Test.txt");
* In.Open(RIO::Read);
* unsigned int len=In.Read(Buffer,80);
* Buffer[len]=0;
* cout<<Buffer<<endl;
* @endcode
* @author Pascal Francq
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
	size_t Size;

	/**
	* Current position in the file.
	*/
	size_t Pos;

	/**
	 * Downloader for non-local files.
	 */
	static RDownload Get;
	
	/**
	 * File which is really treated.
	 */
	RString File;
	
	/**
	 * Internal Buffer.
	 */
	char* Internal;
	
	/**
	 * Number of internal bytes to read.
	 */
	size_t InternalToRead;
	
	/**
	 * Physical (real) position in the file.
	 */
	size_t RealPos;
	
	/**
	 * Current byte to read.
	 */
	char* CurByte;
	
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
	* Open the file
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

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
	 * Read a given number of bytes from the current position of the file but
	 * without to move the internal pointers.
	 * @param buffer         Buffer (must be allocated).
	 * @param nb             Number of bytes to read.
	 * @return Number of bytes readed.
	 */
//	size_t NeutralRead(char* buffer,unsigned int nb);	

	/**
	* Read a given number of bytes at the current position of the file.
	* @param buffer         Buffer (must be allocated).
	* @param nb             Number of bytes to read.
	* @return Number of bytes readed.
	*/
	size_t Read(char* buffer,unsigned int nb,bool move=true);

	/**
	 * Read one character at the current position of the file.
	 */
//	char Read(void);
	
	/**
	* Write the first number of bytes of a buffer in the current position of
	* the file.
	* @param buffer         Buffer.
	* @param nb             Number of bytes to read.
	*/
	void Write(const char* buffer,unsigned int nb);

	/**
	* Go to a specific position of the file.
	* @param pos            Position to reach.
	*/
	void Seek(unsigned long long pos);

	/**
	* Move for a given number of bytes from the current position of the file.
	* @param pos            Relative position.
	*/
	void SeekRel(long long pos);

	/**
	* Return true if the file is at the end.
	*/
	inline bool End(void) const {return(Pos>=Size);}

	/**
	* Return the size of the file.
	*/
	inline size_t GetSize(void) const {return(Size);}

	/**
	* Return the current position in the file.
	*/
	inline size_t GetPos(void) const {return(Pos);}

	/**
	* Destructs the file.
	*/
	virtual ~RIOFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
