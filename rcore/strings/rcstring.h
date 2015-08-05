/*

	R Project Library

	RCString.h

	C String - Header.

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
#ifndef RCString_H
#define RCString_H


//-----------------------------------------------------------------------------
// include file for ANSI C/C++
#include <string.h>
#include <stdlib.h>


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rcontainer.h>
#include <basicstring.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// forward declaration
class RCString;


//-----------------------------------------------------------------------------
/**
* This class implements a C string (char*) as a class. The class uses a
* shared approach. For example in the following code:
* @code
* RString str1("Hello Wold");
* RString str2;
* str2=str1;
* @endcode
* The strings str1 and str2 use (as long as none of them is modified) the same
* copy of the characters string.
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
	* Construct a string from a C-style string.
	* @param src             C-style string used as reference.
	*/
	RCString(const char* src);

	/**
	* Construct a string by doing a deep copy of the first characters of a
	* C-style string.
	* @param src             C string used as reference.
	* @param len             Length.
	*/
	RCString(const char* src,size_t len);

	/**
	* Construct a string from a C-style string.
	* @param src             C-style string used as reference.
	*/
	RCString(const std::string& src);

	/**
	* Construct a string from a character.
	* @param car             Character.
	*/
	explicit RCString(char car);

	/**
	* Construct a string from another string.
	* @param src             String used as reference.
	*/
	RCString(const RCString& src);

