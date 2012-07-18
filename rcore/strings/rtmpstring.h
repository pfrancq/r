/*

	R Project Library

	RTmpString.h

	Temporary String - Header.

	Copyright 2011-2012 by Pascal Francq (pascal@francq.info).

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
#ifndef RTmpString_H
#define RTmpString_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rchar.h>
#include <stringmethods.h>
#include <rtextencoding.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<class C,bool bAlloc,bool bOrder> class RContainer;


//-----------------------------------------------------------------------------
/**
* This class implements a temporary string of Unicode characters (RChar). The
* template proposes to instantiate strings of a given size. Contrary to
* RString, RTmpString does not allocate the text on the heap but on the stack.
* It should therefore be optimized for temporary string (as the name suggests)
* where a maximum size is known.
* @warning Some compilers generate the codes for each instantiation. It is
* therefore a good practice to limit the number of instantiations. Moreover,
* since these strings occupy the stack, using too many of them with large sizes
* can lead to a problem (in particular for recursive functions).
* @tparam MaxLen             Maximum size of the string.
* @author Pascal Francq
* @short Temporary String.
*/
template<size_t MaxLen>
	class RTmpString
{
	/**
	 * Text.
	 */
	RChar Text[MaxLen+1];

	/**
	 * Actual length.
	 */
	size_t Len;

	/**
	 * Latin1 version of the string.
	 */
	char Latin1[MaxLen+1];

public:

	/**
	* Construct a null string.
	*/
	RTmpString(void);

	/**
	* Copy constructor.
	* @param str             Original string.
	*/
	RTmpString(const RTmpString& str);

	/**
	* Construct a string with one character.
	* @param car             Character.
	*/
	RTmpString(const RChar car);

	/**
	* Construct a string from an array.
	* @param src             Array used as reference.
	*/
	RTmpString(const char* src);

	/**
	* Construct a string from an array.
	* @param src             Array used as reference.
	*/
	RTmpString(const RChar* src);

	/**
	* Construct a string by doing a deep copy of the first characters of a
	* C-style string.
	* @param src             C-style string used as reference.
	* @param len             Length.
	*/
	RTmpString(const RChar* src,size_t len);

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	void Copy(const RChar* text,size_t nb);

private:

	/**
	* Transform the RString into C string. The resulting C string should be
	* destroyed by the caller of the function.
	*/
	void UnicodeToLatin1(void) const;

public:

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
	inline const RChar* ToUTF16(void) const {return(Text);}

	/**
	* Get a normal C++ string representing the current string.
	* @return std::string.
	*/
	std::string ToString(void) const;

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RTmpString& operator=(const RTmpString& src);

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RTmpString& operator=(const char* src);

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	RTmpString& operator=(const RChar* src);


	/**
	* Return the length of the string.
	*/
	inline size_t GetLen(void) const {return(Len);}

	/**
	* Return the maximal length of the string.
	*/
	inline size_t GetMaxLen(void) const {return(MaxLen);}

	/**
	* Set the length of the string. If the length is shorter that the current
	* one, the string is truncated. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	* @param len             Length of the string.
	*/
	void SetLen(size_t len);

	/**
	* Set the length of the string. If the length is shorter that the current
	* one, the string is truncated. If the length is greater than the current
	* one, the second string is used to fill the first string (eventually it is
	* copied several times).
	* @param len             Length of the string.
	* @param str             String used to fill.
	*/
	void SetLen(size_t len,const RChar* str);

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	inline bool IsEmpty(void) const {return(!Len);}

	/**
	* Clear the content of the string.
	*/
	void Clear(void);

	/**
	* Get a uppercase version of the string.
	* @return String.
	*/
	void ToUpper(void) const;

	/**
	* Get a lowercase version of the string.
	* @return String.
	*/
	void ToLower(void) const;

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	* @return String.
	*/
	void Trim(void) const;

	/**
	* This function return a string by stripping a given sub-string from the
	* beginning and end of the string.
	* @param str             Sub-string to strip.
	* @return String.
	*/
	void Trim(const RChar* str) const;

	/**
	 * Look if the string contains only spaces.
	 */
	bool ContainOnlySpaces(void) const {return(StringMethods::ContainOnlySpaces<RChar>(Text));}

	/**
	* Add another string.
	* @param src             Source string.
	*/
	RTmpString& operator+=(const RTmpString& src);

	/**
	* Add a string to the string.
	* @param src             Source string.
	*/
	RTmpString& operator+=(const char* src);

	/**
	* Add a string to the string.
	* @param src             Source string.
	*/
	RTmpString& operator+=(const RChar* src);

