/*

	R Project Library

	RIO.cpp

	Generic Class - Header.

	Copyright 2002-2005 by the Université Libre de Bruxelles.

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
#ifndef RIO_H
#define RIO_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>


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
* @author Pascal Francq
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

	/**
	 * This class provides a "smart" temporary file. In practice, if the class
	 * is asked for a temporary file, this file is automatically removed. The
	 * temporary file may be explicitly leave. It can be useful in an exception
	 * environment, since temporary files are always removed.
	 * @author Pascal Francq
	 * @short Smart Temporary File.
	 */
	class RSmartTempFile
	{
		/**
		 * Name of the temporary file.
		 */
		R::RString Name;

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
		RString GetName(bool remove=true);

		/**
		 * Destruct. If the name was requested, the temporary file is
		 * removed.
		 */
		~RSmartTempFile(void);
	};
};


//------------------------------------------------------------------------------
/**
* The RIOException class provides a basic representation for an IO exception.
* @author Pascal Francq
* @short IO Exception.
*/
class RIOException : public RException
{
public:

	/**
	* Construct an IO exception.
	* @param str                      Message of the error.
	*/
	RIOException(const char* str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param str                      Message of the error.
	*/
	RIOException(const RFile* file,const char* str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param str                      Message of the error.
	*/
	RIOException(const RIOFile* file,const char* str);

	/**
	* Construct an IO exception.
	* @param file                     File process when the error occurs.
	* @param str                      Message of the error.
	*/
	RIOException(const RTextFile* file,const char* str);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
