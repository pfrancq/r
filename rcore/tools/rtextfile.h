/*

	R Project Library

	RTextFile.h

	Text File - Header.

	(C) 1999-2002 by P. Francq.

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
#ifndef RTextFileH
#define RTextFileH


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstring.h>
#include <rio/rio.h>


//-----------------------------------------------------------------------------
namespace RIO{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* The RTextFile class implements some basic functions needed when working
* with text files.
*
* The file can contain some comments. These comments can be either on a
* single line, beginning with a given set of characters ('%', by default) and
* ending at the end of the line, or be multi-line, beginning and ending with
* corresponding set of characters (C comments delimeters by default).
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
class RTextFile
{
public:
	/**
	* The RemType enum represents the different style of comments used wich text
	* files.
	*/
	enum RemType {NoComment,SingleLineComment,MultiLineComment,SingleMultiLineComment};

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
	* Buffer containing the file (Used only if read mode).
	*/
	char *Buffer;

	/**
	* Internal pointer used to read the file (Used only if read mode).
	*/
	char *ptr;

	/**
	* All the file reads at once  (Only true is supported yet).
	*/
	bool All;

	/**
	* At NewLine? (Used only if created or append mode).
	*/
	bool NewLine;

	/**
	* This string represent a single line comment.
	*/
	RStd::RString Rem;

	/**
	* This string represent the beginning of a multi-line comment.
	*/
	RStd::RString BeginRem;

	/**
	* This string represent the ending of a multi-line comment.
	*/
	RStd::RString EndRem;

	/**
	* The type of comments that are using for this file.
	*/
	RemType CommentType;

	/**
	* The type of comments currently activ.
	*/
	RemType ActivComment;

	/**
	* This string represent a separator for different elements on the same line
	*/
	RStd::RString Separator;

	/**
	* This variable is holding the current line number.
	*/
	unsigned int Line;

	/**
	* This variable is holding the last line where something was readed (or written).
	*/
	unsigned int LastLine;

public:

	/**
	* Construct a text file.
	* @param name           The name of the file.
	* @param mode           The open mode for the file.
	*/
	RTextFile(const RStd::RString &name,RIO::ModeType mode) throw(bad_alloc,RStd::RString);

	/**
	* Construct a text file in read mode.
	* @param name           The name of the file.
	* @param all            The text file is open in one time (Only Supported yet).
	*/
	RTextFile(const RStd::RString &name,bool all=true) throw(bad_alloc,RStd::RString);

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
	* This function skip spaces (Only used if read mode).
	*/
	void SkipComments(void);

	/**
	* This function skip spaces (Only used if read mode).
	*/
	void SkipSpaces(void);

	/**
	* This function returns true if the current position is the beginning of a
	* comment (Only used if in read mode).
	*/
	bool BeginComment(void);

	/**
	* This function returns true if hte current position is the ending of a
	* comment and skip the end characters if MultiLineComment.
	*/
	bool EndComment(void);

	/**
	* Return a temporary C string.
	* @returns the C string.
	*/
	char* GetCharPtr(void);

public:

	/**
	* Return true if the file is been treated.
	*/
	inline bool Eof(void) { return(!(*ptr)); }

	/**
	* Return the next character.
	*/
	char GetNextChar(void) {return(*ptr);}

	/**
	* Get the rest of the file.
	* @param buffer         Buffer that will hold the rest of the file.
	*/
	void GetUntilEnd(char* &buffer);

	/**
	* Set the style of comments.
	* @param style          The Style.
	*/
	void SetRemStyle(RemType style) {CommentType=style;}

	/**
	* Set the string of comments when single line.
	* @param c              This string represent the begin of a comment.
	*/
	void SetRem(const char *c) {Rem=c;}

	/**
	* Set the begin and the end strings of comments when in multiline.
	* @param b              This string represent the begin of a comment.
	* @param e              This string represent the end of a comment.
	*/
	void SetRem(const char *b,const char *e) {BeginRem=b; EndRem=e; }

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
	char *GetWord(void) throw(RStd::RString);

	/**
	* Return the next entire line in the file.
	*/
	char *GetLine(void) throw(RStd::RString);

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
	* Return the actual line number.
	*/
	unsigned long ActualLine(void) {return(Line);}

	/**
	* Return the last line number where something was read.
	*/
	unsigned long GetLastLine(void) {return(LastLine);}

	/**
	* Specify the set of characters used as separator.
	*/
	void SetSeparator(const RStd::RString& str) {Separator=str;}

	/**
	* Specify the set of characters used as separator.
	*/
	void SetSeparator(const char* str) {Separator=str;}

	/**
	* Destructs the file.
	*/
	virtual ~RTextFile(void);
};


}  //-------- End of namespace RIO --------------------------------------------


//-----------------------------------------------------------------------------
// Global functions
extern "C++"
{
	/**
	* Write an end-of-line in a RTextFile.
	*/
	extern RIO::RTextFile& endl(RIO::RTextFile &file);

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
