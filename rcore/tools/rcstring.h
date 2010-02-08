/*

	R Project Library

	RCString.h

	C String - Header.

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
	/** @copydoc BasicString::Copy(const C*,size_t)*/
	inline void Copy(const char* text,size_t nb) {BasicString<char,RCString>::Copy(text,nb);}

	inline size_t GetLen(void) const {return(BasicString<char,RCString>::GetLen());}

	inline size_t GetMaxLen(void) const {return(BasicString<char,RCString>::GetMaxLen());}

	/** @copydoc BasicString::SetLen(size_t) */
	inline void SetLen(size_t len) {BasicString<char,RCString>::SetLen(len);}

	/** @copydoc BasicString::SetLen(size_t,const S&) */
	inline void SetLen(size_t len,const RCString& str) {BasicString<char,RCString>::SetLen(len,str);}

	inline bool IsEmpty(void) const {return(BasicString<char,RCString>::IsEmpty());}

	inline bool ContainOnlySpaces(void) const {return(BasicString<char,RCString>::ContainOnlySpaces());}
	//@} Internal methods

	/** @name Manipulation methods	*/ // @{
	inline void Clear(void) {BasicString<char,RCString>::Clear();}

	inline RCString ToUpper(void) const {return(BasicString<char,RCString>::ToUpper());}


	inline RCString ToLower(void) const {return(BasicString<char,RCString>::ToLower());}

	inline RCString Trim(void) const {return(BasicString<char,RCString>::Trim());}

	/** @copydoc BasicString::Trim(const S&) const */
	inline RCString Trim(const RCString& str) const {return(BasicString<char,RCString>::Trim(str));}

	/** @copydoc BasicString::Find(const C,int,bool) const */
	inline int Find(const char car,int pos=0,bool CaseSensitive=true) const {return(BasicString<char,RCString>::Find(car,pos,CaseSensitive));}

	/** @copydoc BasicString::FindStr(const S&,int,bool) const */
	inline int FindStr(const RCString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<char,RCString>::FindStr(str,pos,CaseSensitive));}

	/** @copydoc BasicString::FindAnyStr(const S&,int,bool) const */
	inline int FindAnyStr(const RCString& str,int pos=0,bool CaseSensitive=true) const {return(BasicString<char,RCString>::FindAnyStr(str,pos,CaseSensitive));}

	/** @copydoc BasicString::Replace(const C,const C,bool,int) */
	inline void Replace(const char search,const char rep,bool first=false,int pos=0) {BasicString<char,RCString>::Replace(search,rep,first,pos);}

	/** @copydoc BasicString::ReplaceStr(const S&,const S&,bool,int) */
	inline void ReplaceStr(const RCString& search,const RCString& rep,bool first=false,int pos=0) {BasicString<char,RCString>::ReplaceStr(search,rep,first,pos);}

	inline RCString Mid(size_t idx,size_t len=(size_t)-1) const {return(BasicString<char,RCString>::Mid(idx,len));}

	/** @copydoc BasicString::IsAt(const S&,int) const */
	inline bool IsAt(const RCString& sub,int pos) const  {return(BasicString<char,RCString>::IsAt(sub,pos));}

	/** @copydoc BasicString::Insert(const S&,int,size_t) */
	void Insert(const RCString& sub,int pos,size_t del=0)  {BasicString<char,RCString>::Insert(sub,pos,del);}

	/** @copydoc BasicString::Split(RContainer<S,true,false>&,const C,const C) const */
	inline void Split(RContainer<RCString,true,false>& elements,const char car,const char del=0) const {BasicString<char,RCString>::Split(elements,car,del);}
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

	inline char& operator[](size_t idx) {return(BasicString<char,RCString>::operator[](idx));}

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

	/** @copydoc BasicString::operator=(const S&) */
	RCString& operator=(const RCString& src) {return(BasicString<char,RCString>::operator=(src));}

	/** @copydoc BasicString::operator=(const C*) */
	RCString& operator=(const char* src) {return(BasicString<char,RCString>::operator=(src));}

	/**
	* Assignment operator using a string.
	* @param src             Source string.
	*/
	RCString& operator=(const std::string& src);

	/** @copydoc BasicString::operator+=(const S&) */
	inline RCString& operator+=(const RCString& src) {return(BasicString<char,RCString>::operator+=(src));}

	/** @copydoc BasicString::operator+=(const C*) */
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
