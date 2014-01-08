/*

	R Project Library

	RTextFile.h

	Text File - Header.

	Copyright 1999-2014 by Pascal Francq (pascal@francq.info).
	Copyright 1999-2008 by the Université Libre de Bruxelles (ULB).

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
#include <riofile.h>
#include <rcstring.h>


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
* corresponding set of characters (C comments delimiters by default).
*
* When multiple elements are inserted on the same text line, before each
* insertion, and except for the first one, a set of characters representing
* a separator (' ' by default) are added.
*
* The user can write his own operators to read and write with RTextFile. Here
* is an example:
* @code
* #include <rtextfile.h>
* using namespace R;
*
* class Point
* {
* public:
*    int X,Y;
*    Point(void) : X(0), Y(0) {}
*    Point(int x,int y) : X(x), Y(y) {}
* };
*
* RTextFile& operator<<(RTextFile &f,Point &pt)
* {
*    return(f<<pt.X<<pt.Y);
* }
*
* RTextFile& operator>>(RTextFile &f,Point &pt)
* {
*    return(f>>pt.X>>pt.Y);
* }
*
* int main(int argc, char *argv[])
* {
*    Point Pt1(2,3);
*    Point Pt2;
*    RTextFile Out("/home/user/Test.txt");
*    Out.Open(RIO::Create);
*    Out<<Pt1;
*    RTextFile In("/home/user/Test.txt");
*    In.Open(RIO::Read);
*    In>>Pt2;
*    cout<<"Pt2=("<<Pt2.X<<","<<Pt2.Y<<")"<<endl;
* }
* @endcode
* \note
* Internally, when a file is read, a buffer holds a small number of Unicode
* characters (Chars), and a buffer holds the size in bytes of each character
* read (SizeChars). Two pointers (NextRead and SizeNextRead) parse these buffers
* when characters already read must be treated. Two other buffers (NextWrite and
* SizeNextWrite) parse the buffers when new characters must be read from the
* file.
* \par
* Each character is decoded one by one. This multiplies the call to
* RTextEnconding which is time-consumming, but ensure that the file always now
* the exact position (in byte) of every Unicode character read.
* @author Pascal Francq
* @short Text File.
*/
class RTextFile : public RIOFile
{
public:
	/**
	* The RemType enum represents the different style of comments used which text
	* files. The comments are skipped when the file is read.
	*/
	enum RemType
	{
		NoComment                /** No comments are possible. */,
		SingleLineComment        /** Only single line comments are possible. */,
		MultiLineComment         /** Only multiple line comments are possible. */,
		SingleMultiLineComment   /** Both type of comments are possible. */
	};

	/**
	 * The ParseSpaceType enum represents the different way to handle spaces.
	 * If SkipAllSpaces is chosen, leading spaces and empty lines are not seen
	 * by the programmer.
	 */
	enum ParseSpaceType
	{
		LeaveSpaces              /** Leave spaces. */,
		SkipAllSpaces            /** Skip all spaces. */
	};

private:

	/**
	* Temporary buffer (Used only if read mode).
	*/
	char Buffer[512];

	/**
	 * Buffer of Unicode characters already read.
	 */
	RChar Chars[40];

	/**
	 * Size of Unicode characters already read in the buffer.
	 */
	size_t SizeChars[40];

	/**
	 * Resting (if any) character if a 4-byte Unicode character is read.
	 */
	RChar Resting;

	/**
	 * Position of the next Unicode to handle.
	 */
	size_t PosChars;

	/**
	 * Next character to handle.
	 */
	RChar* NextRead;

	/**
	 * Size of the next character to handle.
	 */
	size_t* SizeNextRead;

	/**
	 * Next character to treat.
	 */
	RChar* NextWrite;

	/**
	 * Size of the next character to treat.
	 */
	size_t* SizeNextWrite;

	/**
	 * Number of bytes to skip the next time Unicode characters must be extracted.
	 */
	size_t SkipBytes;

