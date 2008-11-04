/*

	R Project Library

	BasicString.h

	Generic String - Header.

	Copyright 2005 by the Université Libre de Bruxelles.

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
#ifndef BasicString_H
#define BasicString_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rshareddata.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/**
* This class implements a generic string. It is used to instantiate RString and
* RCString. Its role is to centralize as much code as possible.
* @author Pascal Francq
* @short Generic String
*/
template<class C,class S>
	class BasicString
{
public:

	class Ref
	{
	public:
		size_t Pos;
		size_t Len;
		S* Str;
	};

protected:

	/**
	 * This class implement a string that can be shared between strings.
	 */
	class CharBuffer : public RSharedData
	{
	public:
		C* Text;       // Text
		size_t Len;    // Actual length
		size_t MaxLen; // Maximum length
		char* Latin1;  // Latin1 version of the string.

		CharBuffer(void)
			: RSharedData(), Text(0), Len(0), MaxLen(0), Latin1(0) {}
		CharBuffer(C* tab,size_t len,size_t maxlen)
			: RSharedData(), Text(tab), Len(len), MaxLen(maxlen),Latin1(0) {}
		void Verify(size_t maxlen);
		void InvalidLatin1(void) {delete[] Latin1; Latin1=0;}
		~CharBuffer(void){delete[] Text; delete[] Latin1;}
	};

	/**
	* Pointer to the buffer representing the null string.
	*/
	static CharBuffer* DataNull;

	/**
	* Pointer to the buffer of the string.
	*/
	CharBuffer* Data;

	/**
	* Construct a null string.
	*/
	BasicString(void);

	/**
	* Copy constructor.
	* @param str             Original string.
	*/
	BasicString(const BasicString& str);

	/**
	* Construct a string from an array.
	* @param src             Array used as reference.
	*/
	BasicString(const C* src);

	/**
	* Construct a string by doing a deep copy of the first characters of a "C"
	* string.
	* @param src             C string used as reference.
	* @param len             Length.
	*/
	BasicString(const C* src,size_t len);

protected:

	/**
	* Return the pointer to the "null" buffer. If it is not created, create it.
	* @return Pointer to the "null" buffer.
	*/
	static CharBuffer* GetDataNull(void);

	/**
	* Deep copy of the string if necessary, i.e. when the string points to an
	* internal buffer referenced by other strings, make a copy of it.
	*/
	void Copy(void);

public:

	/**
	* Copy a certain number of characters in the string.
	* @param text            Text to copy.
	* @param nb              Number of characters to copy.
	*/
	void Copy(const C* text,size_t nb);

	/**
	* Assignment operator using another string.
	* @param src             Source string.
	*/
	BasicString& operator=(const BasicString& src);

	/**
	* Return the length of the string.
	*/
	inline size_t GetLen(void) const {return(Data->Len);}

	/**
	* Return the maximal length of the string.
	*/
	inline size_t GetMaxLen(void) const {return(Data->MaxLen);}

	/**
	* Set the length of the string. If the length is greater than the current
	* one, the internal buffer is updated. Any new space allocated contains
	* arbitrary data.
	* @param len             Length of the string.
	*/
	void SetLen(size_t len);

	/**
	* Set the length of the string. If the length is greater than the current
	* one, the second string is used to fill the first string (eventually it is
	* copied several times).
	* @param len             Length of the string.
	* @param str             String used to fill.
	*/
	void SetLen(size_t len,const S& str);

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	inline bool IsEmpty(void) const {return(!Data->Len);}

	/**
	* Clear the content of the string.
	*/
	void Clear(void);

	/**
	* Get a uppercase version of the string.
	* @return String.
	*/
	S ToUpper(void) const;

	/**
	* Get a lowercase version of the string.
	* @return String.
	*/
	S ToLower(void) const;

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	* @return String.
	*/
	S Trim(void) const;

	/**
	 * Look if the string contains only spaces.
	 */
	bool ContainOnlySpaces(void) const;

	/**
	* Add another string.
	* @param src             Source string.
	*/
	S& operator+=(const S& src);

	/**
	* Add a string to the string.
	* @param src             Source string.
	*/
	S& operator+=(const C* src);

	/**
	* Find the position of a given character in the string.
	* @param car             Character to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	int Find(const C car,int pos=0,bool CaseSensitive=true) const;

	/**
	* Find the position of a given string in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	int FindStr(const S& str,int pos=0,bool CaseSensitive=true) const;

	/**
	 * Replace a given character in the string.
	 * @param search         Character to search.
	 * @param rep            Character that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	void Replace(const C search,const C rep,bool first=false,int pos=0);

	/**
	 * Replace a given sub-string in the string.
	 * @param search         String to search.
	 * @param rep            String that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	void ReplaceStr(const S& search,const S& rep,bool first=false,int pos=0);

	/**
	* This function returns the character at a given position in the string.
	* (Read only).
	* @param idx             Position of the character.
	*/
	const C& operator[](size_t idx) const;

	/**
	* This function returns the character at a given position in the string.
	* @param idx             Position of the character.
	*/
	C& operator[](size_t idx);

	/**
	* Get a sub-string of a given string.
	* @param idx             Index of the first character.
	* @param len             Length of the sub-string. If the length is not
	*                        specified, the end of the string is copied.
	* @returns A string containing the substring.
	*/
	S Mid(size_t idx,size_t len=(size_t)-1) const;

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements        Container that will hold the results. It is not
	*                        emptied by the method.
	* @param car             Character used as separator.
	*/
	void Split(RContainer<S,true,false>& elements,const C car) const;

	/**
	 * Destruct the string.
	 */
	~BasicString(void);

protected:

	void Dummy(void);
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
