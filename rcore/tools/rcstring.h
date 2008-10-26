/*

	R Project Library

	RCString.h

	C String - Header.

	Copyright 1999-2008 by the Université libre de Bruxelles.

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
// forward declaration
class RCString;


//-----------------------------------------------------------------------------
/**
* This class implements a C string (char*) as a class.
* @author Pascal Francq
* @short C String
*/
class RCString : public BasicString<char,RCString>
{
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
	* Construct a string by doing a deep copy of the first characters of a "C"
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
	* Construct a string from a character.
	* @param src             Character.
	*/
	RCString(char src);

	/**
	* Construct a string from another string.
	* @param src             String used as reference.
	*/
	RCString(const RCString& src);

public:

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
	* Add another string.
	* @param src             Source string.
	*/
	inline RCString& operator+=(const RCString& src) {return(BasicString<char,RCString>::operator+=(src));}

	/**
	* Add another string.
	* @param src             Source string.
	*/
	inline RCString& operator+=(const char* src) {return(BasicString<char,RCString>::operator+=(src));}

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
	* Return a number between 0 and 26 according to the character at position
	* idx in the string.
	* @remark Supported values for idx are 1 and 2.
	* @param idx             Index of hash asked.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	size_t HashIndex(size_t idx) const;

private:

	void Dummy(void);

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

/**
* Add a string and a character.
* @param arg1                Character.
* @param arg2                String.
*/
inline const RCString operator+(char arg1,const RCString& arg2) {return(RCString(arg1)+=arg2);}

/**
* Add a character and a string.
* @param arg1                String.
* @param arg2                Character.
*/
inline const RCString operator+(const RCString& arg1,char arg2) {return(RCString(arg1)+=arg2);}


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