	/**
	* At NewLine? (Used only if created or append mode).
	*/
	bool NewLine;

	/**
	* String representing a single line comment.
	*/
	RString Rem;

	/**
	* String representing the beginning of a multi-line comment.
	*/
	RString BeginRem;

	/**
	* String representing the ending of a multi-line comment.
	*/
	RString EndRem;

	/**
	* The type of comments that are using for this file.
	*/
	RemType CommentType;

	/**
	* The type of comments currently active.
	*/
	RemType ActivComment;

	/**
	 * How to handle spaces.
	 */
	ParseSpaceType ParseSpace;

	/**
	* This string represent a separator for different elements on the same line
	*/
	RString Separator;

	/**
	* This variable is holding the current line number.
	*/
	size_t Line;

	/**
	* This variable is holding the last line where something was read (or written).
	*/
	size_t LastLine;

	/**
	* Encoding of the text file.
	*/
	RTextEncoding* Codec;

	/**
	* Current character in the buffer.
	*/
	RChar Cur;

public:

	/**
	 * Default constructor.
	 */
	RTextFile(void);

	/**
	* Construct a text file.
	* @param uri            URI of the file.
	* @param encoding       The encoding scheme of the file.
	*/
	RTextFile(const RURI& uri,const RCString& encoding="Latin1");

	/**
	* Construct a text file.
	* @param file           A generic input/output file that should be treated
	*                       as text file.
	* @param encoding       The encoding scheme of the file.
	*/
	RTextFile(RIOFile& file,const RCString& encoding="Latin1");

	/**
	* Open the file
	* @param mode           The open mode for the file.
	*/
	virtual void Open(RIO::ModeType mode=RIO::Read);

	/**
	 * Open the file
	 * @param uri            URI of the file.
	 * @param mode           The open mode for the file.
	 * @param encoding       The encoding scheme of the file.
	 */
	void Open(const RURI& uri,RIO::ModeType mode=RIO::Read,const RCString& encoding="Latin1");

	/**
	* Close the file.
	*/
	virtual void Close(void);

	/**
	* Go to a specific position of the file.
	* @warning this method doesn't take the count of lines into account.
	* @param pos            Position to reach.
	*/
	virtual void Seek(off_t pos);

	/**
	* Move for a given number of bytes from the current position of the file.
	* @warning this method doesn't take the count of lines into account.
	* @param pos            Relative position.
	*/
	virtual void SeekRel(off_t pos);

	/**
	* Read the next character but without to move the internal pointer of the
	* file.
	*/
	inline RChar GetNextChar(void) const
	{
		if(End())
			return(RChar());
		return(*NextRead);
	}

	/**
	* Read the next character and move the internal pointer of the file.
	*/
	inline RChar GetChar(void)
	{
		if(End())
			return(RChar());
		Next();
		return(Cur);
	}

	/**
	 * Get the last character read in the buffer.
    */
	inline const RChar GetCur(void) const
	{
		return(Cur);
	}

	/**
	* Get the next characters.
	* @param size            Number to characters to read.
	* @return a RString.
	*/
	RString GetChars(size_t size);

	/**
	* Read the rest of the file.
	* @return A RString containing the file.
	*/
	RString GetUntilEnd(void);

private:

	/**
	* Go to the beginning of the file.
	*/
	void Begin(void);

	/**
	 * Must the buffer be filled again.
	 */
	inline void FillBuffer(void);

	/**
	* Read a series of 20 characters of variable size and store the result in
	* an internal buffer. If the file is at the end, nothing is done.
	*/
	void ReadChars(void);

public:

	/**
	* This function skip a end of line.
	* @internal This function is for internal uses only
	*/
	inline void SkipEol(void)
	{
		if(((*NextRead)==10)||((*NextRead)==13))
			Next();
	}

private:

	/**
	* This function returns true if the current position is the beginning of a
	* comment (Only used if in read mode).
	*/
	bool BeginComment(void);

