/*

	R Project Library

	RString.h

	Unicode String - Header.

	Copyright 1999-2003 by the Universit�Libre de Bruxelles.

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
#ifndef RString_H
#define RString_H


//------------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd/rstd.h>
#include <rstd/rcontainer.h>
#include <rstd/rcstring.h>
#include <rstd/rchar.h>
#include <rstd/rshareddata.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
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
class RString
{
protected:

	// Internal Class representing a buffer of unicode characters.
	class CharBuffer : public RSharedData
	{
	public:
		RChar* Text;         // Text
		unsigned int Len;    // Actual length
		unsigned int MaxLen; // Maximum length
		char* Latin1;        // Latin1 version of the string.

		CharBuffer(void)
			: RSharedData(), Text(0), Len(0), MaxLen(0), Latin1(0) {}
		CharBuffer(RChar* tab,unsigned int len,unsigned int maxlen)
			: RSharedData(), Text(tab), Len(len), MaxLen(maxlen), Latin1(0) {}
		void InvalidLatin1(void) {if(Latin1) {delete[] Latin1; Latin1=0;}}
		void Verify(const unsigned int maxlen);
		~CharBuffer(void)
			{if(Text) delete[] Text;
			 if(Latin1) delete[] Latin1;}
	};

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
	static const RString Null;

	/**
	* Construct a empty string.
	*/
	RString(void);

	/**
	* Construct a string from a "C string".
	* @param text           The "C string" used as reference.
	*/
	RString(const char* text);

	/**
	* Construct a string from a Unicode array.
	* @param text           The array used as reference.
	*/
	RString(const RChar* text);

	/**
	* Construct a string from a string.
	* @param text           The string used as reference.
	*/
	RString(const std::string& text);

	/**
	* Construct an empty string with a maximal size.
	* @param maxlen         Initial maximal length of the string.
	*/
	RString(unsigned int maxlen);

	/**
	* Construct a string from another string.
	* @param str            String used.
	*/
	RString(const RString& str);

	/**
	* Construct a string from another string.
	* @param str            Pointer to the string.
	*/
	RString(const RString* str);

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
	RString& operator=(const RString& str);

	/**
	* Assignment operator using a "C string".
	*/
	RString& operator=(const char* text);

	/**
	* Assignment operator using a string.
	*/
	RString& operator=(const std::string& text);

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

	/**
	* Copy a certain number of characters in the string.
	* @param text           Text to copy.
	* @param nb             Numbre of characters to copy.
	*/
	void Copy(const RChar* text,unsigned int nb);

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
	RString ToUpper(void) const;

	/**
	* Get a lowercase version of the string.
	* @return RString.
	*/
	RString ToLower(void) const;

	/**
	* Return the length of the string.
	*/
	inline unsigned int GetLen(void) const {return(Data->Len);}

	/**
	* Set the length of the string. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	*/
	void SetLen(unsigned int len);

	/**
	* Return the maximal length of the string.
	*/
	inline unsigned int GetMaxLen(void) const {return(Data->MaxLen);}

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	inline bool IsEmpty(void) const {return(!Data->Len);}

	/**
	* Transform the string into a "C String" in Latin1 encoding. The resulting
	* array should be copied (and not destroyed) since it is an internal
	* structure.
	* @return C String
	*/
	const char* Latin1(void) const;

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	*/
	RString Trim(void) const;

	/**
	* This function returns the character at a given position in the string. If
	* the position is outside the string, the null character is returned.
	* @param pos            Position of the character.
	* @returns RChar.
	*/
	RChar operator[](int pos) const;

	/**
	* Find the position of a given character in the string.
	* @param car            Character to find.
	* @param pos            Position to start the search. Negative values start
	*                       the search from the end.
	* @param CaseSensitive  Is the search case sensitive.
	* @return The position of the first occurence or -1 if the character was not
	*         found.
	*/
	int Find(const RChar car,int pos=0,bool CaseSensitive=true) const;

	/**
	* Find the position of a given string in the string.
	* @param str            String to find.
	* @param pos            Position to start the search. Negative values start
	*                       the search from the end.
	* @param CaseSensitive  Is the search case sensitive.
	* @return The position of the first occurence or -1 if the character was not
	*         found.
	*/
	int FindStr(const RString str,int pos=0,bool CaseSensitive=true) const;

	/**
	* Get a sub-string of a given string.
	* @param idx            Index of the first character.
	* @param len            Length of the sub-string. If the legnth is not
	*                       specified, the end of the string is copied.
	* @returns A RString containing the substring.
	*/
	RString Mid(unsigned int idx,unsigned int len = 0xFFFFFFFF) const;

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements       Container that will hold the results.
	* @param car            Character used as separator.
	*/
	void Split(RContainer<RString,true,false>& elements,const RChar car) const;

	/**
	* Add another string.
	*/
	RString& operator+=(const RString& str);

	/**
	* Add a "C string" to the string.
	*/
	RString& operator+=(const char* text);

	/**
	* Add a string to the string.
	*/
	RString& operator+=(const RChar* text);

	/**
	* Add a character to the string.
	*/
	RString& operator+=(const char c);

	/**
	* Add a character to the string.
	*/
	RString& operator+=(const RChar c);

	/**
	* Return the string in UTF16.
	*/
	inline const RChar* UTF16(void) const {return(Data->Text);}

	/**
	* Return the string.
	*/
	inline const RChar* operator()(void) const {return(Data->Text);}

	/**
	* Return the string.
	*/
	operator const char* ();

	/**
	* Get a normal C++ string representring the current string.
	* @return std::string.
	*/
	operator std::string () const;

	/**
	* Get a normal C++ string representring the current string.
	* @return std::string.
	*/
	std::string ToString(void) const {return(operator std::string());}

	/**
	* Equal operator.
	*/
	bool operator==(const RString& str) const;

	/**
	* Equal operator.
	*/
	bool operator==(const char* str) const;

	/**
	* Equal operator.
	*/
	bool operator==(const RChar* str) const;

	/**
	* Non-equal operator.
	*/
	bool operator!=(const RString& str) const;

	/**
	* Non-equal operator.
	*/
	bool operator!=(const char* str) const;

	/**
	* Non-equal operator.
	*/
	bool operator!=(const RChar* str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString& str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RString* str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const char* str) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	*/
	int Compare(const RChar* str) const;

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
	* Transform a C string into an array of RChar. The resulting array should be
	* destroyed by the caller of the function.
	* @param str             Source "C String".
	* @param len             Length of the string (computed by the function).
	* @param maxlen          Maximum length (may be updated by the function).
	*/
	static RChar* Latin1ToUnicode(const char* str,unsigned int& len, unsigned int& maxlen);

	/**
	* Transform an array of RChar into C string. The resulting C string should
	* be destroyed by the caller of the function.
	* @param tab             Source array of RChar.
	* @param len             Number of characters in the array.
	*/
	static char* UnicodeToLatin1(const RChar* tab,unsigned int len);

	/**
	* Destruct the string.
	*/
	virtual ~RString(void);

	// friend classes
	friend class RCharCursor;
};


