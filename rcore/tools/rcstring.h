/*

	R Project Library

	RCString.h

	C String - Header.

	Copyright 1999-2004 by the Université libre de Bruxelles.

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
#ifndef RCString_H
#define RCString_H

//------------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rcontainer.h>
#include <rstd/rshareddata.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* This class implements a C string (char*) as a class.
* @author Pascal Francq
* @short C String
*/
class RCString
{
	class CharBuffer;

protected:

	/**
	* Pointer to the buffer of the string.
	*/
	CharBuffer* Data;

	/**
	* Pointer to the buffer representing the null string.
	*/
	static CharBuffer* DataNull;

public:

	/**
	* Null string.
	*/
	static const RCString Null;

	/**
	* Construct a empty string.
	*/
	RCString(void);

public:

	/**
	* Construct a string from a "C string".
	* @param text           The "C string" used as reference.
	*/
	RCString(const char* text);

	/**
	* Construct a string by doing a deep copy of the first caracters of a "C"
	* string.
	* @param text           The "C string" used as reference.
	* @param len            Length.
	*/
	RCString(const char* text,unsigned int len);

	/**
	* Construct a string from a "C string".
	* @param text           The "C string" used as reference.
	*/
	RCString(const std::string& text);

	/**
	* Construct an empty string with a maximal size.
	* @param maxlen         Initial maximal length of the string.
	*/
	RCString(const unsigned int maxlen);

	/**
	* Construct a string from another string.
	* @param str            The string used as reference.
	*/
	RCString(const RCString& str);

private:

	/**
	* Return the pointer to the "null" buffer. If it is not created, create it.
	* @return Pointer to the "null" buffer.
	*/
	static CharBuffer* GetDataNull(void);

public:

	/**
	* Assignment operator using another string.
	*/
	RCString& operator=(const RCString &str);

	/**
	* Assignment operator using a "C string".
	*/
	RCString& operator=(const char* text);

	/**
	* Assignment operator using a string.
	*/
	RCString& operator=(const std::string& text);

	/**
	* Clear the content of the string.
	*/
	void Clear(void);

	/**
	* Copy a certain number of characters in the string.
	* @param text           Text to copy.
	* @param nb             Numbre of characters to copy.
	*/
	void Copy(const char* text,unsigned int nb);

private:

	/**
	* Deep copy of the string if necessary, i.e. when the string points to an
	* internal buffer referenced by other strings, make a copy of it.
	*/
	void Copy(void);

public:

	/**
	* Get a uppercase version of the string.
	* @return RString.
	*/
	RCString ToUpper(void) const;

	/**
	* Get a lowercase version of the string.
	* @return RString.
	*/
	RCString ToLower(void) const;

	/**
	* Return the length of the string.
	*/
	unsigned int GetLen(void) const;

	/**
	* Set the length of the string. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	*/
	void SetLen(unsigned int len);

	/**
	* Return the maximal length of the string.
	*/
	unsigned int GetMaxLen(void) const;

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	bool IsEmpty(void) const;

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	*/
	RCString Trim(void) const;

	/**
	* This function returns the character at a given position in the string. If
	* the position is outside the string, the null character is returned.
	* @param pos            Position of the character.
	* @returns RChar.
	*/
	char operator[](int pos) const;

	/**
	* Find the position of a given character in the string.
	* @param car            Character to find.
	* @param pos            Position to start the search. Negative values start
	*                       the search from the end.
	* @param CaseSensitive  Is the search case sensitive.
	* @return The position of the first occurence or -1 if the character was not
	*         found.
	*/
	int Find(char car,int pos=0,bool CaseSensitive=true) const;

	/**
	* Find the position of a given string in the string.
	* @param str            String to find.
	* @param pos            Position to start the search. Negative values start
	*                       the search from the end.
	* @param CaseSensitive  Is the search case sensitive.
	* @return The position of the first occurence or -1 if the character was not
	*         found.
	*/
	int FindStr(const RCString str,int pos=0,bool CaseSensitive=true) const;

	/**
	* Get a sub-string of a given string.
	* @param idx            Index of the first character.
	* @param len            Length of the sub-string. If the legnth is not
	*                      specified, the end of the string is copied.
	* @returns A RCString containing the substring.
	*/
	RCString Mid(unsigned int idx,unsigned int len = 0xFFFFFFFF) const;

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements       Container that will hold the results.
	* @param car            Character used as separator.
	*/
	void Split(RContainer<RCString,true,false>& elements,const char car) const;

	/**
	* Add another string.
	*/
	RCString& operator+=(const RCString& str);

	/**
	* Add a "C string" to the string.
	*/
	RCString& operator+=(const char* text);

	/**
	* Add a character to the string.
	*/
	RCString& operator+=(const char c);

	/**
	* Return the string containing the string.
	*/
	const char* operator()(void) const;

	/**
	* Return the string.
	*/
	operator std::string () const;

	/**
	* Return the string containing the string.
	*/
	operator const char* () const;

	/**
	* Equal operator.
	*/
	bool operator==(const RCString& str) const;

	/**
	* Equal operator.
	*/
	bool operator==(const char* str) const;

	/**
	* Non-equal operator.
	*/
	bool operator!=(const RCString& str) const;

	/**
	* Non-equal operator.
	*/
	bool operator!=(const char* str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RCString& str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RCString* str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const char* str) const;

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const RCString* str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const RCString& str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RHashContainer class.
	*/
	static int HashIndex(const char* str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const RCString* str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const RCString& str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string. It is used for the RStd::RDblHashContainer class.
	*/
	static int HashIndex2(const char* str);

public:

	/**
	* Destruct the string.
	*/
	virtual ~RCString(void);
};


//------------------------------------------------------------------------------
// Operators

/**
* Add two strings together.
*/
RCString operator+(const RCString& arg1,const RCString& arg2);

/**
* Add a string and a "C string" together.
*/
RCString operator+(const RCString& arg1,const char* arg2);

/**
* Add a "C string" and a string together.
*/
RCString operator+(const char* arg1,const RCString& arg2);


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
