/*

	R Project Library

	RTextEnconding.h

	Generic Text Encoding Scheme - Header.

	Copyright 2004-2010 by Pascal Francq (pascal@francq.info).
	Copyright 2004-2008 by the Université Libre de Bruxelles (ULB).

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/



//------------------------------------------------------------------------------
#ifndef RTextEncoding_H
#define RTextEncoding_H



//------------------------------------------------------------------------------
// include files for ANSI C/C++
#include <iconv.h>


//------------------------------------------------------------------------------
// include files for R Project
#include <rstd.h>
#include <rstring.h>
#include <rcstring.h>


//------------------------------------------------------------------------------
namespace R{
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// New Exceptions

/**
* Exception specifying that the chosen encoding is not supported.
*/
NEWREXCEPTION(EncodingNotSupported);

/**
* Exception specifying that an invalid byte sequence cannot be handle.
*/
NEWREXCEPTION(InvalidByteSequence);

/**
* Exception specifying that the byte sequence is incomplete.
*/
NEWREXCEPTION(IncompleteByteSequence);


//------------------------------------------------------------------------------
/**
* This class provides a representation for a generic text encoding scheme other
* than Unicode UTF-16 used internally by the R library.
*
* Actually, there is only a single class implementing the functions proposed in
* <iconv.h>
* @short Generic Text Encoding Scheme.
* @author Pascal Francq.
*/
class RTextEncoding
{
public:

	/**
	 * Structure representing a Unicode character encoded using a RChar. If
	 * only one RChar is necessary to store the character, the second one is
	 * set to null.
	 */
	struct UnicodeCharacter
	{
		/**
		 * Maximum two RChar are necessary to store a Unicode character.
		 */
		RChar Codes[2];

		/**
		 * Valid Unicode character ?
		 */
		bool Valid;
	};


private:

	/**
	* The name of the encoding (always in upper case).
	*/
	RCString Name;

	/**
	* Descriptor used to make the conversion to Unicode UTF-16.
	*/
	iconv_t ToUTF16;

	/**
	* Descriptor used to make the conversion from Unicode UTF-16.
	*/
	iconv_t FromUTF16;

	/**
	* Order of the bytes.
	*/
	bool Order;

public:

	/**
	* Construct a particular encoding.
	* @param name           Name of the encoding.
	*/
	RTextEncoding(const RCString& name);

	/**
	* Initialize the text encoding.
	*/
	virtual void Init(void) const;

	/**
	* Return the name of the file.
	* @returns a string containing the name.
	*/
	const RCString& GetName(void) const {return(Name);}

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* enc.
	* @param enc             Encoding to compare with.
	* @see R::RContainer.
	*/
	int Compare(const RTextEncoding& enc) const;

	/**
	* Lexically compares two strings and returns an integer less than, equal
	* to, or greater than zero if this is less than, equal to, or greater than
	* name.
	* @param name           Name of encoding.
	* @see R::RContainer.
	*/
	int Compare(const RCString& name) const;

	/**
	* Transform a string of the given encoding to a string in Unicode.
	* @param text           Text in the given encoding.
	* @param len            Number of character to convert.
	* @return RString.
	*/
	virtual RString ToUnicode(const char* text,size_t len) const;

	/**
	* Read the next Unicode character contained in a string of the given
	* encoding.
	* @param text           Text in the given encoding.
	* @param len            Number of character to analyzed. After the call,
	*                       this parameters contains the number of bytes read.
	* @param invalid        If true, invalid characters are allowed. If false,
	*                       an exception is generated.
	*/
	virtual UnicodeCharacter NextUnicode(const char* text,size_t& len,bool invalid=false) const;

	/**
	* Transform a string in Unicode to a string of the given encoding.
	* @param text           Unicode string.
	* @return RCString.
	*/
	virtual RCString FromUnicode(const RString& text) const;

	/**
	* This method returns a text encoding scheme based on a name. The method
	* construct a text encoding for UTF-16 the first time it is used.
	*
	* The encoding schemes are added when they are requested.
	* @param name           Name of encoding.
	* @return Pointer to a RTextEncoding.
	*/
	static RTextEncoding* GetTextEncoding(const RCString& name);

	/**
	* Destruct the encoding.
	*/
	virtual ~RTextEncoding(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif

