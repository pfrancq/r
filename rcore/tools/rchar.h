/*

	R Project Library

	RChar.h

	Unicode character - Header.

	Copyright 2003 by the Universit�Libre de Bruxelles.

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
#ifndef RChar_H
#define RChar_H


//------------------------------------------------------------------------------
// include files for C++
#include <ctype.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
typedef unsigned short UChar; // Unicode code for a character.


//------------------------------------------------------------------------------
/**
* This class implements a unicode character as a class.
* @author Pascal Francq
* @short Unicode Character
*/
class RChar
{
	/**
	* The character.
	*/
	UChar Car;

public:

	/**
	* Construct a null character.
	*/
	RChar(void) : Car(0) {}

	/**
	* Copy constructor.
	* @param c               ASCII/Latin-1 character.
	*/
	RChar(const char c) : Car((const unsigned char)c) {}

	/**
	* Copy constructor.
	* @param c               ASCII/Latin-1 character.
	*/
	explicit RChar(const unsigned char c)  : Car(c) {}

	/**
	* Copy constructor.
	* @param c               Unicode character.
	*/
	explicit RChar(const UChar c) : Car(c) {}

	/**
	* Copy constructor.
	* @param c               Unicode character.
	*/
	explicit RChar(const long c) : Car((UChar)c) {}

	/**
	* Copy constructor.
	* @param c               Unicode character.
	*/
	explicit RChar(const unsigned int c) : Car((UChar)(c&0xFFFF)) {}

	/**
	* Copy constructor.
	* @param c               Unicode character.
	*/
	explicit RChar(const int c) : Car((UChar)(c&0xFFFF)) {}


	/**
	* Assignment operator.
	* @param c               ASCII/Latin-1 character.
	*/
	RChar& operator=(const char c) { Car=(unsigned char)c; return(*this); }

	/**
	* Assignment operator.
	* @param c               ASCII/Latin-1 character.
	*/
	RChar& operator=(const unsigned char c) { Car=c; return(*this); }

	/**
	* Assignment operator.
	* @param c               Unicode character.
	*/
	RChar& operator=(const unsigned short c) { Car=c; return(*this); }

	/**
	* Assignment operator.
	* @param c               Unicode character.
	*/
	RChar& operator=(const short c) { Car=(UChar)c; return(*this); }

	/**
	* Assignment operator.
	* @param c               Unicode character.
	*/
	RChar& operator=(const unsigned int c) { Car=(UChar)(c&0xFFFF); return(*this); }

	/**
	* Assignment operator.
	* @param c               Unicode character.
	*/
	RChar& operator=(const int c) { Car=(UChar)(c&0xFFFF); return(*this); }

	/**
	* Assignment operator.
	* @param c               Character.
	*/
	bool operator==(const char c) const {return(((unsigned char)c)==Car);}

	/**
	* Comparison == operator.
	* @param c               Character.
	*/
	bool operator==(const RChar c) const {return(Car==c.Car);}

	/**
	* Comparison != operator.
	* @param c               Character.
	*/
	bool operator!=(const char c) const {return(((unsigned char)c)!=Car);}

	/**
	* Comparison != operator.
	* @param c               Character.
	*/
	bool operator!=(const RChar c) const {return(Car!=c.Car);}

	/**
	* Comparison < operator.
	* @param c               Character.
	*/
	bool operator<(const char c) const {return(((unsigned char)c)<Car);}

	/**
	* Comparison < operator.
	* @param c               Character.
	*/
	bool operator<(const RChar c) const {return(Car<c.Car);}

	/**
	* Comparison <= operator.
	* @param c               Character.
	*/
	bool operator<=(const char c) const {return(((unsigned char)c)<=Car);}

	/**
	* Comparison <= operator.
	* @param c               Character.
	*/
	bool operator<=(const RChar c) const {return(Car<=c.Car);}

	/**
	* Comparison > operator.
	* @param c               Character.
	*/
	bool operator>(const char c) const {return(((unsigned char)c)>Car);}

	/**
	* Comparison > operator.
	* @param c               Character.
	*/
	bool operator>(const RChar c) const {return(Car>c.Car);}

	/**
	* Comparison >= operator.
	* @param c               Character.
	*/
	bool operator>=(const char c) const {return(((unsigned char)c)>=Car);}

	/**
	* Comparison >= operator.
	* @param c               Character.
	*/
	bool operator>=(const RChar c) const {return(Car>=c.Car);}

	/**
	* Get a Latin1 character.
	* @return The character or 0 if is not Latin1.
	*/
	char Latin1(void) const                       { return(Car>0xff?0:(char)Car); }

