/*

	R Project Library

	RFile.h

	Generic File - Header.

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
#ifndef RFile_H
#define RFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <ruri.h>
#include <rio.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward declaration
class RIOFile;
class RDir;


//------------------------------------------------------------------------------
/**
* The RFile class represents a file (text files, directories...).
* @short Generic File.
*/
class RFile
{
protected:

	/**
	* How the file has to be used.
	*/
	RIO::ModeType Mode;

	/**
	* URI of the File.
	*/
	RURI URI;

public:

	/**
	 * Default constructor.
	 */
	RFile(void);

	/**
	* Construct a file.
	* @param uri             URI of the file.
	*/
	RFile(const RURI& uri);

	/**
	* Copy constructor of a file.
	* @param file           File.
	*/
	RFile(const RFile& file);

	/**
	* Open the file
	* @param mode            The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode);

	/**
	* Open the file
	* @param uri             URI of the file.
	* @param mode            The open mode for the file.
	*/
	void Open(const RURI& uri,RIO::ModeType mode);

	/**
	* Close the file.
	*/
	virtual void Close(void);

	/**
	* Compare two files by comparing their name.
	* @see R::RContainer
	* @param file            File.
	* @return int
	*/
	int Compare(const RFile& file) const;

	/**
	* Compare two files by comparing their name.
	* @see R::RContainer
	* @param file            File.
	* @return int
	*/
	int Compare(const RFile* file) const;

	/**
	* Compare the uri of a file with a given string.
	* @see R::RContainer
	* @param uri             URI.
	* @return int
	*/
	int Compare(const RString& uri) const;

	/**
	* Return the URI of the file including.
	*/
	const RURI& GetURI(void) const {return(URI);}

	/**
	* Set the URI of the file.
	* @param uri             URI of the file.
	*/
	void SetURI(const RURI& uri);

	/**
	* Return the name of the file. If the fullname doesn't have a slash at the
	* end, the part between the last slash and the end of the fullname is
	* considered to be the file name. If the fullname has a slash at the end,
	* an empty string is returned here.
	* @returns a string containing the name.
	*/
	const RString GetFileName(void) const;

	/**
	* Get the separator used for the directories.
	*/
	static RChar GetDirSeparator(void);

	/**
	* Remove a file from the disk.
	* @param uri             URI of the file.
	*/
	static void RemoveFile(const RURI& uri);

	/**
	* Rename a file from the disk.
	* @param olduri          Old URI of the file.
	* @param newuri          New URI of the file.
	*/
	static void RenameFile(const RURI& olduri,const RURI& newuri);

	/**
	* Get a name for a temporary file.
	* @attention Under Unix-like OS (such as Linux), a empty file is created by
	* the OS to ensure that a given name cannot be used by multiple calls. It can
	* be necessary to delete manually the file (normally, the temporary directory
	* is automatically emptied).
	* @return Name of the file.
	*/
	static RURI GetTempFile(void);

	/**
	 * Test if an URI correspond to an existing file or directory that can be
	 * accessed. If the URI is correct but the calling application has the not
	 * the correct rights, the function returns false.
	 * @param uri            URI to test.
	 */
	static bool Exists(const RURI& uri);

	/**
	 * Test if an URI is a directory or not. If the directory exists but the
	 * calling application has the not the correct rights, the function returns
	 * false.
	 * @param uri            URI to test.
	 */
	static bool IsDir(const RURI& uri);

	/**
	* Destruct the file.
	*/
	virtual ~RFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
