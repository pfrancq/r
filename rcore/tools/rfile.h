/*

	R Project Library

	RFile.h

	Generic File - Header.

	Copyright 1999-2004 by the Université libre de Bruxelles.

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
#ifndef RFile_H
#define RFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rstd/rio.h>


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
* @author Pascal Francq
* @short Generic File.
*/
class RFile
{
protected:

	/**
	* How to file has to be used.
	*/
	RIO::ModeType Mode;

	/**
	* Name of the File.
	*/
	RString Name;

public:

	/**
	* Construct a file.
	* @param name           The name of the file.
	*/
	RFile(const RString &name);

	/**
	* Copy constructor of a file.
	* @param file           File.
	*/
	RFile(RFile& file);

	/**
	* Open the file
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

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
	* Compare the name of a file with a given string.
	* @see R::RContainer
	* @param name            String.
	* @return int
	*/
	int Compare(const RString& name) const;

	/**
	* Return the name of the file.
	* @returns a string containing the name.
	*/
	const RString& GetName(void) const;

	/**
	* Get the separator used for the directories.
	*/
	static RChar GetDirSeparator(void);

	/**
	* Destructs the file.
	*/
	virtual ~RFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