	/**
	* Find the position of a given character in the string.
	* @param car             Character to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	int Find(const RChar car,int pos=0,bool CaseSensitive=true) const {return(StringMethods::ContainOnlySpaces<RChar>(Text));}

	/**
	* Find the position of a given string in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	int FindStr(const RTmpString& str,int pos=0,bool CaseSensitive=true) const {return(StringMethods::FindStr<RChar>(Text,Len,pos,CaseSensitive));}

	/**
	* Find the position of a character of a given set in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	int FindAnyStr(const RTmpString& str,int pos=0,bool CaseSensitive=true) const {return(StringMethods::FindAnyStr<RChar>(Text,Len,str.Text,str.Len,pos,CaseSensitive));}

	/**
	 * Replace a given character in the string.
	 * @param search         Character to search.
	 * @param rep            Character that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	void Replace(const RChar search,const RChar rep,bool first=false,int pos=0);

	/**
	 * Replace a given sub-string in the string.
	 * @param search         String to search.
	 * @param rep            String that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	void ReplaceStr(const RTmpString& search,const RTmpString& rep,bool first=false,int pos=0);

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline RChar* operator()(void) {return(Text);}

	/**
	* Return the string.  The resulting array should be copied (and not
	* destroyed) since it is an internal structure.
	*/
	inline const RChar* operator()(void) const {return(Text);}

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
	const RChar& operator[](size_t idx) const;

	/**
	* This function returns the character at a given position in the string.
	* @param idx             Position of the character.
	*/
	RChar& operator[](size_t idx);

	/**
	* Look if a string begins with a given sub-string.
   * @param begin           Sub-string to search for.
   * @param skip            If true, leading spaces are skipped for the
	*                        comparison.
	* @param CaseSensitive   Is the search case sensitive.
   * @return true if the string begins with the sub-string.
   */
	bool Begins(const RTmpString& begin,bool skip=false,bool CaseSensitive=true) const {return(StringMethods::Begins<RChar>(Text,Len,begin.Text,begin.Len,skip,CaseSensitive));}

	/**
	* Look if a string ends with a given sub-string.
   * @param begin           Sub-string to search for.
   * @param skip            If true, ending spaces are skipped for the
	*                        comparison.
	* @param CaseSensitive   Is the search case sensitive.
   * @return true if the string ends with the sub-string.
   */
	bool Ends(const RTmpString& end,bool skip=false,bool CaseSensitive=true) const {return(StringMethods::Ends<RChar>(Text,Len,end.Text,end.Len,skip,CaseSensitive));}

	/**
	* Get a sub-string of a given string.
	* @param idx             Index of the first character.
	* @param len             Length of the sub-string. If the length is not
	*                        specified, the end of the string is copied.
	* @returns A string containing the substring.
	*/
	void Mid(RTmpString& to,size_t idx,size_t len=(size_t)-1) const;

	/**
	 * Verify if a given sub-string is at a given position.
	 * @param sub            Sub-string.
	 * @param pos            Position. If negative, the sub-string is searched
	 *                       at a given position of the end.
	  @param CaseSensitive   Is the search case sensitive.
	 * @return true if the string at a given position.
	 */
	bool IsAt(const RTmpString& sub,int pos,bool CaseSensitive=true) const {return(StringMethods::IsAt<RChar>(Text,Len,sub.Text,sub.Len,pos,CaseSensitive));}

	/**
	 * Insert a sub-string at a given position of the string.
	 * @param sub            Sub-string to insert.
	 * @param pos            Position. If negative, the sub-string is searched
	 *                       at a given position of the end.
	 * @param del            Number of characters that will be replaced by the
	 *                       sub-string. If del=(size_t)-1, all the characters
	 *                       after pos are deleted.
	 */
	void Insert(const RTmpString& sub,int pos,size_t del=0);

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements        Container that will hold the results. It is not
	*                        emptied by the method.
	* @param car             Character used as separator.
	* @param del             Delimiter of an element.
	*/
	inline void Split(RContainer<RTmpString,true,false>& elements,const RChar car,const RChar del) const;

	/**
	 * Concatenate a series of elements and, eventually, separated them by a
	 * given character.
	 * @param elements       Container of elements to concatenate.
	 * @param car            Character used as separator.
    */
	template<bool a,bool o> inline void Concat(const RContainer<RTmpString,a,o>& elements,const RChar car);

	/**
	* Return a number between 0 and 26 according to the character at position
	* idx in the string.
	* @remark Supported values for idx are 1 and 2.
	* @param idx             Index of hash asked.
	* @see R::RHashContainer and R::RDblHashContainer.
	*/
	size_t HashIndex(size_t idx) const;

	/**
	 * Destruct the string.
	 */
	~RTmpString(void);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <rtmpstring.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