public:

	/** @name Internal methods	*/ // @{

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	inline void Copy(const char* text,size_t nb) {BasicString<char,RCString>::Copy(text,nb);}

	inline size_t GetLen(void) const {return(BasicString<char,RCString>::GetLen());}

	inline size_t GetMaxLen(void) const {return(BasicString<char,RCString>::GetMaxLen());}

	/**
	* Set the length of the string. If the length is shorter that the current
	* one, the string is truncated. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	* @param len             Length of the string.
	*/
	inline void SetLen(size_t len) {BasicString<char,RCString>::SetLen(len);}

	/**
	* Set the length of the string. If the length is shorter that the current
	* one, the string is truncated. If the length is greater than the current
	* one, the second string is used to fill the first string (eventually it is
	* copied several times).
	* @param len             Length of the string.
	* @param str             String used to fill.
	*/
	inline void SetLen(size_t len,const RCString& str) {BasicString<char,RCString>::SetLen(len,str);}

	inline bool IsEmpty(void) const {return(BasicString<char,RCString>::IsEmpty());}

	inline bool ContainOnlySpaces(void) const {return(BasicString<char,RCString>::ContainOnlySpaces());}
	//@} Internal methods

	/** @name Manipulation methods	*/ // @{

	/**
	* Clear the content of the string.
	*/
	inline void Clear(void) {BasicString<char,RCString>::Clear();}

	/**
	* Get a uppercase version of the string.
	* @return String.
	*/
	inline RCString ToUpper(void) const {return(BasicString<char,RCString>::ToUpper());}

	/**
	* Get a lowercase version of the string.
	* @return String.
	*/
	inline RCString ToLower(void) const {return(BasicString<char,RCString>::ToLower());}

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	* @return String.
	*/
	inline RCString Trim(void) const {return(BasicString<char,RCString>::Trim());}

	/**
	* This function return a string by stripping a given sub-string from the
	* beginning and end of the string.
	* @param str             Sub-string to strip.
	* @return String.
	*/
	inline RCString Trim(const RCString& str) const {return(BasicString<char,RCString>::Trim(str));}

	/**
	* Find the position of a given character in the string.
	* @param car             Character to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int Find(const char car,int pos=0,bool CaseSensitive=true) const {return(BasicString<char,RCString>::Find(car,pos,CaseSensitive));}

	/**
	* Find the position of a given string in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int FindStr(const RCString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<char,RCString>::FindStr(str,pos,CaseSensitive));}

	/**
	* Find the position of a character of a given set in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int FindAnyStr(const RCString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<char,RCString>::FindAnyStr(str,pos,CaseSensitive));}

	/**
	 * Replace a given character in the string.
	 * @param search         Character to search.
	 * @param rep            Character that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	inline void Replace(const char search,const char rep,bool first=false,int pos=0) {BasicString<char,RCString>::Replace(search,rep,first,pos);}

	/**
	 * Replace a given sub-string in the string.
	 * @param search         String to search.
	 * @param rep            String that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	inline void ReplaceStr(const RCString& search,const RCString& rep,bool first=false,int pos=0) {BasicString<char,RCString>::ReplaceStr(search,rep,first,pos);}

	/**
	* Look if a string begins with a given sub-string.
   * @param begin           Sub-string to search for.
   * @param skip            If true, leading spaces are skipped for the
	*                        comparison.
   * @return true if the string begins with the sub-string.
   */
	inline bool Begins(const RCString& begin,bool skip=false) const {return(BasicString<char,RCString>::Begins(begin,skip));}

	/**
	* Look if a string ends with a given sub-string.
   * @param end             Sub-string to search for.
   * @param skip            If true, ending spaces are skipped for the
	*                        comparison.
   * @return true if the string ends with the sub-string.
   */
	inline bool Ends(const RCString& end,bool skip=false) const {return(BasicString<char,RCString>::Ends(end,skip));}

	/**
	* Get a sub-string of a given string.
	* @param idx             Index of the first character.
	* @param len             Length of the sub-string. If the length is not
	*                        specified, the end of the string is copied.
	* @returns A string containing the substring.
	*/
	inline RCString Mid(size_t idx,size_t len=(size_t)-1) const {return(BasicString<char,RCString>::Mid(idx,len));}

	/**
	 * Verify if a given sub-string is at a given position.
	 * @param sub            Sub-string.
	 * @param pos            Position. If negative, the sub-string is searched
	 *                       at a given position of the end.
	 * @return true if the string at a given position.
	 */
	inline bool IsAt(const RCString& sub,int pos) const  {return(BasicString<char,RCString>::IsAt(sub,pos));}

	/**
	 * Insert a sub-string at a given position of the string.
	 * @param sub            Sub-string to insert.
	 * @param pos            Position. If negative, the sub-string is searched
	 *                       at a given position of the end.
	 * @param del            Number of characters that will be replaced by the
	 *                       sub-string. If del=(size_t)-1, all the characters
	 *                       after pos are deleted.
	 */
	void Insert(const RCString& sub,int pos,size_t del=0)  {BasicString<char,RCString>::Insert(sub,pos,del);}

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements        Container that will hold the results. It is not
	*                        emptied by the method.
	* @param car             Character used as separator.
	* @param del             Delimiter of an element.
	*/
	void Split(iRContainer<RCString>& elements,const char car,const char del=0) const;

	/**
	* Try to guess the different words in a given string. In practice, it
	* supposes that certain characters separates words (spaces, ponctuations,
	* uppercase, etc.).
	* @param elements        Container that will hold the results. It is not
	*                        emptied by the method.
	* @param hyphen          Should a hyphen be considered as a separator?
	*/
	void GuessWords(iRContainer<RCString>& elements,bool hyphen) const;

	/**
	 * Concatenate a series of elements and, eventually, separated them by a
	 * given character.
	 * @param elements       Container of elements to concatenate.
	 * @param car            Character used as separator.
    */
	void Concat(const iRContainer<RCString>& elements,const char car=0);

	//@} Manipulation methods

	/** @name Methods related to R::RContainer	*/ // @{
	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	inline int Compare(const RCString& src) const {return(strcmp(Data->Text,src.Data->Text));}

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* src.
	* @param src             String to compare with.
	* @see R::RContainer.
	*/
	inline int Compare(const char* src) const {return(strcmp(Data->Text,src)); }

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
	inline const char* operator()(void) const {return(Data->Text);}

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline operator const char* (void) const {return(Data->Text);}

	/**
	* Get a normal C++ string representing the current string.
	* @return std::string.
	*/
	operator std::string () const {return(Data->Text);}

	inline const char& operator[](size_t idx) const {return(BasicString<char,RCString>::operator[](idx));}

	inline char& operator[](size_t idx) {BasicString<char,RCString>::Copy(); return(BasicString<char,RCString>::operator[](idx));}

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
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RCString& operator=(const RCString& src) {return(BasicString<char,RCString>::operator=(src));}

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RCString& operator=(const char* src) {return(BasicString<char,RCString>::operator=(src));}

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
	* @param src             Character.
	*/
	RCString& operator+=(const char src);

	//@} Operators
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
