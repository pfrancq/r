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
	* This variable is holding the current record number.
	*/
	unsigned int Rec;

public:

	/**
	* Construct a binary file.
	* @param name           The name of the file.
	* @param mode           The open mode for the file.
	*/
	RRecFile(const RStd::RString &name,RIO::ModeType mode) throw(bad_alloc,RStd::RString);

	/**
	* Construct a binary file in read mode.
	* @param name           The name of the file.
	*/
	RRecFile(const RStd::RString &name) throw(bad_alloc,RStd::RString);

	/**
	* Re-initialise the file.
	*/
	void Init(void) throw(bad_alloc,RStd::RString);

	/**
	* Go to the begining of the file.
	*/
	void Begin(void) throw(RStd::RString);

protected:

	/**
	* Return a temporary C string.
	* @returns the C string.
	*/
	char* GetCharPtr(void);

public:

	/**
	* Return true if the file is been treated.
	*/
	inline bool Eof(void);

	/**
	* Return the next integer contained in the file.
	*/
	long GetInt(void) throw(RStd::RString);

	/**
	* Return the next unsigned integer contained in the file.
	*/
	unsigned long GetUInt(void) throw(RStd::RString);

	/**
	* >> Operator for char.
	*/
	RTextFile& operator>>(char& nb) throw(RStd::RString);

	/**
	* >> Operator for unsigned char.
	*/
	RTextFile& operator>>(unsigned char& nb) throw(RStd::RString);

	/**
	* >> Operator for short.
	*/
	RTextFile& operator>>(short& nb) throw(RStd::RString);

	/**
	* >> Operator for unsigned short.
	*/
	RTextFile& operator>>(unsigned short& nb) throw(RStd::RString);

	/**
	* >> Operator for int.
	*/
	RTextFile& operator>>(int& nb) throw(RStd::RString);

	/**
	* >> Operator for unsigned int.
	*/
	RTextFile& operator>>(unsigned int& nb) throw(RStd::RString);

	/**
	* >> Operator for long.
	*/
	RTextFile& operator>>(long& nb) throw(RStd::RString);

	/**
	* >> Operator for unsigned long.
	*/
	RTextFile& operator>>(unsigned long& nb) throw(RStd::RString);

	/**
	* Return the next float contained in the file.
	*/
	float GetFloat(void) throw(RStd::RString);

	/**
	* >> Operator for float.
	*/
	RTextFile& operator>>(float& nb) throw(RStd::RString);

	/**
	* >> Operator for a double.
	*/
	RTextFile& operator>>(double& nb) throw(RStd::RString);

	/**
	* Return the next word contained in the file. A word is a suite of characters
	* delemited by spaces.
	*/
	char* GetWord(void) throw(RStd::RString);

	/**
	* Return the next entire line in the file.
	*/
	char* GetLine(void) throw(RStd::RString);

	/**
	* Write an end of line in the file.
	*/
	void WriteLine(void) throw(RStd::RString);
	
	/**
	* Write a long in the file. If the number is not the first thing on the line,
	* a space is add before it.
	*/
	void WriteLong(const long nb) throw(RStd::RString);

	/**
	* << Operator for char.
	*/
	RTextFile& operator<<(const char nb) throw(RStd::RString);

	/**
	* << Operator for short.
	*/
	RTextFile& operator<<(const short nb) throw(RStd::RString);

	/**
	* << Operator for int.
	*/
	RTextFile& operator<<(const int nb) throw(RStd::RString);

	/**
	* << Operator for long.
	*/
	RTextFile& operator<<(const long nb) throw(RStd::RString);

	/**
	* Write a unsigned long in the file. If the number is not the first thing on
	* the line, a separator is add before it.
	*/
	void WriteULong(const unsigned long nb) throw(RStd::RString);

	/**
	* << Operator for unsigned char.
	*/
	RTextFile& operator<<(const unsigned char nb) throw(RStd::RString);

	/**
	* << Operator for unsigned int.
	*/
	RTextFile& operator<<(const unsigned int nb) throw(RStd::RString);

	/**
	* << Operator for unsigned long.
	*/
	RTextFile& operator<<(const unsigned long nb) throw(RStd::RString);

	/**
	* Write a string in the file. If the string is not the first thing on the
	* line, a separator is add before it.
	*/
	void WriteStr(const char *c) throw(RStd::RString);

	/**
	* Write a string of a give length in the file.
	*/
	void WriteStr(const char *c,unsigned int l) throw(RStd::RString);

	/**
	* << Operator for char *.
	*/
	RTextFile& operator<<(const char *c) throw(RStd::RString);

	/**
	* Write a string in the file. If the string is not the first thing on the
	* line, a separator is add before it.
	*/
	void WriteStr(const RStd::RString &str) throw(RStd::RString);

	/**
	* << Operator for RStd::RString.
	*/
	RTextFile& operator<<(const RStd::RString &str) throw(RStd::RString);

	/**
	* Write a bool in the file as '1' or '0' depend on his value. If the number
	* is not the first thing on the line, a separator is add before it.
	*/
	void WriteBool(const bool b) throw(RStd::RString);

	/**
	* << Operator for bool.
	*/
	RTextFile& operator<<(const bool b) throw(RStd::RString);

	/**
	* Write a char in the file.
	*/
	void WriteChar(const char c) throw(RStd::RString);

	/**
	* Write a double
	*/
	void WriteDouble(const double d) throw(RStd::RString);

	/**
	* << Operator for double.
	*/
	RTextFile& operator<<(const double d) throw(RStd::RString);

	/**
	* Write the time in the file. If the time is not the first thing on the line,
	* a separator is add before it.
	*/
	void WriteTime(void) throw(RStd::RString);

	/**
	* Write a log entry in the file. First the time is written, and then the
	* entry. The entry is alone on a line, so end-of-lines are inserted if
	* ncessary before or after it.
	*/
	void WriteLog(const char *entry) throw(RStd::RString);

	/**
	* Return the actual recnumber number.
	*/
	unsigned long ActualRec(void) {return(Rec);}


	/**
	* Destructs the file.
	*/
	virtual ~RRecFile(void);
};


}  //-------- End of namespace RIO --------------------------------------------


//-----------------------------------------------------------------------------
// Global functions
extern "C++"
{
	/**
	* Write the current time and date in a RTextFile.
	*/
	extern RIO::RTextFile& Time(RIO::RTextFile &file);

	/**
	* Operator needed for using generic writing functions.
	*/
	inline RIO::RTextFile& operator<<(RIO::RTextFile& o, RIO::RTextFile& (&f)(RIO::RTextFile&)) {return(f(o));}
}


//-----------------------------------------------------------------------------
#endif
