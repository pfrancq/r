/*

	R Project Library

	RTextFile.h

	Text File - Header.

	Copyright 1999-2005 by the Université libre de Bruxelles.

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
#ifndef RTextFile_H
#define RTextFile_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/riofile.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Forward class declaration
class RTextEncoding;


//------------------------------------------------------------------------------
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
* @code
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
* @endcode
* @author Pascal Francq
* @short Text File.
*/
class RTextFile : public RIOFile
{
public:
	/**
	* The RemType enum represents the different style of comments used wich text
	* files. The comments are skipped when the file is read.
	*/
	enum RemType
	{
		NoComment                /** No comments are possible. */,
		SingleLineComment        /** Only single line comments are possible. */,
		MultiLineComment         /** Only multiple line comments are possible. */,
		SingleMultiLineComment   /** Both type of comments are possible. */
	};

protected:

	/**
	* Buffer containing the file (Used only if read mode).
	*/
	char* Buffer;

	/**
	* Internal pointer used to read the file (Used only if read mode).
	*/
	char* ptr;

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
	RString Rem;

	/**
	* This string represent the beginning of a multi-line comment.
	*/
	RString BeginRem;

	/**
	* This string represent the ending of a multi-line comment.
	*/
	RString EndRem;

	/**
	* The type of comments that are using for this file.
	*/
	RemType CommentType;

	/**
	* The type of comments currently activ.
	*/
	RemType ActivComment;

protected:

	/**
	* This string represent a separator for different elements on the same line
	*/
	RString Separator;

	/**
	* This variable is holding the current line number.
	*/
	unsigned int Line;

	/**
	* This variable is holding the last line where something was readed (or written).
	*/
	unsigned int LastLine;

	/**
	* Enconding of the text file.
	*/
	RTextEncoding* Codec;

	/**
	* Current character in the buffer.
	*/
	RChar Cur;

	/**
	* Number of bytes used to code the current character.
	*/
	unsigned int CurLen;

	/**
	* Number of bytes left in the file.
	*/
	unsigned int Len;

	/**
	* Total number of bytes in the file.
	*/
	unsigned int TotalLen;

public:

	/**
	* Construct a text file.
	* @param name           The name of the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RTextFile(const RString &name,const RString& encoding="Latin1");

	/**
	* Construct a text file.
	* @param file           A generic input/output file that should be treated
	*                       as text file.
	* @param encoding       The encoding scheme of the file.
	*/
	RTextFile(RIOFile& file,const RString& encoding="Latin1");

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
	* Go to the begining of the file.
	*/
	void Begin(void) throw(RIOException);

	/**
	* Return the current character.
	*/
	RChar GetCur(void) const {return(Cur);}

	/**
	* Goes to the next character.
	*/
	void Next(void) throw(RIOException);

	/**
	* Return the next character but without to move internal pointer of the
	* file.
	*/
	RChar GetNextCur(void);

	/**
	* Get the rest of the file.
	* @return A RString containing the file.
	*/
	RString GetUntilEnd(void);

protected:

	/**
	* Read a character of variable size and store the resulting char in a RChar.
	* If the character to be read is Invalid, it is skipped and the function
	* returns the number of characters skipped.
	* @param ptr            Pointer to a buffer of character to read.
	* @param len            Number of character in the buffer. After the call,
	*                       this parameter contains the number bytes read.
	* @param read           The resulting character
	* @return unsigned int  return the number of bytes that have been skipped.
	*			0 is returned when the char has been read
	*			x is returned : the number of char that have been skipped.
	*/
	unsigned int ReadCar(char* &ptr,unsigned int &len,RChar& read) const throw(InvalidFile);

	/**
	* Return true if a string begin with a end of line.
	* @param ptr            Pointer to a string.
	*/
	static bool Eol(RChar car);

	/**
	* This function skip a end of line.
	*/
	void SkipEol(void);

	/**
	* Verify if a string is the next one in the buffer.
	* @param str           The string to find
	* @param CaseSensitive  Is the search case sensitive.
	* @return bool         return true if the string is found.
	*/
	bool CurString(const RString& str,bool CaseSensitive=true) const;

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

public:

	/**
	* Set the encoding of the file.
	* @param name           Name of the encoding.
	*/
	void SetEncoding(const RString& name) throw(RIOException);

	/**
	* Get the encoding of the file.
	* @return RString.
	*/
	RString GetEncoding(void) const;

	/**
	* Return true if the file is been treated.
	*/
	inline bool Eof(void) { return(Len==0); }

	/**
	* Set the style of comments.
	* @param style          The Style.
	*/
	void SetRemStyle(RemType style) {CommentType=style;}

	/**
	* Set the string of comments when single line.
	* @param c              This string represent the begin of a comment.
	*/
	void SetRem(RString c) {Rem=c;}

	/**
	* Set the begin and the end strings of comments when in multiline.
	* @param b              This string represent the begin of a comment.
	* @param e              This string represent the end of a comment.
	*/
	void SetRem(RString b,RString e) {BeginRem=b; EndRem=e; }

	/**
	* Return the next word contained in the file. A word is a suite of characters
	* delemited by spaces.
	*/
	RString GetWord(void) throw(RIOException);

	/**
	* Return the next entire line in the file.
	*/
	RString GetLine(bool SkipEmpty=true) throw(RIOException);

	/**
	* Return the next integer contained in the file.
	*/
	long GetInt(void) throw(RIOException);

	/**
	* Return the next unsigned integer contained in the file.
	*/
	unsigned long GetUInt(void) throw(RIOException);

	/**
	* >> Operator for string.
	*/
	RTextFile& operator>>(RString& str) throw(RIOException);