	/**
	* Get a Unicode character.
	* @return UChar.
	*/
	UChar Unicode(void) const            { return(Car); }

	/**
	* Test if the character is a null one.
	* @return bool.
	*/
	bool IsNull(void) const                       { return(Unicode()==0); }

	/**
	* Test if the character is a null one.
	* @return bool.
	*/
	bool operator!() const                       { return(Unicode()==0); }

	/**
	* Test if the character is a printable one.
	* @return bool.
	*/
	bool IsPrint(void) const                      { return(iswprint(Unicode())); }

	/**
	* Test if the character is a punctuation one.
	* @return bool.
	*/
	bool IsPunct(void) const                      { return(iswpunct(Unicode())); }

	/**
	* Test if the character is a space one.
	* @return bool.
	*/
	bool IsSpace(void) const                      { return(iswspace(Unicode())); }

	/**
	* Test if the character is a letter.
	* @return bool.
	*/
	bool IsAlpha(void) const                      { return(iswalpha(Unicode())); }

	/**
	* Test if the character is a digit.
	* @return bool.
	*/
	bool IsDigit(void) const                      { return(iswdigit(Unicode())); }

	/**
	* Test if the character is alphanumeric.
	* @return bool.
	*/
	bool IsAlNum(void) const                      { return(iswalnum(Unicode())); }

	/**
	* Test if the character is a graphical one.
	* @return bool.
	*/
	bool IsGraph(void) const                      { return(iswgraph(Unicode())); }

	/**
	* Transform an Unicode character in lowercase.
	* @param c               Character.
	* return RChar.
	*/
	static RChar ToLower(const RChar c)                 { return(RChar(towlower(c.Unicode()))); }

	/**
	* Transform an Unicode character in upperrcase.
	* @param c               Character.
	* return RChar.
	*/
	static RChar ToUpper(const RChar c)                 { return(RChar((UChar)towupper(c.Unicode()))); }

	/**
	* Compute the length of an array of RChar.
	* @return int.
	*/
	static int StrLen(const RChar* str);

	/**
	* Compare two RChar array.
	* @return 0 if the two strings are identical, >0 of the first string is
	* "greater" tahn the second and <0 else.
	*/
	static int StrCmp(const RChar* str1,const RChar* str2);

	/**
	* Compare a RChar array and a "C" string.
	* @return 0 if the two strings are identical, >0 of the first string is
	* "greater" tahn the second and <0 else.
	*/
	static int StrCmp(const RChar* str1,const char* str2);

	/**
	* Compare a RChar array and a "C" string.
	* @return 0 if the two strings are identical, >0 of the first string is
	* "greater" tahn the second and <0 else.
	*/
	static int StrCmp(const char* str1,const RChar* str2);

	/**
	* Test if a given character is in a string.
	* @return 0 if the the character was not found, or point to the first
	* occurence of the character in the string.
	*/
	static const RChar* StrChr(const RChar* str1,const RChar c);
};


/**
* Test if the character is a printable one.
* @return bool.
*/
inline bool isprint(const RChar c)                      { return(c.IsPrint()); }

/**
* Test if the character is a punctuation one.
* @return bool.
*/
inline bool ispunct(const RChar c)                      { return(c.IsPunct()); }

/**
* Test if the character is a space one.
* @return bool.
*/
inline bool isspace(const RChar c)                      { return(c.IsSpace()); }

/**
* Test if the character is a letter.
* @return bool.
*/
inline bool isalpha(const RChar c)                      { return(c.IsAlpha()); }

/**
* Test if the character is a digit.
* @return bool.
*/
inline bool isdigit(const RChar c)                     { return(c.IsDigit()); }

/**
* Test if the character is alphanumeric.
* @return bool.
*/
inline bool isalnum(const RChar c)                      { return(c.IsAlNum()); }

/**
* Test if the character is a graphical one.
* @return bool.
*/
inline bool isgraph(const RChar c)                      { return(c.IsGraph()); }

/**
* Transform an Unicode character in lowercase.
* @param c               Character.
* return RChar.
*/
inline RChar tolower(const RChar c)                { return(RChar::ToLower(c)); }

/**
* Transform a character in lowercase.
* @param c               Character.
* return char.
*/
inline char tolower(const char c)                   { return(std::tolower(c)); }

/**
* Transform an Unicode character in upperrcase.
* @param c               Character.
* return RChar.
*/
inline RChar toupper(const RChar c)                { return(RChar::ToUpper(c)); }

/**
* Transform a character in uppercase.
* @param c               Character.
* return char.
*/
inline char toupper(const char c)                   { return(std::toupper(c)); }


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
