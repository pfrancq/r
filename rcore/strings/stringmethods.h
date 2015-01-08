/*

	R Project Library

	StringMethods.h

	Generic String Methods - Header.

	Copyright 2011-2015 by Pascal Francq (pascal@francq.info).

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
#ifndef StringMethods_H
#define StringMethods_H


//-----------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rchar.h>
#include <rshareddata.h>


//-----------------------------------------------------------------------------
namespace R{
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
* This class proposes some generic static methods working on array of characters
* ending with a 0.
* @author Pascal Francq
* @short Generic String Methods.
*/
class StringMethods
{
public:

	/**
	 * Look if the string contains only spaces.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 */
	template<class C> static bool ContainOnlySpaces(const C* src,size_t len);

	/**
	 * Find the position of a given character in the string.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 * @param car            Character to find.
	 * @param pos            Position to start the search. Negative values start
	 *                       the search from the end.
	 * @param CaseSensitive  Is the search case sensitive.
	 * @return The position of the first occurrence or -1 if the character was
	 *         not found.
	 */
	template<class C> static int Find(const C* src,size_t len,const C car,int pos=0,bool CaseSensitive=true);

	/**
	 * Find the position of a given string in the string.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 * @param str            String to find.
	 * @param strlen         Length of the string to find.
	 * @param pos            Position to start the search. Negative values start
	 *                       the search from the end.
	 * @param CaseSensitive  Is the search case sensitive.
	 * @return The position of the first occurrence or -1 if the string was not
	 *         found.
	 */
	template<class C> static int FindStr(const C* src,size_t len,const C* str,size_t strlen,int pos=0,bool CaseSensitive=true);

	/**
	 * Find the position of a character of a given set in the string.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 * @param str            String to find.
	 * @param strlen         Length of the string to find.
	 *
	 * @param pos            Position to start the search. Negative values start
	 *                       the search from the end.
	 * @param CaseSensitive  Is the search case sensitive.
	 * @return The position of the first occurrence or -1 if the character was
	 *         not found.
	 */
	template<class C> static int FindAnyStr(const C* src,size_t len,const C* str,size_t strlen,int pos=0,bool CaseSensitive=true);

	/**
	 * Look if a string begins with a given sub-string.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 * @param begin          String to to search for.
	 * @param beginlen       Length of the string to find.
	 * @param CaseSensitive  Is the search case sensitive.
    * @param skip           If true, leading spaces are skipped for the
	 *                       comparison.
    * @return true if the string begins with the sub-string.
    */
	template<class C> static bool Begins(const C* src,size_t len,const C* begin,size_t beginlen,bool CaseSensitive=true,bool skip=false);

	/**
	 * Look if a string ends with a given sub-string.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 * @param end            String to to search for.
	 * @param endlen         Length of the string to find.
	 * @param CaseSensitive  Is the search case sensitive.
    * @param skip            If true, ending spaces are skipped for the
	 *                        comparison.
    * @return true if the string ends with the sub-string.
    */
	template<class C> static bool Ends(const C* src,size_t len,const C* end,size_t endlen,bool CaseSensitive=true,bool skip=false);

	/**
	 * Verify if a given sub-string is at a given position.
	 * @tparam C             Type of characters.
	 * @param src            Character array.
	 * @param len            Length of the character array.
	 * @param sub            String to to search for.
	 * @param sublen         Length of the string to find.
	 * @param pos            Position to start the search. Negative values start
	 *                       the search from the end.
	 * @param CaseSensitive  Is the search case sensitive.
	 * @return true if the string at a given position.
	 */
	template<class C> static bool IsAt(const C* src,size_t len,const C* sub,size_t sublen,int pos,bool CaseSensitive=true);
};


//-----------------------------------------------------------------------------
// Template implementation
#include <stringmethods.hh>


}  //-------- End of namespace R ----------------------------------------------


//-----------------------------------------------------------------------------
#endif
