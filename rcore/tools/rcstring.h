/*

	R Project Library

	RCString.h

	C String - Header.

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



//-----------------------------------------------------------------------------
#ifndef RCString_H
#define RCString_H

//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rshareddata.h>
#include <basicstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class implements a C string (char*) as a class.
* @author Pascal Francq
* @short C String
*/
class RCString : public BasicString<char,RCString>
{
protected:

	/**
	* Pointer to the buffer representing the null string.
	*/
	static BasicString<char,RCString>::BasicCharBuffer* DataNull;

public:

	/**
	* Null string.
	*/
	static const RCString Null;

	/**
	* Construct a empty string.
	*/
	RCString(void);

	/**
	* Construct a string from a "C string".
	* @param src             C string used as reference.
	*/
	RCString(const char* src);

	/**
	* Construct a string by doing a deep copy of the first caracters of a "C"
	* string.
	* @param src             C string used as reference.
	* @param len             Length.
	*/
	RCString(const char* src,size_t len);

	/**
	* Construct a string from a "C string".
	* @param src             String" used as reference.
	*/
	RCString(const std::string& src);

	/**
	* Construct an empty string with a maximal size.
	* @param maxlen          Initial maximal length of the string.
	*/
	explicit RCString(size_t maxlen);

	/**
	* Construct a string from another string.
	* @param src             String used as reference.
	*/
	RCString(const RCString& src);

private:

	/**
	* Return the pointer to the "null" buffer. If it is not created, create it.
	* @return Pointer to the "null" buffer.
	*/
	static BasicCharBuffer* GetDataNull(void);

public:

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RCString& operator=(const RCString &src);

	/**
	* Assignment operator using a "C string".
	* @param src             Source string.
	*/
	RCString& operator=(const char* src);

	/**
	* Assignment operator using a string.
	* @param src             Source string.
	*/
	RCString& operator=(const std::string& src);

	/**
	* Clear the content of the string.
	*/
	void Clear(void);

	/**
	* Copy a certain number of characters in the string.
	* @param src             Text to copy.
	* @param nb              Number of characters to copy.
	*/
	void Copy(const char* src,unsigned int nb);

public:

	/**
	* Deep copy of the string if necessary, i.e. when the string points to an
	* internal buffer referenced by other strings, make a copy of it.
	*/
	void Copy(void);

public:

	/**
	* Set the length of the string. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	* @param len             Length of the string.
	*/
	inline void SetLen(size_t len) {BasicString<char,RCString>::SetLen<BasicCharBuffer>(len);}

	/**
	* This function returns the character at a given position in the string.
	* (Read-Only).
	* @param idx             Index of the character.
	* @returns RChar.
	*/
	const char& operator[](size_t idx) const;

	/**
	* This function returns the character at a given position in the string.
	* @param idx             Index of the character.
	* @returns RChar.
	*/
	char& operator[](size_t idx);

	/**
	* Get a sub-string of a given string.
	* @param idx             Index of the first character.
	* @param len             Length of the sub-string. If the legnth is not
	*                        specified, the end of the string is copied.
	* @returns A RString containing the substring.
	*/
	inline RCString Mid(size_t idx,int len=-1) const {return(BasicString<char,RCString>::Mid<BasicCharBuffer>(idx,len));}

	/**
	* Add another string.
	* @param src             Source string.
	*/
	RCString& operator+=(const RCString& src);

	/**
	* Add a "C string" to the string.
	* @param src             Source string.
	*/
	RCString& operator+=(const char* src);

	/**
	* Add a character to the string.
	* @param src             Source string.
	*/
	RCString& operator+=(const char src);

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	const char* operator()(void) const;

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	operator std::string () const;

	/**
	* Return the string containing the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	operator const char* () const;

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	int Compare(const RCString& src) const;

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	int Compare(const char* src) const;

	/**
	* Equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator==(const RCString& src) const {return(Compare(src)==0);}

	/**
	* Equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator==(const char* src) const {return(Compare(src)==0);}

	/**
	* Non-equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator!=(const RCString& src) const {return(Compare(src));}

	/**
	* Non-equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator!=(const char* src) const {return(Compare(src));}

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex(const RCString& src);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex(const char* src);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex2(const RCString& src);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex2(const char* src);

public:

	/**
	* Destruct the string.
	*/
	virtual ~RCString(void);

	friend class BasicString<char,RCString>;
};


//-----------------------------------------------------------------------------
// C++ operators

/**
* Add two strings.
* @param arg1                First string.
* @param arg2                Second string.
*/
inline RCString operator+(const RCString& arg1,const RCString& arg2) {return(RCString(arg1)+=arg2);}

/**
* Add two strings.
* @param arg1                First string.
* @param arg2                Second string.
*/
inline RCString operator+(const RCString& arg1,const char* arg2) {return(RCString(arg1)+=arg2);}

/**
* Add two strings.
* @param arg1                First string.
* @param arg2                Second string.
*/
inline RCString operator+(const char* arg1,const RCString& arg2) {return(RCString(arg1)+=arg2);}


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
