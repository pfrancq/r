/*

	R Project Library

	RTextEnconding.h

	Genereic Text Encoding Scheme - Header.

	Copyright 2004-2005 by the Université Libre de Bruxelles.

	Authors:
		Pascal Francq (pfrancq@ulb.ac.be).

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
// New Exception
NEWREXCEPTION(EncodingNotSupported);
NEWREXCEPTION(InvalidByteSequence);
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
	/**
	* The name of the enconding (always in uppercas).
	*/
	RString Name;

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
	* Construct a particular enconding.
	* @param name           Name of the encoding.
	*/
	RTextEncoding(const RString& name);

	/**
	* Initialize the text encoding.
	*/
	virtual void Init(void) const;

	/**
	* Return the name of the file.
	* @returns a string containing the name.
	*/
	const RString& GetName(void) const {return(Name);}

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param enc            Encoding.
	*/
	int Compare(const RTextEncoding& enc) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param enc            Encoding.
	*/
	int Compare(const RTextEncoding* enc) const;

	/**
	* Compare function like strcmp used in particular for RContainer class.
	* @param name           Name of encoding.
	*/
	int Compare(const RString& name) const;

	/**
	* Transform a string of the given encoding to a string in Unicode.
	* @param text           Text in the given encoding.
	* @param len            Number of character to convert.
	* @return RString.
	*/
	virtual RString ToUnicode(const char* text,unsigned int len) const;

	/**
	* Read the next unicode character contained in a string of the given
	* encoding.
	* @param text           Text in the given encoding.
	* @param len            Number of character to analyzed. After the call,
	*                       this parameters contains the number of bytes read.
	* @return RChar.
	*/
	virtual RChar NextUnicode(const char* text,unsigned int& len) const;

	/**
	* Transform a Unicode string to a string of the given encoding.
	* @param text           Unicode string.
	* @return RCString.
	*/
	virtual RCString FromUnicode(const RString& text) const;

	/**
	* This method returns a text encoding scheme based on a name. The method
	* construct a text encoding for UTF-16 tbe first time it is used.
	*
	* The encoding schemes are added when they are requested.
	* @param name           Name of encoding.
	* @return Pointer to a RTextEncoding.
	*/
	static RTextEncoding* GetTextEncoding(const RString& name);

	/**
	* Destructor of the encoding.
	*/
	virtual ~RTextEncoding(void);
};


}  //-------- End of namespace R -----------------------------------------------


//------------------------------------------------------------------------------
#endif

