/*

	R Project Library

	RTextFile.h

	Text File - Header.

	Copyright 1999-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef RStringBuilder_H
#define RStringBuilder_H


//------------------------------------------------------------------------------
// include files for R Project
#include <rstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
 * The RStringBuilder provides a	class to build efficiently a RString by adding
 * a small number of characters (eventually one). It avoids to reallocate
 * infinitively the internal buffer of a RString.
 *
 * The implementation of the RTextFile::GetChars illustrates how it works:
 * @code
 * RString RTextFile::GetChars(size_t size)
 * {
 * 	RStringBuilder Res;
 *
 * 	while((!End())&&(size--))
 * 		Res+=GetChar();
 * 	return(Res());
 * }
 * @endcode
 * @short String Builder
 */
class	RStringBuilder
{
	/**
	 * A static internal buffer used to hold intermediate characters.
	 */
	RChar Buffer[1025];

	/**
	 * The string to build.
	 */
	RString String;

	/**
	 * Number of characters actually added but not concatenated to the string.
    */
	size_t Len;

	/**
	 * Pointer to the current character in the buffer.
	 */
	RChar* CurChar;

public:

	/**
	 * Constructor of a string builder.
    */
	RStringBuilder(void);

	/**
	* Add a character to the string.
	* @param src             Character.
	*/
	RStringBuilder& operator+=(const RChar src);

	/**
	* Add a character to the string.
	* @param src             Character.
	*/
	inline RStringBuilder& operator+=(const char src)
	{
		return(operator+=(RChar(src)));
	}

	/**
	* Add a RString to the string.
	* @param src             Source string.
	*/
	RStringBuilder& operator+=(const RString& src);

	/**
	* Add some characters to the string.
	* @param src             Source string.
	*/
	RStringBuilder& operator+=(const RChar* src);

	/**
	* Add a "C string" to the string.
	* @param src             Source string.
	*/
	RStringBuilder& operator+=(const char* src);

	/**
	 * Get the string to build. If some characters are not concatenated, the
	 * method add them before returning the string.
    * @return the string.
    */
	const RString& operator()(void);

	/**
	 * Clear the string builder to construct a new string.
    */
	void Clear(void);

private:

	/**
	 * Append the current content of the internal buffer to the string.
	 */
	void AppendBuffer(void);
};


}  //-------- End of namespace std ---------------------------------------------


//------------------------------------------------------------------------------
#endif


