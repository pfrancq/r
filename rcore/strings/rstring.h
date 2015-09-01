/*

	R Project Library

	RString.h

	Unicode String - Header.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).
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



//-----------------------------------------------------------------------------
#ifndef RString_H
#define RString_H


//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <cstring>


//-----------------------------------------------------------------------------
// include files for R Project
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
* str2=str1;
* @endcode
* The strings str1 and str2 use (as long as none of them is modified) the same
* copy of the characters string.
*
* The RStringBuilder class is an efficient implementation to build string by
* adding characters.
* @short Unicode String
*/
class RString : public BasicString<RChar,RString>
{
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
	* Construct a string from a C-style string.
	* @param src             C-style string used as reference.
	*/
	RString(const char* src);

	/**
	* Construct a string from a Unicode array.
	* @param src             Array used as reference.
	*/
	RString(const RChar* src);

	/**
	* Construct a string by doing a deep copy of the first characters of a
	* C-style string.
	* @param src             C-style string used as reference.
	* @param len             Length.
	*/
	RString(const RChar* src,size_t len);

	/**
	* Construct a string from a string.
	* @param src             String used as reference.
	*/
	RString(const std::string& src);

	/**
	* Construct a string from a character.
	* @param car             Character.
	*/
	explicit RString(char car);

	/**
	* Construct a string from a character.
	* @param car             Character.
	*/
	explicit RString(RChar car);

	/**
	* Construct a string from another string.
	* @param src             Source string.
	*/
	RString(const RString& src);

	/** @name Internal methods	*/ // @{

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	inline void Copy(const RChar* text,size_t nb) {BasicString<RChar,RString>::Copy(text,nb);}

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	void Copy(const char* text,size_t nb);

	/**
	* Transform the string into a "C String" in Latin1 encoding. The resulting
	* array should be copied (and not destroyed) since it is an internal
	* structure.
	* @return C String
	*/
	const char* ToLatin1(void) const;

	/**
	* Return the string in UTF16.
	*/
	inline const RChar* ToUTF16(void) const {return(Data->Text);}

	/**
	* Get a normal C++ string representing the current string.
	* @return std::string.
	*/
	std::string ToString(void) const;

private:

	/**
	* Transform a C string into an array of RChar. The resulting array should be
	* destroyed by the caller of the function.
	* @param src             C-style string.
	* @param len             Length of the string (computed by the function).
	* @param maxlen          Maximum length (may be updated by the function).
	*/
	static RChar* Latin1ToUnicode(const char* src,size_t& len,size_t& maxlen);

	/**
	* Transform the RString into C string. The resulting C string should be
	* destroyed by the caller of the function.
	* @param escape         Escape invalid sequences of bytes?
	*/
	char* UnicodeToLatin1(bool escape=false) const;

	/**
	 * Generate an exception related to this string. Its contains:
	 *
	 * "'string': msg".
	 * @param msg            Message to add.
	 */
	void GenerateException(const RString& msg) const;

public:

	inline size_t GetLen(void) const {return(BasicString<RChar,RString>::GetLen());}

	inline size_t GetMaxLen(void) const {return(BasicString<RChar,RString>::GetMaxLen());}

	/**
	* Set the length of the string. If the length is shorter that the current
	* one, the string is truncated. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	* @param len             Length of the string.
	*/
	inline void SetLen(size_t len)
	{
		BasicString<RChar,RString>::SetLen(len);
		if(Data)
			Data->InvalidLatin1();
	}

	/**
	* Set the length of the string. If the length is shorter that the current
	* one, the string is truncated. If the length is greater than the current
	* one, the second string is used to fill the first string (eventually it is
	* copied several times).
	* @param len             Length of the string.
	* @param str             String used to fill.
	*/
	inline void SetLen(size_t len,const RString& str) {BasicString<RChar,RString>::SetLen(len,str);}

	inline bool IsEmpty(void) const {return(BasicString<RChar,RString>::IsEmpty());}

	inline bool ContainOnlySpaces(void) const {return(BasicString<RChar,RString>::ContainOnlySpaces());}
	//@} Internal methods

	/** @name Manipulation methods	*/ // @{

	/**
	* Clear the content of the string. The string is deallocated and set to
	* RSting::Null. If the memory must not be deallocated (for example if the
	* string holds multiple temporary values), it is better to use
	* RString::SetLen.
	*/
	inline void Clear(void) {BasicString<RChar,RString>::Clear();}

