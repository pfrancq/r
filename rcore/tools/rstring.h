/*

	R Project Library

	RString.h

	Unicode String - Header.

	Copyright 1999-2005 by the Université Libre de Bruxelles.

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
#ifndef RString_H
#define RString_H


//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <rcstring.h>
#include <rchar.h>
#include <basicstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class implements a unicode string (RChar*) as a class. The class uses a
* shared approach. For example in the following code:
* @code
* RString str1("Hello Wold");
* RString str2;
* str2=str2;
* @endcode
* The strings str1 and str2 use (as long as none of tehm is modified) the same
* copy of the characters.
* @author Pascal Francq
* @short Unicode String
*/
class RString : public BasicString<RChar,RString>
{
	class CharBuffer : public BasicCharBuffer
	{
	public:
		char* Latin1;        // Latin1 version of the string.

		CharBuffer(void)
			: BasicCharBuffer(), Latin1(0) {}
		CharBuffer(RChar* tab,size_t len,size_t maxlen)
			: BasicCharBuffer(tab,len,maxlen), Latin1(0) {}
		void InvalidLatin1(void) {delete[] Latin1; Latin1=0;}
		~CharBuffer(void) {delete[] Latin1;}
	};

	/**
	* Pointer to the buffer representing the null string.
	*/
	static CharBuffer* DataNull;

public:

	/**
	* Null string.
	*/
	static const RString Null;

	/**
	* Construct a empty string.
	*/
	RString(void);

	/**
	* Construct a string from a "C string".
	* @param src             C string" used as reference.
	*/
	RString(const char* src);

	/**
	* Construct a string from a Unicode array.
	* @param src             Array used as reference.
	*/
	RString(const RChar* src);

	/**
	* Construct a string from a string.
	* @param src             String used as reference.
	*/
	RString(const std::string& src);

	/**
	* Construct an empty string with a maximal size.
	* @param maxlen          Initial maximal length of the string.
	*/
	explicit RString(size_t maxlen);

	/**
	* Construct a string from another string.
	* @param src             Source string.
	*/
	RString(const RString& src);

private:

	/**
	* Return the pointer to the "null" buffer. If it is not created, create it.
	* @return Pointer to the "null" buffer.
	*/
	static CharBuffer* GetDataNull(void);

public:

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RString& operator=(const RString& src);

	/**
	* Assignment operator using a "C string".
	* @param src             Source string.
	*/
	RString& operator=(const char* src);

	/**
	* Assignment operator using a string.
	* @param src             Source string.
	*/
	RString& operator=(const std::string& src);

	/**
	* Clear the content of the string.
	*/
	void Clear(void);

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	void Copy(const char* text,size_t nb);

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	void Copy(const RChar* text,size_t nb);

	/**
	* Deep copy of the string if necessary, i.e. when the string points to an
	* internal buffer referenced by other strings, make a copy of it.
	*/
	void Copy(void);

	/**
	* Set the length of the string. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	* @param len             Length of the string.
	*/
	inline void SetLen(size_t len) {BasicString<RChar,RString>::SetLen<CharBuffer>(len);}

	/**
	* Transform the string into a "C String" in Latin1 encoding. The resulting
	* array should be copied (and not destroyed) since it is an internal
	* structure.
	* @return C String
	*/
	const char* Latin1(void) const;

	/**
	* This function returns the character at a given position in the string.
	* (Read only).
	* @param idx             Position of the character.
	* @returns RChar.
	*/
	const RChar& operator[](size_t idx) const;

	/**
	* This function returns the character at a given position in the string.
	* @param idx             Position of the character.
	* @returns RChar.
	*/
	RChar& operator[](size_t idx);

	/**
	* Get a sub-string of a given string.
	* @param idx             Index of the first character.
	* @param len             Length of the sub-string. If the legnth is not
	*                        specified, the end of the string is copied.
	* @returns A RString containing the substring.
	*/
	inline RString Mid(size_t idx,int len=-1) const {return(BasicString<RChar,RString>::Mid<CharBuffer>(idx,len));}

	/**
	* Add another string.
	* @param src             Source string.
	*/
	RString& operator+=(const RString& src);

	/**
	* Add a "C string" to the string.
	* @param src             Source string.
	*/
	RString& operator+=(const char* src);

	/**
	* Add a string to the string.
	* @param src             Source string.
	*/
	RString& operator+=(const RChar* src);

	/**
	* Add a character to the string.
	* @param src             Character.
	*/
	RString& operator+=(const char src);

	/**
	* Add a character to the string.
	* @param src             Character.
	*/
	RString& operator+=(const RChar src);

	/**
	* Return the string in UTF16.
	*/
	const RChar* UTF16(void) const;

	/**
	* Return the string.
	*/
	const RChar* operator()(void) const;

	/**
	* Return the string.
	*/
	operator const char* () const;

	/**
	* Get a normal C++ string representring the current string.
	* @return std::string.
	*/
	operator std::string () const;

	/**
	* Get a normal C++ string representring the current string.
	* @return std::string.
	*/
	std::string ToString(void) const;

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	int Compare(const RString& src) const;

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	int Compare(const char* src) const;

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	int Compare(const RChar* src) const;

	/**
	* Equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator==(const RString& src) const {return(Compare(src)==0);}

	/**
	* Equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator==(const char* src) const {return(Compare(src)==0);}

	/**
	* Equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator==(const RChar* src) const {return(Compare(src)==0);}

	/**
	* Non-equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator!=(const RString& src) const {return(Compare(src));}

	/**
	* Non-equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator!=(const char* src) const {return(Compare(src));}

	/**
	* Non-equal operator.
	* @param src             String to compare with.
	*/
	inline bool operator!=(const RChar* src) const {return(Compare(src));}

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex(const RString& str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex(const char* str);

	/**
	* Return a number between 0 and 26 according to the first character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex(const RChar* str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex2(const RString& str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex2(const char* str);

	/**
	* Return a number between 0 and 26 according to the second character of the
	* string.
	* @param str             String.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	static int HashIndex2(const RChar* str);

private:

	/**
	* Transform a C string into an array of RChar. The resulting array should be
	* destroyed by the caller of the function.
	* @param src             Source "C String".
	* @param len             Length of the string (computed by the function).
	* @param maxlen          Maximum length (may be updated by the function).
	*/
	static RChar* Latin1ToUnicode(const char* src,size_t& len,size_t& maxlen);

	/**
	* Transform an array of RChar into C string. The resulting C string should
	* be destroyed by the caller of the function.
	* @param src             Source array of RChar.
	* @param len             Number of characters in the array.
	*/
	static char* UnicodeToLatin1(const RChar* src,size_t len);

public:

	/**
	* Destruct the string.
	*/
	~RString(void);

	// friend classes
	friend class RCharCursor;
	friend class BasicString<RChar,RString>;
};


//-----------------------------------------------------------------------------
// Operators+
inline const RString operator+(const RString& arg1,const RString& arg2) {return(RString(arg1)+=arg2);}
inline const RString operator+(const RString& arg1,const char* arg2) {return(RString(arg1)+=arg2);}
inline const RString operator+(const char* arg1,const RString& arg2) {return(RString(arg1)+=arg2);}


//-----------------------------------------------------------------------------
// Conversion functions

/**
* Transform an int to a string.
*/
RString itou(const int nb);

/**
* Transform an unsigned int to a string.
*/
RString itou(const unsigned int nb);

/**
* Transform a long to a string.
*/
RString ltou(const long nb);

/**
* Transform an unsigned char to a string.
*/
RString chrtou(const unsigned char c);

/**
* Transform an unsigned long to a string.
*/
RString ltou(const unsigned long nb);

/**
* Transform an unsigned long long to a string.
*/
RString lltou(const unsigned long long nb);

/**
* Transform a float to a string.
*/
RString ftou(const float nb);

/**
* Transform a double to a string.
*/
RString dtou(const double nb);


//-----------------------------------------------------------------------------
/**
* This class represent a cursor to iterate a RString. When the string parsed by
* the cursor is modified, the cursor is not valid anymore.
*
* Here is an example of a cursor used:
* @code
* int example(RString& str)
* {
* 	RCharCursor Cur();
*
* 	Cur.Set(str);
* 	for(Cur.Start();!Cur.End();Cur.Next())
* 		cout<<Cur()->Latin1();
* }
* @endcode
*
* @author Pascal Francq
* @short Characters Cursor.
*/
class RCharCursor
{
	/**
	* This variable is used to go through the container.
	*/
	const RChar* Current;

	/**
	* This variable is used to see if the end of the string is reached.
	*/
	size_t ActPtr;

	/**
	* A Pointer to the string used.
	*/
	const RString* Str;

public:

	/**
	* Construct the cursor.
	*/
	RCharCursor(void);

	/**
	* Construct the cursor.
	* param src              String to iterate.
	*/
	RCharCursor(const RString& src);

	/**
	* Start the iterator to go trough the string.
	*/
	void Start(void);

	/**
	* Set the string and start the iterator.
	* param src              String to iterate.
	*/
	void Set(const RString& src);

	/**
	* Go to the i-th character of the string.
	* @param idx             Index of the character.
	*/
	void GoTo(size_t idx);

	/**
	* Return the number of characters in the string.
	*/
	unsigned int GetNb(void);

	/**
	* Return the position of the cursor in the string.
	*/
	unsigned int GetPos(void);

	/**
	* Test if the end of the string is reached.
	*/
	bool End(void) const;

	/**
	* Goto the next character, if the end is reached, go to the beginning.
	*/
	void Next(void);

	/**
	* Return the current character.
	*/
	RChar operator()(void) const;

	/**
	* This function returns the character at a given position in the string
	* iterated.
	* @param idx             Index of the character.
	* @returns RChar.
	*/
	RChar operator[](size_t idx) const;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