	/**
	* This function returns true if the current position is the ending of a
	* comment and skip the end characters if MultiLineComment.
	*/
	bool EndComment(void);

protected:

	/**
	* Goes to the next character.
	*/
	void Next(void);

public:

	/**
	* Return true if the character defines the end of a line.
	* @param car            Character.
	*/
	static inline bool Eol(RChar car)
	{return((car==10)||(car==13));}

	/**
	* Verify if a string is the next one in the buffer. If the string is found,
	* it may be skipped.
	* @param str             The string to find
	* @param CaseSensitive   Is the search case sensitive.
	* @param skip            Must the string be skipped?
	* @return True if the string is found.
	*/
	bool CurString(const RString& str,bool CaseSensitive=true,bool skip=true);

	/**
	* This function skip comments (Only used if read mode).
	*/
	void SkipComments(void);

	/**
	* This function skip spaces (Only used if read mode).
	*/
	void SkipSpaces(void);

	/**
	* This function skip spaces and counts the number of a given character at
	* the beginning on the last line read (Only used if read mode).
	* @param car             Character (must be a space character such as a
	*                        tab).
	*/
	size_t SkipCountSpaces(RChar car);

	/**
	* Set the encoding of the file.
	* @param name           Name of the encoding.
	*/
	virtual void SetEncoding(const RCString& name);

	/**
	* Get the encoding of the file.
	* @return RString.
	*/
	RCString GetEncoding(void) const;

	/**
	* Set the style of comments.
	* @param style          The Style.
	*/
	void SetRemStyle(RemType style) {CommentType=style;}

	/**
	 * Set the way spaces are treated.
	 */
	void SetParseSpace(ParseSpaceType parse) {ParseSpace=parse;}

	/**
	 * Set the way spaces are treated.
	 */
	ParseSpaceType GetParseSpace(void) const {return(ParseSpace);}

	/**
	* Set the string of comments when single line.
	* @param c              This string represent the begin of a comment.
	*/
	void SetRem(const RString& c);

	/**
	* Set the begin and the end strings of comments when in multi-line.
	* @param b              This string represent the begin of a comment.
	* @param e              This string represent the end of a comment.
	*/
	void SetRem(const RString& b,const RString& e) ;

	/**
	* Get the next word contained in the file. A word is a suite of characters
	* delimited by spaces.
	*/
	RString GetWord(void);

	/**
	* Get the next token contained in the file. A token is a suite of
	* characters delimited either by spaces or by a specific ending character.
	* The ending character is not skipped.
	* @param endingchar      String containing all possible ending characters.
	*/
	RString GetToken(const RString& endingchar);

	/**
	* Get the next token contained in the file, i.e a suite of characters
	* delimited either by spaces or by a specific string.
	* The ending string is not skipped.
	* @param endingstr       Ending String.
	*/
	RString GetTokenString(const RString& endingstr);

	/**
	* Return the next entire line in the file.
	* @param skipempty       Skip empty lines.
	*/
	RString GetLine(bool skipempty=true);

	/**
	* Return the next integer contained in the file.
	*/
	long GetInt(void);

	/**
	* Return the next unsigned integer contained in the file.
	*/
	unsigned long GetUInt(void);

	/**
	* >> Operator for string.
	*/
	RTextFile& operator>>(RString& str);

	/**
	* >> Operator for char.
	*/
	RTextFile& operator>>(char& nb);

	/**
	* >> Operator for unsigned char.
	*/
	RTextFile& operator>>(unsigned char& nb);

	/**
	* >> Operator for short.
	*/
	RTextFile& operator>>(short& nb);

	/**
	* >> Operator for unsigned short.
	*/
	RTextFile& operator>>(unsigned short& nb);

	/**
	* >> Operator for int.
	*/
	RTextFile& operator>>(int& nb);

	/**
	* >> Operator for size_t.
	*/
	RTextFile& operator>>(unsigned int& nb);