	/**
	* Get a uppercase version of the string.
	* @return String.
	*/
	inline RString ToUpper(void) const {return(BasicString<RChar,RString>::ToUpper());}

	/**
	* Get a lowercase version of the string.
	* @return String.
	*/
	inline RString ToLower(void) const {return(BasicString<RChar,RString>::ToLower());}

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	* @return String.
	*/
	inline RString Trim(void) const {return(BasicString<RChar,RString>::Trim());}

	/**
	* This function return a string by stripping a given sub-string from the
	* beginning and end of the string.
	* @param str             Sub-string to strip.
	* @return String.
	*/
	inline RString Trim(const RString& str) const {return(BasicString<RChar,RString>::Trim(str));}

	/**
	* Find the position of a given character in the string.
	* @param car             Character to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int Find(const RChar car,int pos=0,bool CaseSensitive=true) const {return(BasicString<RChar,RString>::Find(car,pos,CaseSensitive));}

	/**
	* Find the position of a given string in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int FindStr(const RString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<RChar,RString>::FindStr(str,pos,CaseSensitive));}

	/**
	* Find the position of a character of a given set in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int FindAnyStr(const RString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<RChar,RString>::FindAnyStr(str,pos,CaseSensitive));}

	/**
	 * Replace a given character in the string.
	 * @param search         Character to search.
	 * @param rep            Character that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	inline void Replace(const RChar search,const RChar rep,bool first=false,int pos=0) {BasicString<RChar,RString>::Replace(search,rep,first,pos);}

	/**
	 * Replace a given sub-string in the string.
	 * @param search         String to search.
	 * @param rep            String that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	inline void ReplaceStr(const RString& search,const RString& rep,bool first=false,int pos=0) {BasicString<RChar,RString>::ReplaceStr(search,rep,first,pos);}

	/**
	* Look if a string begins with a given sub-string.
   * @param begin           Sub-string to search for.
   * @param skip            If true, leading spaces are skipped for the
	*                        comparison.
   * @return true if the string begins with the sub-string.
   */
	inline bool Begins(const RString& begin,bool skip=false) const {return(BasicString<RChar,RString>::Begins(begin,skip));}

	/**
	* Look if a string ends with a given sub-string.
   * @param end             Sub-string to search for.
   * @param skip            If true, ending spaces are skipped for the
	*                        comparison.
   * @return true if the string ends with the sub-string.
   */
	inline bool Ends(const RString& end,bool skip=false) const {return(BasicString<RChar,RString>::Ends(end,skip));}

	/**
	* Get a sub-string of a given string.
	* @param idx             Index of the first character.
	* @param len             Length of the sub-string. If the length is not
	*                        specified, the end of the string is copied.
	* @returns A string containing the substring.
	*/
	inline RString Mid(size_t idx,size_t len=(size_t)-1) const {return(BasicString<RChar,RString>::Mid(idx,len));}

	/**
	 * Verify if a given sub-string is at a given position.
	 * @param sub            Sub-string.
	 * @param pos            Position. If negative, the sub-string is searched
	 *                       at a given position of the end.
	 * @return true if the string at a given position.
	 */
	inline bool IsAt(const RString& sub,int pos) const  {return(BasicString<RChar,RString>::IsAt(sub,pos));}

	/**
	 * Insert a sub-string at a given position of the string.
	 * @param sub            Sub-string to insert.
	 * @param pos            Position. If negative, the sub-string is searched
	 *                       at a given position of the end.
	 * @param del            Number of characters that will be replaced by the
	 *                       sub-string. If del=(size_t)-1, all the characters
	 *                       after pos are deleted.
	 */
	inline void Insert(const RString& sub,int pos,size_t del=0)  {BasicString<RChar,RString>::Insert(sub,pos,del);}

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements        Container that will hold the results. It is not
	*                        emptied by the method.
	* @param car             Character used as separator.
	* @param del             Delimiter of an element.
	*/
	void Split(iRContainer<RString>& elements,const RChar car,const RChar del=RChar(0)) const;

	/**
	* Try to guess the different words in a given string. In practice, it
	* supposes that certain characters separates words (spaces, ponctuations,
	* uppercase, etc.).
	* @param elements        Container that will hold the results. It is not
	*                        emptied by the method.
	* @param hyphen          Should a hyphen be considered as a separator?
	*/
	void GuessWords(iRContainer<RString>& elements,bool hyphen=false) const;

	/**
	 * Concatenate a series of elements and, eventually, separated them by a
	 * given character.
	 * @param elements       Container of elements to concatenate.
	 * @param car            Character used as separator.
    */
	void Concat(const iRContainer<RString>& elements,const RChar car=RChar(0));

	//@} Manipulation methods

	/** @name Methods related to  containers */
	// @{
	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	inline int Compare(const RString& src) const {return(strcmp(Data->Text,src.Data->Text));}

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	inline int Compare(const char* src) const {return(strcmp(Data->Text,src)); }

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	inline int Compare(const RChar* src) const {return(strcmp(Data->Text,src));}

	/**
	* Compute a hash code that in the range [0,max-1]. In practice, in computes
	* a sum based on the Unicode values the characters.
	*
	* This method is R::RHashContainer to manage string.
	* @param max             Maximal value.
	* @return a number in the range [0,max-1].
	*/
	size_t HashCode(size_t max) const;

	/**
	 * Static function used to order a container of strings in ascending order.
    * @param a              First string.
	 * @param b              Second string.
	 * @see R::RContainer.
    */
	static int SortOrder(const void* a,const void* b);

	/**
	 * Static function used to order a container of  strings in descending order.
    * @param a              First string.
	 * @param b              Second string.
	 * @see R::RContainer.
    */
	static int SortDesOrder(const void* a,const void* b);
	//@} Methods related to containers

	/** @name Operators	*/ // @{
	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline RChar* operator()(void) {BasicString<RChar,RString>::Copy(); return(Data->Text);}

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline const RChar* operator()(void) const {return(Data->Text);}

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline operator const char* (void) const {return(ToLatin1());}

	/**
	* Get a normal C++ string representing the current string.
	* @return std::string.
	*/
	operator std::string () const;

	/**
	* This function returns the character at a given position in the string.
	* (Read only).
	* @param idx             Position of the character.
	*/
	inline const RChar& operator[](size_t idx) const {return(BasicString<RChar,RString>::operator[](idx));}

	/**
	* This function returns the character at a given position in the string.
	* @param idx             Position of the character.
	*/
	inline RChar& operator[](size_t idx) {return(BasicString<RChar,RString>::operator[](idx));}

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
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RString& operator=(const RString& src) {return(BasicString<RChar,RString>::operator=(src));}

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RString& operator=(const RChar* src) {return(BasicString<RChar,RString>::operator=(src));}

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
	* Add another string.
	* @param src             Source string.
	*/
	inline RString& operator+=(const RString& src) {return(BasicString<RChar,RString>::operator+=(src));}

	/**
	* Add another string.
	* @param src             Source string.
	*/
	inline RString& operator+=(const RChar* src) {return(BasicString<RChar,RString>::operator+=(src));}

	/**
	* Add a "C string" to the string.
	* @param src             Source string.
	*/
	RString& operator+=(const char* src);

	/**
	* Add a character to the string. If mutliple characters must be added, it is
	* more efficient to use RStringBuilder.
	* @param src             Character.
	*/
	RString& operator+=(const char src);

	/**
	* Add a character to the string. If mutliple characters must be added, it is
	* more efficient to use RStringBuilder.
	* @param src             Character.
	*/
	RString& operator+=(const RChar src);
	//@} Operators

	//-----------------------------------------------------------------------------
	// Static conversion functions
	/** @name Number to string methods	*/ // @{
	/**
	 * Transform an int to a string.
	 * @param nb             Number.
	 */
	static RString Number(const int nb);

	/**
	 * Transform an size_t to a string.
	 * @param nb             Number.
	 */
	static RString Number(const unsigned int nb);

	/**
	 * Transform a long to a string.
	 * @param nb             Number.
	 */
	static RString Number(const long nb);

	/**
	 * Transform an unsigned long to a string.
	 * @param nb             Number.
	 */
	static RString Number(const unsigned long nb);

	/**
	 * Transform an unsigned long long to a string.
	 * @param nb             Number.
	 */
	static RString Number(const long long nb);

	/**
	 * Transform an unsigned long long to a string.
	 * @param nb             Number.
	 */
	static RString Number(const unsigned long long nb);

	/**
	 * Transform a float to a string.
	 * @param nb             Number.
	 * @param format         String representing the format.
	 */
	static RString Number(const float nb,const char* format="%E");

	/**
	 * Transform a double to a string.
	 * @param nb             Number.
	 * @param format         String representing the format.
	 */
	static RString Number(const double nb,const char* format="%E");

	/**
	 * Transform a long double to a string.
	 * @param nb             Number.
	 * @param format         String representing the format.
	 */
	static RString Number(const long double nb,const char* format="%LE");
	//@} Number to string methods

	/** @name String to number methods	*/ // @{
	/**
	* Try to transform a string into a char (as a number).
	* @param valid           Variable becomes true if the conversion was done.
	*/
	char ToChar(bool& valid) const;

	/**
	* Try to transform a string into a char (as a number). This version generates an
	* exception if the string does not contain a valid number.
	*/
	char ToChar(void) const
	{
		bool Valid;
		char Val(ToChar(Valid));
		if(!Valid)
			GenerateException("is not a char");
		return(Val);
	}

	/**
	* Try to transform a string into an integer.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	int ToInt(bool& valid) const;

	/**
	* Try to transform a string into an integer. This version generates an
	* exception if the string does not contain a valid number.
	*/
	int ToInt(void) const
	{
		bool Valid;
		int Val(ToInt(Valid));
		if(!Valid)
			GenerateException("is not an int");
		return(Val);
	}

	/**
	* Try to transform a string into an unsigned integer.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	unsigned int ToUInt(bool& valid) const;

	/**
	* Try to transform a string into an unsigned integer. This version generates an
	* exception if the string does not contain a valid number.
	*/
	unsigned int ToUInt(void) const
	{
		bool Valid;
		unsigned int Val(ToUInt(Valid));
		if(!Valid)
			GenerateException("is not an unsigned int");
		return(Val);
	}

	/**
	* Try to transform a string into a long.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	long ToLong(bool& valid) const;

	/**
	* Try to transform a string into a long. This version generates an
	* exception if the string does not contain a valid number.
	*/
	long ToLong(void) const
	{
		bool Valid;
		long Val(ToLong(Valid));
		if(!Valid)
			GenerateException("is not a long");
		return(Val);
	}

	/**
	* Try to transform a string into an unsigned long.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	unsigned long ToULong(bool& valid) const;

	/**
	* Try to transform a string into an unsigned long. This version generates an
	* exception if the string does not contain a valid number.
	*/
	unsigned long ToULong(void) const
	{
		bool Valid;
		unsigned long Val(ToULong(Valid));
		if(!Valid)
			GenerateException("is not an unsigned long");
		return(Val);
	}

	/**
	* Try to transform a string into a size_t.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	size_t ToSizeT(bool& valid) const;

	/**
	* Try to transform a string into a size_t. This version generates an
	* exception if the string does not contain a valid number.
	*/
	size_t ToSizeT(void) const
	{
		bool Valid;
		size_t Val(ToSizeT(Valid));
		if(!Valid)
			GenerateException("is not a size_t");
		return(Val);
	}

	/**
	* Try to transform a string into a off_t.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	off_t ToOffT(bool& valid) const;

	/**
	* Try to transform a string into a off_t. This version generates an
	* exception if the string does not contain a valid number.
	*/
	inline off_t ToOffT(void) const
	{
		bool Valid;
		off_t Val(ToOffT(Valid));
		if(!Valid)
			GenerateException("is not an off_t");
		return(Val);
	}

	/**
	* * Try to transform a string into a float.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	float ToFloat(bool& valid) const;

	/**
	* * Try to transform a string into a float. This version generates an
	* exception if the string does not contain a valid number.
	*/
	float ToFloat(void) const
	{
		bool Valid;
		float Val(ToFloat(Valid));
		if(!Valid)
			GenerateException("is not a float");
		return(Val);
	}

	/**
	* * Try to transform a string into a double.
	* @param valid           Variable becomes true if the conversion was done.
	*/
	double ToDouble(bool& valid) const;

	/**
	* * Try to transform a string into a double. This version generates an
	* exception if the string does not contain a valid number.
	*/
	double ToDouble(void) const
	{
		bool Valid;
		double Val(ToDouble(Valid));
		if(!Valid)
			GenerateException("is not a double");
		return(Val);
	}

	/**
	* Try to transform a string into a boolean value. It recognizes the strings
	* "0", "1", "true" and "false".
	* @param valid           Variable becomes true if the conversion was done.
	* @param strict          Define if the string recognition is strict ("true"
	*                        and "false") or if upper case letters are allowed.
	*/
	bool ToBool(bool& valid,bool strict) const;

	/**
	* Try to transform a string into a boolean value. It recognizes the strings
	* "0", "1", "true" and "false".
	* @param strict          Define if the string recognition is strict ("true"
	*                        and "false") or if upper case letters are allowed.
	*
	* This version generates an exception if the string does not contain a
	* valid boolean value.
	*/
	bool ToBool(bool strict) const
	{
		bool Valid;
		bool Val(ToBool(Valid,strict));
		if(!Valid)
			GenerateException("is not a valid bool");
		return(Val);
	}
	//@} String to number methods

	// friend classes
	friend class RCharCursor;
};


//-----------------------------------------------------------------------------
// C++ operators

/**
* Add two strings.
* @param arg1                First string.
* @param arg2                Second string.
*/
inline const RString operator+(const RString& arg1,const RString& arg2) {return(RString(arg1)+=arg2);}

/**
* Add two strings.
* @param arg1                First string.
* @param arg2                Second string.
*/
inline const RString operator+(const RString& arg1,const char* arg2) {return(RString(arg1)+=arg2);}

/**
* Add two strings.
* @param arg1                First string.
* @param arg2                Second string.
*/
inline const RString operator+(const char* arg1,const RString& arg2) {return(RString(arg1)+=arg2);}

/**
* Add a string and a character.
* @param arg1                Character.
* @param arg2                String.
*/
inline const RString operator+(char arg1,const RString& arg2) {return(RString(arg1)+=arg2);}

/**
* Add a string and a character.
* @param arg1                Character.
* @param arg2                String.
*/
inline const RString operator+(RChar arg1,const RString& arg2) {return(RString(arg1)+=arg2);}

/**
* Add a character and a string.
* @param arg1                String.
* @param arg2                Character.
*/
inline const RString operator+(const RString& arg1,char arg2) {return(RString(arg1)+=arg2);}

/**
* Add a string and a character.
* @param arg1                Character.
* @param arg2                String.
*/
inline const RString operator+(const RString& arg1,RChar arg2) {return(RString(arg1)+=arg2);}



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
* 		cout<<Cur().ToLatin1();
* }
* @endcode
*
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

	/**
	* The number of characters in the cursor.
	*/
	size_t CurNbPtr;

	/**
	* The first position in the string handled by the cursor.
	*/
	size_t FirstPtr;

	/**
	* The last position in the string handled by the cursor.
	*/
	size_t LastPtr;

public:

	/**
	* Construct the cursor.
	*/
	RCharCursor(void);

	/**
	* Construct the cursor.
	* param src              String to iterate.
	* @param min             Minimum position of the string to iterate.
	* @param max             Maximum position of the string to iterate (included max).
	*                        If SIZE_MAX, iterate until the end of the string.
	*/
	RCharCursor(const RString& src,size_t min=0,size_t max=SIZE_MAX);

	/**
	* Set the string and start the iterator.
	* param src              String to iterate.
	* @param min             Minimum position of the string to iterate.
	* @param max             Maximum position of the string to iterate (included max).
	*                        If SIZE_MAX, iterate until the end of the string.
	*/
	void Set(const RString& src,size_t min=0,size_t max=SIZE_MAX);

	/**
	* Start the iterator to go trough the string.
	*/
	void Start(void);

	/**
	 * Start the iterator at the end of the string.
	 */
	void StartFromEnd(void);

	/**
	* Go to the i-th character of the string.
	* @param idx             Index of the character.
	*/
	void GoTo(size_t idx);

	/**
	* Return the number of characters in the string.
	*/
	size_t GetNb(void);

	/**
	* Return the position of the cursor in the string.
	*/
	size_t GetPos(void);

	/**
	* Test if the begin of the cursor is reached.
	*/
	bool Begin(void) const;

	/**
	* Test if the end of the string is reached.
	*/
	bool End(void) const;

	/**
	* Goto the next character, if the end is reached, go to the beginning.
	* @param inc             Number of elements to go to.
	*/
	void Next(size_t inc=1);

	/**
	* Go to a given number of previous elements. If the begin is reached, go to the
	* end.
	* @param inc             Number of elements to go to.
	*/
	void Prev(size_t inc=1);

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

	/**
	 * Get the pointer to the current place of the string.
    * @return a constant pointer on the current character.
    */
	const RChar* GetCurrent(void) const {return(Current);}
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
