/*

	R Project Library

	RIO.cpp

	Generic Class - Header.

	Copyright 2002-2015 by Pascal Francq (pascal@francq.info).
	Copyright 2002-2008 by the Université Libre de Bruxelles (ULB).

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
#ifndef RIO_H
#define RIO_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <ruri.h>
#include <rexception.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward declaration
class RFile;
class RIOFile;
class RTextFile;


//------------------------------------------------------------------------------
/**
* The RIO class provides some I/O types.
* @short I/O Types.
*/
class RIO
{
public:

	/**
	* The ModeType enum represents the different mode that can be used to open a
	* file.
	* @short File Access Mode
	*/
	enum ModeType
	{
		Append                       /** Append to an existing file. If the file
		                                 does not exist, it is created.*/,
		Read                         /** Open a file to read it.*/,
		Write                        /** Open a file to write it. If the file
		                                 doesn't exist, it is created.*/,
		ReadWrite                    /** Open a file to read and write it. If
		                                 the file doesn't exist, it is created.*/,
		Create                       /** Create a new file. If the file already
		                                 exists, it is destruct.*/,
		Undefined                    /** The mode of the file is undefined.*/
	};
};


//------------------------------------------------------------------------------
/**
 * This class provides a "smart" temporary file. In practice, once the name of
 * a temporary file is asked, when the instance is deleted, the temporary file
 * is created. Of course, the temporary file may be explicitly leave if
 * necessary.
 *
 * It can be useful in an exception environment, since temporary files are
 * always removed.
 * @code
 * void BadFunction(void)
 * {
 * 	RSmartTempFile Tmp;
 *
 * 	RTextFile Text(Tmp.GetName());
 * 	Text.Open(RIO::Create);
 * 	int Wrong (2/0); // This will generate an exception -> Temporary file is removed
 * }
 * @endcode
 * @short Smart Temporary File.
 */
class RSmartTempFile
{
	/**
	 * Name of the temporary file.
	 */
	RURI Name;

	/**
	 * Name requested?
	 */
	bool Requested;

	/**
	 * Temporary file must be removed.
	 */
	bool Remove;

public:

	/**
	 * Constructor.
	 */
	RSmartTempFile(void);

	/**
	 * Get the name of the temporary file.
	 * @param remove     File must be removed?
	 */
	RURI GetName(bool remove=true);

	/**
	 * Destruct. If the name was requested, the temporary file is
	 * removed.
	 */
	~RSmartTempFile(void);
};


//------------------------------------------------------------------------------
/**
* The RIOException class provides a basic representation for an IO exception.
* @short IO Exception.
*/
class RIOException : public RException
{
public:

	/**
	* Construct an IO exception.
	* @param str                      Message of the error.
	*/
	RIOException(const RString& str);

	/**
	* Construct an exception with the message "func [where]: str". A typical use is:
	* @code
	* if(!ptr)
	* 	throw RIOException(__PRETTY_FUNCTION__,__LINE__,"ptr cannot be a null pointer");
	* @endcode
	* @see The mThrowRIOException marco.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	RIOException(const RString& func,long where,const RString& str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param str                      Message of the error.
	*/
	RIOException(const RFile* file,const RString& str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	RIOException(const RFile* file,const RString& func,long where,const RString& str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param str                      Message of the error.
	*/
	RIOException(const RIOFile* file,const RString& str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	RIOException(const RIOFile* file,const RString& func,long where,const RString& str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param str                      Message of the error.
	*/
	RIOException(const RTextFile* file,const RString& str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param func                     Function producing the error.
	* @param where                    Line position of the error.
	* @param str                      Message of the error.
	*/
	RIOException(const RTextFile* file,const RString& func,long where,const RString& str);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
// Macro to generate a RIOException
#define mThrowRIOException(file,msg) throw R::RIOException(file,__PRETTY_FUNCTION__,__LINE__,msg)


//------------------------------------------------------------------------------
#endif
