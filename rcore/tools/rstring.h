/*

	R Project Library

	RString.h

	Unicode String - Header.

	Copyright 1999-2003 by the Université Libre de Bruxelles.

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
#ifndef RString_H
#define RString_H

//------------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rcursor.h>
#include <rstd/rchar.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a unicode string (RChar*) as a class.
* @author Pascal Francq
* @short Unicode String
*/
class RString
{
protected:

	/**
	* The Text containing the string.
	*/
	RChar* Text;

	/**
	* The length of the string.
	*/
	unsigned int Len;

	/**
	* The maximal length of the string. When necessary, the class increase his size.
	*/
	unsigned int MaxLen;

public:

	/**
	* Construct a empty string. It is a string with a maximal length of 200.
	*/
	RString(void) throw(bad_alloc);

protected:

	/**
	* Verify if the string can hold maxlen characters and extend the array if
	* necessary.
	*/
	inline void Verify(const unsigned int maxlen) throw(bad_alloc);

public:

	/**
	* Construct a string from a "C string".
	* @param text           The "C string" used as reference.
	*/
	RString(const char* text) throw(bad_alloc);

	/**
	* Construct a string from a Unicode array.
	* @param text           The array used as reference.
	*/
	RString(const RChar* text) throw(bad_alloc);

	/**
	* Construct a string from a string.
	* @param text           The string used as reference.
	*/
	RString(const std::string& text) throw(bad_alloc);

	/**
	* Construct an empty string with a maximal size.
	* @param maxlen         Initial maximal length of the string.
	*/
	RString(const unsigned int maxlen) throw(bad_alloc);

	/**
	* Construct a string from another string.
	* @param str            The string used as reference.
	*/
	RString(const RString& str) throw(bad_alloc);

	/**
	* Construct a string from another string.
	* @param str            The string used as reference.
	*/
	RString(const RString* str) throw(bad_alloc);

	/**
	* Assignment operator using another string.
	*/
	RString& operator=(const RString &str) throw(bad_alloc);

	/**
	* Assignment operator using a "C string".
	*/
	RString& operator=(const char* text) throw(bad_alloc);

	/**
	* Assignment operator using a string.
	*/
	RString& operator=(const string& text) throw(bad_alloc);

	/**
	* Copy a certain number of characters in the string.
	* @param text           Text to copy.
	* @param nb             Numbre of characters to copy.
	*/
	void Copy(const char* text,unsigned int nb);

	/**
	* Copy a certain number of characters in the string.
	* @param text           Text to copy.
	* @param nb             Numbre of characters to copy.
	*/
	void Copy(const RChar* text,unsigned int nb);

	/**
	* Make a copy a return a pointer to it.
	*/
	RChar* StrDup(void) const throw(bad_alloc);

	/**
	* Transform the string to uppercase.
	*/
	void StrUpr(void);

	/**
	* Get a uppercase version of the string.
	* @return RString.
	*/
	RString& GetUpr(void) const;

	/**
	* Assign the uppercase version of a "C string".
	* @param text           The "C string" used.
	*/
	void StrUpr(const char* text) throw(bad_alloc);

	/**
	* Assign the uppercase version of a string.
	* @param text           The string used.
	*/
	void StrUpr(const RChar* text) throw(bad_alloc);

	/**
	* Assign the uppercase version of a string.
	* @param str            The string used.
	*/
	void StrUpr(const RString &str) throw(bad_alloc);

	/**
	* Transform the string to lowercase.
	*/
	void StrLwr(void);

	/**
	* Get a lowercase version of the string.
	* @return RString.
	*/
	RString& GetLwr(void) const;

	/**
	* Assign the lowercase version of a "C string".
	* @param text           The "C string" used.
	*/
	void StrLwr(const char* text) throw(bad_alloc);

	/**
	* Assign the lowercase version of a string.
	* @param text           The string used.
	*/
	void StrLwr(const RChar* text) throw(bad_alloc);

	/**
	* Assign the lowercase version of a string.
	* @param str            The string used.
	*/
	void StrLwr(const RString &str) throw(bad_alloc);

	/**
	* Return the length of the string.
	*/
	inline unsigned int GetLen(void) const {return(Len);}

