/*

	R Project Library

	RRecFile.h

	Binary file for records - Header.

	(C) 2002 by P. Francq.

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



//-----------------------------------------------------------------------------
#ifndef RRecFileH
#define RRecFileH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rio/rio.h>


//-----------------------------------------------------------------------------
namespace RIO{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RRecFile class implements some basic functions needed when working
* with binary files for records.
*
* When multiple elements are inserted on the same text line, before each
* insertion, and except for the first one, a set of characters representing
* a separator (' ' by default) are added.
*
* The user can write his own operators to read and write with RTextFile. Here
* is an example:
* <pre>
* class Point
* {
* public:
* 	int X,Y;
* }
*
* RTextFile& operator<<(RTextFile &f,Point &pt)
* {
* 	return(f<<pt.X<<pt.Y);
* }
*
* RTextFile& operator>>(RTextFile &f,Point &pt)
* {
* 	return(f>>pt.X>>pt.Y);
* }
* </pre>
* @author Pascal Francq
* @short Text File.
*/
template<class C,unsigned int S,bool bOrder=false>
	class RRecFile
{
protected:

	/**
	* How to file has to be used.
	*/
	RIO::ModeType Mode;

	/**
	* Name of the File.
	*/
	RStd::RString Name;

	/**
	* Internal Handle of the file.
	*/
	int handle;

	/**
	* This variable is holding the record number.
	*/
	unsigned int NbRecs;

	/**
	* Is it end of file?
	*/
	bool eof;

	/**
	* Current record;
	*/
	C Current;

public:

	/**
	* Construct a binary file.
	* @param name           The name of the file.
	* @param mode           The open mode for the file.
	*/
	RRecFile(const RStd::RString &name,RIO::ModeType mode) throw(bad_alloc,RStd::RString);

public:

	/**
	* Return true if the file is been treated.
	*/
	inline bool Eof(void) {return(eof);}

	/**
	* >> Operator for unsigned int.
	*/
	RRecFile& operator>>(unsigned int& nb) throw(RStd::RString);

	/**
	* >> Operator for a double.
	*/
	RRecFile& operator>>(double& nb) throw(RStd::RString);

	/**
	* << Operator for unsigned char.
	*/
	RRecFile& operator<<(const unsigned char nb) throw(RStd::RString);

	/**
	* << Operator for unsigned int.
	*/
	RRecFile& operator<<(const unsigned int nb) throw(RStd::RString);

	/**
	* << Operator for unsigned long.
	*/
	RRecFile& operator<<(const unsigned long nb) throw(RStd::RString);

	/**
	* << Operator for double.
	*/
	RRecFile& operator<<(const double d) throw(RStd::RString);

	/**
	* Read a record.
	* @return true if the record could be read.
	*/
	bool Read(C& rec) throw(RStd::RString);

	/**
	* This function returns the index of an element represented by tag, and it
	* is used when the elements are to be ordered.
	* @param TUse           The type of tag, the container uses the Compare(TUse &)
	*                       member function of the elements.
	* @param tag            The tag used.
	* @param Find           If the element represented by tag exist, bFind is set to
	*                       true.
	* @return Returns the index of the element if it exists orthe index where
	* is has to inserted.
	*/
	template<class TUse> unsigned int GetId(const TUse tag,bool &Find);

	/**
	* Get the Record in the parameter.
	* @returns true if the record was found.
	*/
	bool GetRec(C& Rec) throw(RStd::RString);

	/**
	* Destructs the file.
	*/
	virtual ~RRecFile(void);
};


//---------------------------------------------------------------------------
// inline implementation
#include <rio/rrecfile.hh>


}  //-------- End of namespace RIO --------------------------------------------


//-----------------------------------------------------------------------------
#endif