	/**
	* >> Operator for char.
	*/
	RTextFile& operator>>(char& nb) throw(RIOException);

	/**
	* >> Operator for unsigned char.
	*/
	RTextFile& operator>>(unsigned char& nb) throw(RIOException);

	/**
	* >> Operator for short.
	*/
	RTextFile& operator>>(short& nb) throw(RIOException);

	/**
	* >> Operator for unsigned short.
	*/
	RTextFile& operator>>(unsigned short& nb) throw(RIOException);

	/**
	* >> Operator for int.
	*/
	RTextFile& operator>>(int& nb) throw(RIOException);

	/**
	* >> Operator for unsigned int.
	*/
	RTextFile& operator>>(unsigned int& nb) throw(RIOException);

	/**
	* >> Operator for long.
	*/
	RTextFile& operator>>(long& nb) throw(RIOException);

	/**
	* >> Operator for unsigned long.
	*/
	RTextFile& operator>>(unsigned long& nb) throw(RIOException);

	/**
	* Return the next float contained in the file.
	*/
	float GetFloat(void) throw(RIOException);

	/**
	* >> Operator for float.
	*/
	RTextFile& operator>>(float& nb) throw(RIOException);

	/**
	* >> Operator for a double.
	*/
	RTextFile& operator>>(double& nb) throw(RIOException);

	/**
	* Write an end of line in the file.
	*/
	void WriteLine(void) throw(RIOException);

protected:

	/**
	* Write a separator if the current pointer of the file is not at the
	* beginning of a new line.
	*/
	void WriteSeparator(void);

public:

	/**
	* Write a string in the file. If the string is not the first thing on the
	* line, a separator is add before it.
	*/
	void WriteStr(const RString& str) throw(RIOException);

	/**
	* Write a string in the file. If the string is not the first thing on the
	* line, a separator is add before it.
	*/
	void WriteStr(const char* c) throw(RIOException);

	/**
	* Write a string of a give length in the file.
	*/
	void WriteStr(const char* c,unsigned int l) throw(RIOException);

	/**
	* << Operator for char *.
	*/
	RTextFile& operator<<(const char* c) throw(RIOException);

	/**
	* << Operator for RString.
	*/
	RTextFile& operator<<(const RString& str) throw(RIOException);

	/**
	* Write a long in the file. If the number is not the first thing on the line,
	* a space is add before it.
	*/
	void WriteLong(const long nb) throw(RIOException);

	/**
	* << Operator for char.
	*/
	RTextFile& operator<<(const char nb) throw(RIOException);

	/**
	* << Operator for short.
	*/
	RTextFile& operator<<(const short nb) throw(RIOException);

	/**
	* << Operator for int.
	*/
	RTextFile& operator<<(const int nb) throw(RIOException);

	/**
	* << Operator for long.
	*/
	RTextFile& operator<<(const long nb) throw(RIOException);

	/**
	* Write a unsigned long in the file. If the number is not the first thing on
	* the line, a separator is add before it.
	*/
	void WriteULong(const unsigned long nb) throw(RIOException);

	/**
	* << Operator for unsigned char.
	*/
	RTextFile& operator<<(const unsigned char nb) throw(RIOException);

	/**
	* << Operator for unsigned int.
	*/
	RTextFile& operator<<(const unsigned int nb) throw(RIOException);

	/**
	* << Operator for unsigned long.
	*/
	RTextFile& operator<<(const unsigned long nb) throw(RIOException);

	/**
	* Write a bool in the file as '1' or '0' depend on his value. If the number
	* is not the first thing on the line, a separator is add before it.
	*/
	void WriteBool(const bool b) throw(RIOException);

	/**
	* << Operator for bool.
	*/
	RTextFile& operator<<(const bool b) throw(RIOException);

	/**
	* Write a char in the file.
	*/
	void WriteChar(const char c) throw(RIOException);

	/**
	* Write a double
	*/
	void WriteDouble(const double d) throw(RIOException);

	/**
	* << Operator for double.
	*/
	RTextFile& operator<<(const double d) throw(RIOException);

	/**
	* Write the time in the file. If the time is not the first thing on the line,
	* a separator is add before it.
	*/
	void WriteTime(void) throw(RIOException);

	/**
	* Write a log entry in the file. First the time is written, and then the
	* entry. The entry is alone on a line, so end-of-lines are inserted if
	* ncessary before or after it.
	*/
	void WriteLog(const RString& entry) throw(RIOException);

	/**
	* Return the actual line number.
	*/
	unsigned long ActualLine(void) const {return(Line);}

	/**
	* Return the last line number where something was read.
	*/
	unsigned long GetLastLine(void) const {return(LastLine);}

	/**
	* Specify the set of characters used as separator.
	*/
	void SetSeparator(const RString& str) {Separator=str;}

	/**
	* Specify the set of characters used as separator.
	*/
	void SetSeparator(const char* str) {Separator=str;}

	/**
	* Get the separator used for the directories.
	*/
	static RChar GetDirSeparator(void);

	/**
	* Destructs the file.
	*/
	virtual ~RTextFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
namespace std{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Global functions
extern "C++"
{
	/**
	* Write an end-of-line in a RTextFile.
	*/
	extern R::RTextFile& endl(R::RTextFile &file);

	/**
	* Write the current time and date in a RTextFile.
	*/
	extern R::RTextFile& Time(R::RTextFile &file);

	/**
	* Operator needed for using generic writing functions.
	*/
	inline R::RTextFile& operator<<(R::RTextFile& o, R::RTextFile& (&f)(R::RTextFile&)) {return(f(o));}
}


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif
