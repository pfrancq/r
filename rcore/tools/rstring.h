/*

	Rainbow Library Project

	RString.h

	String - Header.

	(C) 1999-2000 by P. Francq.

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
#ifndef RStringH
#define RStringH


//-----------------------------------------------------------------------------
// include files for ANSI C/C++
#include <new.h>


//-----------------------------------------------------------------------------
// Generic defines
#ifndef __RMAXSTRING__
	#define __RMAXSTRING__ 30
#endif


//-----------------------------------------------------------------------------
namespace RStd{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** This class implements the traditional "C string" (char*) as a class.
* @author Pascal Francq
* @short String Class
*/
class RString
{
	/**
	* The Text containing the string.
	*/
	char *Text;

	/**
	* The length of the string.
	*/
	int Len;
	/**
	* The maximal length of the string. When necessary, the class increase his size.
	*/
	int MaxLen;

public:

	/**
	* Construct a empty string. It is a string with a maximal length of 200.
	*/
	RString(void) throw(bad_alloc);

	/**
	* Construct a string from a "C string".
	* @param text		The "C string" used as reference.
	*/
	RString(const char *text) throw(bad_alloc);

	/**
	* Construct an empty string with a maximal size.
	* @param maxlen		Initial maximal length of the string.
	*/
	RString(const int maxlen) throw(bad_alloc);

	/**
	* Construct a string from another string.
	* @param str		The string used as reference.
	*/
	RString(const RString& str) throw(bad_alloc);

	/**
	* Construct a string from another string.
	*	@param str		The string used as reference.
	*/
	RString(const RString* str) throw(bad_alloc);

	/**
	* Assignment operator using another string.
	*/
	RString& operator=(const RString &str) throw(bad_alloc);

	/**
	* Assignment operator using a "C string".
	*/
	RString& operator=(const char *text) throw(bad_alloc);

	/**
	* Make a copy a return a pointer to it.
	*/
	char* StrDup(void) const throw(bad_alloc);

	/**
	* Transform the string to uppercase.
	*/
	inline void StrUpr(void);

	/**
	* Assign the uppercase version of a "C string".
	* @param text		The "C string" used.
	*/
	void StrUpr(const char *text) throw(bad_alloc);

	/**
	* Assign the uppercase version of a string.
	* @param str		The string used.
	*/
	void StrUpr(const RString &str) throw(bad_alloc);

	/**
	* Transform the string to lowercase.
	*/
 	inline void StrLwr(void);

	/**
	* Assign the lowercase version of a "C string".
	* @param text		The "C string" used.
	*/
	void StrLwr(const char *text) throw(bad_alloc);

	/**
	* Assign the lowercase version of a string.
	* @param str		The string used.
	*/
	void StrLwr(const RString &str) throw(bad_alloc);

	/**
	* Add another string.
	*/
	RString& operator+=(const RString &str) throw(bad_alloc);

	/**
	* Add a "C string" to the string.
	*/
	RString& operator+=(const char *text) throw(bad_alloc);

	/**
	* Return the character at position i. The first letter is at position 0.
	*/
	inline char operator[](int i) const	{return(Text[i]);}

	/**
	* Return the "C string" containing the string.
	*/
	inline const char* operator()(void) const {return(Text);}

	/**
	* Return the "C string" containing the string.
	*/
	inline operator const char* () const {return(Text);}

	/**
	* Equal operator.
	*/
	bool operator==(const RString &str) const;

	/**
	* Non-equal operator.
	*/
	bool operator!=(const RString &str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString &str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString *str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const char *str) const;

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string. It is used for the RHashContainer class.
	*/
	char HashIndex(void) const;

	/**
	* Return the length of the string.
	*/
	inline int GetLen(void) const {return(Len);}

	/**
	* Return the maximal length of the string.
	*/
	inline int GetMaxLen(void) const {return(MaxLen);}

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	inline bool IsEmpty(void) const {return(Len);}

	/**
	* Need to manage temporary strings.
	*/
	static RString* GetString(void);

	/**
	* Need to manage temporary C strings.
	*/
	static char* GetCString(void);

protected:

	/**
	* Verify if the string can hold maxlen characters and extend the array if
	* necessary.
	*/
	inline void Verify(const int maxlen) throw(bad_alloc);	

public:

  /**
	* Destruct the string.
	*/
	virtual ~RString(void);
};


//-----------------------------------------------------------------------------
// Operators

/**
* Add two strings together.
*/
RString& operator+(const RString &arg1,const RString &arg2);

/**
* Add a string and a "C string" together.
*/
RString& operator+(const RString &arg1,const char *arg2);

/**
* Add a "C string" and a string together.
*/
RString& operator+(const char *arg1,const RString &arg2);


//-----------------------------------------------------------------------------
// Conversion functions

/**
* Transform an int to a string.
*/
RString& itoa(const int nb);

/**
* Transform an unsigned int to a string.
*/
RString& itoa(const unsigned int nb);

/**
* Transform a long to a string.
*/
RString& ltoa(const long nb);

/**
* Transform an unsigned char to a string.
*/
RString& chr(const unsigned char c);

/**
* Transform an unsigned long to a string.
*/
RString& ltoa(const unsigned long nb);

/**
* Transform a float to a string.
*/
RString& ftoa(const float nb);

/**
* Transform a double to a string.
*/
RString& dtoa(const double nb);


}  //-------- End of namespace RStd -------------------------------------------


//-----------------------------------------------------------------------------
#endif
