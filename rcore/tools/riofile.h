/*

	R Project Library

	RIOFile.h

	Generic File for Input/Output - Header.

	Copyright 1999-2005 by the Universit�Libre de Bruxelles.

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
	/**
	* Internal Handle of the file.
	*/
	int handle;

	/**
	* Size of the file.
	*/
	unsigned int Size;

	/**
	* Current position in the file.
	*/
	unsigned int Pos;

public:

	/**
	* Construct a file.
	* @param name           The name of the file.
	*/
	RIOFile(const RString& name);

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
	* Read a given number of bytes at the current position of the file.
	* @param buffer         Buffer (must be allocated).
	* @param nb             Number of bytes to read.
	* @return Number of bytes readed.
	*/
	unsigned int Read(char* buffer,unsigned int nb);

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
	void Seek(unsigned int pos);

	/**
	* Return true if the file is at the end.
	*/
	bool End(void) const;

	/**
	* Return the size of the file.
	*/
	unsigned int GetSize(void) const;

	/**
	* Return the current position in the file.
	*/
	unsigned int GetPos(void) const;

	/**
	* Destructs the file.
	*/
	virtual ~RIOFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
