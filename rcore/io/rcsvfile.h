/*

	R Project Library

	RCSVFile.h

	CSV File - Header.

	Copyright 2009 by the Université libre de Bruxelles.

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
#ifndef RCSVFileH
#define RCSVFileH


//------------------------------------------------------------------------------
// include files for R Project
#include <rtextfile.h>
#include <rcontainer.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RCSVFile provides a class for manipulating CSV text files. Currently,
 * these files can only be opened. When the file is opened, the fist line is
 * read. If this first line contains the headers, the programmer should called
 * the method 'Read' before reading the first values.
 * @author Pascal Francq.
 * @short CVS File.
 */
class RCSVFile : private RTextFile
{
	/**
	 * Separator of the values.
	 */
	RChar Sep;

	/**
	 * Current values.
	 */
	RContainer<RString,true,false> Values;

public:

	/**
	 * Constructor of the CSV file.
	 * @param uri            URI of the file.
	 * @param sep            Separator (comma by default).
	 * @param encoding       The encoding scheme of the file.
	 * @return
	 */
	RCSVFile(const RURI& uri,RChar sep=',',const RString& encoding="Latin1");

	/**
	 * Open the file and read the first line.
	 * @param mode           The open mode for the file.
	 */
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	 * Open the file and read the first line.
	 * @param uri            URI of the file.
	 * @param sep            Separator (comma by default).
	 * @param mode           The open mode for the file.
	 * @param encoding       The encoding scheme of the file.
	 */
	void Open(const RURI& uri,RChar sep,RIO::ModeType mode=RIO::Read,const RString& encoding="Latin1");

	/**
	 * @return true if the end of the file is reached.
	 */
	inline bool End(void) const {return(RTextFile::End());}

	/**
	 * Read and analyze a line.
	 */
	void Read(void);

	/**
	 * @return a RString corresponding to a given value of the current line.
	 * @param idx            Index of the value.
	 */
	RString Get(size_t idx) const;

	/**
	 * @return a size_t corresponding to a given value of the current line. if
	 * the value is not an integer, an exception is generated.
	 * @param idx            Index of the value.
	 */
	size_t GetSizeT(size_t idx) const;
};


}  //-------- End of namespace R ----------------------------------------------


//------------------------------------------------------------------------------
#endif
