/*

	R Project Library

	RDir.h

	Directory - Header.

	Copyright 2004 by the Universit�Libre de Bruxelles.

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




//-----------------------------------------------------------------------------
#ifndef RDir_H
#define RDir_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rfile.h>
#include <rstd/rcontainer.h>
#include <rstd/rcursor.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


/**
*/
class RDir : public RFile
{
	struct Internal;

	/**
	* Internal structure representing a directory.
	*/
	Internal* Data;

	/**
	* Entries of the directory.
	*/
	RContainer<RFile,true,true> Entries;

public:

	/**
	* Construct a directory.
	* @param name           The name of the directory.
	*/
	RDir(const RString& name);

	/**
	* Open the directory in a given mode.
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

private:

	/**
	* Read the entries of the directory.
	*/
	void OpenEntries(void);

public:

	/**
	* Get a cursor on the entries of the directory.
	*/
	RCursor<RFile> GetEntries(void) const;

	/**
	* Close the file.
	*/
	virtual void Close(void);

	/**
	* Destruct the directory object.
	*/
	virtual ~RDir(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif

