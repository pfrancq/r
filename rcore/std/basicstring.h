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
namespace R{
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/*
* This class implements a generic string.
* @author Pascal Francq
* @short Generic String
*/
template<class C,class S>
	class BasicString
{
protected:

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

	/*
	* Pointer to the buffer of the string.
	*/
	CharBuffer* Data;

	/*
	* Construct a null string.
	*/
	BasicString(void);

	/*
	* Copy constructor.
	*/
	BasicString(const BasicString& str);

public:

	/**
	* Return the length of the string.
	*/
	inline size_t GetLen(void) const {return(Data->Len);}

	/**
	* Return the maximal length of the string.
	*/
	inline size_t GetMaxLen(void) const {return(Data->MaxLen);}

	/**
	* Look if the string is empty.
	* @returns true if the length is null, false else.
	*/
	inline bool IsEmpty(void) const {return(!Data->Len);}

	/**
	* Deep copy of the string if necessary, i.e. when the string points to an
	* internal buffer referenced by other strings, make a copy of it.
	*/
	void Copy(void);

protected:

	/**
	* Get a uppercase version of the string.
	* @return String.
	*/
	inline S ToUpper(void) const;

	/**
	* Get a lowercase version of the string.
	* @return String.
	*/
	inline S ToLower(void) const;

	/**
	* This function return a string by stripping whitespace (or other
	* characters) from the beginning and end of the string.
	* @return String.
	*/
	inline S Trim(void) const;

	/**
	* Find the position of a given character in the string.
	* @param car             Character to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int Find(const C car,int pos=0,bool CaseSensitive=true) const;

	/**
	* Find the position of a given string in the string.
	* @param str             String to find.
	* @param pos             Position to start the search. Negative values
	*                        start the search from the end.
	* @param CaseSensitive   Is the search case sensitive.
	* @return The position of the first occurrence or -1 if the character was not
	*         found.
	*/
	inline int FindStr(const S& str,int pos=0,bool CaseSensitive=true) const;

	/**
	 * Replace a given character in the string.
	 * @param search         Character to search.
	 * @param rep            Character that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	inline void Replace(const C search,const C rep,bool first=false,int pos=0);

	/**
	 * Replace a given sub-string in the string.
	 * @param search         String to search.
	 * @param rep            String that will put in.
	 * @param first          Must it stops after the first occurrence.
	 * @param pos            Position to start. Negative values start the
	 *                       search from the end.
	 */
	template<class B> inline void ReplaceStr(const S& search,const S& rep,bool first=false,int pos=0);

	// Get a sub-string of a given string.
	template<class B> inline S Mid(size_t idx,int len=-1) const;

	// Set a new length to the string.
	template<class B> inline void SetLen(size_t len);

	// Set a new length to the string and fill it.
	template<class B> inline void SetLen(size_t len,const S& str);

	// Look if a string contains spaces only.
	inline bool ContainOnlySpaces(void) const;

	/**
	* Split the string to find all the elements separated by a given character.
	* @param elements        Container that will hold the results.
	* @param car             Character used as separator.
	*/
	inline void Split(RContainer<S,true,false>& elements,const C car) const;
};


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
