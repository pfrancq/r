/*

	R Project Library

	RIO.cpp

	Generic Class - Header.

	Copyright 2002-2003 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

	Version $Revision$

	Last Modify: $Date$

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
#include <rstd/rstd.h>
#include <rstd/rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* The ModeType enum represents the different mode that can be used to open a
* file.
*/
enum ModeType
{
	Append,
	Read,
	Create
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
	RIOException(const char* str) throw(bad_alloc);
};



}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