	/**
	* Return the maximal length of the string.
	*/
	inline unsigned int GetMaxLen(void) const {return(MaxLen);}

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	inline bool IsEmpty(void) const {return(!Len);}

	/**
	* Transform the string into a "C String" in Latin1 encoding. The resulting
	* array should be copied (and not destroyed) since it is an internal
	* structure.
	* @return "C String" (char*)
	*/
	char* Latin1(void) const;

	/**
	* Add another string.
	*/
	RString& operator+=(const RString &str) throw(bad_alloc);

	/**
	* Add a "C string" to the string.
	*/
	RString& operator+=(const char* text) throw(bad_alloc);

	/**
	* Add a string to the string.
	*/
	RString& operator+=(const RChar* text) throw(bad_alloc);

	/**
	* Add a character to the string.
	*/
	RString& operator+=(const char c) throw(bad_alloc);

	/**
	* Add a character to the string.
	*/
	RString& operator+=(const RChar c) throw(bad_alloc);

	/**
	* Return the string containing the string.
	*/
	inline const RChar* operator()(void) const {return(Text);}

	/**
	* Return the string containing the string.
	*/
	inline operator const char* () const {return(Latin1());}

	/**
	* Return the string containing the string.
	*/
	inline operator std::string () const {return(Latin1());}

	/**
	* Equal operator.
	*/
	bool operator==(const RString& str) const {return(RChar::StrCmp(Text,str.Text)==0);}

	/**
	* Equal operator.
	*/
	bool operator==(const char* str) const {return(RChar::StrCmp(Text,str)==0);}

	/**
	* Equal operator.
	*/
	bool operator==(const RChar* str) const {return(RChar::StrCmp(Text,str)==0);}

	/**
	* Non-equal operator.
	*/
	bool operator!=(const RString& str) const {return(RChar::StrCmp(Text,str.Text));}

	/**
	* Non-equal operator.
	*/
	bool operator!=(const char* str) const {return(RChar::StrCmp(Text,str));}

	/**
	* Non-equal operator.
	*/
	bool operator!=(const RChar* str) const {return(RChar::StrCmp(Text,str));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString &str) const {return(RChar::StrCmp(Text,str.Text));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString* str) const {return(RChar::StrCmp(Text,str->Text));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const char* str) const {return(RChar::StrCmp(Text,str));}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RChar* str) const {return(RChar::StrCmp(Text,str));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const RString* str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const RString& str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const char* str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const RChar* str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const RString* str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const RString& str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const char* str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const RChar* str);

	/**
	* Need to manage temporary strings.
	*/
	static RString* GetString(void);

	/**
	* Transfrom a C string into an array of RChar. The resulting arrey should be
	* destroyed by the caller of the function.
	* @param str             Source "C String".
	* @param len             Length of the string (computed by the function).
	* @param maxlen          Maximum length (may be updated by the function).
	*/
	static RChar* Latin1ToUnicode(const char *str,unsigned int& len, unsigned int& maxlen);

public:

	/**
	* Destruct the string.
	*/
	virtual ~RString(void);
};


//------------------------------------------------------------------------------
// Operators

/**
* Add two strings together.
*/
RString& operator+(const RString& arg1,const RString& arg2);

/**
* Add a string and a "C string" together.
*/
RString& operator+(const RString& arg1,const char* arg2);

/**
* Add a "C string" and a string together.
*/
RString& operator+(const char* arg1,const RString& arg2);


//------------------------------------------------------------------------------
// Conversion functions

/**
* Transform an int to a string.
*/
RString& itou(const int nb);

/**
* Transform an unsigned int to a string.
*/
RString& itou(const unsigned int nb);

/**
* Transform a long to a string.
*/
RString& ltou(const long nb);

/**
* Transform an unsigned char to a string.
*/
RString& chrtou(const unsigned char c);

/**
* Transform an unsigned long to a string.
*/
RString& ltou(const unsigned long nb);

/**
* Transform a float to a string.
*/
RString& ftou(const float nb);

/**
* Transform a double to a string.
*/
RString& dtou(const double nb);


//------------------------------------------------------------------------------
/**
* The RStringCursor class provides a way to go trough a set of strings.
* @short Unicode Strings Cursor
*/
CLASSCURSOR(RStringCursor,RString,unsigned int)


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