//------------------------------------------------------------------------------
// Operators

/**
* Add two strings together.
*/
RString operator+(const RString& arg1,const RString& arg2);

/**
* Add a string and a "C string" together.
*/
RString operator+(const RString& arg1,const char* arg2);

/**
* Add a "C string" and a string together.
*/
RString operator+(const char* arg1,const RString& arg2);


//------------------------------------------------------------------------------
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
* Transform a float to a string.
*/
RString ftou(const float nb);

/**
* Transform a double to a string.
*/
RString dtou(const double nb);


//------------------------------------------------------------------------------
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
	unsigned int ActPtr;

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
	* param str             String to iterate.
	*/
	RCharCursor(const RString& str);

	/**
	* Construct the cursor.
	* param str             String to iterate.
	*/
	RCharCursor(const RString* str);

	/**
	* Start the iterator to go trough the string.
	*/
	void Start(void) throw(RException);

	/**
	* Set the string and start the iterator.
	* param str             String to iterate.
	*/
	void Set(const RString& str);

	/**
	* Set the string and start the iterator.
	* param str             String to iterate.
	*/
	void Set(const RString* str);

	/**
	* Go to the i-th character of the string.
	*/
	void GoTo(const unsigned int i) throw(RException);

	/**
	* Return the number of characters in the string.
	*/
	unsigned int GetNb(void)  throw(RException);

	/**
	* Return the position of the cursor in the string.
	*/
	unsigned int GetPos(void) throw(RException);

	/**
	* Test if the end of the string is reached.
	*/
	bool End(void) const throw(RException);

	/**
	* Goto the next character, if the end is reached, go to the beginning.
	*/
	void Next(void) throw(RException);

	/**
	* Return the current character.
	*/
	RChar operator()(void) const throw(RException);

	/**
	* This function returns the character at a given position in the string
	* iterated. If the position is outside this string, an exception
	* is generated.
	* @param pos            Position of the character.
	* @returns RChar.
	*/
	RChar operator[](unsigned int pos) const throw(RException);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif
