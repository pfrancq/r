/*

	R Project Library

	RIOFile.h

	Generic File for Input/Output - Header.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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
#include <rstd/rfile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The RIOFile class represents a file that can do some input and/or output
* tasks.
* @author Pascal Francq
* @short Generic File for Input/Output.
*/
class RIOFile : public RFile
{
protected:

	/**
	* Internal Handle of the file.
	*/
	int handle;

public:

	/**
	* Construct a file.
	* @param name           The name of the file.
	*/
	RIOFile(const RString &name);

	/**
	* Open the file
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=Read);

	/**
	* Close the file.
	*/
	virtual void Close(void);

	/**
	* Read a given number of bytes at the current position of the file.
	* @param buffer         Buffer (must be allocated).
	* @param nb             Number of bytes to read.
	*/
	void Read(char* buffer,unsigned int nb);

	/**
	* Write the first number of bytes of a buffer in the current position of
	* the file.
	* @param buffer         Buffer.
	* @param nb             Number of bytes to read.
	*/
	void Write(char* buffer,unsigned int nb);

	/**
	* Destructs the file.
	*/
	virtual ~RIOFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