	/**
	* >> Operator for long.
	*/
	RTextFile& operator>>(long& nb);

	/**
	* >> Operator for unsigned long.
	*/
	RTextFile& operator>>(unsigned long& nb);

protected:

	/**
	* Return a next string contained in the that should be a real number with
	* the following pattern:
	* @code
	* [whitespaces][+|-][nnnnn][.nnnnn][e|E[+|-]nnnn]
	* @endcode
	* @return RString.
	*/
	RString GetRealNb(void);

public:

	/**
	* >> Operator for float.
	*/
	RTextFile& operator>>(float& nb);

	/**
	* >> Operator for a double.
	*/
	RTextFile& operator>>(double& nb);

	/**
	* >> Operator for a long double.
	*/
	RTextFile& operator>>(long double& nb);

	/**
	* Write an end of line in the file.
	*/
	void WriteLine(void);

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
	void WriteStr(const RString& str);

	/**
	* Write a string in the file. If the string is not the first thing on the
	* line, a separator is add before it.
	*/
	void WriteStr(const char* c);

	/**
	* Write a string of a give length in the file.
	*/
	void WriteStr(const char* c,size_t l);

	/**
	* << Operator for char *.
	*/
	RTextFile& operator<<(const char* c);

	/**
	* << Operator for RString.
	*/
	RTextFile& operator<<(const RString& str);

	/**
	* Write a long in the file. If the number is not the first thing on the line,
	* a space is add before it.
	*/
	void WriteLong(const long nb);

	/**
	* << Operator for char.
	*/
	RTextFile& operator<<(const char nb);

	/**
	* << Operator for short.
	*/
	RTextFile& operator<<(const short nb);

	/**
	* << Operator for int.
	*/
	RTextFile& operator<<(const int nb);

	/**
	* << Operator for long.
	*/
	RTextFile& operator<<(const long nb);

	/**
	* Write a unsigned long in the file. If the number is not the first thing on
	* the line, a separator is add before it.
	*/
	void WriteULong(const unsigned long nb);

	/**
	* << Operator for unsigned char.
	*/
	RTextFile& operator<<(const unsigned char nb);

	/**
	* << Operator for size_t.
	*/
	RTextFile& operator<<(const unsigned int nb);

	/**
	* << Operator for unsigned long.
	*/
	RTextFile& operator<<(const unsigned long nb);

	/**
	* Write a bool in the file as '1' or '0' depend on his value. If the number
	* is not the first thing on the line, a separator is add before it.
	*/
	void WriteBool(const bool b);

	/**
	* << Operator for bool.
	*/
	RTextFile& operator<<(const bool b);

	/**
	* Write a char in the file.
	*/
	void WriteChar(const char c);

	/**
	* Write a float.
	*/
	void WriteFloat(const float nb);

	/**
	* << Operator for float.
	*/
	RTextFile& operator<<(const float nb);

	/**
	* Write a double.
	*/
	void WriteDouble(const double nb);

	/**
	* << Operator for double.
	*/
	RTextFile& operator<<(const double nb);

	/**
	* Write a long double
	*/
	void WriteLongDouble(const long double nb);

	/**
	* << Operator for long double.
	*/
	RTextFile& operator<<(const long double nb);

	/**
	* Write the time in the file. If the time is not the first thing on the line,
	* a separator is add before it.
	*/
	void WriteTime(void);

	/**
	* Write a log entry in the file. First the time is written, and then the
	* entry. The entry is alone on a line, so end-of-lines are inserted if
	* necessary before or after it.
	*/
	void WriteLog(const RString& entry);

	/**
	* Return the actual line number.
	*/
	unsigned long GetLineNb(void) const {return(Line);}

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
	* Destruct the file.
	*/
	virtual ~RTextFile(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
namespace std{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Global functions
#ifndef __APPLE__
extern "C++"
{
#endif
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
#ifndef __APPLE__
}
#endif


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif
