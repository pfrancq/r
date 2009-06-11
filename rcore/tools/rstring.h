/*

	R Project Library

	RString.h

	Unicode String - Header.

	Copyright 1999-2009 by Pascal Francq (pascal@francq.info).
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
* @author Pascal Francq
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
	* Construct a string by doing a deep copy of the first characters of a "C"
	* string.
	* @param src             C string used as reference.
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

	/** @copydoc BasicString::Copy(const C*,size_t)*/
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
	const char* Latin1(void) const;

	/**
	* Return the string in UTF16.
	*/
	inline const RChar* UTF16(void) const {return(Data->Text);}

	/**
	* Get a normal C++ string representing the current string.
	* @return std::string.
	*/
	std::string ToString(void) const;

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

	/**
	 * Generate an exception related to this string. Its contains:
	 *
	 * "'string': msg".
	 * @param msg            Message to add.
	 */
	void GenerateException(const RString& msg);

public:

	inline void SetLen(size_t len) {BasicString<RChar,RString>::SetLen(len);}

	/** @copydoc BasicString::SetLen(size_t,const S&) */
	inline void SetLen(size_t len,const RString& str) {BasicString<RChar,RString>::SetLen(len,str);}
	//@} Internal methods

	/** @name Manipulation methods	*/ // @{
	inline RString ToUpper(void) const {return(BasicString<RChar,RString>::ToUpper());}

	inline RString ToLower(void) const {return(BasicString<RChar,RString>::ToLower());}

	inline RString Trim(void) const {return(BasicString<RChar,RString>::Trim());}

	/** @copydoc BasicString::Trim(const S&) const */
	inline RString Trim(const RString& str) const {return(BasicString<RChar,RString>::Trim(str));}

	inline bool ContainOnlySpaces(void) const {return(BasicString<RChar,RString>::ContainOnlySpaces());}

	/** @copydoc BasicString::Find(const C,int,bool) const */
	inline int Find(const RChar car,int pos=0,bool CaseSensitive=true) const {return(BasicString<RChar,RString>::Find(car,pos,CaseSensitive));}

	/** @copydoc BasicString::FindStr(const S&,int,bool) const */
	inline int FindStr(const RString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<RChar,RString>::FindStr(str,pos,CaseSensitive));}

	/** @copydoc BasicString::Replace(const C,const C,bool,int) */
	inline void Replace(const RChar search,const RChar rep,bool first=false,int pos=0) {BasicString<RChar,RString>::Replace(search,rep,first,pos);}

	/** @copydoc BasicString::ReplaceStr(const S&,const S&,bool,int) */
	inline void ReplaceStr(const RString& search,const RString& rep,bool first=false,int pos=0) {BasicString<RChar,RString>::ReplaceStr(search,rep,first,pos);}

	inline RString Mid(size_t idx,size_t len=(size_t)-1) const {return(BasicString<RChar,RString>::Mid(idx,len));}

	/** @copydoc BasicString::Split(RContainer<S,true,false>&,const C) const */
	inline void Split(RContainer<RString,true,false>& elements,const RChar car) const {BasicString<RChar,RString>::Split(elements,car);}
	//@} Manipulation methods

	/** @name Methods related to R::RContainer	*/ // @{
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
	* Return a number between 0 and 26 according to the character at position
	* idx in the string.
	* @remark Supported values for idx are 1 and 2.
	* @param idx             Index of hash asked.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	size_t HashIndex(size_t idx) const;
	//@} Methods related to R::RContainer

	/** @name Operators	*/ // @{
	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline const RChar* operator()(void) const {return(Data->Text);}

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline operator const char* (void) const {return(Latin1());}

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

	/** @copydoc BasicString::operator=(const S&) */
	RString& operator=(const RString& src) {return(BasicString<RChar,RString>::operator=(src));}

	/** @copydoc BasicString::operator=(const C*) */
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

	/** @copydoc BasicString::operator+=(const S&) */
	inline RString& operator+=(const RString& src) {return(BasicString<RChar,RString>::operator+=(src));}

	/** @copydoc BasicString::operator+=(const C*) */
	inline RString& operator+=(const RChar* src) {return(BasicString<RChar,RString>::operator+=(src));}

	/**
	* Add a "C string" to the string.
	* @param src             Source string.
	*/
	RString& operator+=(const char* src);

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
	char ToChar(bool& valid);

	/**
	* Try to transform a string into a char (as a number). This version generates an
	* exception if the string does not contain a valid number.
	*/
	char ToChar(void)
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
	int ToInt(bool& valid);

	/**
	* Try to transform a string into an integer. This version generates an
	* exception if the string does not contain a valid number.
	*/
	int ToInt(void)
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
	unsigned int ToUInt(bool& valid);

	/**
	* Try to transform a string into an unsigned integer. This version generates an
	* exception if the string does not contain a valid number.
	*/
	unsigned int ToUInt(void)
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
	long ToLong(bool& valid);

	/**
	* Try to transform a string into a long. This version generates an
	* exception if the string does not contain a valid number.
	*/
	long ToLong(void)
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
	unsigned long ToULong(bool& valid);

	/**
	* Try to transform a string into an unsigned long. This version generates an
	* exception if the string does not contain a valid number.
	*/
	unsigned long ToULong(void)
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
	size_t ToSizeT(bool& valid);

	/**
	* Try to transform a string into a size_t. This version generates an
	* exception if the string does not contain a valid number.
	*/
	size_t ToSizeT(void)
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
	off_t ToOffT(bool& valid);

	/**
	* Try to transform a string into a off_t. This version generates an
	* exception if the string does not contain a valid number.
	*/
	inline off_t ToOffT(void)
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
	float ToFloat(bool& valid);

	/**
	* * Try to transform a string into a float. This version generates an
	* exception if the string does not contain a valid number.
	*/
	float ToFloat(void)
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
	double ToDouble(bool& valid);

	/**
	* * Try to transform a string into a double. This version generates an
	* exception if the string does not contain a valid number.
	*/
	double ToDouble(void)
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
	bool ToBool(bool& valid,bool strict);

	/**
	* Try to transform a string into a boolean value. It recognizes the strings
	* "0", "1", "true" and "false".
	* @param strict          Define if the string recognition is strict ("true"
	*                        and "false") or if upper case letters are allowed.
	*
	* This version generates an exception if the string does not contain a
	* valid boolean value.
	*/
	bool ToBool(bool strict)
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
	size_t GetNb(void);

	/**
	* Return the position of the cursor in the string.
	*/
	size_t GetPos(void);

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
