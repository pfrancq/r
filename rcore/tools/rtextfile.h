/*

  RTextFile.h

  Text File - Header.

  (C) 1999-2000 by P. Francq.

  Version $Revision$

  Last Modify: $Date$

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//---------------------------------------------------------------------------
#ifndef RTEXTFILE_H
#define RTEXTFILE_H


//---------------------------------------------------------------------------
#include "rstring.h"
using namespace RStd;


//---------------------------------------------------------------------------
namespace RStd{
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
/** This class implements some basic functions needed when working with text files.
  *
	* The spaces (spaces, ends of line, tabs, feeds) are skipped when reading. The file
	* can contain some comments that are not treated when reading the file. This comments
	* bezgin with a character '%' and ending at the end of the current line.
	* @author Pascal Francq
	* @short Text File.
	*/
class RTextFile
{
	enum ModeType {Append,Read,Create};

	/** How to file has to be used. */
  ModeType Mode;
	/** Name of the File. */
	RString Name;
	/** Internal Handle of the file.*/
	int handle;
	/** Buffer containing the file (Used only if read mode).*/
	char *Buffer;
	/** Internal pointer used to read the file (Used only if read mode).*/
	char *ptr;
	/** All the file reads at once  (Only true is supported yet).*/
	bool All;
	/** At NewLine? (Used only if created or append mode).*/
  bool NewLine;

	/** This function skip spaces (Only used if read mode).*/
	void SkipSpaces(void);
public:

	/** Construct a text file.
		* @param name		The name of the file.
		* @param mode		The open mode for the file.
		*/
	RTextFile(const RString &name,ModeType mode) throw(bad_alloc,RString);

	/** Construct a text file in read mode.
		* @param name		The name of the file.
		* @param all		The text file is open in one time (Only Supported yet).
		*/
	RTextFile(const RString &name,bool all=true) throw(bad_alloc,RString);

	/** Re-initialise the file.*/
  void Init(void) throw(bad_alloc,RString);


	/** Go to the begining of the file.*/
  void Begin(void) throw(RString);

	/** Return true if the file is been treated.*/
	inline bool Eof(void) { return(!(*ptr)); }

	/** Return the next integer contained in the file.*/
	long int GetInt(void) throw(RString);

	/** Return the next float contained in the file.*/
	float GetFloat(void) throw(RString);

	/** Return the next word contained in the file. A word is a suite of characters
		* delemited by spaces.*/
	char *GetWord(void) throw(RString);

	/** Return the next entire line in the file.*/
	char *GetLine(void) throw(RString);


	/** Write an end of line in the file.*/
  void WriteLine(void) throw(RString);

	/** Write a long in the file. If the number is not the first thing on the line,
		*	a space is add before it.*/
  void WriteLong(long nb) throw(RString);

	/** Write a unsigned long in the file. If the number is not the first thing on
		* the line,	a space is add before it.*/
  void WriteULong(unsigned long nb) throw(RString);

	/** Write a string in the file. If the string is not the first thing on the line,
		*	a space is add before it.*/
  void WriteStr(char *c) throw(RString);

	/** Write a bool in the file as '1' or '0' depend on his value. If the number is
		* not the first thing on the line, a space is add before it.*/
  void WriteBool(bool b) throw(RString);

	/** Write the time in the file. If the time is not the first thing on the line,
		*	a space is add before it.*/
  void WriteTime(void) throw(RString);

	/** Write a log entry in the file. First the time is written, and then the entry.
		* The entry is alone on a line, so end-of-lines are inserted if ncessary before
		* or after it.*/
  void WriteLog(char *entry) throw(RString);
  
	/** Destructs the file.*/
	~RTextFile(void);
};


}  //-------- End of namespace RStd ---------------------------------------


//---------------------------------------------------------------------------
#endif
